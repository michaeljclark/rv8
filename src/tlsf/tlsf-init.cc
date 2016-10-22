//
//  tlsf-init.cc
//

#include <cstdio>
#include <cstdint>
#include <cstdlib>

#include <sys/mman.h>

#include "tlsf.h"

#define RISCV_TLSF_OVERRIDE_MALLOC
#define RISCV_TLSF_USE_GLIBC_MALLOC_HOOK

#if defined (RISCV_TLSF_OVERRIDE_MALLOC)

#if defined (__GLIBC__) && defined (RISCV_TLSF_USE_GLIBC_MALLOC_HOOK)
#include <malloc.h>
#else
#endif

static const uintptr_t heap_loc = 0x80000000; // 2 GiB
static const size_t heap_size   = 0x10000000; // 1 GiB
static void *tlsf_heap_mmap     = nullptr;
static tlsf_t tlsf_heap         = nullptr;

static void tlsf_initialize(void)
{
	tlsf_heap_mmap = mmap((void*)heap_loc, heap_size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_FIXED | MAP_SHARED, -1, 0);
	if (tlsf_heap_mmap == MAP_FAILED) {
		perror("mmap");
		exit(9);
	}
	tlsf_heap = tlsf_create_with_pool(tlsf_heap_mmap, heap_size);
}

#if defined (__GLIBC__) && defined (RISCV_TLSF_USE_GLIBC_MALLOC_HOOK)

static void* tlsf_malloc_hook(size_t size, const void *caller)
{
	return tlsf_malloc(tlsf_heap, size);
}

static void* tlsf_realloc_hook(void *ptr, size_t size, const void *caller)
{
	return tlsf_realloc(tlsf_heap, ptr, size);
}

static void tlsf_free_hook(void *ptr, const void *caller)
{
	tlsf_free(tlsf_heap, ptr);
}

static void tlsf_initialize_hook(void)
{
	tlsf_initialize();
	__malloc_hook = tlsf_malloc_hook;
	__realloc_hook = tlsf_realloc_hook;
	__free_hook = tlsf_free_hook;
}

void (*__MALLOC_HOOK_VOLATILE __malloc_initialize_hook) (void) = tlsf_initialize_hook;

#else

extern "C" void *malloc(size_t size)
{
	if (tlsf_heap_mmap == nullptr) tlsf_initialize();
	return tlsf_malloc(tlsf_heap, size);
}

extern "C" void *realoc(void *ptr, size_t size)
{
	return tlsf_realloc(tlsf_heap, ptr, size);
}

extern "C" void free(void *ptr)
{
	tlsf_free(tlsf_heap, ptr);
}

#endif

void* operator new(std::size_t sz) { return std::malloc(sz); }
void* operator new[](std::size_t sz) { return std::malloc(sz); }
void operator delete(void* ptr) noexcept { std::free(ptr); }
void operator delete[](void* ptr) noexcept { std::free(ptr); }

#endif
