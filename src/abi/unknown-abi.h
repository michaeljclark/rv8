//
//  proxy.h
//

#ifndef rv_proxy_h
#define rv_proxy_h

namespace riscv {

	enum abi_syscall
	{
		abi_syscall_ioctl = 29,
		abi_syscall_openat = 56,
		abi_syscall_close = 57,
		abi_syscall_lseek = 62,
		abi_syscall_read = 63,
		abi_syscall_write = 64,
		abi_syscall_readv = 65,
		abi_syscall_writev = 66,
		abi_syscall_pread = 67,
		abi_syscall_pwrite = 68,
		abi_syscall_fstat = 80,
		abi_syscall_exit = 93,
		abi_syscall_exit_group = 94,
		abi_syscall_set_tid_address = 96,
		abi_syscall_uname = 160,
		abi_syscall_gettimeofday = 169,
		abi_syscall_brk = 214,
		abi_syscall_munmap = 215,
		abi_syscall_mmap = 222,
		abi_syscall_open = 1024,
		abi_syscall_unlink = 1026,
		abi_syscall_stat = 1038,
		abi_syscall_chown = 1039,
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

	const int NEW_UTS_LEN = 64;

	struct abi_new_utsname {
		char sysname[NEW_UTS_LEN + 1];
		char nodename[NEW_UTS_LEN + 1];
		char release[NEW_UTS_LEN + 1];
		char version[NEW_UTS_LEN + 1];
		char machine[NEW_UTS_LEN + 1];
		char domainname[NEW_UTS_LEN + 1];
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

	enum {
		abi_ioctl_TIOCGWINSZ = 0x5413
	};

	enum {
		abi_errno_EINVAL = 22
	};

	struct abi_winsize {
		unsigned short ws_row;
		unsigned short ws_col;
		unsigned short ws_xpixel;
		unsigned short ws_ypixel;
	};

	template <typename P>
	struct abi_iovec {
		typename P::ux iov_base;
		typename P::ux iov_len;
	};

	template <typename P> void abi_sys_ioctl(P &proc)
	{
		switch (proc.ireg[rv_ireg_a1]) {
			case abi_ioctl_TIOCGWINSZ: {
				struct winsize wsz;
				int ret = ioctl(proc.ireg[rv_ireg_a0], TIOCGWINSZ, &wsz);
				if (!ret) {
					abi_winsize *abi_wsz = (abi_winsize*)(uintptr_t)proc.ireg[rv_ireg_a2].r.xu.val;
					abi_wsz->ws_row = wsz.ws_row;
					abi_wsz->ws_col = wsz.ws_col;
					abi_wsz->ws_xpixel = wsz.ws_xpixel;
					abi_wsz->ws_ypixel = wsz.ws_ypixel;
				}
				proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
				break;
			}
			default:
				proc.ireg[rv_ireg_a0] = -abi_errno_EINVAL;
				break;
		}
	}

	template <typename P> void abi_sys_openat(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		int ret = openat(proc.ireg[rv_ireg_a0], pathname, proc.ireg[rv_ireg_a2], proc.ireg[rv_ireg_a3]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_close(P &proc)
	{
		int ret = close(proc.ireg[rv_ireg_a0]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_lseek(P &proc)
	{
		int ret = lseek(proc.ireg[rv_ireg_a0],
			proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_read(P &proc)
	{
		int ret = read(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_write(P &proc)
	{
		int ret = write(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_readv(P &proc)
	{
		int fd = proc.ireg[rv_ireg_a0];
		int iovcnt = proc.ireg[rv_ireg_a2];
		struct iovec *host_iov = (struct iovec*)alloca(sizeof(struct iovec) * iovcnt);
		struct abi_iovec<P> *abi_iov = (abi_iovec<P>*)(addr_t)proc.ireg[rv_ireg_a1];
		for (int i = 0; i < iovcnt; i++) {
			host_iov[i].iov_base = (void*)(addr_t)abi_iov[i].iov_base;
			host_iov[i].iov_len = (size_t)abi_iov[i].iov_len;
		}
		int ret = readv(fd, host_iov, iovcnt);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_writev(P &proc)
	{
		int fd = proc.ireg[rv_ireg_a0];
		int iovcnt = proc.ireg[rv_ireg_a2];
		struct iovec *host_iov = (struct iovec*)alloca(sizeof(struct iovec) * iovcnt);
		struct abi_iovec<P> *abi_iov = (abi_iovec<P>*)(addr_t)proc.ireg[rv_ireg_a1];
		for (int i = 0; i < iovcnt; i++) {
			host_iov[i].iov_base = (void*)(addr_t)abi_iov[i].iov_base;
			host_iov[i].iov_len = (size_t)abi_iov[i].iov_len;
		}
		int ret = writev(fd, host_iov, iovcnt);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_pread(P &proc)
	{
		int ret = pread(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2],
			proc.ireg[rv_ireg_a3]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_pwrite(P &proc)
	{
		int ret = pwrite(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2],
			proc.ireg[rv_ireg_a3]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_fstat(P &proc)
	{
		struct stat host_stat;
		memset(&host_stat, 0, sizeof(host_stat));
		int ret = fstat(proc.ireg[rv_ireg_a0], &host_stat);
		abi_stat<P> *guest_stat = (abi_stat<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		cvt_abi_stat(guest_stat, &host_stat);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_open(P &proc)
	{
		int lxflags = proc.ireg[rv_ireg_a1], hostflags = 0;
		if (lxflags & 01) hostflags |= O_WRONLY;
		if (lxflags & 02) hostflags |= O_RDWR;
		if (lxflags & 0100) hostflags |= O_CREAT;
		if (lxflags & 0200) hostflags |= O_EXCL;
		if (lxflags & 01000) hostflags |= O_TRUNC;
		if (lxflags & 02000) hostflags |= O_APPEND;
		if (lxflags & 04000) hostflags |= O_NONBLOCK;
		if (lxflags & 04010000) hostflags |= O_SYNC;
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		int ret = open(pathname, hostflags, proc.ireg[rv_ireg_a2].r.xu.val);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_unlink(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		int ret = unlink(pathname);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_stat(P &proc)
	{
		struct stat host_stat;
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		memset(&host_stat, 0, sizeof(host_stat));
		int ret = stat(pathname, &host_stat);
		abi_stat<P> *guest_stat = (abi_stat<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		cvt_abi_stat(guest_stat, &host_stat);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_chown(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		int ret = chown(pathname, (uid_t)proc.ireg[rv_ireg_a1].r.xu.val, (gid_t)proc.ireg[rv_ireg_a2].r.xu.val);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_exit(P &proc)
	{
		proc.exit(proc.ireg[rv_ireg_a0]);
		exit(proc.ireg[rv_ireg_a0]);
	}

	template <typename P> void abi_sys_set_tid_address(P &proc)
	{
		proc.clear_child_tid = *(int*)(uintptr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		proc.ireg[rv_ireg_a0].r.xu.val = 1; /* tid is 1 for now */
	}

	template <typename P> void abi_sys_uname(P &proc)
	{
		abi_new_utsname *ustname = (abi_new_utsname*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		struct utsname host_utsname;
		int ret = uname(&host_utsname);
		strncpy(ustname->sysname, host_utsname.sysname, NEW_UTS_LEN);
		strncpy(ustname->nodename, host_utsname.nodename, NEW_UTS_LEN);
		strncpy(ustname->release, host_utsname.release, NEW_UTS_LEN);
		strncpy(ustname->version, host_utsname.version, NEW_UTS_LEN);
		strncpy(ustname->machine, host_utsname.machine, NEW_UTS_LEN);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_gettimeofday(P &proc)
	{
		struct timeval host_tp;
		struct timezone host_tzp;
		memset(&host_tp, 0, sizeof(host_tp));
		memset(&host_tzp, 0, sizeof(host_tzp));
		int ret =  gettimeofday(&host_tp, &host_tzp);
		if (proc.ireg[rv_ireg_a0].r.xu.val != 0) {
			abi_timeval<P> *guest_tp = (abi_timeval<P>*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
			guest_tp->tv_sec = host_tp.tv_sec;
			guest_tp->tv_usec = host_tp.tv_usec;
		}
		if (proc.ireg[rv_ireg_a1].r.xu.val != 0) {
			abi_timezone<P> *guest_tzp = (abi_timezone<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
			guest_tzp->tz_minuteswest = host_tzp.tz_minuteswest;
			guest_tzp->tz_dsttime = host_tzp.tz_dsttime;
		}
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_brk(P &proc)
	{
		// calculate the new heap address rounded up to the nearest page
		addr_t new_brk = proc.ireg[rv_ireg_a0];
		addr_t new_heap_end = round_up(new_brk, page_size);

		if (proc.log & proc_log_memory) {
			debug("sys_brk: brk: %llx begin: %llx end: %llx",
				proc.mmu.mem->brk, proc.mmu.mem->heap_begin, proc.mmu.mem->heap_end);
		}

		/* return current brk */
		if (new_brk == 0) {
			proc.ireg[rv_ireg_a0] = proc.mmu.mem->brk;
			return;
		}

		/* shrink brk */
		if (new_brk <= proc.mmu.mem->heap_end) {
			proc.ireg[rv_ireg_a0] = proc.mmu.mem->brk = new_brk;
			return;
		}

		/* map a new heap segment */
		void *addr = mmap((void*)proc.mmu.mem->heap_end, new_heap_end - proc.mmu.mem->heap_end,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			debug("sys_brk: error: mmap: %s", strerror(errno));
			proc.ireg[rv_ireg_a0] = -ENOMEM;
		} else {
			// keep track of the mapped segment and set the new heap_end
			proc.mmu.mem->segments.push_back(std::pair<void*,size_t>((void*)proc.mmu.mem->heap_end,
				new_heap_end - proc.mmu.mem->heap_end));
			if (proc.log & proc_log_memory) {
				debug("mmap-brk :%016llx-%016llx +R+W",
					proc.mmu.mem->heap_end, new_heap_end);
			}
			proc.mmu.mem->heap_end = new_heap_end;
			proc.ireg[rv_ireg_a0] = proc.mmu.mem->brk = new_brk;
		}
	}

	template <typename P> void abi_sys_munmap(P &proc)
	{
		int ret = munmap((void*)(uintptr_t)proc.ireg[rv_ireg_a0], proc.ireg[rv_ireg_a1]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_mmap(P &proc)
	{
		proc.ireg[rv_ireg_a0].r.xu.val = (uintptr_t)mmap(
			(void*)(uintptr_t)proc.ireg[rv_ireg_a0], proc.ireg[rv_ireg_a1],
			proc.ireg[rv_ireg_a2], proc.ireg[rv_ireg_a3],
			proc.ireg[rv_ireg_a4], proc.ireg[rv_ireg_a5]);
	}

	template <typename P> void proxy_syscall(P &proc)
	{
		switch (proc.ireg[rv_ireg_a7]) {
			case abi_syscall_ioctl:           abi_sys_ioctl(proc); break;
			case abi_syscall_openat:          abi_sys_openat(proc); break;
			case abi_syscall_close:           abi_sys_close(proc); break;
			case abi_syscall_lseek:           abi_sys_lseek(proc); break;
			case abi_syscall_read:            abi_sys_read(proc);  break;
			case abi_syscall_write:           abi_sys_write(proc); break;
			case abi_syscall_readv:           abi_sys_readv(proc);  break;
			case abi_syscall_writev:          abi_sys_writev(proc); break;
			case abi_syscall_pread:           abi_sys_pread(proc); break;
			case abi_syscall_pwrite:          abi_sys_pwrite(proc); break;
			case abi_syscall_fstat:           abi_sys_fstat(proc); break;
			case abi_syscall_exit:            abi_sys_exit(proc); break;
			case abi_syscall_exit_group:      abi_sys_exit(proc); break;
			case abi_syscall_set_tid_address: abi_sys_set_tid_address(proc); break;
			case abi_syscall_uname:           abi_sys_uname(proc); break;
			case abi_syscall_gettimeofday:    abi_sys_gettimeofday(proc);break;
			case abi_syscall_brk:             abi_sys_brk(proc); break;
			case abi_syscall_munmap:          abi_sys_munmap(proc); break;
			case abi_syscall_mmap:            abi_sys_mmap(proc); break;
			case abi_syscall_open:            abi_sys_open(proc); break;
			case abi_syscall_unlink:          abi_sys_unlink(proc); break;
			case abi_syscall_stat:            abi_sys_stat(proc); break;
			case abi_syscall_chown:           abi_sys_chown(proc); break;
			default: panic("unknown syscall: %d", proc.ireg[rv_ireg_a7].r.xu.val);
		}
	}

}

#endif
