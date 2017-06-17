/*
 *  mmap-linux.c
 *
 *  mmap interception to move maps with default address to high memory
 */

#define _GNU_SOURCE
#include <stdint.h>
#include <unistd.h>
#include <sys/mman.h>
#include <dlfcn.h>

static size_t page_size = 0;
static uintptr_t map_base = 0x7fff00000000UL;
static void* (*real_mmap)(void *addr, size_t len, int prot, int flags, int fd, off_t offset) = NULL;

static uintptr_t round_page(uintptr_t x)
{
	if (!page_size) {
		page_size = getpagesize();
	}
	return ((x + page_size - 1UL) & ~(page_size - 1UL));
}

void* mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	if (!page_size) {
		page_size = getpagesize();
	}
	if (!real_mmap) {
		*(void **)(&real_mmap) = dlsym(RTLD_NEXT, "mmap");
	}
	size_t incr = 0;
	if (addr == 0) {
		addr = (void*)map_base;
		incr = round_page(len + page_size);
	}
	void *rv = real_mmap(addr, len, prot, flags, fd, offset);
	if (rv != MAP_FAILED) map_base += incr;
	return rv;
}
