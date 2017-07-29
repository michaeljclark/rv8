/*
 *  mmap-core.h
 *
 *  mmap bitmap tracker
 */

#ifndef _mmap_core_h
#define _mmap_core_h

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*munmap_fn)(void *addr, size_t len);
typedef void* (*mmap_fn)(void *addr, size_t len, int prot, int flags, int fd, off_t offset);

int guest_munmap(void *addr, size_t len);
void* guest_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset);

int __host_munmap(munmap_fn munmap, void *addr, size_t len);
int __guest_munmap(munmap_fn munmap, void *addr, size_t len);
void* __host_mmap(mmap_fn mmap, void *addr, size_t len, int prot, int flags, int fd, off_t offset);
void* __guest_mmap(mmap_fn mmap, void *addr, size_t len, int prot, int flags, int fd, off_t offset);

#ifdef __cplusplus
}
#endif

#endif