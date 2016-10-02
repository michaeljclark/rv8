//
//  riscv-operators.h
//

#ifndef riscv_operators_h
#define riscv_operators_h


namespace riscv
{

	/* u128 - bitwise operators cause the compiler to generate constant time code */

	inline bool operator==(const u128 &x, const u128 &y)
	{
		return (x.r.d.lo == y.r.d.lo) & (x.r.d.hi == y.r.d.hi);
	}

	inline bool operator!=(const u128 &x, const u128 &y)
	{
		return (x.r.d.lo != y.r.d.lo) | (x.r.d.hi != y.r.d.hi);
	}

	inline bool operator<(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo < y.r.d.lo));
	}

	inline bool operator>(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo > y.r.d.lo));
	}

	inline bool operator>=(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo >= y.r.d.lo));
	}

	inline bool operator<=(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo <= y.r.d.lo));
	}

	inline u128 operator&(const u128 &x, const u128 &y)
	{
		return u128(x.r.d.hi & y.r.d.hi, x.r.d.lo & y.r.d.lo);
	}

	inline u128 operator|(const u128 &x, const u128 &y)
	{
		return u128(x.r.d.hi | y.r.d.hi, x.r.d.lo | y.r.d.lo);
	}

	inline u128 operator^(const u128 &x, const u128 &y)
	{
		return u128(x.r.d.hi ^ y.r.d.hi, x.r.d.lo ^ y.r.d.lo);
	}

	inline u128 operator~(const u128 &x)
	{
		return u128(~x.r.d.hi, ~x.r.d.lo);
	}

	inline u128 operator&(const u128 &x, s32 y)
	{
		return u128(x.r.d.hi, x.r.d.lo & y);
	}

	inline u128 operator|(const u128 &x, s32 y)
	{
		return u128(x.r.d.hi, x.r.d.lo | y);
	}

	inline u128 operator^(const u128 &x, s32 y)
	{
		return u128(x.r.d.hi, x.r.d.lo ^ y);
	}

	inline u128 operator<<(const u128 &x, int shift)
	{
		// todo - use predicate logic instead of ternary operator - violates constant time property
		u64 hi = (shift == 0) ? x.r.d.hi : (shift < 64) ? (x.r.d.hi << shift) | (x.r.d.lo >> (64 - shift)) : x.r.d.lo << (shift - 64);
		u64 lo = (shift < 64) ? (x.r.d.lo << shift) : 0;
		return u128(hi, lo);
	}

	inline u128 operator>>(const u128 &x, int shift)
	{
		// todo - use predicate logic instead of ternary operator - violates constant time property
		u64 hi = (shift < 64) ? (x.r.d.hi >> shift) : 0;
		u64 lo = (shift == 0) ? x.r.d.lo : (shift < 64) ? (x.r.d.lo >> shift) | (x.r.d.hi << (64 - shift)) : x.r.d.hi >> (shift - 64);
		return u128(hi, lo);
	}

	inline u128 operator+(const u128 &x, const u128 &y)
	{
		u64 lo = x.r.d.lo + y.r.d.lo;
		u64 c = lo < x.r.d.lo;
		u64 hi = x.r.d.hi + y.r.d.hi + c;
		return u128(hi, lo);
	}

	inline u128 operator-(const u128 &x, const u128 &y)
	{
		u64 lo = x.r.d.lo - y.r.d.lo;
		u64 b = lo > x.r.d.lo;
		u64 hi = x.r.d.hi - y.r.d.hi - b;
		return u128(hi, lo);
	}

	inline u128 operator-(const u128 &x)
	{
		return ~x + 1;
	}

	inline u128 operator*(const u128 &x, const u128 &y)
	{
		u128 z0 = x.r.d.lo * y.r.d.lo;
		u128 z1 = x.r.d.hi * y.r.d.lo;
		u128 z2 = x.r.d.lo * y.r.d.hi;
		u128 z4 = z1 + z2;
		u128 c1 = z4 < z1;
		return ((z4 + c1) << 64) + z0;
	}

	inline u128 operator/(const u128 &x, const u128 &y)
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

	inline u128 operator%(const u128 &x, const u128 &y)
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

	inline bool operator==(const s128 &x, const s128 &y)
	{
		return (x.r.d.lo == y.r.d.lo) & (x.r.d.hi == y.r.d.hi);
	}

	inline bool operator!=(const s128 &x, const s128 &y)
	{
		return (x.r.d.lo != y.r.d.lo) | (x.r.d.hi != y.r.d.hi);
	}

	inline bool operator<(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo < y.r.d.lo));
	}

	inline bool operator>(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo > y.r.d.lo));
	}

	inline bool operator>=(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo >= y.r.d.lo));
	}

	inline bool operator<=(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi < y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo <= y.r.d.lo));
	}

	inline s128 operator&(const s128 &x, const s128 &y)
	{
		return s128(x.r.d.hi & y.r.d.hi, x.r.d.lo & y.r.d.lo);
	}

	inline s128 operator|(const s128 &x, const s128 &y)
	{
		return s128(x.r.d.hi | y.r.d.hi, x.r.d.lo | y.r.d.lo);
	}

	inline s128 operator^(const s128 &x, const s128 &y)
	{
		return s128(x.r.d.hi ^ y.r.d.hi, x.r.d.lo ^ y.r.d.lo);
	}

	inline s128 operator~(const s128 &x)
	{
		return s128(~x.r.d.hi, ~x.r.d.lo);
	}

	inline s128 operator&(const s128 &x, s32 y)
	{
		return s128(x.r.d.hi, x.r.d.lo & y);
	}

	inline s128 operator|(const s128 &x, s32 y)
	{
		return s128(x.r.d.hi, x.r.d.lo | y);
	}

	inline s128 operator^(const s128 &x, s32 y)
	{
		return s128(x.r.d.hi, x.r.d.lo ^ y);
	}

	inline s128 operator<<(const s128 &x, int shift)
	{
		// todo - use predicate logic instead of ternary operator - violates constant time property
		u64 hi = (shift == 0) ? x.r.d.hi : (shift < 64) ? (x.r.d.hi << shift) | (x.r.d.lo >> (64 - shift)) : x.r.d.lo << (shift - 64);
		u64 lo = (shift < 64) ? (x.r.d.lo << shift) : 0;
		return s128(hi, lo);
	}

	inline s128 operator>>(const s128 &x, int shift)
	{
		// todo - use predicate logic instead of ternary operator - violates constant time property
		s64 hi = (shift < 64) ? (x.r.d.hi >> shift) : -(u64(x.r.d.hi) >> 63);
		u64 lo = (shift == 0) ? x.r.d.lo : (shift < 64) ? (x.r.d.lo >> shift) | (x.r.d.hi << (64 - shift)) : x.r.d.hi >> (shift - 64);
		return s128(hi, lo);
	}

	inline s128 operator+(const s128 &x, const s128 &y)
	{
		u64 lo = x.r.d.lo + y.r.d.lo;
		u64 c = lo < x.r.d.lo;
		s64 hi = x.r.d.hi + y.r.d.hi + c;
		return s128(hi, lo);
	}

	inline s128 operator-(const s128 &x, const s128 &y)
	{
		u64 lo = x.r.d.lo - y.r.d.lo;
		u64 b = lo > x.r.d.lo;
		s64 hi = x.r.d.hi - y.r.d.hi - b;
		return s128(hi, lo);
	}

	inline s128 operator-(const s128 &x)
	{
		return ~x + 1;
	}

	inline s128 operator*(const s128 &x, const s128 &y)
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
