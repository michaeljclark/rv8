#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cerrno>

#include <sys/mman.h>

static const uintptr_t morecore_page_size    = 0x10000;     /* 64KiB */
static const uintptr_t morecore_heap_offset  = 0x80000000;  /* 2GiB */
static const uintptr_t morecore_heap_size    = 0x40000000;  /* 1GiB */
static const int       morecore_pool_entries = 512;
static int             morecore_pool_count = 0;
static void*           morecore_pool_mem[morecore_pool_entries] = { 0 };
static size_t          morecore_pool_size[morecore_pool_entries] = { 0 };

void os_morecore_cleanup(void)
{
  void **ptr;
  size_t *sz;

  for (ptr = morecore_pool_mem, sz = morecore_pool_size;
       ptr < &morecore_pool_mem[morecore_pool_entries]; ptr++, sz++)
  {
    if (*ptr) {
       munmap(*ptr, *sz);
       *ptr = 0;
       *sz = 0;
    }
  }
}

void *os_more_core(int size)
{
  void *ptr = 0;
  static void *sbrk_top = 0;

  if (size < 0) {
    return (void *)-1;
  } else if (size == 0) {
    return sbrk_top;
  }

  /*
  if (sbrk_top == 0) {
    atexit(os_morecore_cleanup);
  }
  */

  if (morecore_pool_count + 1 >= morecore_pool_entries) {
    fprintf(stderr, "osMoreCore: out of memory\n");
    exit(9);
  }

  /* align size to block size and calculate location */
  size = (size + (morecore_page_size-1)) & ~(morecore_page_size-1);
  ptr = (void*)(morecore_pool_count == 0 ? morecore_heap_offset :
    (uintptr_t)morecore_pool_mem[morecore_pool_count - 1] + morecore_pool_size[morecore_pool_count - 1]);
  if ((uintptr_t)ptr + size - morecore_heap_offset > morecore_heap_size) {
    size = morecore_heap_size - (uintptr_t)ptr;
  }
  if (size < 0) {
    return (void *)-1;
  }

  ptr = mmap((void*)ptr, size, PROT_READ | PROT_WRITE, MAP_ANONYMOUS | MAP_FIXED | MAP_SHARED, -1, 0);
  if (ptr == MAP_FAILED) {
    fprintf(stderr, "osMoreCore: mmap failed: %s", strerror(errno));
    exit(9);
  }

  // save ptrs so they can be freed during cleanup
  morecore_pool_mem[morecore_pool_count] = ptr;
  morecore_pool_size[morecore_pool_count] = size;
  morecore_pool_count++;
  sbrk_top = (char *)ptr + size;

  return ptr;
}


#define RISCV_OVERRIDE_MALLOC
#define RISCV_USE_GLIBC_MALLOC_HOOK

#if defined (RISCV_OVERRIDE_MALLOC)
#include <unistd.h>
extern "C" void* dlmalloc(size_t);
extern "C" void* dlrealloc(void*, size_t);
extern "C" void  dlfree(void*);

#if defined (__GLIBC__) && defined (RISCV_USE_GLIBC_MALLOC_HOOK)
#include <malloc.h>
#else
#endif

#if defined (__GLIBC__) && defined (RISCV_USE_GLIBC_MALLOC_HOOK)

static void* local_malloc_hook(size_t size, const void *caller)
{
  return dlmalloc(size);
}

static void* local_realloc_hook(void *ptr, size_t size, const void *caller)
{
  return dlrealloc(ptr, size);
}

static void local_free_hook(void *ptr, const void *caller)
{
  dlfree(ptr);
}

static void local_initialize_hook(void)
{
  __malloc_hook = local_malloc_hook;
  __realloc_hook = local_realloc_hook;
  __free_hook = local_free_hook;
}

void (*__MALLOC_HOOK_VOLATILE __malloc_initialize_hook) (void) = local_initialize_hook;

#else

extern "C" void *malloc(size_t size)
{
  return dlmalloc(size);
}

extern "C" void *realoc(void *ptr, size_t size)
{
  return dlrealloc(ptr, size);
}

extern "C" void free(void *ptr)
{
  dlfree(ptr);
}

#endif

void* operator new(std::size_t sz) { return std::malloc(sz); }
void* operator new[](std::size_t sz) { return std::malloc(sz); }
void operator delete(void* ptr) noexcept { std::free(ptr); }
void operator delete[](void* ptr) noexcept { std::free(ptr); }

#endif