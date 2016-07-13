//
//  tlsf-init.cc
//

#include <cstdio>
#include <cstdint>
#include <cstdlib>

#include <sys/mman.h>

#include "tlsf.h"

static const uintptr_t heap_loc = 0x780000000000; // 120 TiB
static const size_t heap_size   = 0x10000000;     // 1GiB
static void *heap_map           = NULL;
static tlsf_t heap_tlsf         = NULL;

void *malloc(size_t size)
{
	if (heap_map == NULL) {
		heap_map = mmap((void*)heap_loc, heap_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_FIXED | MAP_SHARED, -1, 0);
		if (heap_map == MAP_FAILED) {
			perror("mmap");
			exit(9);
		}
		heap_tlsf = tlsf_create_with_pool(heap_map, heap_size);
	}
	return tlsf_malloc(heap_tlsf, size);
}

void free(void *ptr)
{
	tlsf_free(heap_tlsf, ptr);
}

void* operator new(std::size_t sz) {
    return malloc(sz);
}

void operator delete(void* ptr) noexcept
{
    free(ptr);
}
