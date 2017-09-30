/*
 *  mmap-macos.c
 *
 *  mmap interception to move maps with default address to high memory
 */

#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <unistd.h>
#include <sys/mman.h>
#include <mach/vm_map.h>
#include <mach/vm_param.h>
#include <mach/mach_vm.h>

#include "mmap-core.h"

static uintptr_t map_base = 0x7ff000000000UL;
static const uintptr_t entropy_mask = 0xffffffUL;

__attribute__((__always_inline__)) static uintptr_t round_page(uintptr_t x)
{
	return (x & ~(PAGE_SIZE - 1UL));
}

int guest_munmap(void *addr, size_t len)
{
	return __guest_munmap(munmap, addr, len);
}

void* guest_mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	return __guest_mmap(mmap, addr, len, prot, flags, fd, offset);
}

static void*
__mmap(void *addr, size_t len, int prot, int flags, int fd, off_t offset)
{
	size_t incr = 0;
	if (addr == 0) {
		addr = (void*)map_base;
		incr = round_page(len);
	}
	void *rv = mmap(addr, len, prot, flags, fd, offset);
	if (rv != MAP_FAILED) map_base += incr;
	return rv;
}

static int
__munmap(void *addr, size_t len)
{
	return munmap(addr, len);
}

static kern_return_t
__vm_allocate(vm_map_t task, vm_address_t *addr, vm_size_t size, int flags)
{
	if (flags & VM_FLAGS_ANYWHERE) {
		*addr = round_page(map_base + (*addr & entropy_mask));
		flags &= ~VM_FLAGS_ANYWHERE;
	}
	int rv = vm_allocate(task, addr, size, flags);
	if (!rv) {
		map_base = round_page(*addr + size);
	}
	return rv;
}

static kern_return_t
__mach_vm_allocate(vm_map_t task, mach_vm_address_t *addr, vm_size_t size, int flags)
{
	if (flags & VM_FLAGS_ANYWHERE) {
		*addr = round_page(map_base + (*addr & entropy_mask));
		flags &= ~VM_FLAGS_ANYWHERE;
	}
	int rv = mach_vm_allocate(task, addr, size, flags);
	if (!rv) {
		map_base = round_page(*addr + size);
	}
	return rv;
}

static kern_return_t
__vm_map(vm_map_t task, vm_address_t *addr, vm_size_t size, vm_offset_t mask,
	int flags, mem_entry_name_port_t object, memory_object_offset_t offset, boolean_t copy,
	vm_prot_t cur_protection, vm_prot_t max_protection, vm_inherit_t inheritance)
{
	if (flags & VM_FLAGS_ANYWHERE) {
		*addr = round_page(map_base + (*addr & entropy_mask));
	}
	int rv = vm_map(task, addr, size, mask, flags, object, offset, copy,
		cur_protection, max_protection, inheritance);
	if (!rv) {
		map_base = round_page(*addr + size);
	}
	return rv;
}

static kern_return_t
__mach_vm_map(vm_map_t task, mach_vm_address_t *addr, vm_size_t size, mach_vm_offset_t mask,
	int flags, mem_entry_name_port_t object, memory_object_offset_t offset, boolean_t copy,
	vm_prot_t cur_protection, vm_prot_t max_protection, vm_inherit_t inheritance)
{
	if (flags & VM_FLAGS_ANYWHERE) {
		*addr = round_page(map_base + (*addr & entropy_mask));
	}
	int rv = mach_vm_map(task, addr, size, mask, flags, object, offset, copy,
		cur_protection, max_protection, inheritance);
	if (!rv) {
		map_base = round_page(*addr + size);
	}
	return rv;
}

typedef struct { const void* replacement; const void* replacee; } interpose_t;

__attribute__((used)) static const interpose_t __interposers[]
__attribute__ ((section("__DATA, __interpose"))) = {
	{ (void *)__mmap,             (void *)mmap },
	{ (void *)__munmap,           (void *)munmap },
	{ (void *)__vm_allocate,      (void *)vm_allocate },
	{ (void *)__vm_map,           (void *)vm_map },
	{ (void *)__mach_vm_allocate, (void *)mach_vm_allocate },
	{ (void *)__mach_vm_map,      (void *)mach_vm_map },
};
