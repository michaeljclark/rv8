//
//  riscv-abi.h
//

#ifndef riscv_abi_h
#define riscv_abi_h

#ifdef __cplusplus
extern "C" {
#endif

struct riscv_abi_stat
{
	uint64_t _dev;
	uint64_t _ino;
	uint32_t _mode;
	uint32_t _nlink;
	uint32_t _uid;
	uint32_t _gid;
	uint64_t _rdev;
	uint64_t __pad1;
	int64_t  _size;
	int32_t  _blksize;
	int32_t  __pad2;
	int64_t  _blocks;
	int64_t  _atime;
	uint64_t _atime_nsec;
	int64_t  _mtime;
	uint64_t _mtime_nsec;
	int64_t  _ctime;
	uint64_t _ctime_nsec;
	uint32_t __unused4;
	uint32_t __unused5;
};

#ifdef __cplusplus
}
#endif

#endif