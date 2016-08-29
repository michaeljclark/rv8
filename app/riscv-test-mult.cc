//
//  riscv-test-mult.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-bits.h"

using namespace riscv;

struct _u16
{
	typedef u8 utype;

	union {
		u8                          b[2];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u8 lo;  u8 hi; }   p;
	#else
		struct { u8 hi;  u8 lo; }   p;
	#endif
	} val;

	inline _u16() : val{ .b = { 0 } } {}
	inline _u16(const _u16 &o) : val{ .p = o.val.p } {}

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _u16(u8 hi, u8 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _u16(u8 hi, u8 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _u32
{
	typedef u16 utype;

	union {
		u8                          b[4];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u16 lo; u16 hi; }  p;
	#else
		struct { u16 hi; u16 lo; }  p;
	#endif
	} val;

	inline _u32() : val{ .b = { 0 } } {}
	inline _u32(const _u32 &o) : val{ .p = o.val.p } {}

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _u32(u16 hi, u16 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _u32(u16 hi, u16 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _u64
{
	typedef u32 utype;

	union {
		u8                          b[8];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u32 lo; u32 hi; }  p;
	#else
		struct { u32 hi; u32 lo; }  p;
	#endif
	} val;

	inline _u64() : val{ .b = { 0 } } {}
	inline _u64(const _u64 &o) : val{ .p = o.val.p } {}

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _u64(u32 hi, u32 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _u64(u32 hi, u32 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _u128
{
	typedef u64 utype;

	union {
		u8                          b[16];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u64 lo; u64 hi; }  p;
	#else
		struct { u64 hi; u64 lo; }  p;
	#endif
	} val;

	inline _u128() : val{ .b = { 0 } } {}
	inline _u128(const _u128 &o) : val{ .p = o.val.p } {}

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _u128(u64 hi, u64 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _u128(u64 hi, u64 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

template <typename R>
R mult(typename R::utype x, typename R::utype y)
{
	/*
	 *  Babbage multiplication, Passages from the Life of
	 *  a Philosopher, Larger numbers treated, page 125
	 *
	 *                               |       x0 * y0       |
	 *      +             |       x1 * y0       |
	 *      +             |       x0 * y1       |
	 *      +  |       x1 * y1       |
	 *      +  |         C|         C|
	 *
	 *  z0  =  x0 * y0
	 *  z1  =  x1 * y0
	 *  z2  =  x0 * y1
	 *  z3  =  x1 * y1
	 *
	 *  r   =  z0 + ((z1 + z2) << qb) + (z3 << hb)
	 */

	typedef typename R::utype T;
	const int qb = sizeof(T) << 2;
	const T   mask = (T(1) << qb) - 1;

	T x0 =    x       & mask;
	T x1 =    x >> qb & mask;
	T y0 =    y       & mask;
	T y1 =    y >> qb & mask;
	T z0 =    x0 * y0;
	T z1 =    x1 * y0;
	T z2 =    x0 * y1;
	T z3 =    x1 * y1;
	T z4 =    z1 + z2;
	T c1 =    z4 < z1;
	T lo =    z0 + (z4 << qb);
	T c2 =    lo < z0;
	T hi =    z3 + (z4 >> qb) + (c1 << qb) + c2;

	return R(hi, lo);
}

template <typename L, typename R>
void test_mult(typename R::utype x, typename R::utype y)
{
	R xy = mult<R>(x, y);
	L xya = (L(xy.val.p.hi) << (sizeof(L) << 2)) | L(xy.val.p.lo);
	L xyc = L(x) * L(y);
	bool pass = (xya == xyc);
	char fmt[256];
	const int hw = sizeof(L);
	snprintf(fmt, sizeof(fmt), "%%s "
		"%%-11sx=%%-20llu (0x%%0%ullx)\n"
		"%%16sy=%%-20llu (0x%%0%ullx)\n"
		"%%12sxy.hi=%%-20llu (0x%%0%ullx)\n"
		"%%12sxy.lo=%%-20llu (0x%%0%ullx)\n"
		"%%9sxy.check=%%-20llu (0x%%0%ullx)\n",
		hw,hw,hw,hw,hw<<1);
	printf(fmt, pass ? "PASS" : "FAIL", __func__,
		(u64)x, (u64)x, "",
		(u64)x, (u64)y, "",
		(u64)xy.val.p.hi, (u64)xy.val.p.hi, "",
		(u64)xy.val.p.lo, (u64)xy.val.p.lo, "",
		(u64)xyc, (u64)xyc);
}

int main()
{
	test_mult<u16,_u16>(53, 63);
	test_mult<u16,_u16>(53, 67);
	test_mult<u16,_u16>(63, 191);
	test_mult<u16,_u16>(253, 253);
	test_mult<u32,_u32>(12345, 999);
	test_mult<u32,_u32>(12345, 63000);
	test_mult<u32,_u32>(65533, 65533);
	test_mult<u64,_u64>(123450, 9990);
	test_mult<u64,_u64>(123450, 630000);
	test_mult<u64,_u64>(87652393, 87652393);
	test_mult<u64,_u64>(613566756, 613566756);
	test_mult<u64,_u64>(4294967295, 630000);
	test_mult<u64,_u64>(4294967295, 87652393);
	test_mult<u64,_u64>(4294967295, 613566756);
	test_mult<u64,_u64>(4294967295, 4294967295);
	test_mult<unsigned __int128,_u128>(18446744073709551615ULL,18446744073709551615ULL);

	return 0;
}
