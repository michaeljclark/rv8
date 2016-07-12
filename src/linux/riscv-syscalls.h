//
//  riscv-syscalls.h
//

#ifndef riscv_syscalls_h
#define riscv_syscalls_h

template <typename T> void riscv_linux_sys_write(T &proc)
{
	proc.ireg[riscv_ireg_a0] = write(proc.ireg[riscv_ireg_a0],
		(void*)(uintptr_t)proc.ireg[riscv_ireg_a1], proc.ireg[riscv_ireg_a2]);
}

inline void riscv_linux_cvtstat(riscv_linux_stat *lx_stat, struct stat *h_stat)
{
	lx_stat->_dev        = h_stat->st_dev;
	lx_stat->_ino        = h_stat->st_ino;
	lx_stat->_mode       = h_stat->st_mode;
	lx_stat->_nlink      = h_stat->st_nlink;
	lx_stat->_uid        = h_stat->st_uid;
	lx_stat->_gid        = h_stat->st_gid;
	lx_stat->_rdev       = h_stat->st_rdev;
	lx_stat->_size       = h_stat->st_size;
	lx_stat->_blocks     = h_stat->st_blocks;
	lx_stat->_blksize    = h_stat->st_blksize;
#if defined (__APPLE__)
	lx_stat->_atime      = h_stat->st_atimespec.tv_sec;
	lx_stat->_atime_nsec = h_stat->st_atimespec.tv_nsec;
	lx_stat->_mtime      = h_stat->st_mtimespec.tv_sec;
	lx_stat->_mtime_nsec = h_stat->st_mtimespec.tv_nsec;
	lx_stat->_ctime      = h_stat->st_ctimespec.tv_sec;
	lx_stat->_ctime_nsec = h_stat->st_ctimespec.tv_nsec;
#else
	lx_stat->_atime      = h_stat->st_atim.tv_sec;
	lx_stat->_atime_nsec = h_stat->st_atim.tv_nsec;
	lx_stat->_mtime      = h_stat->st_mtim.tv_sec;
	lx_stat->_mtime_nsec = h_stat->st_mtim.tv_nsec;
	lx_stat->_ctime      = h_stat->st_ctim.tv_sec;
	lx_stat->_ctime_nsec = h_stat->st_ctim.tv_nsec;
#endif
}

template <typename T> void riscv_linux_sys_fstat(T &proc)
{
	struct stat h_stat;
	memset(&h_stat, 0, sizeof(h_stat));
	if ((proc.ireg[riscv_ireg_a0] = fstat(proc.ireg[riscv_ireg_a0], &h_stat)) == 0) {
		riscv_linux_stat *lx_stat = (riscv_linux_stat*)proc.ireg[riscv_ireg_a1].r.xu.val;
		riscv_linux_cvtstat(lx_stat, &h_stat);
	}
}

template <typename T> void riscv_linux_sys_exit(T &proc)
{
	exit(proc.ireg[riscv_ireg_a0]);
}

#endif