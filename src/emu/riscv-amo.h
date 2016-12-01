//
//  riscv-amo.h
//

#ifndef riscv_amo_h
#define riscv_amo_h

namespace riscv {

	/* AMO operations */

	enum amo_op {
		amoswap,
		amoadd,
		amoxor,
		amoor,
		amoand,
		amomin,
		amomax,
		amominu,
		amomaxu
	};

	template <typename UX> UX amoswap_fn(UX x, UX y) { return y; }
	template <typename UX> UX amoadd_fn(UX x, UX y) { return x + y; }
	template <typename UX> UX amoxor_fn(UX x, UX y) { return x ^ y; }
	template <typename UX> UX amoor_fn(UX x, UX y) { return x | y; }
	template <typename UX> UX amoand_fn(UX x, UX y) { return x & y; }

	template <typename UX> UX amomin_fn(UX x, UX y) {
		return std::min(typename std::make_signed<UX>::type(x),
			typename std::make_signed<UX>::type(y));
	}

	template <typename UX> UX amomax_fn(UX x, UX y) {
		return std::max(typename std::make_signed<UX>::type(x),
			typename std::make_signed<UX>::type(y));
	}

	template <typename UX> UX amominu_fn(UX x, UX y) {
		return std::min(typename std::make_unsigned<UX>::type(x),
			typename std::make_unsigned<UX>::type(y));
	}

	template <typename UX> UX amomaxu_fn(UX x, UX y) {
		return std::max(typename std::make_unsigned<UX>::type(x),
			typename std::make_unsigned<UX>::type(y));
	}

	template <typename UX> UX amo_fn(amo_op op, UX x, UX y) {
		switch (op) {
			case amoswap: return amoswap_fn(x, y);
			case amoadd:  return amoadd_fn (x, y);
			case amoxor:  return amoxor_fn (x, y);
			case amoor:   return amoor_fn  (x, y);
			case amoand:  return amoand_fn (x, y);
			case amomin:  return amomin_fn (x, y);
			case amomax:  return amomax_fn (x, y);
			case amominu: return amominu_fn(x, y);
			case amomaxu: return amomaxu_fn(x, y);
		}
	}
}

#endif
