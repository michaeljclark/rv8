//
//  riscv-fpu.h
//

#ifndef riscv_fpu_h
#define riscv_fpu_h

namespace riscv {

	/* IEEE-754 Single-precision floating point */

	union f32_bits {
		f32 f;
		u32 u;
		struct {
		#if _BYTE_ORDER == _LITTLE_ENDIAN
			u32 man  : 23;
			u32 exp  : 8;
			u32 sign : 1;
		#else
			u32 sign : 1;
			u32 exp  : 8;
			u32 man  : 23;
		#endif
		} r;
	};

	struct f32_type {
		typedef f32      float_type;
		typedef u32      size_type;
		typedef f32_bits bits_type;
		enum {
			man_size = 23,
			man_digits = man_size + 1,
			exp_size = 8,
			exp_max = 128,
			exp_denorm = 0,
			exp_inf = (1 << exp_size) - 1,
			dec_digits = 9
		};
	};

	/* IEEE-754 Double-precision floating point */

	union f64_bits {
		f64 f;
		u64 u;
		struct {
		#if _BYTE_ORDER == _LITTLE_ENDIAN
			u64 man  : 52;
			u64 exp  : 11;
			u64 sign : 1;
		#else
			u64 sign : 1;
			u64 exp  : 11;
			u64 man  : 52;
		#endif
		} r;
		struct {
		#if _BYTE_ORDER == _LITTLE_ENDIAN
			u32 d1;
			u32 d0;
		#else
			u32 d0;
			u32 d1;
		#endif
		} w;
	};

	struct f64_type {
		typedef f64      float_type;
		typedef u64      size_type;
		typedef f64_bits bits_type;
		enum {
			man_size = 52,
			man_digits = man_size + 1,
			exp_size = 11,
			exp_max = 1024,
			exp_denorm = 0,
			exp_inf = (1 << exp_size) - 1,
			dec_digits = 17
		};
	};

	/* get accrued exception flags (C11) */

	template <typename T>
	inline void fenv_getflags(T &fcsr)
	{
		int flags = fetestexcept(FE_ALL_EXCEPT);
		if (flags & FE_DIVBYZERO) fcsr |= riscv_fcsr_DZ;
		if (flags & FE_INEXACT) fcsr |= riscv_fcsr_NX;
		if (flags & FE_INVALID) fcsr |= riscv_fcsr_NV;
		if (flags & FE_OVERFLOW) fcsr |= riscv_fcsr_OF;
		if (flags & FE_UNDERFLOW) fcsr |= riscv_fcsr_UF;
	}

	/* clear accrued exception flags (C11) */

	template <typename T>
	inline void fenv_clearflags(T &fcsr)
	{
		int flags = 0;
		if (!(fcsr & riscv_fcsr_DZ)) flags |= FE_DIVBYZERO;
		if (!(fcsr & riscv_fcsr_NX)) flags |= FE_INEXACT;
		if (!(fcsr & riscv_fcsr_NV)) flags |= FE_INVALID;
		if (!(fcsr & riscv_fcsr_OF)) flags |= FE_OVERFLOW;
		if (!(fcsr & riscv_fcsr_UF)) flags |= FE_UNDERFLOW;
		feclearexcept(flags);
	}

	/* set round mode (C11) */

	inline void fenv_setrm(int rm)
	{
		if (rm == 0b111) return;
		switch (rm) {
			case riscv_rm_rne: fesetround(FE_TONEAREST); /* ties to Even */ break;
			case riscv_rm_rtz: fesetround(FE_TOWARDZERO); break;
			case riscv_rm_rdn: fesetround(FE_DOWNWARD); break;
			case riscv_rm_rup: fesetround(FE_UPWARD); break;
			case riscv_rm_rmm: fesetround(FE_TONEAREST); /* ties to Max Magnitude */ break;
		}
	}

	/* convert single or double to signed word (32-bit) */

	template <typename T, typename F>
	inline s32 fcvt_w(T &fcsr, F f)
	{
		return (((f > std::numeric_limits<s32>::max()) & (s32(f) < 0)) | std::isnan(f))
			? std::numeric_limits<s32>::max()
			: s32(f);
	}

	/* convert single or double to unsigned word (32-bit) */

	template <typename T, typename F>
	inline s32 fcvt_wu(T &fcsr, F f)
	{
		return (std::isnan(f) | ((f >= 0) & std::isinf(f)))
			? std::numeric_limits<u32>::max()
			: f >= 0
			? s32(u32(f))
			: f > -1
			? (fcsr |= riscv_fcsr_NX, s32(0))
			: (fcsr |= riscv_fcsr_NV, s32(0));
	}

	/* convert single or double to signed long (64-bit) */

	template <typename T, typename F>
	inline s64 fcvt_l(T &fcsr, F f)
	{
		return (((f > std::numeric_limits<s64>::max()) & (s64(f) < 0)) | std::isnan(f))
			? std::numeric_limits<s64>::max()
			: s64(f);
	}

	/* convert single or double to unsigned long (64-bit) */

	template <typename T, typename F>
	inline s64 fcvt_lu(T &fcsr, F f)
	{
		return (std::isnan(f) | ((f >= 0) & std::isinf(f)))
			? std::numeric_limits<u64>::max()
			: f >= 0
			? s64(u64(f))
			: f > -1
			? (fcsr |= riscv_fcsr_NX, s64(0))
			: (fcsr |= riscv_fcsr_NV, s64(0));
	}

	/* floating point square root */

	inline f32 f32_sqrt(f32 a) { return std::sqrt(a); }
	inline f64 f64_sqrt(f64 a) { return std::sqrt(a); }

	/* floating point classify */

	template <typename F>
	inline int float_classify(typename F::float_type f)
	{
		typename F::bits_type v{ .f = f };
		typename F::size_type neg = v.r.sign, exp = v.r.exp, man = v.r.man;
		return (exp == F::exp_denorm)
			? (man == 0)
			? (neg ? riscv_fclass_neg_zero : riscv_fclass_pos_zero)
			: (neg ? riscv_fclass_neg_subnorm : riscv_fclass_pos_subnorm)
			: (exp == F::exp_inf)
			? (man == 0)
			? (neg ? riscv_fclass_neg_inf : riscv_fclass_pos_inf)
			: (man & 1 ? riscv_fclass_signaling_nan : riscv_fclass_quiet_nan)
			: (neg ? riscv_fclass_neg_norm : riscv_fclass_pos_norm);
	}

	inline int f32_classify(f32 f) { return float_classify<f32_type>(f); }
	inline int f64_classify(f64 f) { return float_classify<f64_type>(f); }

}

#endif
