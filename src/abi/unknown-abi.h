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
		abi_syscall_fstatat = 79,
		abi_syscall_fstat = 80,
		abi_syscall_exit = 93,
		abi_syscall_exit_group = 94,
		abi_syscall_set_tid_address = 96,
		abi_syscall_clock_gettime = 113,
		abi_syscall_uname = 160,
		abi_syscall_getrusage = 165,
		abi_syscall_gettimeofday = 169,
		abi_syscall_brk = 214,
		abi_syscall_munmap = 215,
		abi_syscall_mmap = 222,
		abi_syscall_mprotect = 226,
		abi_syscall_madvise = 233,
		abi_syscall_prlimit64 = 261,
		abi_syscall_open = 1024,
		abi_syscall_unlink = 1026,
		abi_syscall_stat = 1038,
		abi_syscall_chown = 1039,
	};

	enum {
		abi_mmap_PROT_READ = 1,
		abi_mmap_PROT_WRITE = 2,
		abi_mmap_PROT_EXEC = 4
	};

	enum {
		abi_mmap_MAP_SHARED = 0x1,
		abi_mmap_MAP_PRIVATE = 0x2,
		abi_mmap_MAP_FIXED = 0x10,
		abi_mmap_MAP_ANON = 0x20
	};

	enum {
		abi_ioctl_TIOCGWINSZ = 0x5413
	};

	enum {
		abi_errno_EINVAL = 22
	};

	enum {
		abi_clock_CLOCK_REALTIME = 0,
		abi_clock_CLOCK_MONOTONIC = 1
	};

	enum {
		abi_utsname_NEW_UTS_LEN = 64
	};

	enum {
		abi_fstatat_AT_SYMLINK_NOFOLLOW = 0x100
	};

	enum {
		abi_rusage_RUSAGE_SELF = 0,
		abi_rusage_RUSAGE_CHILDREN = -1
	};

	enum {
		abi_rlimit_RLIMIT_CPU = 0,
		abi_rlimit_RLIMIT_FSIZE = 1,
		abi_rlimit_RLIMIT_DATA = 2,
		abi_rlimit_RLIMIT_STACK = 3,
		abi_rlimit_RLIMIT_CORE = 4,
		abi_rlimit_RLIMIT_RSS = 5,
		abi_rlimit_RLIMIT_NPROC = 6,
		abi_rlimit_RLIMIT_NOFILE = 7,
		abi_rlimit_RLIMIT_MEMLOCK = 8,
		abi_rlimit_RLIMIT_AS = 9,
		abi_rlimit_RLIMIT_LOCKS = 10,
		abi_rlimit_RLIMIT_SIGPENDING = 11,
		abi_rlimit_RLIMIT_MSGQUEUE = 12,
		abi_rlimit_RLIMIT_NICE = 13,
		abi_rlimit_RLIMIT_RTPRIO = 14,
		abi_rlimit_RLIMIT_NLIMITS = 15
	};

	struct abi_new_utsname {
		char sysname[abi_utsname_NEW_UTS_LEN + 1];
		char nodename[abi_utsname_NEW_UTS_LEN + 1];
		char release[abi_utsname_NEW_UTS_LEN + 1];
		char version[abi_utsname_NEW_UTS_LEN + 1];
		char machine[abi_utsname_NEW_UTS_LEN + 1];
		char domainname[abi_utsname_NEW_UTS_LEN + 1];
	};

	struct abi_winsize {
		unsigned short ws_row;
		unsigned short ws_col;
		unsigned short ws_xpixel;
		unsigned short ws_ypixel;
	};

	template <typename P> struct abi_iovec {
		typename P::ux iov_base;
		typename P::ux iov_len;
	};

	template <typename P> struct abi_timespec {
		typename P::long_t tv_sec;
		typename P::long_t tv_nsec;
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

	template <typename P> struct abi_rusage
	{
        abi_timeval<P> ru_utime;
        abi_timeval<P> ru_stime;
        typename P::long_t ru_maxrss;
        typename P::long_t ru_ixrss;
        typename P::long_t ru_idrss;
        typename P::long_t ru_isrss;
        typename P::long_t ru_minflt;
        typename P::long_t ru_majflt;
        typename P::long_t ru_nswap;
        typename P::long_t ru_inblock;
        typename P::long_t ru_oublock;
        typename P::long_t ru_msgsnd;
        typename P::long_t ru_msgrcv;
        typename P::long_t ru_nsignals;
        typename P::long_t ru_nvcsw;
        typename P::long_t ru_nivcsw;
        typename P::long_t __reserved[16];
	};

	template <typename P> struct abi_rlimit
	{
        u64 rlim_cur;
        u64 rlim_max;
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

	template <typename P> void abi_sys_fstatat(P &proc)
	{
		struct stat host_stat;
		const char* path = (const char*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		int abi_flags = (proc.ireg[rv_ireg_a3] == abi_fstatat_AT_SYMLINK_NOFOLLOW)
			? AT_SYMLINK_NOFOLLOW : 0;
		memset(&host_stat, 0, sizeof(host_stat));
		int ret = fstatat(proc.ireg[rv_ireg_a0], path, &host_stat, abi_flags);
		abi_stat<P> *guest_stat = (abi_stat<P>*)(addr_t)proc.ireg[rv_ireg_a2].r.xu.val;
		cvt_abi_stat(guest_stat, &host_stat);
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

	template <typename P> void abi_sys_clock_gettime(P &proc)
	{
		abi_timespec<P> *abi_ts = (abi_timespec<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		if (abi_ts) {
			uint64_t t = host_cpu::get_instance().get_time_ns();
			abi_ts->tv_sec = (typename P::long_t)(t / 1000000000);
			abi_ts->tv_nsec = (typename P::long_t)(t % 1000000000);
			proc.ireg[rv_ireg_a0] = 0;
		} else {
			proc.ireg[rv_ireg_a0] = -EFAULT;
		}
	}

	template <typename P> void abi_sys_uname(P &proc)
	{
		abi_new_utsname *ustname = (abi_new_utsname*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		struct utsname host_utsname;
		int ret = uname(&host_utsname);
		strncpy(ustname->sysname, host_utsname.sysname, abi_utsname_NEW_UTS_LEN);
		strncpy(ustname->nodename, host_utsname.nodename, abi_utsname_NEW_UTS_LEN);
		strncpy(ustname->release, host_utsname.release, abi_utsname_NEW_UTS_LEN);
		strncpy(ustname->version, host_utsname.version, abi_utsname_NEW_UTS_LEN);
		strncpy(ustname->machine, host_utsname.machine, abi_utsname_NEW_UTS_LEN);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_getrusage(P &proc)
	{
		struct rusage host_rusage;
		int who;
		switch (proc.ireg[rv_ireg_a0]) {
			case abi_rusage_RUSAGE_SELF: who = RUSAGE_SELF; break;
			case abi_rusage_RUSAGE_CHILDREN: who = RUSAGE_CHILDREN; break;
			default: who = 0; break;
		}
		int ret = getrusage(who, &host_rusage);
		abi_rusage<P> *guest_rusage = (abi_rusage<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		guest_rusage->ru_utime.tv_sec = host_rusage.ru_utime.tv_sec;
		guest_rusage->ru_utime.tv_usec = host_rusage.ru_utime.tv_usec;
		guest_rusage->ru_stime.tv_sec = host_rusage.ru_stime.tv_sec;
		guest_rusage->ru_stime.tv_usec = host_rusage.ru_stime.tv_usec;
		guest_rusage->ru_maxrss = host_rusage.ru_maxrss;
		guest_rusage->ru_ixrss = host_rusage.ru_ixrss;
		guest_rusage->ru_idrss = host_rusage.ru_idrss;
		guest_rusage->ru_isrss = host_rusage.ru_isrss;
		guest_rusage->ru_minflt = host_rusage.ru_minflt;
		guest_rusage->ru_majflt = host_rusage.ru_majflt;
		guest_rusage->ru_nswap = host_rusage.ru_nswap;
		guest_rusage->ru_inblock = host_rusage.ru_inblock;
		guest_rusage->ru_oublock = host_rusage.ru_oublock;
		guest_rusage->ru_msgsnd = host_rusage.ru_msgsnd;
		guest_rusage->ru_msgrcv = host_rusage.ru_msgrcv;
		guest_rusage->ru_nsignals = host_rusage.ru_nsignals;
		guest_rusage->ru_nvcsw = host_rusage.ru_nvcsw;
		guest_rusage->ru_nivcsw = host_rusage.ru_nivcsw;
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
		int ret = guest_munmap((void*)(uintptr_t)proc.ireg[rv_ireg_a0], proc.ireg[rv_ireg_a1]);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_mmap(P &proc)
	{
		int prot = 0, flags = 0;
		int abi_prot = proc.ireg[rv_ireg_a2], abi_flags = proc.ireg[rv_ireg_a3];
		prot  |= (abi_prot  & abi_mmap_PROT_READ)   ? PROT_READ   : 0;
		prot  |= (abi_prot  & abi_mmap_PROT_WRITE)  ? PROT_WRITE  : 0;
		prot  |= (abi_prot  & abi_mmap_PROT_EXEC)   ? PROT_EXEC   : 0;
		flags |= (abi_flags & abi_mmap_MAP_SHARED)  ? MAP_SHARED  : 0;
		flags |= (abi_flags & abi_mmap_MAP_PRIVATE) ? MAP_PRIVATE : 0;
		flags |= (abi_flags & abi_mmap_MAP_FIXED)   ? MAP_FIXED   : 0;
		flags |= (abi_flags & abi_mmap_MAP_ANON)    ? MAP_ANON    : 0;
		proc.ireg[rv_ireg_a0].r.xu.val = (uintptr_t)guest_mmap(
			(void*)(uintptr_t)proc.ireg[rv_ireg_a0], proc.ireg[rv_ireg_a1],
			prot, flags, proc.ireg[rv_ireg_a4], proc.ireg[rv_ireg_a5]);
	}

	template <typename P> void abi_sys_mprotect(P &proc)
	{
		int prot = 0;
		int abi_prot = proc.ireg[rv_ireg_a2];
		prot  |= (abi_prot  & abi_mmap_PROT_READ)   ? PROT_READ   : 0;
		prot  |= (abi_prot  & abi_mmap_PROT_WRITE)  ? PROT_WRITE  : 0;
		prot  |= (abi_prot  & abi_mmap_PROT_EXEC)   ? PROT_EXEC   : 0;
		int ret = mprotect((void*)(uintptr_t)proc.ireg[rv_ireg_a0],
			proc.ireg[rv_ireg_a1], abi_prot);
		proc.ireg[rv_ireg_a0] = ret >= 0 ? ret : -errno;
	}

	template <typename P> void abi_sys_madvise(P &proc)
	{
		proc.ireg[rv_ireg_a0] = 0; /* nop */
	}

	template <typename P> void abi_sys_prlimit64(P &proc)
	{
		int resource, ret;
		switch (proc.ireg[rv_ireg_a0]) {
			case abi_rlimit_RLIMIT_CPU: resource = RLIMIT_CPU;
			case abi_rlimit_RLIMIT_FSIZE: resource = RLIMIT_FSIZE;
			case abi_rlimit_RLIMIT_DATA: resource = RLIMIT_DATA;
			case abi_rlimit_RLIMIT_STACK: resource = RLIMIT_STACK;
			case abi_rlimit_RLIMIT_CORE: resource = RLIMIT_CORE;
			case abi_rlimit_RLIMIT_RSS: resource = RLIMIT_RSS;
			case abi_rlimit_RLIMIT_NPROC: resource = RLIMIT_NPROC;
			case abi_rlimit_RLIMIT_NOFILE: resource = RLIMIT_NOFILE;
			case abi_rlimit_RLIMIT_MEMLOCK: resource = RLIMIT_MEMLOCK;
			case abi_rlimit_RLIMIT_AS: resource = RLIMIT_AS;
			default:
				proc.ireg[rv_ireg_a0] = abi_errno_EINVAL;
				return;
		}
		abi_rlimit<P> *guest_new_limit = (abi_rlimit<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		abi_rlimit<P> *guest_old_limit = (abi_rlimit<P>*)(addr_t)proc.ireg[rv_ireg_a2].r.xu.val;
		struct rlimit host_new_limit;
		struct rlimit host_old_limit;
		if (guest_old_limit) {
			ret = getrlimit(resource, &host_old_limit);
			if (ret) {
				proc.ireg[rv_ireg_a0] = -errno;
				return;
			}
			guest_old_limit->rlim_cur = host_old_limit.rlim_cur;
			guest_old_limit->rlim_max = host_old_limit.rlim_max;
		}
		if (guest_new_limit) {
			host_new_limit.rlim_cur = guest_new_limit->rlim_cur;
			host_new_limit.rlim_max = guest_new_limit->rlim_max;
			ret = setrlimit(resource, &host_new_limit);
			if (ret) {
				proc.ireg[rv_ireg_a0] = -errno;
				return;
			}
		}
		proc.ireg[rv_ireg_a0] = 0;
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
			case abi_syscall_fstatat:         abi_sys_fstatat(proc); break;
			case abi_syscall_fstat:           abi_sys_fstat(proc); break;
			case abi_syscall_exit:            abi_sys_exit(proc); break;
			case abi_syscall_exit_group:      abi_sys_exit(proc); break;
			case abi_syscall_set_tid_address: abi_sys_set_tid_address(proc); break;
			case abi_syscall_clock_gettime:   abi_sys_clock_gettime(proc); break;
			case abi_syscall_uname:           abi_sys_uname(proc); break;
			case abi_syscall_getrusage:       abi_sys_getrusage(proc); break;
			case abi_syscall_gettimeofday:    abi_sys_gettimeofday(proc);break;
			case abi_syscall_brk:             abi_sys_brk(proc); break;
			case abi_syscall_munmap:          abi_sys_munmap(proc); break;
			case abi_syscall_mmap:            abi_sys_mmap(proc); break;
			case abi_syscall_mprotect:        abi_sys_mprotect(proc); break;
			case abi_syscall_madvise:         abi_sys_madvise(proc); break;
			case abi_syscall_prlimit64:       abi_sys_prlimit64(proc); break;
			case abi_syscall_open:            abi_sys_open(proc); break;
			case abi_syscall_unlink:          abi_sys_unlink(proc); break;
			case abi_syscall_stat:            abi_sys_stat(proc); break;
			case abi_syscall_chown:           abi_sys_chown(proc); break;
			default: panic("unknown syscall: %d", proc.ireg[rv_ireg_a7].r.xu.val);
		}
	}

}

#endif
