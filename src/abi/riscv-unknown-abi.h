//
//  riscv-proxy.h
//

#ifndef riscv_proxy_h
#define riscv_proxy_h

namespace riscv {

	template <typename UX>
	struct proxy_memory
	{
		std::vector<std::pair<void*,size_t>> segments;
		addr_t heap_begin;
		addr_t heap_end;

		proxy_memory() : segments(), heap_begin(0), heap_end(0) {}
	};

	template <typename UX, typename MEMORY = proxy_memory<UX>>
	struct mmu_proxy
	{
		/*
		 * Define top of emulator address space, beginning of emulator text.
		 * Note: due to memory model limitations the executable is linked below 2GB.
		 *
		 * MACOS_LDFLAGS = -Wl,-pagezero_size,0x1000 -Wl,-no_pie -image_base 0x40000000
		 * LINUX_LDFLAGS = -Wl,--section-start=.text=0x40000000 -static
		 */

		enum : addr_t {
			memory_top = 0x40000000
		};

		MEMORY mem;

		template <typename P> inst_t inst_fetch(P &proc, UX pc, addr_t &pc_offset)
		{
			return riscv::inst_fetch(pc, pc_offset);
		}

		/* Note: in this simple proxy MMU model, stores beyond memory top wrap */

		template <typename P, typename T> void load(P &proc, UX va, T &val)
		{
			val = UX(*(T*)addr_t(va & (memory_top - 1)));
		}

		template <typename P, typename T> void store(P &proc, UX va, T val)
		{
			*((T*)addr_t(va & (memory_top - 1))) = val;
		}
	};

	using mmu_proxy_rv32 = mmu_proxy<u32>;
	using mmu_proxy_rv64 = mmu_proxy<u64>;

	enum abi_syscall
	{
		abi_syscall_close = 57,
		abi_syscall_lseek = 62,
		abi_syscall_read = 63,
		abi_syscall_write = 64,
		abi_syscall_pread = 67,
		abi_syscall_pwrite = 68,
		abi_syscall_fstat = 80,
		abi_syscall_exit = 93,
		abi_syscall_gettimeofday = 169,
		abi_syscall_brk = 214,
	};

	template <typename P> struct abi_timeval {
		typename P::long_t tv_sec;
		typename P::long_t tv_usec;
	};

	template <typename P> struct abi_timezone {
		typename P::int_t tz_minuteswest;
		typename P::int_t tz_dsttime;
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

	template <typename P> void abi_sys_lseek(P &proc)
	{
		proc.ireg[riscv_ireg_a0] = lseek(proc.ireg[riscv_ireg_a0],
			proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2]);
	}

	template <typename P> void abi_sys_read(P &proc)
	{
		proc.ireg[riscv_ireg_a0] = read(proc.ireg[riscv_ireg_a0],
			(void*)(addr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2]);
	}

	template <typename P> void abi_sys_write(P &proc)
	{
		proc.ireg[riscv_ireg_a0] = write(proc.ireg[riscv_ireg_a0],
			(void*)(addr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2]);
	}

	template <typename P> void abi_sys_pread(P &proc)
	{
		proc.ireg[riscv_ireg_a0] = pread(proc.ireg[riscv_ireg_a0],
			(void*)(addr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2],
			proc.ireg[riscv_ireg_a3]);
	}

	template <typename P> void abi_sys_pwrite(P &proc)
	{
		proc.ireg[riscv_ireg_a0] = pwrite(proc.ireg[riscv_ireg_a0],
			(void*)(addr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2],
			proc.ireg[riscv_ireg_a3]);
	}

	template <typename P> void abi_sys_fstat(P &proc)
	{
		struct stat host_stat;
		memset(&host_stat, 0, sizeof(host_stat));
		if ((proc.ireg[riscv_ireg_a0] = fstat(proc.ireg[riscv_ireg_a0], &host_stat)) == 0) {
			abi_stat<P> *guest_stat = (abi_stat<P>*)(addr_t)proc.ireg[riscv_ireg_a1].r.xu.val;
			cvt_abi_stat(guest_stat, &host_stat);
		}
	}

	template <typename P> void abi_sys_exit(P &proc)
	{
		exit(proc.ireg[riscv_ireg_a0]);
	}

	template <typename P> void abi_sys_gettimeofday(P &proc)
	{
		struct timeval host_tp;
		struct timezone host_tzp;
		memset(&host_tp, 0, sizeof(host_tp));
		memset(&host_tzp, 0, sizeof(host_tzp));
		if ((proc.ireg[riscv_ireg_a0] = gettimeofday(&host_tp, &host_tzp)) == 0) {
			if (proc.ireg[riscv_ireg_a0].r.xu.val != 0) {
				abi_timeval<P> *guest_tp = (abi_timeval<P>*)(addr_t)proc.ireg[riscv_ireg_a0].r.xu.val;
				guest_tp->tv_sec = host_tp.tv_sec;
				guest_tp->tv_usec = host_tp.tv_usec;
			}
			if (proc.ireg[riscv_ireg_a1].r.xu.val != 0) {
				abi_timezone<P> *guest_tzp = (abi_timezone<P>*)(addr_t)proc.ireg[riscv_ireg_a1].r.xu.val;
				guest_tzp->tz_minuteswest = host_tzp.tz_minuteswest;
				guest_tzp->tz_dsttime = host_tzp.tz_dsttime;
			}
		}
	}

	template <typename P> void abi_sys_brk(P &proc)
	{
		// calculate the new heap address rounded up to the nearest page
		addr_t new_addr = proc.ireg[riscv_ireg_a0];
		addr_t curr_heap_end = round_up(proc.mmu.mem.heap_end, page_size);
		addr_t new_heap_end = round_up(new_addr, page_size);

		// return if the heap is already big enough
		if (proc.mmu.mem.heap_end >= new_heap_end || new_heap_end == curr_heap_end) {
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
			proc.mmu.mem.segments.push_back(std::pair<void*,size_t>((void*)curr_heap_end, new_heap_end - curr_heap_end));
			proc.mmu.mem.heap_end = new_heap_end;
			if (proc.log & proc_log_memory) {
				debug("mmap brk :%016" PRIxPTR "-%016" PRIxPTR " +R+W",
					curr_heap_end, new_heap_end);
			}
			proc.ireg[riscv_ireg_a0] = new_addr;
		}
	}

	template <typename P> void proxy_syscall(P &proc)
	{
		switch (proc.ireg[riscv_ireg_a7]) {
			case abi_syscall_close:         abi_sys_close(proc); break;
			case abi_syscall_lseek:         abi_sys_lseek(proc); break;
			case abi_syscall_read:          abi_sys_read(proc);  break;
			case abi_syscall_write:         abi_sys_write(proc); break;
			case abi_syscall_pread:         abi_sys_pread(proc); break;
			case abi_syscall_pwrite:        abi_sys_pwrite(proc); break;
			case abi_syscall_fstat:         abi_sys_fstat(proc); break;
			case abi_syscall_exit:          abi_sys_exit(proc); break;
			case abi_syscall_gettimeofday:  abi_sys_gettimeofday(proc);break;
			case abi_syscall_brk:           abi_sys_brk(proc); break;
			default: panic("unknown syscall: %d", proc.ireg[riscv_ireg_a7]);
		}
	}

}

#endif
