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

	inline bool operator>=(const u128 &x, const u128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo >= y.r.d.lo));
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

	inline u128 operator<<(const u128 &x, int shift)
	{
		u64 lo = x.r.d.lo << shift;
		u64 hi = (x.r.d.hi << shift) | (x.r.d.lo >> (64 - shift));
		return u128(hi, lo);
	}

	inline u128 operator>>(const u128 &x, int shift)
	{
		u64 lo = (x.r.d.lo >> shift) | (x.r.d.hi << (64 - shift));
		u64 hi = x.r.d.hi >> shift;
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

	inline bool operator>=(const s128 &x, const s128 &y)
	{
		return (x.r.d.hi > y.r.d.hi) | ((x.r.d.hi == y.r.d.hi) & (x.r.d.lo >= y.r.d.lo));
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

	inline s128 operator>>(const s128 &x, int shift)
	{
		u64 lo = (x.r.d.lo >> shift) | (x.r.d.hi << (64 - shift));
		s64 hi = x.r.d.hi >> shift;
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

}

#endif
