//
//  alu.h
//

#ifndef rv_alu_h
#define rv_alu_h

namespace riscv {

	/* multiply high unsigned */

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

	/* multiply high signed */

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

	/* divide signed */

	template <typename S>
	S div(S x, S y)
	{
		return (x == std::numeric_limits<S>::min() && y == -1) ? std::numeric_limits<S>::min() : (y == 0) ? S(-1) : x / y;
	}

	/* divide unsigned */

	template <typename U>
	U divu(U x, U y)
	{
		return (y == 0) ? U(-1) : x / y;
	}

	/* remainder signed */

	template <typename S>
	S rem(S x, S y)
	{
		return (x == std::numeric_limits<S>::min() && y == -1) ? 0 : (y == 0) ? x : x % y;
	}

	/* remainder unsigned */

	template <typename U>
	U remu(U x, U y)
	{
		return (y == 0) ? x : x % y;
	}

}

#endif
