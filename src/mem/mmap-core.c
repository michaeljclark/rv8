/*
 *  mmap-core.c
 *
 *  mmap bitmap tracker
 */

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <pthread.h>
#include <sys/mman.h>

#include "mmap-core.h"

static _Bool map_inited = false;
static _Bool map_debug = false;
static pthread_mutex_t meta_lock = PTHREAD_MUTEX_INITIALIZER;

static const uintptr_t GUEST_MMAP_BASE = 0x40000000ULL;
static const uintptr_t HOST_MMAP_BASE = 0x7fff00000000UL;
static const uintptr_t METADATA_BASE = 0x7f0000000000UL;
static const uintptr_t PAGE_SHIFT = 12;
static const uintptr_t PAGE_SIZE = (1ULL << 12);
static const uintptr_t PAGE_MASK = ((1ULL << 12) - 1);
static const uintptr_t LEVEL_SIZE = (1ULL << 12);
static const uintptr_t LEVEL_BYTES = ((1ULL << 12) >> 3);
static const int METADATA_PROT = PROT_READ | PROT_WRITE;
static const int METADATA_FLAGS = MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE;

static void mark_used(uintptr_t start_of_range, size_t len);
static void mark_free(uintptr_t start_of_range, size_t len);
static _Bool page_present(uintptr_t addr);

static void init_mmap()
{
	if(map_inited) return;
	if (mmap((void*)METADATA_BASE, 4096, METADATA_PROT, METADATA_FLAGS, -1, 0) == MAP_FAILED) {
		fprintf(stderr, "mmap failed: %s", strerror(errno));
		exit(1);
	}
	map_inited = 1;
}

static uintptr_t level_bmap(size_t level, uintptr_t addr, uintptr_t *offset)
{
	uintptr_t bmap;
	switch (level) {
		case 3:
			bmap = METADATA_BASE;
			break;
		case 2:
			bmap = METADATA_BASE + LEVEL_BYTES + ((addr >> 36) & 0xfff) * LEVEL_BYTES;
			break;
		case 1:
			bmap = METADATA_BASE + LEVEL_BYTES + (LEVEL_SIZE * LEVEL_BYTES) + ((addr >> 24) & 0xffffff) * LEVEL_BYTES;
			break;
		default:
			bmap = -1;
			break;
	}
	if (offset) {
		*offset = ((addr >> (12 * level)) & PAGE_MASK);
	}
	return bmap;
}

static void reify_bmap(char *bmap)
{
	for (size_t level = 2; level > 0; level--) {
		uintptr_t offset1;
		char *bmap1 = (char*)level_bmap(level + 1, (uintptr_t)bmap, &offset1);
		if ((bmap1[offset1 >> 3] & (1 << (offset1 & 7))) == 0) {
			uintptr_t offset2;
			char *bmap2 = (char*)level_bmap(level, (uintptr_t)bmap, &offset2);
			void *rv, *map_addr = (void*)((uintptr_t)bmap2 & ~(PAGE_SIZE-1));
			if (map_debug) {
				printf("reifying addr=%p level=%zu %p:%zu -> %p:%zu mmap(%p,%p)\n",
					bmap, level, bmap1, offset1, bmap2, offset2, map_addr, (void*)PAGE_SIZE);
			}
			if ((rv = mmap(map_addr, PAGE_SIZE, METADATA_PROT, METADATA_FLAGS, -1, 0)) == MAP_FAILED) {
				fprintf(stderr, "mmap failed: %s\n", strerror(errno));
				exit(1);
			} else if (rv != map_addr) {
				fprintf(stderr, "mmap failed: invalid address\n");
				exit(1);
			}
			bmap1[offset1 >> 3] = 0xff;
		}
	}
}

static _Bool page_present(uintptr_t addr)
{
	for (size_t level = 3; level > 0; level--) {
		uintptr_t offset;
		char *bmap = (char*)level_bmap(level, addr, &offset);
		_Bool is_present = bmap[offset >> 3] & 1 << (offset & 7);
		if (!is_present) return false;
	}
	return true;
}

static uintptr_t find_free(uintptr_t start, uintptr_t end, size_t len)
{
	if (map_debug) {
		printf("find_free range=(%p-%p) len=%zu\n",
			(void*)start, (void*)end, len);
	}
	uintptr_t start_of_range;
	for (uintptr_t addr = start; addr < end; addr += PAGE_SIZE) {
		start_of_range = addr;
		reify_bmap((char*)addr);
		for (; addr < end && !page_present(addr); addr += PAGE_SIZE) {
			if (addr - start_of_range >= len) {
				if (map_debug) {
					printf("find_free found=(%p-%p) scanned=%zu\n",
						(void*)start_of_range, (void*)(start_of_range + len),
						(start_of_range - start) >> PAGE_SHIFT);
				}
				return start_of_range;
			}
		}
	}
	return 0;
}

static void mark_used(uintptr_t start_of_range, size_t len)
{
	if (map_debug) {
		printf("mark_used range=(%p-%p) len=%zu\n",
			(void*)start_of_range, (void*)(start_of_range + len), len);
	}
	for (uintptr_t addr = start_of_range; addr < start_of_range + len; addr += PAGE_SIZE) {
		reify_bmap((char*)addr);
		uintptr_t offset;
		char *bmap = (char*)level_bmap(1, addr, &offset);
		bmap[offset >> 3] |= 1 << (offset & 7);
	}
}

static void mark_free(uintptr_t start_of_range, size_t len)
{
	if (map_debug) {
		printf("mark_free range=(%p-%p) len=%zu\n",
			(void*)start_of_range, (void*)(start_of_range + len), len);
	}
	for (uintptr_t addr = start_of_range; addr < start_of_range + len; addr += PAGE_SIZE) {
		reify_bmap((char*)addr);
		uintptr_t offset;
		char *bmap = (char*)level_bmap(1, addr, &offset);
		bmap[offset >> 3] &= ~(1 << (offset & 7));
	}
}

static int __munmap(munmap_fn munmap, void *addr, size_t len)
{
	pthread_mutex_lock(&meta_lock);
	init_mmap();
	int rv = munmap(addr, len);
	if (rv == 0) {
		mark_free((uintptr_t)addr, len);
	}
	pthread_mutex_unlock(&meta_lock);
	return rv;
}

int __host_munmap(munmap_fn munmap, void *addr, size_t len)
{
	int rv = __munmap(munmap, addr, len);
	if (map_debug) {
		printf("host_munmap addr=%p len=%zu result=%d\n", addr, len, rv);
	}
	return rv;
}

int __guest_munmap(munmap_fn munmap, void *addr, size_t len)
{
	int rv = __munmap(munmap, addr, len);
	if (map_debug) {
		printf("guest_munmap addr=%p len=%zu result=%d\n", addr, len, rv);
	}
	return rv;
}

void* __host_mmap(mmap_fn mmap, void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	if (addr && (uintptr_t)addr < HOST_MMAP_BASE) {
		return MAP_FAILED;
	}
	pthread_mutex_lock(&meta_lock);
	init_mmap();
	if (addr == NULL) {
		addr = (void*)find_free(HOST_MMAP_BASE, -1, len);
		if (!addr) {
			pthread_mutex_unlock(&meta_lock);
			return addr;
		}
		flags |= MAP_FIXED;
	}
	void *rv = mmap((void*)addr, len, prot, flags, fd, offset);
	if (map_debug) {
		printf("host_mmap addr=%p len=%zu result=%p\n", addr, len, rv);
	}
	if (rv != MAP_FAILED) {
		mark_used((uintptr_t)rv, len);
	}
	pthread_mutex_unlock(&meta_lock);
	return rv;
}

void* __guest_mmap(mmap_fn mmap, void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	if ((uintptr_t)(addr + len) > HOST_MMAP_BASE) {
		return MAP_FAILED;
	}
	pthread_mutex_lock(&meta_lock);
	init_mmap();
	if (addr == NULL) {
		addr = (void*)find_free(GUEST_MMAP_BASE, HOST_MMAP_BASE, len);
		if (!addr) {
			pthread_mutex_unlock(&meta_lock);
			return addr;
		}
		flags |= MAP_FIXED;
	}
	void *rv = mmap((void*)addr, len, prot, flags, fd, offset);
	if (map_debug) {
		printf("guest_mmap addr=%p len=%zu result=%p\n", addr, len, rv);
	}
	if (rv != MAP_FAILED) {
		mark_used((uintptr_t)rv, len);
	}
	pthread_mutex_unlock(&meta_lock);
	return rv;
}
