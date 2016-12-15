//
//  operators.h
//

#ifndef rv_operators_h
#define rv_operators_h

#ifdef NO_INLINE
#define MAYBE_INLINE
#else
#if __GNUC__ >= 5
#define MAYBE_INLINE __attribute__((gnu_inline)) inline
#else
#define MAYBE_INLINE inline
#endif
#endif

namespace riscv
{

	/* u128 - bitwise operators cause the compiler to generate constant time code */

	MAYBE_INLINE bool operator==(const u128 &x, const u128 &y)
	{
		return (x.r.d.lo == y.r.d.lo) & (x.r.d.hi == y.r.d.hi);
	}

	MAYBE_INLINE bool operator!=(const u128 &x, const u128 &y)
	{
		return (x.r.d.lo != y.r.d.lo) | (x.r.d.hi != y.r.d.hi);
	}

	MAYBE_INLINE bool operator<(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo < y.r.d.lo));
	}

	MAYBE_INLINE bool operator>(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo > y.r.d.lo));
	}

	MAYBE_INLINE bool operator>=(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo >= y.r.d.lo));
	}

	MAYBE_INLINE bool operator<=(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo <= y.r.d.lo));
	}

	MAYBE_INLINE u128 operator&(const u128 &x, const u128 &y)
	{
		return u128(x.r.d.hi & y.r.d.hi, x.r.d.lo & y.r.d.lo);
	}

	MAYBE_INLINE u128 operator|(const u128 &x, const u128 &y)
	{
		return u128(x.r.d.hi | y.r.d.hi, x.r.d.lo | y.r.d.lo);
	}

	MAYBE_INLINE u128 operator^(const u128 &x, const u128 &y)
	{
		return u128(x.r.d.hi ^ y.r.d.hi, x.r.d.lo ^ y.r.d.lo);
	}

	MAYBE_INLINE u128 operator~(const u128 &x)
	{
		return u128(~x.r.d.hi, ~x.r.d.lo);
	}

	MAYBE_INLINE u128 operator&(const u128 &x, s32 y)
	{
		return u128(x.r.d.hi, x.r.d.lo & y);
	}

	MAYBE_INLINE u128 operator|(const u128 &x, s32 y)
	{
		return u128(x.r.d.hi, x.r.d.lo | y);
	}

	MAYBE_INLINE u128 operator^(const u128 &x, s32 y)
	{
		return u128(x.r.d.hi, x.r.d.lo ^ y);
	}

	MAYBE_INLINE u128 operator<<(const u128 &x, int shamt)
	{
#if defined PREDICATED_SHIFT
		u64 s = shamt & 127;
		u64 sg0 = s > 0, sl64 = s < 64, sge64 = s >= 64;
		u64 hi0 = -sl64 & (x.r.d.hi << s);
		u64 hi1 = -(sg0 & sl64) & ((x.r.d.lo >> (64 - s)));
		u64 hi2 = -sge64 & (x.r.d.lo << (s - 64));
		u64 hi = hi0 | hi1 | hi2;
		u64 lo = -sl64 & (x.r.d.lo << s);
		return u128(hi, lo);
#else
		u64 s = shamt & 127;
		if (s == 0) {
			return x;
		} else if (s < 64) {
			u64 hi = (x.r.d.hi << s) | (x.r.d.lo >> (64 - s));
			u64 lo = x.r.d.lo << s;
			return u128(hi, lo);
		} else {
			u64 hi = x.r.d.lo << (s - 64);
			return u128(hi, 0);
		}
#endif
	}

	MAYBE_INLINE u128 operator>>(const u128 &x, int shamt)
	{
#if defined PREDICATED_SHIFT
		u64 s = shamt & 127;
		u64 sg0 = s > 0, sl64 = s < 64, sge64 = s >= 64;
		u64 hi = -sl64 & (x.r.d.hi >> s);
		u64 lo0 = -sl64 & (x.r.d.lo >> s);
		u64 lo1 = -(sg0 & sl64) & (x.r.d.hi << (64 - s));
		u64 lo2 = -sge64 & (x.r.d.hi >> (s - 64));
		u64 lo = lo0 | lo1 | lo2;
		return u128(hi, lo);
#else
		u64 s = shamt & 127;
		if (s == 0) {
			return x;
		} else if (s < 64) {
			u64 hi = x.r.d.hi >> s;
			u64 lo = (x.r.d.lo >> s) | (x.r.d.hi << (64 - s));
			return u128(hi, lo);
		} else {
			u64 lo = x.r.d.hi >> (s - 64);
			return u128(0, lo);
		}
#endif
	}

	MAYBE_INLINE u128 operator+(const u128 &x, const u128 &y)
	{
		u64 lo = x.r.d.lo + y.r.d.lo;
		u64 c = lo < x.r.d.lo;
		u64 hi = x.r.d.hi + y.r.d.hi + c;
		return u128(hi, lo);
	}

	MAYBE_INLINE u128 operator-(const u128 &x, const u128 &y)
	{
		u64 lo = x.r.d.lo - y.r.d.lo;
		u64 b = lo > x.r.d.lo;
		u64 hi = x.r.d.hi - y.r.d.hi - b;
		return u128(hi, lo);
	}

	MAYBE_INLINE u128 operator-(const u128 &x)
	{
		return ~x + 1;
	}

	MAYBE_INLINE u128 operator*(const u128 &x, const u128 &y)
	{
		u128 z0 = x.r.d.lo * y.r.d.lo;
		u128 z1 = x.r.d.hi * y.r.d.lo;
		u128 z2 = x.r.d.lo * y.r.d.hi;
		u128 z4 = z1 + z2;
		u128 c1 = z4 < z1;
		return ((z4 + c1) << 64) + z0;
	}

	MAYBE_INLINE u128 operator/(const u128 &x, const u128 &y)
	{
		if (y == 0) return u128(-1,-1);
		u128 q = 0, r = 0;
		for (int i = 127; i >= 0; i--) {
			r = r << 1;
			r = r | ((x >> i) & u128(1));
			u128 d = r >= y;
			r = r - (y & -d);
			q = q | (d << i);
		}
		return q;
	}

	MAYBE_INLINE u128 operator%(const u128 &x, const u128 &y)
	{
		if (y == 0) return x;
		u128 q = 0, r = 0;
		for (int i = 127; i >= 0; i--) {
			r = r << 1;
			r = r | ((x >> i) & u128(1));
			u128 d = r >= y;
			r = r - (y & -d);
			q = q | (d << i);
		}
		return r;
	}

	/* s128 - bitwise operators cause the compiler to generate constant time code */

	MAYBE_INLINE bool operator==(const s128 &x, const s128 &y)
	{
		return (x.r.d.lo == y.r.d.lo) & (x.r.d.hi == y.r.d.hi);
	}

	MAYBE_INLINE bool operator!=(const s128 &x, const s128 &y)
	{
		return (x.r.d.lo != y.r.d.lo) | (x.r.d.hi != y.r.d.hi);
	}

	MAYBE_INLINE bool operator<(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo < y.r.d.lo));
	}

	MAYBE_INLINE bool operator>(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo > y.r.d.lo));
	}

	MAYBE_INLINE bool operator>=(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo >= y.r.d.lo));
	}

	MAYBE_INLINE bool operator<=(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo <= y.r.d.lo));
	}

	MAYBE_INLINE s128 operator&(const s128 &x, const s128 &y)
	{
		return s128(x.r.d.hi & y.r.d.hi, x.r.d.lo & y.r.d.lo);
	}

	MAYBE_INLINE s128 operator|(const s128 &x, const s128 &y)
	{
		return s128(x.r.d.hi | y.r.d.hi, x.r.d.lo | y.r.d.lo);
	}

	MAYBE_INLINE s128 operator^(const s128 &x, const s128 &y)
	{
		return s128(x.r.d.hi ^ y.r.d.hi, x.r.d.lo ^ y.r.d.lo);
	}

	MAYBE_INLINE s128 operator~(const s128 &x)
	{
		return s128(~x.r.d.hi, ~x.r.d.lo);
	}

	MAYBE_INLINE s128 operator&(const s128 &x, s32 y)
	{
		return s128(x.r.d.hi, x.r.d.lo & y);
	}

	MAYBE_INLINE s128 operator|(const s128 &x, s32 y)
	{
		return s128(x.r.d.hi, x.r.d.lo | y);
	}

	MAYBE_INLINE s128 operator^(const s128 &x, s32 y)
	{
		return s128(x.r.d.hi, x.r.d.lo ^ y);
	}

	MAYBE_INLINE s128 operator<<(const s128 &x, int shamt)
	{
#if defined PREDICATED_SHIFT
		u64 s = shamt & 127;
		u64 sg0 = s > 0, sl64 = s < 64, sge64 = s >= 64;
		u64 hi0 = -sl64 & (x.r.d.hi << s);
		u64 hi1 = -(sg0 & sl64) & ((x.r.d.lo >> (64 - s)));
		u64 hi2 = -sge64 & (x.r.d.lo << (s - 64));
		u64 hi = hi0 | hi1 | hi2;
		u64 lo = -sl64 & (x.r.d.lo << s);
		return s128(hi, lo);
#else
		u64 s = shamt & 127;
		if (s == 0) {
			return x;
		} else if (s < 64) {
			u64 hi = (x.r.d.hi << s) | (x.r.d.lo >> (64 - s));
			u64 lo = x.r.d.lo << s;
			return s128(hi, lo);
		} else {
			u64 hi = x.r.d.lo << (s - 64);
			return s128(hi, 0);
		}
#endif
	}

	MAYBE_INLINE s128 operator>>(const s128 &x, int shamt)
	{
#if defined PREDICATED_SHIFT
		u64 s = shamt & 127;
		u64 sg0 = s > 0, sl64 = s < 64, sge64 = s >= 64;
		u64 hi0 = -sl64 & (x.r.d.hi >> s);
		u64 hi1 = -!sl64 & -(u64(x.r.d.hi) >> 63);
		u64 hi = hi0 | hi1;
		u64 lo0 = -sl64 & (x.r.d.lo >> s);
		u64 lo1 = -(sg0 & sl64) & (x.r.d.hi << (64 - s));
		u64 lo2 = -sge64 & (x.r.d.hi >> (s - 64));
		u64 lo = lo0 | lo1 | lo2;
		return s128(hi, lo);
#else
		u64 s = shamt & 127;
		if (s == 0) {
			return x;
		} else if (s < 64) {
			u64 hi = x.r.d.hi >> s;
			u64 lo = (x.r.d.lo >> s) | (x.r.d.hi << (64 - s));
			return s128(hi, lo);
		} else {
			u64 hi = -(u64(x.r.d.hi) >> 63);
			u64 lo = x.r.d.hi >> (s - 64);
			return s128(hi, lo);
		}
#endif
	}

	MAYBE_INLINE s128 operator+(const s128 &x, const s128 &y)
	{
		u64 lo = x.r.d.lo + y.r.d.lo;
		u64 c = lo < x.r.d.lo;
		s64 hi = x.r.d.hi + y.r.d.hi + c;
		return s128(hi, lo);
	}

	MAYBE_INLINE s128 operator-(const s128 &x, const s128 &y)
	{
		u64 lo = x.r.d.lo - y.r.d.lo;
		u64 b = lo > x.r.d.lo;
		s64 hi = x.r.d.hi - y.r.d.hi - b;
		return s128(hi, lo);
	}

	MAYBE_INLINE s128 operator-(const s128 &x)
	{
		return ~x + 1;
	}

	MAYBE_INLINE s128 operator*(const s128 &x, const s128 &y)
	{
		s128 z0 = x.r.d.lo * y.r.d.lo;
		s128 z1 = x.r.d.hi * y.r.d.lo;
		s128 z2 = x.r.d.lo * y.r.d.hi;
		s128 z4 = z1 + z2;
		s128 c1 = z4 < z1;
		return ((z4 + c1) << 64) + z0;
	}

}

#endif
