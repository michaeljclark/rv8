//
//  riscv-syscalls-posix.h
//

#ifndef riscv_syscalls_posix_h
#define riscv_syscalls_posix_h

template <typename P> void riscv_sys_close(P &proc)
{
	proc.ireg[riscv_ireg_a0] = close(proc.ireg[riscv_ireg_a0]);
}

template <typename P> void riscv_sys_write(P &proc)
{
	proc.ireg[riscv_ireg_a0] = write(proc.ireg[riscv_ireg_a0],
		(void*)(uintptr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2]);
}

inline void riscv_cvtstat(riscv_abi_stat *abi_stat, struct stat *host_stat)
{
	abi_stat->_dev        = host_stat->st_dev;
	abi_stat->_ino        = host_stat->st_ino;
	abi_stat->_mode       = host_stat->st_mode;
	abi_stat->_nlink      = host_stat->st_nlink;
	abi_stat->_uid        = host_stat->st_uid;
	abi_stat->_gid        = host_stat->st_gid;
	abi_stat->_rdev       = host_stat->st_rdev;
	abi_stat->_size       = host_stat->st_size;
	abi_stat->_blocks     = host_stat->st_blocks;
	abi_stat->_blksize    = host_stat->st_blksize;
#if defined (__APPLE__)
	abi_stat->_atime      = host_stat->st_atimespec.tv_sec;
	abi_stat->_atime_nsec = host_stat->st_atimespec.tv_nsec;
	abi_stat->_mtime      = host_stat->st_mtimespec.tv_sec;
	abi_stat->_mtime_nsec = host_stat->st_mtimespec.tv_nsec;
	abi_stat->_ctime      = host_stat->st_ctimespec.tv_sec;
	abi_stat->_ctime_nsec = host_stat->st_ctimespec.tv_nsec;
#else
	abi_stat->_atime      = host_stat->st_atim.tv_sec;
	abi_stat->_atime_nsec = host_stat->st_atim.tv_nsec;
	abi_stat->_mtime      = host_stat->st_mtim.tv_sec;
	abi_stat->_mtime_nsec = host_stat->st_mtim.tv_nsec;
	abi_stat->_ctime      = host_stat->st_ctim.tv_sec;
	abi_stat->_ctime_nsec = host_stat->st_ctim.tv_nsec;
#endif
}

template <typename P> void riscv_sys_fstat(P &proc)
{
	struct stat host_stat;
	memset(&host_stat, 0, sizeof(host_stat));
	if ((proc.ireg[riscv_ireg_a0] = fstat(proc.ireg[riscv_ireg_a0], &host_stat)) == 0) {
		riscv_abi_stat *abi_stat = (riscv_abi_stat*)proc.ireg[riscv_ireg_a1].r.xu.val;
		riscv_cvtstat(abi_stat, &host_stat);
	}
}

template <typename P> void riscv_sys_exit(P &proc)
{
	exit(proc.ireg[riscv_ireg_a0]);
}

template <typename P> void riscv_sys_brk(P &proc)
{
	static const uintptr_t page_size = 4096;

	// calculate the new heap address rounded up to the nearest page
	uintptr_t new_addr = proc.ireg[riscv_ireg_a0];
	uintptr_t curr_heap_end = (proc.heap_end + page_size - 1) & ~(page_size-1);
	uintptr_t new_heap_end = (new_addr + page_size - 1) & ~(page_size-1);

	// return if the heap is already big enough
	if (proc.heap_end >= new_heap_end || new_heap_end == curr_heap_end) {
		proc.ireg[riscv_ireg_a0] = new_addr;
		return;
	}

	// map a new heap segment
	void *addr = mmap((void*)curr_heap_end, new_heap_end - curr_heap_end,
		PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
	if (addr == MAP_FAILED) {
		debug("brk: error: mmap: %s", strerror(errno));
		proc.ireg[riscv_ireg_a0] = -ENOMEM;
	} else {
		// keep track of the mapped segment and set the new heap_end
		proc.mapped_segments.push_back(std::pair<void*,size_t>((void*)curr_heap_end, new_heap_end - curr_heap_end));
		proc.heap_end = new_heap_end;
		if (proc.flags & riscv_processor_flag_emulator_debug) {
			debug("brk: mmap: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
				curr_heap_end, new_heap_end);
		}
		proc.ireg[riscv_ireg_a0] = new_addr;
	}
}

#endif