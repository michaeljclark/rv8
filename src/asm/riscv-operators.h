//
//  riscv-operators.h
//

#ifndef riscv_operators_h
#define riscv_operators_h


namespace riscv
{

	/* bitwise operators cause the compiler to generate constant time code */

	inline bool operator==(u128 const&x, u128 const& y)
	{
		return (x.r.d.lo == y.r.d.lo) & (x.r.d.hi == y.r.d.hi);
	}

	inline bool operator!=(u128 const&x, u128 const& y)
	{
		return (x.r.d.lo != y.r.d.lo) | (x.r.d.hi != y.r.d.hi);
	}

	inline bool operator==(s128 const&x, s128 const& y)
	{
		return (x.r.d.lo == y.r.d.lo) & (x.r.d.hi == y.r.d.hi);
	}

	inline bool operator!=(s128 const&x, s128 const& y)
	{
		return (x.r.d.lo != y.r.d.lo) | (x.r.d.hi != y.r.d.hi);
	}

}

#endif
