//
//  riscv-proxy.h
//

#ifndef riscv_proxy_h
#define riscv_proxy_h

namespace riscv {

	struct mmu_proxy
	{
		std::vector<std::pair<void*,size_t>> segments;
		uintptr_t heap_begin;
		uintptr_t heap_end;

		mmu_proxy() : segments(), heap_begin(0), heap_end(0) {}
	};

	enum abi_syscall
	{
		abi_syscall_close = 57,
		abi_syscall_write = 64,
		abi_syscall_fstat = 80,
		abi_syscall_exit = 93,
		abi_syscall_brk = 214,
	};

	template <typename P> struct abi_stat
	{
		typename P::ulong_t dev;
		typename P::ulong_t ino;
		typename P::uint_t  mode;
		typename P::uint_t  nlink;
		typename P::uint_t  uid;
		typename P::uint_t  gid;
		typename P::ulong_t rdev;
		typename P::ulong_t __pad1;
		typename P::long_t  size;
		typename P::int_t   blksize;
		typename P::int_t   __pad2;
		typename P::long_t  blocks;
		typename P::long_t  atime;
		typename P::ulong_t atime_nsec;
		typename P::long_t  mtime;
		typename P::ulong_t mtime_nsec;
		typename P::long_t  ctime;
		typename P::ulong_t ctime_nsec;
		typename P::uint_t  __unused4;
		typename P::uint_t  __unused5;
	};

	template <typename P>
	void cvt_abi_stat(abi_stat<P> *guest_stat, struct stat *host_stat)
	{
		guest_stat->dev        = host_stat->st_dev;
		guest_stat->ino        = host_stat->st_ino;
		guest_stat->mode       = host_stat->st_mode;
		guest_stat->nlink      = host_stat->st_nlink;
		guest_stat->uid        = host_stat->st_uid;
		guest_stat->gid        = host_stat->st_gid;
		guest_stat->rdev       = host_stat->st_rdev;
		guest_stat->size       = host_stat->st_size;
		guest_stat->blocks     = host_stat->st_blocks;
		guest_stat->blksize    = host_stat->st_blksize;
	#if defined (__APPLE__)
		guest_stat->atime      = host_stat->st_atimespec.tv_sec;
		guest_stat->atime_nsec = host_stat->st_atimespec.tv_nsec;
		guest_stat->mtime      = host_stat->st_mtimespec.tv_sec;
		guest_stat->mtime_nsec = host_stat->st_mtimespec.tv_nsec;
		guest_stat->ctime      = host_stat->st_ctimespec.tv_sec;
		guest_stat->ctime_nsec = host_stat->st_ctimespec.tv_nsec;
	#else
		guest_stat->atime      = host_stat->st_atim.tv_sec;
		guest_stat->atime_nsec = host_stat->st_atim.tv_nsec;
		guest_stat->mtime      = host_stat->st_mtim.tv_sec;
		guest_stat->mtime_nsec = host_stat->st_mtim.tv_nsec;
		guest_stat->ctime      = host_stat->st_ctim.tv_sec;
		guest_stat->ctime_nsec = host_stat->st_ctim.tv_nsec;
	#endif
	}

	template <typename P> void abi_sys_close(P &proc)
	{
		proc.ireg[riscv_ireg_a0] = close(proc.ireg[riscv_ireg_a0]);
	}

	template <typename P> void abi_sys_write(P &proc)
	{
		proc.ireg[riscv_ireg_a0] = write(proc.ireg[riscv_ireg_a0],
			(void*)(uintptr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2]);
	}

	template <typename P> void abi_sys_fstat(P &proc)
	{
		struct stat host_stat;
		memset(&host_stat, 0, sizeof(host_stat));
		if ((proc.ireg[riscv_ireg_a0] = fstat(proc.ireg[riscv_ireg_a0], &host_stat)) == 0) {
			abi_stat<P> *guest_stat = (abi_stat<P>*)(uintptr_t)proc.ireg[riscv_ireg_a1].r.xu.val;
			cvt_abi_stat(guest_stat, &host_stat);
		}
	}

	template <typename P> void abi_sys_exit(P &proc)
	{
		exit(proc.ireg[riscv_ireg_a0]);
	}

	template <typename P> void abi_sys_brk(P &proc)
	{
		// calculate the new heap address rounded up to the nearest page
		uintptr_t new_addr = proc.ireg[riscv_ireg_a0];
		uintptr_t curr_heap_end = round_up(proc.mmu.heap_end, page_size);
		uintptr_t new_heap_end = round_up(new_addr, page_size);

		// return if the heap is already big enough
		if (proc.mmu.heap_end >= new_heap_end || new_heap_end == curr_heap_end) {
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
			proc.mmu.segments.push_back(std::pair<void*,size_t>((void*)curr_heap_end, new_heap_end - curr_heap_end));
			proc.mmu.heap_end = new_heap_end;
			if (proc.flags & processor_flag_emulator_debug) {
				debug("brk: mmap: 0x%016" PRIxPTR " - 0x%016" PRIxPTR " +R+W",
					curr_heap_end, new_heap_end);
			}
			proc.ireg[riscv_ireg_a0] = new_addr;
		}
	}

	template <typename P> void proxy_syscall(P &proc)
	{
		switch (proc.ireg[riscv_ireg_a7]) {
			case abi_syscall_close:  abi_sys_close(proc); break;
			case abi_syscall_write:  abi_sys_write(proc); break;
			case abi_syscall_fstat:  abi_sys_fstat(proc); break;
			case abi_syscall_exit:   abi_sys_exit(proc);  break;
			case abi_syscall_brk:    abi_sys_brk(proc);   break;
			default: panic("unknown syscall: %d", proc.ireg[riscv_ireg_a7]);
		}
	}

}

#endif
