//
//  proxy.h
//

#ifndef rv_proxy_h
#define rv_proxy_h

namespace riscv {

	enum abi_syscall
	{
		abi_syscall_getcwd = 17,
		abi_syscall_fcntl = 25,
		abi_syscall_ioctl = 29,
		abi_syscall_unlinkat = 35,
		abi_syscall_faccessat = 48,
		abi_syscall_openat = 56,
		abi_syscall_close = 57,
		abi_syscall_lseek = 62,
		abi_syscall_read = 63,
		abi_syscall_write = 64,
		abi_syscall_readv = 65,
		abi_syscall_writev = 66,
		abi_syscall_pread = 67,
		abi_syscall_pwrite = 68,
		abi_syscall_readlinkat = 78,
		abi_syscall_fstatat = 79,
		abi_syscall_fstat = 80,
		abi_syscall_exit = 93,
		abi_syscall_exit_group = 94,
		abi_syscall_set_tid_address = 96,
		abi_syscall_clock_gettime = 113,
		abi_syscall_rt_sigaction = 134,
		abi_syscall_rt_sigprocmask = 135,
		abi_syscall_times = 153,
		abi_syscall_uname = 160,
		abi_syscall_getrusage = 165,
		abi_syscall_gettimeofday = 169,
		abi_syscall_gettid = 178,
		abi_syscall_sysinfo = 179,
		abi_syscall_brk = 214,
		abi_syscall_munmap = 215,
		abi_syscall_clone = 220,
		abi_syscall_execve = 221,
		abi_syscall_mmap = 222,
		abi_syscall_mprotect = 226,
		abi_syscall_madvise = 233,
		abi_syscall_wait4 = 260,
		abi_syscall_prlimit64 = 261,
		abi_syscall_open = 1024,
		abi_syscall_unlink = 1026,
		abi_syscall_stat = 1038,
		abi_syscall_chown = 1039,
	};

	enum
	{
		abi_clock_CLOCK_REALTIME = 0,
		abi_clock_CLOCK_MONOTONIC = 1,

		abi_errno_EPERM = 1,
		abi_errno_ENOENT= 2,
		abi_errno_ESRCH = 3,
		abi_errno_EINTR = 4,
		abi_errno_EIO = 5,
		abi_errno_ENXIO = 6,
		abi_errno_E2BIG = 7,
		abi_errno_ENOEXEC = 8,
		abi_errno_EBADF = 9,
		abi_errno_ECHILD = 10,
		abi_errno_EAGAIN = 11,
		abi_errno_ENOMEM = 12,
		abi_errno_EACCES = 13,
		abi_errno_EFAULT = 14,
		abi_errno_ENOTBLK = 15,
		abi_errno_EBUSY = 16,
		abi_errno_EEXIST = 17,
		abi_errno_EXDEV = 18,
		abi_errno_ENODEV = 19,
		abi_errno_ENOTDIR = 20,
		abi_errno_EISDIR = 21,
		abi_errno_EINVAL = 22,
		abi_errno_ENFILE = 23,
		abi_errno_EMFILE = 24,
		abi_errno_ENOTTY = 25,
		abi_errno_ETXTBSY = 26,
		abi_errno_EFBIG = 27,
		abi_errno_ENOSPC = 28,
		abi_errno_ESPIPE = 29,
		abi_errno_EROFS = 30,
		abi_errno_EMLINK = 31,
		abi_errno_EPIPE = 32,
		abi_errno_EDOM = 33,
		abi_errno_ERANGE = 34,
		abi_errno_EDEADLK = 35,
		abi_errno_ENAMETOOLONG = 36,
		abi_errno_ENOLCK = 37,
		abi_errno_ENOSYS = 38,

		abi_fcntl_F_DUPFD = 0,
		abi_fcntl_F_GETFD = 1,
		abi_fcntl_F_SETFD = 2,
		abi_fcntl_F_GETFL = 3,
		abi_fcntl_F_SETFL = 4,
		abi_fcntl_F_SETOWN = 8,
		abi_fcntl_F_GETOWN = 9,
		abi_fcntl_F_SETSIG = 10,
		abi_fcntl_F_GETSIG = 11,
		abi_fcntl_F_GETLK = 5,
		abi_fcntl_F_SETLK = 6,
		abi_fcntl_F_SETLKW = 7,
		abi_fcntl_F_DUPFD_CLOEXEC = 1030,

		abi_ioctl_TIOCGWINSZ = 0x5413,

		abi_mmap_PROT_READ = 1,
		abi_mmap_PROT_WRITE = 2,
		abi_mmap_PROT_EXEC = 4,

		abi_mmap_MAP_SHARED = 0x1,
		abi_mmap_MAP_PRIVATE = 0x2,
		abi_mmap_MAP_FIXED = 0x10,
		abi_mmap_MAP_ANON = 0x20,

		abi_rusage_RUSAGE_SELF = 0,
		abi_rusage_RUSAGE_CHILDREN = -1,

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
		abi_rlimit_RLIMIT_NLIMITS = 15,

		abi_signal_SIGHUP = 1,
		abi_signal_SIGINT = 2,
		abi_signal_SIGQUIT = 3,
		abi_signal_SIGILL = 4,
		abi_signal_SIGTRAP = 5,
		abi_signal_SIGABRT = 6,
		abi_signal_SIGIOT = abi_signal_SIGABRT,
		abi_signal_SIGBUS = 7,
		abi_signal_SIGFPE = 8,
		abi_signal_SIGKILL = 9,
		abi_signal_SIGUSR1 = 10,
		abi_signal_SIGSEGV = 11,
		abi_signal_SIGUSR2 = 12,
		abi_signal_SIGPIPE = 13,
		abi_signal_SIGALRM = 14,
		abi_signal_SIGTERM = 15,
		abi_signal_SIGSTKFLT = 16,
		abi_signal_SIGCHLD = 17,
		abi_signal_SIGCONT = 18,
		abi_signal_SIGSTOP = 19,
		abi_signal_SIGTSTP = 20,
		abi_signal_SIGTTIN = 21,
		abi_signal_SIGTTOU = 22,
		abi_signal_SIGURG = 23,
		abi_signal_SIGXCPU = 24,
		abi_signal_SIGXFSZ = 25,
		abi_signal_SIGVTALRM = 26,
		abi_signal_SIGPROF = 27,
		abi_signal_SIGWINCH = 28,
		abi_signal_SIGIO = 29,
		abi_signal_SIGPOLL = abi_signal_SIGIO,
		abi_signal_SIGPWR = 30,
		abi_signal_SIGSYS = 31,
		abi_signal_NSIG = 65,

		abi_wait_WNOHANG = 1,
		abi_wait_WUNTRACED = 2,
		abi_wait_WSTOPPED = 2,
		abi_wait_WEXITED = 4,
		abi_wait_WCONTINUED = 8,

		abi_AT_FDCWD = -100,
		abi_AT_REMOVEDIR = 0x200,
		abi_AT_EACCESS = 0x200,
		abi_AT_SYMLINK_NOFOLLOW = 0x100,

		abi_PATH_MAX = 4096,
		abi_NEW_UTS_LEN = 64,
	};

	template <typename P> struct abi_iovec
	{
		typename P::ulong_t iov_base;
		typename P::ulong_t iov_len;
	};

	struct abi_new_utsname
	{
		char sysname[abi_NEW_UTS_LEN + 1];
		char nodename[abi_NEW_UTS_LEN + 1];
		char release[abi_NEW_UTS_LEN + 1];
		char version[abi_NEW_UTS_LEN + 1];
		char machine[abi_NEW_UTS_LEN + 1];
		char domainname[abi_NEW_UTS_LEN + 1];
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

	template <typename P> struct abi_sysinfo
	{
		typename P::ulong_t uptime;
		typename P::ulong_t loads[3];
		typename P::ulong_t totalram;
		typename P::ulong_t freeram;
		typename P::ulong_t sharedram;
		typename P::ulong_t bufferram;
		typename P::ulong_t totalswap;
		typename P::ulong_t freeswap;
		unsigned short procs, pad;
		typename P::ulong_t totalhigh;
		typename P::ulong_t freehigh;
		typename P::uint_t mem_unit;
		char __reserved[256];
	};

	template <typename P> struct abi_timespec
	{
		typename P::long_t tv_sec;
		typename P::long_t tv_nsec;
	};

	template <typename P> struct abi_timeval
	{
		typename P::long_t tv_sec;
		typename P::long_t tv_usec;
	};

	struct abi_timezone
	{
		int tz_minuteswest;
		int tz_dsttime;
	};

	template <typename P> struct abi_tms
	{
		typename P::long_t tms_utime;
		typename P::long_t tms_stime;
		typename P::long_t tms_cutime;
		typename P::long_t tms_cstime;
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

	struct abi_rlimit
	{
		u64 rlim_cur;
		u64 rlim_max;
	};

	struct abi_winsize
	{
		unsigned short ws_row;
		unsigned short ws_col;
		unsigned short ws_xpixel;
		unsigned short ws_ypixel;
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

	template <typename P>
	void cvt_abi_rusage(abi_rusage<P> *guest_rusage, struct rusage *host_rusage)
	{
		guest_rusage->ru_utime.tv_sec = host_rusage->ru_utime.tv_sec;
		guest_rusage->ru_utime.tv_usec = host_rusage->ru_utime.tv_usec;
		guest_rusage->ru_stime.tv_sec = host_rusage->ru_stime.tv_sec;
		guest_rusage->ru_stime.tv_usec = host_rusage->ru_stime.tv_usec;
		guest_rusage->ru_maxrss = host_rusage->ru_maxrss;
		guest_rusage->ru_ixrss = host_rusage->ru_ixrss;
		guest_rusage->ru_idrss = host_rusage->ru_idrss;
		guest_rusage->ru_isrss = host_rusage->ru_isrss;
		guest_rusage->ru_minflt = host_rusage->ru_minflt;
		guest_rusage->ru_majflt = host_rusage->ru_majflt;
		guest_rusage->ru_nswap = host_rusage->ru_nswap;
		guest_rusage->ru_inblock = host_rusage->ru_inblock;
		guest_rusage->ru_oublock = host_rusage->ru_oublock;
		guest_rusage->ru_msgsnd = host_rusage->ru_msgsnd;
		guest_rusage->ru_msgrcv = host_rusage->ru_msgrcv;
		guest_rusage->ru_nsignals = host_rusage->ru_nsignals;
		guest_rusage->ru_nvcsw = host_rusage->ru_nvcsw;
		guest_rusage->ru_nivcsw = host_rusage->ru_nivcsw;
	}

	int cvt_open_flags(int lxflags)
	{
		int hostflags = 0;
		if (lxflags & 01) hostflags |= O_WRONLY;
		if (lxflags & 02) hostflags |= O_RDWR;
		if (lxflags & 0100) hostflags |= O_CREAT;
		if (lxflags & 0200) hostflags |= O_EXCL;
		if (lxflags & 01000) hostflags |= O_TRUNC;
		if (lxflags & 02000) hostflags |= O_APPEND;
		if (lxflags & 04000) hostflags |= O_NONBLOCK;
		if (lxflags & 04010000) hostflags |= O_SYNC;
		return hostflags;
	}

	int cvt_error(int ret)
	{
		if (ret >= 0) return ret;
		switch (errno) {
			case EPERM:    return -abi_errno_EPERM;
			case ENOENT:   return -abi_errno_ENOENT;
			case ESRCH:    return -abi_errno_ESRCH;
			case EINTR:    return -abi_errno_EINTR;
			case EIO:      return -abi_errno_EIO;
			case ENXIO:    return -abi_errno_ENXIO;
			case E2BIG:    return -abi_errno_E2BIG;
			case ENOEXEC:  return -abi_errno_ENOEXEC;
			case EBADF:    return -abi_errno_EBADF;
			case ECHILD:   return -abi_errno_ECHILD;
			case EAGAIN:   return -abi_errno_EAGAIN;
			case ENOMEM:   return -abi_errno_ENOMEM;
			case EACCES:   return -abi_errno_EACCES;
			case EFAULT:   return -abi_errno_EFAULT;
			case ENOTBLK:  return -abi_errno_ENOTBLK;
			case EBUSY:    return -abi_errno_EBUSY;
			case EEXIST:   return -abi_errno_EEXIST;
			case EXDEV:    return -abi_errno_EXDEV;
			case ENODEV:   return -abi_errno_ENODEV;
			case ENOTDIR:  return -abi_errno_ENOTDIR;
			case EISDIR:   return -abi_errno_EISDIR;
			case EINVAL:   return -abi_errno_EINVAL;
			case ENFILE:   return -abi_errno_ENFILE;
			case EMFILE:   return -abi_errno_EMFILE;
			case ENOTTY:   return -abi_errno_ENOTTY;
			case ETXTBSY:  return -abi_errno_ETXTBSY;
			case EFBIG:    return -abi_errno_EFBIG;
			case ENOSPC:   return -abi_errno_ENOSPC;
			case ESPIPE:   return -abi_errno_ESPIPE;
			case EROFS:    return -abi_errno_EROFS;
			case EMLINK:   return -abi_errno_EMLINK;
			case EPIPE:    return -abi_errno_EPIPE;
			case EDOM:     return -abi_errno_EDOM;
			case ERANGE:   return -abi_errno_ERANGE;
			case EDEADLK:  return -abi_errno_EDEADLK;
			case ENAMETOOLONG: return -abi_errno_ENAMETOOLONG;
			case ENOLCK:   return -abi_errno_ENOLCK;
			case ENOSYS:   return -abi_errno_ENOSYS;
			default:       return -abi_errno_EINVAL;
		}
	}

	template <typename P> void abi_sys_getcwd(P &proc)
	{
		char *buf = (char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		char *ret = getcwd(buf, proc.ireg[rv_ireg_a1]);
		if (proc.log & proc_log_syscall) {
			printf("getcwd(%ld,0x%lx) = %d",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				ret ? 0 : -EINVAL);
		}
		proc.ireg[rv_ireg_a0] = ret ? 0 : -EINVAL;
	}

	template <typename P> void abi_sys_fcntl(P &proc)
	{
		int cmd;
		switch(proc.ireg[rv_ireg_a1])
		{
			case abi_fcntl_F_DUPFD: cmd = F_DUPFD; break;
			case abi_fcntl_F_GETFD: cmd = F_GETFD; break;
			case abi_fcntl_F_SETFD: cmd = F_SETFD; break;
			case abi_fcntl_F_GETFL: cmd = F_GETFL; break;
			case abi_fcntl_F_SETFL: cmd = F_SETFL; break;
			case abi_fcntl_F_SETOWN: cmd = F_SETOWN; break;
			case abi_fcntl_F_GETOWN: cmd = F_GETOWN; break;
			case abi_fcntl_F_SETSIG: cmd = -1; break;
			case abi_fcntl_F_GETSIG: cmd = -1; break;
			case abi_fcntl_F_GETLK: cmd = F_GETLK; break;
			case abi_fcntl_F_SETLK: cmd = F_SETLK; break;
			case abi_fcntl_F_SETLKW: cmd = F_SETLKW; break;
			case abi_fcntl_F_DUPFD_CLOEXEC: cmd = F_DUPFD_CLOEXEC; break;
			default: cmd = -1; break;
		}
		if (cmd == -1) {
			if (proc.log & proc_log_syscall) {
				printf("fcntl(%ld,%ld,0x%lx) = %d",
					(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
					(long)proc.ireg[rv_ireg_a2], -EINVAL);
			}
			proc.ireg[rv_ireg_a0] = -EINVAL;
			return;
		}
		int ret = fcntl(proc.ireg[rv_ireg_a0], cmd, proc.ireg[rv_ireg_a2]);
		if (proc.log & proc_log_syscall) {
			printf("fcntl(%ld,%ld,0x%lx) = %d",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
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
				if (proc.log & proc_log_syscall) {
					printf("ioctl(%ld,%ld) = %d\n",
						(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1], cvt_error(ret));
				}
				proc.ireg[rv_ireg_a0] = cvt_error(ret);
				break;
			}
			default:
				if (proc.log & proc_log_syscall) {
					printf("ioctl(%ld,%ld) = %d\n",
						(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1], -abi_errno_EINVAL);
				}
				proc.ireg[rv_ireg_a0] = -abi_errno_EINVAL;
				break;
		}
	}

	template <typename P> void abi_sys_unlinkat(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		int fd;
		switch(proc.ireg[rv_ireg_a0]) {
			case abi_AT_FDCWD:
				fd = AT_FDCWD;
				break;
			default:
				fd = proc.ireg[rv_ireg_a0];
				break;
		}
		int flag = 0;
		if(proc.ireg[rv_ireg_a2].r.xu.val & abi_AT_REMOVEDIR) {
			flag |= AT_REMOVEDIR;
		}
		int ret = unlinkat(fd, pathname, flag);
		if (proc.log & proc_log_syscall) {
			printf("unlinkat(%ld,%s,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], pathname, (long)proc.ireg[rv_ireg_a2],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_faccessat(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		int fd;
		switch(proc.ireg[rv_ireg_a0]) {
			case abi_AT_FDCWD:
				fd = AT_FDCWD;
				break;
			default:
				fd = proc.ireg[rv_ireg_a0];
				break;
		}
		int flag = 0;
		if(proc.ireg[rv_ireg_a3].r.xu.val & abi_AT_EACCESS) {
			flag |= AT_EACCESS;
		}
		int ret = faccessat(fd, pathname, proc.ireg[rv_ireg_a2], flag);
		if (proc.log & proc_log_syscall) {
			printf("faccessat(%ld,%s,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], pathname, (long)proc.ireg[rv_ireg_a2],
				(long)proc.ireg[rv_ireg_a3], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_openat(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		int fd;
		switch(proc.ireg[rv_ireg_a0]) {
			case abi_AT_FDCWD:
				fd = AT_FDCWD;
				break;
			default:
				fd = proc.ireg[rv_ireg_a0];
				break;
		}
		int hostflags = cvt_open_flags(proc.ireg[rv_ireg_a2]);
		int ret = openat(fd, pathname, hostflags, proc.ireg[rv_ireg_a3]);
		if (proc.log & proc_log_syscall) {
			printf("openat(%ld,%s,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], pathname, (long)proc.ireg[rv_ireg_a2],
				(long)proc.ireg[rv_ireg_a3], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_close(P &proc)
	{
		int ret = close(proc.ireg[rv_ireg_a0]);
		if (proc.log & proc_log_syscall) {
			printf("close(%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_lseek(P &proc)
	{
		int ret = lseek(proc.ireg[rv_ireg_a0],
			proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2]);
		if (proc.log & proc_log_syscall) {
			printf("lseek(%ld,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_read(P &proc)
	{
		int ret = read(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2]);
		if (proc.log & proc_log_syscall) {
			printf("read(%ld,0x%lx,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_write(P &proc)
	{
		int ret = write(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2]);
		if (proc.log & proc_log_syscall) {
			printf("write(%ld,0x%lx,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
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
		if (proc.log & proc_log_syscall) {
			printf("readv(%ld,0x%lx,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
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
		if (proc.log & proc_log_syscall) {
			printf("writev(%ld,0x%lx,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_pread(P &proc)
	{
		int ret = pread(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2],
			proc.ireg[rv_ireg_a3]);
		if (proc.log & proc_log_syscall) {
			printf("pread(%ld,0x%lx,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], (long)proc.ireg[rv_ireg_a3],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_pwrite(P &proc)
	{
		int ret = pwrite(proc.ireg[rv_ireg_a0],
			(void*)(addr_t)proc.ireg[rv_ireg_a1], proc.ireg[rv_ireg_a2],
			proc.ireg[rv_ireg_a3]);
		if (proc.log & proc_log_syscall) {
			printf("pwrite(%ld,0x%lx,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], (long)proc.ireg[rv_ireg_a3],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_readlinkat(P &proc)
	{
		int fd;
		switch(proc.ireg[rv_ireg_a0]) {
			case abi_AT_FDCWD:
				fd = AT_FDCWD;
				break;
			default:
				fd = proc.ireg[rv_ireg_a0];
				break;
		}
		const char* path = (const char*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		char* buf = (char*)(addr_t)proc.ireg[rv_ireg_a2].r.xu.val;
		int ret = readlinkat(fd, path, buf, proc.ireg[rv_ireg_a3].r.xu.val);
		if (proc.log & proc_log_syscall) {
			printf("readlinkat(%ld,%s,0x%lx,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], path, (long)proc.ireg[rv_ireg_a2],
				(long)proc.ireg[rv_ireg_a3], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_fstatat(P &proc)
	{
		struct stat host_stat;
		int fd;
		switch(proc.ireg[rv_ireg_a0]) {
			case abi_AT_FDCWD:
				fd = AT_FDCWD;
				break;
			default:
				fd = proc.ireg[rv_ireg_a0];
				break;
		}
		const char* path = (const char*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		int abi_flags = (proc.ireg[rv_ireg_a3] == abi_AT_SYMLINK_NOFOLLOW)
			? AT_SYMLINK_NOFOLLOW : 0;
		memset(&host_stat, 0, sizeof(host_stat));
		int ret = fstatat(fd, path, &host_stat, abi_flags);
		abi_stat<P> *guest_stat = (abi_stat<P>*)(addr_t)proc.ireg[rv_ireg_a2].r.xu.val;
		cvt_abi_stat(guest_stat, &host_stat);
		if (proc.log & proc_log_syscall) {
			printf("fstatat(%ld,%s,0x%lx,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], path, (long)proc.ireg[rv_ireg_a2],
				(long)proc.ireg[rv_ireg_a3], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_fstat(P &proc)
	{
		struct stat host_stat;
		memset(&host_stat, 0, sizeof(host_stat));
		int ret = fstat(proc.ireg[rv_ireg_a0], &host_stat);
		abi_stat<P> *guest_stat = (abi_stat<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		cvt_abi_stat(guest_stat, &host_stat);
		if (proc.log & proc_log_syscall) {
			printf("fstat(%ld,0x%lx) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_open(P &proc)
	{
		int hostflags = cvt_open_flags(proc.ireg[rv_ireg_a1]);
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		int ret = open(pathname, hostflags, proc.ireg[rv_ireg_a2].r.xu.val);
		if (proc.log & proc_log_syscall) {
			printf("open(%s,%ld,%ld) = %d\n",
				pathname, (long)proc.ireg[rv_ireg_a1], (long)proc.ireg[rv_ireg_a2],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_unlink(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		int ret = unlink(pathname);
		if (proc.log & proc_log_syscall) {
			printf("unlink(%s) = %d\n",
				pathname, cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_stat(P &proc)
	{
		struct stat host_stat;
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		memset(&host_stat, 0, sizeof(host_stat));
		int ret = stat(pathname, &host_stat);
		abi_stat<P> *guest_stat = (abi_stat<P>*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		cvt_abi_stat(guest_stat, &host_stat);
		if (proc.log & proc_log_syscall) {
			printf("stat(%s,0x%lx) = %d\n",
				pathname, (long)proc.ireg[rv_ireg_a1],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_chown(P &proc)
	{
		const char* pathname = (const char*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		int ret = chown(pathname, (uid_t)proc.ireg[rv_ireg_a1].r.xu.val, (gid_t)proc.ireg[rv_ireg_a2].r.xu.val);
		if (proc.log & proc_log_syscall) {
			printf("chown(%s,%ld,%ld) = %d\n",
				pathname, (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_exit(P &proc)
	{
		if (proc.log & proc_log_syscall) {
			printf("exit(%ld)\n", (long)proc.ireg[rv_ireg_a0]);
		}
		proc.exit(proc.ireg[rv_ireg_a0]);
		exit(proc.ireg[rv_ireg_a0]);
	}

	template <typename P> void abi_sys_set_tid_address(P &proc)
	{
		proc.clear_child_tid = *(int*)(uintptr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		if (proc.log & proc_log_syscall) {
			printf("sys_set_tid_address(0x%lx) = %d\n",
				(long)proc.ireg[rv_ireg_a0], 1);
		}
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
			if (proc.log & proc_log_syscall) {
				printf("clock_get_time(0x%lx) = %d\n",
					(long)proc.ireg[rv_ireg_a1], 0);
			}
		} else {
			if (proc.log & proc_log_syscall) {
				printf("clock_get_time(0x%lx) = %d\n",
					(long)proc.ireg[rv_ireg_a1], -EFAULT);
			}
			proc.ireg[rv_ireg_a0] = -EFAULT;
		}
	}

	template <typename P> void abi_sys_rt_sigaction(P &proc)
	{
		proc.ireg[rv_ireg_a0] = 0; /* nop */
	}

	template <typename P> void abi_sys_rt_sigprocmask(P &proc)
	{
		proc.ireg[rv_ireg_a0] = 0; /* nop */
	}

	template <typename P> void abi_sys_times(P &proc)
	{
		struct tms host_tms;
		typename P::long_t ret = times(&host_tms);
		abi_tms<P> *guest_tms = (abi_tms<P>*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		guest_tms->tms_utime = host_tms.tms_utime;
		guest_tms->tms_stime = host_tms.tms_stime;
		guest_tms->tms_cutime = host_tms.tms_cutime;
		guest_tms->tms_cstime = host_tms.tms_cstime;
		if (proc.log & proc_log_syscall) {
			printf("times(0x%lx) = %ld\n",
				(long)proc.ireg[rv_ireg_a0], (long)ret);
		}
		proc.ireg[rv_ireg_a0] = ret;
	}

	template <typename P> void abi_sys_uname(P &proc)
	{
		abi_new_utsname *ustname = (abi_new_utsname*)(addr_t)proc.ireg[rv_ireg_a0].r.xu.val;
		struct utsname host_utsname;
		int ret = uname(&host_utsname);
		strncpy(ustname->sysname, host_utsname.sysname, abi_NEW_UTS_LEN);
		strncpy(ustname->nodename, host_utsname.nodename, abi_NEW_UTS_LEN);
		strncpy(ustname->release, host_utsname.release, abi_NEW_UTS_LEN);
		strncpy(ustname->version, host_utsname.version, abi_NEW_UTS_LEN);
		strncpy(ustname->machine, host_utsname.machine, abi_NEW_UTS_LEN);
		if (proc.log & proc_log_syscall) {
			printf("uname(0x%lx) = %d\n",
				(long)proc.ireg[rv_ireg_a0], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
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
		if (guest_rusage) {
			cvt_abi_rusage(guest_rusage, &host_rusage);
		}
		if (proc.log & proc_log_syscall) {
			printf("getrusage(%ld, 0x%lx) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
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
			abi_timezone *guest_tzp = (abi_timezone*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
			guest_tzp->tz_minuteswest = host_tzp.tz_minuteswest;
			guest_tzp->tz_dsttime = host_tzp.tz_dsttime;
		}
		if (proc.log & proc_log_syscall) {
			printf("gettimeofday(0x%lx, 0x%lx) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_gettid(P &proc)
	{
		int tid = getpid();
		if (proc.log & proc_log_syscall) {
			printf("gettid() = %d\n", tid);
		}
		proc.ireg[rv_ireg_a0] = tid;
	}

	template <typename P> void abi_sys_sysinfo(P &proc)
	{
		abi_sysinfo<P> *guest_sysinfo = (abi_sysinfo<P>*)(addr_t)proc.ireg[rv_ireg_a0];
		memset(guest_sysinfo, 0, sizeof(*guest_sysinfo));
		if (proc.log & proc_log_syscall) {
			printf("sysinfo(0x%lx) = %d\n",
				(long)proc.ireg[rv_ireg_a0], 0);
		}
		proc.ireg[rv_ireg_a0] = 0;
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
			if (proc.log & proc_log_syscall) {
				printf("brk(0x%lx) = 0x%llx\n",
					(long)proc.ireg[rv_ireg_a0], proc.mmu.mem->brk);
			}
			proc.ireg[rv_ireg_a0] = proc.mmu.mem->brk;
			return;
		}

		/* shrink brk */
		if (new_brk <= proc.mmu.mem->heap_end) {
			if (proc.log & proc_log_syscall) {
				printf("brk(0x%lx) = 0x%llx\n",
					(long)proc.ireg[rv_ireg_a0], new_brk);
			}
			proc.ireg[rv_ireg_a0] = proc.mmu.mem->brk = new_brk;
			return;
		}

		/* map a new heap segment */
		void *addr = mmap((void*)proc.mmu.mem->heap_end, new_heap_end - proc.mmu.mem->heap_end,
			PROT_READ | PROT_WRITE, MAP_FIXED | MAP_ANONYMOUS | MAP_PRIVATE, -1, 0);
		if (addr == MAP_FAILED) {
			debug("sys_brk: error: mmap: %s", strerror(errno));
			if (proc.log & proc_log_syscall) {
				printf("brk(0x%lx) = %d\n",
					(long)proc.ireg[rv_ireg_a0], -ENOMEM);
			}
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
			if (proc.log & proc_log_syscall) {
				printf("brk(0x%lx) = 0x%llx\n",
					(long)proc.ireg[rv_ireg_a0], new_brk);
			}
			proc.ireg[rv_ireg_a0] = proc.mmu.mem->brk = new_brk;
		}
	}

	template <typename P> void abi_sys_munmap(P &proc)
	{
		int ret = guest_munmap((void*)(uintptr_t)proc.ireg[rv_ireg_a0], proc.ireg[rv_ireg_a1]);
		if (proc.log & proc_log_syscall) {
			printf("munmap(0x%lx,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_clone(P &proc)
	{
		int flags = proc.ireg[rv_ireg_a0];
		if (flags == abi_signal_SIGCHLD) {
			int ret = fork();
			if (proc.log & proc_log_syscall) {
				printf("clone(%ld,%ld) = %d\n",
					(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
					cvt_error(ret));
			}
			proc.ireg[rv_ireg_a0] = cvt_error(ret);
		} else {
			if (proc.log & proc_log_syscall) {
				printf("clone(%ld,%ld) = %d\n",
					(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
					abi_errno_EINVAL);
			}
			proc.ireg[rv_ireg_a0] = -abi_errno_EINVAL;
		}
	}

	template <typename P> void abi_sys_execve(P &proc)
	{
		/* todo */
		proc.ireg[rv_ireg_a0] = -abi_errno_ENOSYS;
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
		uintptr_t ret = (uintptr_t)guest_mmap(
			(void*)(uintptr_t)proc.ireg[rv_ireg_a0], proc.ireg[rv_ireg_a1],
			prot, flags, proc.ireg[rv_ireg_a4], proc.ireg[rv_ireg_a5]);
		if (proc.log & proc_log_syscall) {
			printf("mmap(0x%lx,%ld,%ld,%ld,%ld,%ld) = 0x%lx\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], (long)proc.ireg[rv_ireg_a3],
				(long)proc.ireg[rv_ireg_a4], (long)proc.ireg[rv_ireg_a5],
				ret);
		}
		proc.ireg[rv_ireg_a0].r.xu.val = ret;
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
		if (proc.log & proc_log_syscall) {
			printf("mprotect(0x%lx,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], cvt_error(ret));
		}
		proc.ireg[rv_ireg_a0] = cvt_error(ret);
	}

	template <typename P> void abi_sys_madvise(P &proc)
	{
		if (proc.log & proc_log_syscall) {
			printf("madvise(0x%lx,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], 0);
		}
		proc.ireg[rv_ireg_a0] = 0; /* nop */
	}

	template <typename P> void abi_sys_wait4(P &proc)
	{
		struct rusage host_rusage;
		int ret = wait4(proc.ireg[rv_ireg_a0], (int*)(addr_t)proc.ireg[rv_ireg_a1],
			(long)proc.ireg[rv_ireg_a2], &host_rusage);
		if (proc.log & proc_log_syscall) {
			printf("wait4(%ld,0x%lx,%ld,%ld) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], (long)proc.ireg[rv_ireg_a3],
				cvt_error(ret));
		}
		abi_rusage<P> *guest_rusage = (abi_rusage<P>*)(addr_t)proc.ireg[rv_ireg_a3].r.xu.val;
		if (guest_rusage) {
			cvt_abi_rusage(guest_rusage, &host_rusage);
		}
		int abi_ret = 0;
		if (ret > 0) {
			abi_ret |= WEXITSTATUS(ret) & 0xff << 8;
			abi_ret |= WTERMSIG(ret) & 0x7f;
			/* todo */
		}
		proc.ireg[rv_ireg_a0] = cvt_error(abi_ret);
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
		abi_rlimit *guest_new_limit = (abi_rlimit*)(addr_t)proc.ireg[rv_ireg_a1].r.xu.val;
		abi_rlimit *guest_old_limit = (abi_rlimit*)(addr_t)proc.ireg[rv_ireg_a2].r.xu.val;
		struct rlimit host_new_limit;
		struct rlimit host_old_limit;
		if (guest_old_limit) {
			ret = getrlimit(resource, &host_old_limit);
			if (ret) {
				proc.ireg[rv_ireg_a0] = -errno;
				if (proc.log & proc_log_syscall) {
					printf("prlimit64(%ld,0x%lx,0x%lx) = %d\n",
						(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
						(long)proc.ireg[rv_ireg_a2], -errno);
				}
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
				if (proc.log & proc_log_syscall) {
					printf("prlimit64(%ld,0x%lx,0x%lx) = %d\n",
						(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
						(long)proc.ireg[rv_ireg_a2], -errno);
				}
				return;
			}
		}
		if (proc.log & proc_log_syscall) {
			printf("prlimit64(%ld,0x%lx,0x%lx) = %d\n",
				(long)proc.ireg[rv_ireg_a0], (long)proc.ireg[rv_ireg_a1],
				(long)proc.ireg[rv_ireg_a2], 0);
		}
		proc.ireg[rv_ireg_a0] = 0;
	}

	template <typename P> void proxy_syscall(P &proc)
	{
		switch (proc.ireg[rv_ireg_a7]) {
			case abi_syscall_getcwd:          abi_sys_getcwd(proc); break;
			case abi_syscall_fcntl:           abi_sys_fcntl(proc); break;
			case abi_syscall_ioctl:           abi_sys_ioctl(proc); break;
			case abi_syscall_unlinkat:        abi_sys_unlinkat(proc); break;
			case abi_syscall_faccessat:       abi_sys_faccessat(proc); break;
			case abi_syscall_openat:          abi_sys_openat(proc); break;
			case abi_syscall_close:           abi_sys_close(proc); break;
			case abi_syscall_lseek:           abi_sys_lseek(proc); break;
			case abi_syscall_read:            abi_sys_read(proc);  break;
			case abi_syscall_write:           abi_sys_write(proc); break;
			case abi_syscall_readv:           abi_sys_readv(proc);  break;
			case abi_syscall_writev:          abi_sys_writev(proc); break;
			case abi_syscall_pread:           abi_sys_pread(proc); break;
			case abi_syscall_pwrite:          abi_sys_pwrite(proc); break;
			case abi_syscall_readlinkat:      abi_sys_readlinkat(proc); break;
			case abi_syscall_fstatat:         abi_sys_fstatat(proc); break;
			case abi_syscall_fstat:           abi_sys_fstat(proc); break;
			case abi_syscall_exit:            abi_sys_exit(proc); break;
			case abi_syscall_exit_group:      abi_sys_exit(proc); break;
			case abi_syscall_set_tid_address: abi_sys_set_tid_address(proc); break;
			case abi_syscall_clock_gettime:   abi_sys_clock_gettime(proc); break;
			case abi_syscall_rt_sigaction:    abi_sys_rt_sigaction(proc); break;
			case abi_syscall_rt_sigprocmask:  abi_sys_rt_sigprocmask(proc); break;
			case abi_syscall_times:           abi_sys_times(proc); break;
			case abi_syscall_uname:           abi_sys_uname(proc); break;
			case abi_syscall_getrusage:       abi_sys_getrusage(proc); break;
			case abi_syscall_gettimeofday:    abi_sys_gettimeofday(proc);break;
			case abi_syscall_gettid:          abi_sys_gettid(proc); break;
			case abi_syscall_sysinfo:         abi_sys_sysinfo(proc); break;
			case abi_syscall_brk:             abi_sys_brk(proc); break;
			case abi_syscall_munmap:          abi_sys_munmap(proc); break;
			case abi_syscall_clone:           abi_sys_clone(proc); break;
			case abi_syscall_execve:          abi_sys_execve(proc); break;
			case abi_syscall_mmap:            abi_sys_mmap(proc); break;
			case abi_syscall_mprotect:        abi_sys_mprotect(proc); break;
			case abi_syscall_madvise:         abi_sys_madvise(proc); break;
			case abi_syscall_wait4:           abi_sys_wait4(proc); break;
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
