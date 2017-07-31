//
//  test-mul.cc
//

#undef NDEBUG

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <random>
#include <type_traits>

#include "host-endian.h"
#include "types.h"

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
		u16                         xu;
	} val;

	inline _u16() : val{ .b = { 0 } } {}
	inline _u16(const _u16 &o) : val{ .p = o.val.p } {}
	operator u16() const { return val.xu; }

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _u16(u8 hi, u8 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _u16(u8 hi, u8 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _s16
{
	typedef u8 utype;
	typedef s8 stype;
	typedef _u16 htype;

	union {
		u8                          b[2];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u8 lo;  s8 hi; }   p;
	#else
		struct { s8 hi;  u8 lo; }   p;
	#endif
		s16                         x;
	} val;

	inline _s16() : val{ .b = { 0 } } {}
	inline _s16(const _s16 &o) : val{ .p = o.val.p } {}
	operator s16() const { return val.x; }

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _s16(s8 hi, u8 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _s16(s8 hi, u8 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _u32
{
	typedef u16 utype;
	typedef _u16 htype;

	union {
		u8                          b[4];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u16 lo; u16 hi; }  p;
	#else
		struct { u16 hi; u16 lo; }  p;
	#endif
		u32                         xu;
	} val;

	inline _u32() : val{ .b = { 0 } } {}
	inline _u32(const _u32 &o) : val{ .p = o.val.p } {}
	operator u32() const { return val.xu; }

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _u32(u16 hi, u16 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _u32(u16 hi, u16 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _s32
{
	typedef u16 utype;
	typedef s16 stype;
	typedef _u16 htype;

	union {
		u8                          b[4];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u16 lo; s16 hi; }  p;
	#else
		struct { s16 hi; u16 lo; }  p;
	#endif
		s32                         x;
	} val;

	inline _s32() : val{ .b = { 0 } } {}
	inline _s32(const _s32 &o) : val{ .p = o.val.p } {}
	operator s32() const { return val.x; }

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _s32(s16 hi, u16 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _s32(s16 hi, u16 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _u64
{
	typedef u32 utype;
	typedef _u32 htype;

	union {
		u8                          b[8];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u32 lo; u32 hi; }  p;
	#else
		struct { u32 hi; u32 lo; }  p;
	#endif
		u64                         xu;
	} val;

	inline _u64() : val{ .b = { 0 } } {}
	inline _u64(const _u64 &o) : val{ .p = o.val.p } {}
	operator u64() const { return val.xu; }

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _u64(u32 hi, u32 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _u64(u32 hi, u32 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _s64
{
	typedef u32 utype;
	typedef s32 stype;
	typedef _u32 htype;

	union {
		u8                          b[8];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u32 lo; s32 hi; }  p;
	#else
		struct { s32 hi; u32 lo; }  p;
	#endif
		s64                         x;
	} val;

	inline _s64() : val{ .b = { 0 } } {}
	inline _s64(const _s64 &o) : val{ .p = o.val.p } {}
	operator s64() const { return val.x; }

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _s64(s32 hi, u32 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _s64(s32 hi, u32 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};

struct _u128
{
	typedef u64 utype;
	typedef _u64 htype;

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

struct _s128
{
	typedef u64 utype;
	typedef s64 stype;
	typedef _u64 htype;

	union {
		u8                          b[16];
	#if _BYTE_ORDER == _LITTLE_ENDIAN
		struct { u64 lo; s64 hi; }  p;
	#else
		struct { s64 hi; u64 lo; }  p;
	#endif
	} val;

	inline _s128() : val{ .b = { 0 } } {}
	inline _s128(const _s128 &o) : val{ .p = o.val.p } {}

	#if _BYTE_ORDER == _LITTLE_ENDIAN
	inline _s128(s64 hi, u64 lo) : val{ .p = { .lo = lo, .hi = hi } } {}
	#else
	inline _s128(s64 hi, u64 lo) : val{ .p = { .hi = hi, .lo = lo } } {}
	#endif
};


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


/* multiply unsigned unsigned */

template <typename R>
R mulu(typename R::utype x, typename R::utype y)
{
	typedef typename R::utype U;

	const int qb = sizeof(U) << 2;
	const U   mask = (U(1) << qb) - 1;

	U x0 =    x       & mask;
	U x1 =    x >> qb & mask;
	U y0 =    y       & mask;
	U y1 =    y >> qb & mask;
	U z0 =    mulu<typename R::htype>(x0, y0);
	U z1 =    mulu<typename R::htype>(x1, y0);
	U z2 =    mulu<typename R::htype>(x0, y1);
	U z3 =    mulu<typename R::htype>(x1, y1);
	U z4 =    z1 + (z0 >> qb);
	U c1 =    z2 + (z4 & mask);
	U hi =    z3 + (z4 >> qb) + (c1 >> qb);
	U lo =    (c1 << qb) + (z0 & mask);

	return R(hi, lo);
}

template<>
_u16 mulu<_u16>(typename _u16::utype x, typename _u16::utype y)
{
	u16 r = 0;
	u16 xx = x;
	for(size_t i = 0; i < 4; i++)
	{
		r += (xx & ~((y & 0x1) - 1)); xx <<= 1; y >>= 1;
		r += (xx & ~((y & 0x1) - 1)); xx <<= 1; y >>= 1;
	}
	return _u16(r >> 8, r & 0xff);
}

template<>
_u32 mulu<_u32>(typename _u32::utype x, typename _u32::utype y)
{
	u32 r = 0;
	u32 xx = x;
	for(size_t i = 0; i < 8; i++)
	{
		r += (xx & ~((y & 0x1) - 1)); xx <<= 1; y >>= 1;
		r += (xx & ~((y & 0x1) - 1)); xx <<= 1; y >>= 1;
	}
	return _u32(r >> 16, r & 0xffff);
}


/* multiply signed signed */

template <typename R>
R mul(typename R::stype x, typename R::stype y)
{
	typedef typename R::utype U;

	const int qb = sizeof(U) << 2;
	const U   mask = (U(1) << qb) - 1;

	U xs =    x < 0;
	U ys =    y < 0;
	U xu =    (~xs + 1) ^ (x - xs);
	U yu =    (~ys + 1) ^ (y - ys);
	U x0 =    xu       & mask;
	U x1 =    xu >> qb & mask;
	U y0 =    yu       & mask;
	U y1 =    yu >> qb & mask;
	U z0 =    mulu<typename R::htype>(x0, y0);
	U z1 =    mulu<typename R::htype>(x1, y0);
	U z2 =    mulu<typename R::htype>(x0, y1);
	U z3 =    mulu<typename R::htype>(x1, y1);
	U z4 =    z1 + (z0 >> qb);
	U c1 =    z2 + (z4 & mask);
	U h1 =    z3 + (z4 >> qb) + (c1 >> qb);
	U l1 =    (c1 << qb) + (z0 & mask);
	U rs =    xs ^ ys;
	U l2 =    l1 - rs;
	U hb =    l1 < l2;
	U h2 =    h1 - hb;
	U hi =    -rs ^ h2;
	U lo =    -rs ^ l2;

	return R(hi, lo);
}


/* multiply signed unsigned */

template <typename R>
R mulsu(typename R::stype x, typename R::utype y)
{
	typedef typename R::utype U;

	const int qb = sizeof(U) << 2;
	const U   mask = (U(1) << qb) - 1;

	U xs =    x < 0;
	U xu =    (~xs + 1) ^ (x - xs);
	U x0 =    xu       & mask;
	U x1 =    xu >> qb & mask;
	U y0 =    y        & mask;
	U y1 =    y >> qb  & mask;
	U z0 =    mulu<typename R::htype>(x0, y0);
	U z1 =    mulu<typename R::htype>(x1, y0);
	U z2 =    mulu<typename R::htype>(x0, y1);
	U z3 =    mulu<typename R::htype>(x1, y1);
	U z4 =    z1 + (z0 >> qb);
	U c1 =    z2 + (z4 & mask);
	U h1 =    z3 + (z4 >> qb) + (c1 >> qb);
	U l1 =    (c1 << qb) + (z0 & mask);
	U l2 =    l1 - xs;
	U hb =    l1 < l2;
	U h2 =    h1 - hb;
	U hi =    -xs ^ h2;
	U lo =    -xs ^ l2;

	return R(hi, lo);
}

/* multiply high unsigned unsigned */

template <typename U>
U mulhu(U x, U y)
{
	const int qb = sizeof(U) << 2;
	const U   mask = (U(1) << qb) - 1;

	U x0 =    x       & mask;
	U x1 =    x >> qb & mask;
	U y0 =    y       & mask;
	U y1 =    y >> qb & mask;
	U z0 =    x0 * y0;
	U z1 =    x1 * y0;
	U z2 =    x0 * y1;
	U z3 =    x1 * y1;
	U z4 =    z1 + (z0 >> qb);
	U c1 =    z2 + (z4 & mask);
	U hi =    z3 + (z4 >> qb) + (c1 >> qb);

	return hi;
}


/* multiply high signed signed */

template <typename S>
S mulh(S x, S y)
{
	typedef typename std::make_unsigned<S>::type U;

	const int qb = sizeof(U) << 2;
	const U   mask = (U(1) << qb) - 1;

	U xs =    x < 0;
	U ys =    y < 0;
	U xu =    (~xs + 1) ^ (x - xs);
	U yu =    (~ys + 1) ^ (y - ys);
	U x0 =    xu       & mask;
	U x1 =    xu >> qb & mask;
	U y0 =    yu       & mask;
	U y1 =    yu >> qb & mask;
	U z0 =    x0 * y0;
	U z1 =    x1 * y0;
	U z2 =    x0 * y1;
	U z3 =    x1 * y1;
	U z4 =    z1 + (z0 >> qb);
	U c1 =    z2 + (z4 & mask);
	U h1 =    z3 + (z4 >> qb) + (c1 >> qb);
	U l1 =    (c1 << qb) + (z0 & mask);
	U rs =    xs ^ ys;
	U l2 =    l1 - rs;
	U hb =    l1 < l2;
	U h2 =    h1 - hb;
	U hi =    -rs ^ h2;

	return hi;
}


/* multiply high signed unsigned */

template <typename S, typename U>
S mulhsu(S x, U y)
{
	const int qb = sizeof(U) << 2;
	const U   mask = (U(1) << qb) - 1;

	U xs =    x < 0;
	U xu =    (~xs + 1) ^ (x - xs);
	U x0 =    xu       & mask;
	U x1 =    xu >> qb & mask;
	U y0 =    y        & mask;
	U y1 =    y >> qb  & mask;
	U z0 =    x0 * y0;
	U z1 =    x1 * y0;
	U z2 =    x0 * y1;
	U z3 =    x1 * y1;
	U z4 =    z1 + (z0 >> qb);
	U c1 =    z2 + (z4 & mask);
	U h1 =    z3 + (z4 >> qb) + (c1 >> qb);
	U l1 =    (c1 << qb) + (z0 & mask);
	U l2 =    l1 - xs;
	U hb =    l1 < l2;
	U h2 =    h1 - hb;
	U hi =    -xs ^ h2;

	return hi;
}


/* test wrappers */

template <typename L, typename R>
void test_mul(typename R::stype x, typename R::stype y)
{
	R xy = mul<R>(x, y);
	L xya = (L(xy.val.p.hi) << (sizeof(L) << 2)) | L(xy.val.p.lo);
	L xyc = L(x) * L(y);
	bool pass = (xya == xyc);
	if (pass) return;
	char fmt[256];
	const int hw = sizeof(L);
	snprintf(fmt, sizeof(fmt), "%%s "
		"%%-11sx=%%-20lld (0x%%0%ullx)\n"
		"%%16sy=%%-20lld (0x%%0%ullx)\n"
		"%%12sxy.hi=%%-20lld (0x%%0%ullx)\n"
		"%%12sxy.lo=%%-20llu (0x%%0%ullx)\n"
		"%%9sxy.check=%%-20lld (0x%%0%ullx)\n",
		hw,hw,hw,hw,hw<<1);
	printf(fmt, pass ? "PASS" : "FAIL", __func__,
		(s64)x, (u64)x, "",
		(s64)y, (u64)y, "",
		(s64)xy.val.p.hi, (u64)xy.val.p.hi, "",
		(u64)xy.val.p.lo, (u64)xy.val.p.lo, "",
		(s64)xyc, (u64)xyc);
}

template <typename L, typename R>
void test_mulu(typename R::utype x, typename R::utype y)
{
	R xy = mulu<R>(x, y);
	L xya = (L(xy.val.p.hi) << (sizeof(L) << 2)) | L(xy.val.p.lo);
	L xyc = L(x) * L(y);
	bool pass = (xya == xyc);
	if (pass) return;
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
		(u64)y, (u64)y, "",
		(u64)xy.val.p.hi, (u64)xy.val.p.hi, "",
		(u64)xy.val.p.lo, (u64)xy.val.p.lo, "",
		(u64)xyc, (u64)xyc);
}

template <typename L, typename R>
void test_mulsu(typename R::stype x, typename R::utype y)
{
	R xy = mulsu<R>(x, y);
	L xya = (L(xy.val.p.hi) << (sizeof(L) << 2)) | L(xy.val.p.lo);
	L xyc = L(x) * L(y);
	bool pass = (xya == xyc);
	if (pass) return;
	char fmt[256];
	const int hw = sizeof(L);
	snprintf(fmt, sizeof(fmt), "%%s "
		"%%-11sx=%%-20lld (0x%%0%ullx)\n"
		"%%16sy=%%-20llu (0x%%0%ullx)\n"
		"%%12sxy.hi=%%-20lld (0x%%0%ullx)\n"
		"%%12sxy.lo=%%-20llu (0x%%0%ullx)\n"
		"%%9sxy.check=%%-20lld (0x%%0%ullx)\n",
		hw,hw,hw,hw,hw<<1);
	printf(fmt, pass ? "PASS" : "FAIL", __func__,
		(s64)x, (u64)x, "",
		(u64)y, (u64)y, "",
		(s64)xy.val.p.hi, (u64)xy.val.p.hi, "",
		(u64)xy.val.p.lo, (u64)xy.val.p.lo, "",
		(s64)xyc, (u64)xyc);
}


template <typename L, typename R, void (*T)(typename R::utype, typename R::utype), int N>
void test_random()
{
	std::mt19937 twister;
	twister.seed(5);
	std::uniform_int_distribution<typename R::utype> d1(
		std::numeric_limits<typename R::utype>::min(),
		std::numeric_limits<typename R::utype>::max()
	);
	for (size_t i = 0; i < N; i++) {
		T(d1(twister), d1(twister));
	}
}


template <typename L, typename R, void (*T)(typename R::stype, typename R::stype), int N>
void test_random()
{
	std::mt19937 twister;
	twister.seed(5);
	std::uniform_int_distribution<typename R::stype> d1(
		std::numeric_limits<typename R::stype>::min(),
		std::numeric_limits<typename R::stype>::max()
	);
	for (size_t i = 0; i < N; i++) {
		T(d1(twister), d1(twister));
	}
}

template <typename L, typename R, void (*T)(typename R::stype, typename R::utype), int N>
void test_random()
{
	std::mt19937 twister;
	twister.seed(5);
	std::uniform_int_distribution<typename R::stype> d1(
		std::numeric_limits<typename R::stype>::min(),
		std::numeric_limits<typename R::stype>::max()
	);
	std::uniform_int_distribution<typename R::utype> d2(
		std::numeric_limits<typename R::utype>::min(),
		std::numeric_limits<typename R::utype>::max()
	);
	for (size_t i = 0; i < N; i++) {
		T(d1(twister), d2(twister));
	}
}

/* test program */

int main()
{
	static const int random_iters = 100000;

	// test muls (signed signed)
	test_mul<s16,_s16>(0, 127);
	test_mul<s16,_s16>(77, 127);
	test_mul<s16,_s16>(127, 127);
	test_mul<s16,_s16>(-77, 127);
	test_mul<s16,_s16>(-127, 127);
	test_mul<s16,_s16>(-128, 127);
	test_mul<s16,_s16>(-77, -77);
	test_mul<s16,_s16>(-127, -127);
	test_mul<s16,_s16>(-128, -128);
	test_mul<s32,_s32>(0, 32767);
	test_mul<s32,_s32>(77, 32767);
	test_mul<s32,_s32>(32767, 32767);
	test_mul<s32,_s32>(-77, 32767);
	test_mul<s32,_s32>(-32767, 32767);
	test_mul<s32,_s32>(-32768, 32767);
	test_mul<s32,_s32>(-77, -77);
	test_mul<s32,_s32>(-32767, -32767);
	test_mul<s32,_s32>(-32768, -32768);
	test_mul<s64,_s64>(0, 2147483647);
	test_mul<s64,_s64>(77, 2147483647);
	test_mul<s64,_s64>(-77, 2147483647);
	test_mul<s64,_s64>(-77, -77);
	test_mul<s64,_s64>(2147483647, 2147483647);
	test_mul<s64,_s64>(-2147483647, 2147483647);
	test_mul<s64,_s64>(-2147483648, 2147483647);
	test_mul<s64,_s64>(-2147483647, -2147483647);
	test_mul<s64,_s64>(-2147483648, -2147483648);
	test_mul<signed __int128,_s128>(-9223372036854775807LL,9223372036854775807LL);
	test_mul<signed __int128,_s128>(9223372036854775807LL,9223372036854775807LL);
	test_random<s16,_s16,test_mul<s16,_s16>,random_iters>();
	test_random<s32,_s32,test_mul<s32,_s32>,random_iters>();
	test_random<s64,_s64,test_mul<s64,_s64>,random_iters>();
	test_random<signed __int128,_s128,test_mul<signed __int128,_s128>,random_iters>();

	// test mulsu (signed unsigned)
	test_mulsu<s16,_s16>(0, 127);
	test_mulsu<s16,_s16>(127, 1);
	test_mulsu<s16,_s16>(77, 127);
	test_mulsu<s16,_s16>(127, 127);
	test_mulsu<s16,_s16>(127, 254);
	test_mulsu<s16,_s16>(-127, 254);
	test_mulsu<s16,_s16>(-128, 254);
	test_mulsu<s16,_s16>(-77, 254);
	test_mulsu<s32,_s32>(0, 32767);
	test_mulsu<s32,_s32>(32767, 1);
	test_mulsu<s32,_s32>(77, 32767);
	test_mulsu<s32,_s32>(32767, 32767);
	test_mulsu<s32,_s32>(32767, 65535);
	test_mulsu<s32,_s32>(-32767, 65535);
	test_mulsu<s32,_s32>(-32768, 65535);
	test_mulsu<s32,_s32>(-77, 65535);
	test_mulsu<s64,_s64>(0, 2147483647);
	test_mulsu<s64,_s64>(2147483647, 1);
	test_mulsu<s64,_s64>(77, 2147483647);
	test_mulsu<s64,_s64>(-77, 4294967295);
	test_mulsu<s64,_s64>(2147483647, 2147483647);
	test_mulsu<s64,_s64>(2147483647, 4294967295);
	test_mulsu<s64,_s64>(-2147483647, 4294967295);
	test_mulsu<s64,_s64>(-2147483648, 4294967295);
	test_mulsu<signed __int128,_s128>(-9223372036854775807LL,18446744073709551615ULL);
	test_mulsu<signed __int128,_s128>(9223372036854775807LL,18446744073709551615ULL);
	test_random<s16,_s16,test_mulsu<s16,_s16>,random_iters>();
	test_random<s32,_s32,test_mulsu<s32,_s32>,random_iters>();
	test_random<s64,_s64,test_mulsu<s64,_s64>,random_iters>();
	test_random<signed __int128,_s128,test_mulsu<signed __int128,_s128>,random_iters>();

	// test mulu (unsigned unsigned)
	test_mulu<u16,_u16>(53, 63);
	test_mulu<u16,_u16>(53, 67);
	test_mulu<u16,_u16>(63, 191);
	test_mulu<u16,_u16>(254, 254);
	test_mulu<u32,_u32>(12345, 999);
	test_mulu<u32,_u32>(12345, 63000);
	test_mulu<u32,_u32>(65533, 65533);
	test_mulu<u64,_u64>(123450, 9990);
	test_mulu<u64,_u64>(123450, 630000);
	test_mulu<u64,_u64>(87652393, 87652393);
	test_mulu<u64,_u64>(613566756, 613566756);
	test_mulu<u64,_u64>(4294967295, 630000);
	test_mulu<u64,_u64>(4294967295, 87652393);
	test_mulu<u64,_u64>(4294967295, 613566756);
	test_mulu<u64,_u64>(4294967295, 4294967295);
	test_mulu<u64,_u64>(4294967295, 4294967295);
	test_mulu<unsigned __int128,_u128>(9223372036854775807ULL,18446744073709551615ULL);
	test_mulu<unsigned __int128,_u128>(18446744073709551615ULL,18446744073709551615ULL);
	test_random<u16,_s16,test_mulu<u16,_u16>,random_iters>();
	test_random<u32,_s32,test_mulu<u32,_u32>,random_iters>();
	test_random<u64,_s64,test_mulu<u64,_u64>,random_iters>();
	test_random<unsigned __int128,_s128,test_mulu<unsigned __int128,_u128>,random_iters>();

	// test mulhs (signed signed) high bits
	assert(mulh(s8(0), s8(127)) == s8(0));
	assert(mulh(s8(77), s8(127)) == s8(38));
	assert(mulh(s8(127), s8(127)) == s8(63));
	assert(mulh(s8(-77), s8(127)) == s8(-39));
	assert(mulh(s8(-127), s8(127)) == s8(-64));
	assert(mulh(s8(-128), s8(127)) == s8(-64));
	assert(mulh(s8(-77), s8(-77)) == s8(23));
	assert(mulh(s8(-127), s8(-127)) == s8(63));
	assert(mulh(s8(-128), s8(-128)) == s8(64));
	assert(mulh(s16(0), s16(32767)) == s16(0));
	assert(mulh(s16(77), s16(32767)) == s16(38));
	assert(mulh(s16(32767), s16(32767)) == s16(16383));
	assert(mulh(s16(-77), s16(32767)) == s16(-39));
	assert(mulh(s16(-32767), s16(32767)) == s16(-16384));
	assert(mulh(s16(-32768), s16(32767)) == s16(-16384));
	assert(mulh(s16(-77), s16(-77)) == s16(0));
	assert(mulh(s16(-32767), s16(-32767)) == s16(16383));
	assert(mulh(s16(-32768), s16(-32768)) == s16(16384));
	assert(mulh(s32(0), s32(2147483647)) == s32(0));
	assert(mulh(s32(77), s32(2147483647)) == s32(38));
	assert(mulh(s32(-77), s32(2147483647)) == s32(-39));
	assert(mulh(s32(-77), s32(-77)) == s32(0));
	assert(mulh(s32(2147483647), s32(2147483647)) == s32(1073741823));
	assert(mulh(s32(-2147483647), s32(2147483647)) == s32(-1073741824));
	assert(mulh(s32(-2147483648), s32(2147483647)) == s32(-1073741824));
	assert(mulh(s32(-2147483647), s32(-2147483647)) == s32(1073741823));
	assert(mulh(s32(-2147483648), s32(-2147483648)) == s32(1073741824));
	assert(mulh(s64(-9223372036854775807LL), s64(9223372036854775807LL)) == s64(-4611686018427387904LL));
	assert(mulh(s64(9223372036854775807LL), s64(9223372036854775807LL)) == s64(4611686018427387903LL));

	// test mulhsu (signed unsigned) high bits
	assert(mulhsu(s8(0), u8(127)) == s8(0));
	assert(mulhsu(s8(127), u8(1)) == s8(0));
	assert(mulhsu(s8(77), u8(127)) == s8(38));
	assert(mulhsu(s8(127), u8(127)) == s8(63));
	assert(mulhsu(s8(127), u8(254)) == s8(126));
	assert(mulhsu(s8(-127), u8(254)) == s8(-127));
	assert(mulhsu(s8(-128), u8(254)) == s8(-127));
	assert(mulhsu(s8(-77), u8(254)) == s8(-77));
	assert(mulhsu(s16(0), u16(32767)) == s16(0));
	assert(mulhsu(s16(32767), u16(1)) == s16(0));
	assert(mulhsu(s16(77), u16(32767)) == s16(38));
	assert(mulhsu(s16(32767), u16(32767)) == s16(16383));
	assert(mulhsu(s16(32767), u16(65535)) == s16(32766));
	assert(mulhsu(s16(-32767), u16(65535)) == s16(-32767));
	assert(mulhsu(s16(-32768), u16(65535)) == s16(-32768));
	assert(mulhsu(s16(-77), u16(65535)) == s16(-77));
	assert(mulhsu(s32(0), u32(2147483647)) == s32(0));
	assert(mulhsu(s32(2147483647), u32(1)) == s32(0));
	assert(mulhsu(s32(77), u32(2147483647)) == s32(38));
	assert(mulhsu(s32(-77), u32(4294967295)) == s32(-77));
	assert(mulhsu(s32(2147483647), u32(2147483647)) == s32(1073741823));
	assert(mulhsu(s32(2147483647), u32(4294967295)) == s32(2147483646));
	assert(mulhsu(s32(-2147483647), u32(4294967295)) == s32(-2147483647));
	assert(mulhsu(s32(-2147483648), u32(4294967295)) == s32(-2147483648));
	assert(mulhsu(s64(-9223372036854775807LL), u64(18446744073709551615ULL)) == s64(-9223372036854775807LL));
	assert(mulhsu(s64(9223372036854775807LL), u64(18446744073709551615ULL)) == s64(9223372036854775806LL));

	// test mulhu (unsigned unsigned) high bits
	assert(mulhu(u8(53), u8(63)) == u8(13));
	assert(mulhu(u8(53), u8(67)) == u8(13));
	assert(mulhu(u8(63), u8(191)) == u8(47));
	assert(mulhu(u8(254), u8(254)) == u8(252));
	assert(mulhu(u16(12345), u16(999)) == u16(188));
	assert(mulhu(u16(12345), u16(63000)) == u16(11867));
	assert(mulhu(u16(65533), u16(65533)) == u16(65530));
	assert(mulhu(u32(123450), u32(9990)) == u32(0));
	assert(mulhu(u32(123450), u32(630000)) == u32(18));
	assert(mulhu(u32(87652393), u32(87652393)) == u32(1788824));
	assert(mulhu(u32(613566756), u32(613566756)) == u32(87652393));
	assert(mulhu(u32(4294967295), u32(630000)) == u32(629999));
	assert(mulhu(u32(4294967295), u32(87652393)) == u32(87652392));
	assert(mulhu(u32(4294967295), u32(613566756)) == u32(613566755));
	assert(mulhu(u32(4294967295), u32(4294967295)) == u32(4294967294));
	assert(mulhu(u64(9223372036854775807ULL), u64(18446744073709551615ULL)) == u64(9223372036854775806ULL));
	assert(mulhu(u64(18446744073709551615ULL), u64(18446744073709551615ULL)) == u64(18446744073709551614ULL));

	return 0;
}
