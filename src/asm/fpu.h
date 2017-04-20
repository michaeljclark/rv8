//
//  fpu.h
//

#ifndef rv_fpu_h
#define rv_fpu_h

namespace riscv {

#if defined __GNUC__ && defined __x86_64__ && defined __SSE__
#define HAVE_SSE_MATH
#endif

#if !defined HAVE_SSE_MATH
#undef USE_SSE_MATH
#endif

#if USE_SSE_MATH

	/* i786 RISC-V Floating Point Emulation */

	enum {
		x86_mxcsr_EF_IE = 0x0001,   /* 0 : Invalid operation flag */
		x86_mxcsr_EF_DE = 0x0002,   /* 1 : Denormal flag */
		x86_mxcsr_EF_ZE = 0x0004,   /* 2 : Divide-by-zero flag */
		x86_mxcsr_EF_OE = 0x0008,   /* 3 : Overflow flag */
		x86_mxcsr_EF_UE = 0x0010,   /* 4 : Underflow flag */
		x86_mxcsr_EF_PE = 0x0020,   /* 5 : Precision flag */
		x86_mxcsr_DAZ   = 0x0040,   /* 6 : Denormals are zeros */
		x86_mxcsr_EM_IM = 0x0080,   /* 7 : Invalid operation mask */
		x86_mxcsr_EM_DM = 0x0100,   /* 8 : Denormal mask */
		x86_mxcsr_EM_ZM = 0x0200,   /* 9 : Divide-by-zero mask */
		x86_mxcsr_EM_OM = 0x0400,   /* 10: Overflow mask */
		x86_mxcsr_EM_UM = 0x0800,   /* 11: Underflow mask */
		x86_mxcsr_EM_PM = 0x1000,   /* 12: Precision mask */
		x86_mxcsr_RC_RN = 0x0000,   /*     To nearest rounding mode */
		x86_mxcsr_RC_DN = 0x2000,   /* 13: Toward negative infinity rounding mode */
		x86_mxcsr_RC_UP = 0x4000,   /* 14: Toward positive infinity rounding mode */
		x86_mxcsr_RC_RZ = 0x6000,   /*     Toward zero rounding mode */
		x86_mxcsr_FZ    = 0x8000,   /* 15: Flush to zero */
		x86_mxcsr_PUP   = 0x1F80    /* Power up state = (EM_PM | EM_UM | EM_OM | EM_ZM | EM_DM | EM_IM) */
	};

#endif

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


	inline void fenv_init()
	{
#if defined USE_SSE_MATH
		int x86_mxcsr_val = __builtin_ia32_stmxcsr();
		/* switch off floating point exceptions */
		x86_mxcsr_val |= (x86_mxcsr_EM_IM | x86_mxcsr_EM_DM | x86_mxcsr_EM_ZM | x86_mxcsr_EM_OM | x86_mxcsr_EM_UM | x86_mxcsr_EM_PM);
		/* clear floating point exceptions */
		x86_mxcsr_val &= ~(x86_mxcsr_EF_DE | x86_mxcsr_EF_PE | x86_mxcsr_EF_IE | x86_mxcsr_EF_OE | x86_mxcsr_EF_UE);
		__builtin_ia32_ldmxcsr(x86_mxcsr_val);
#else
		/* clear floating point exceptions */
		feclearexcept(FE_ALL_EXCEPT);
#endif
	}

	/* get accrued exception flags (C11) */

	inline void fenv_getflags(u32 &fcsr)
	{
#if defined USE_SSE_MATH
		int x86_mxcsr_val = __builtin_ia32_stmxcsr();
		if (x86_mxcsr_val & x86_mxcsr_EF_ZE) fcsr |= rv_fcsr_DZ;
		if (x86_mxcsr_val & x86_mxcsr_EF_PE) fcsr |= rv_fcsr_NX;
		if (x86_mxcsr_val & x86_mxcsr_EF_IE) fcsr |= rv_fcsr_NV;
		if (x86_mxcsr_val & x86_mxcsr_EF_OE) fcsr |= rv_fcsr_OF;
		if (x86_mxcsr_val & x86_mxcsr_EF_UE) fcsr |= rv_fcsr_UF;
#else
		int flags = fetestexcept(FE_ALL_EXCEPT);
		if (flags & FE_DIVBYZERO) fcsr |= rv_fcsr_DZ;
		if (flags & FE_INEXACT) fcsr |= rv_fcsr_NX;
		if (flags & FE_INVALID) fcsr |= rv_fcsr_NV;
		if (flags & FE_OVERFLOW) fcsr |= rv_fcsr_OF;
		if (flags & FE_UNDERFLOW) fcsr |= rv_fcsr_UF;
#endif
	}

	/* clear accrued exception flags (C11) */

	inline void fenv_clearflags(u32 &fcsr)
	{
#if defined USE_SSE_MATH
		int x86_mxcsr_val = __builtin_ia32_stmxcsr();
		x86_mxcsr_val &= ~(x86_mxcsr_EF_DE | x86_mxcsr_EF_PE | x86_mxcsr_EF_IE | x86_mxcsr_EF_OE | x86_mxcsr_EF_UE);
		if (!(fcsr & rv_fcsr_DZ)) x86_mxcsr_val |= x86_mxcsr_EF_DE;
		if (!(fcsr & rv_fcsr_NX)) x86_mxcsr_val |= x86_mxcsr_EF_PE;
		if (!(fcsr & rv_fcsr_NV)) x86_mxcsr_val |= x86_mxcsr_EF_IE;
		if (!(fcsr & rv_fcsr_OF)) x86_mxcsr_val |= x86_mxcsr_EF_OE;
		if (!(fcsr & rv_fcsr_UF)) x86_mxcsr_val |= x86_mxcsr_EF_UE;
		__builtin_ia32_ldmxcsr(x86_mxcsr_val);
#else
		int flags = 0;
		if (!(fcsr & rv_fcsr_DZ)) flags |= FE_DIVBYZERO;
		if (!(fcsr & rv_fcsr_NX)) flags |= FE_INEXACT;
		if (!(fcsr & rv_fcsr_NV)) flags |= FE_INVALID;
		if (!(fcsr & rv_fcsr_OF)) flags |= FE_OVERFLOW;
		if (!(fcsr & rv_fcsr_UF)) flags |= FE_UNDERFLOW;
		feclearexcept(flags);
#endif
	}

	/* set round mode (C11) */

	inline void fenv_setrm(int rm)
	{
#if defined USE_SSE_MATH
		int x86_mxcsr_val = __builtin_ia32_stmxcsr();
		x86_mxcsr_val &= ~x86_mxcsr_RC_RZ;
		switch (rm) {
			case rv_rm_rne: x86_mxcsr_val |= x86_mxcsr_RC_RN; break;
			case rv_rm_rtz: x86_mxcsr_val |= x86_mxcsr_RC_RZ; break;
			case rv_rm_rdn: x86_mxcsr_val |= x86_mxcsr_RC_DN; break;
			case rv_rm_rup: x86_mxcsr_val |= x86_mxcsr_RC_UP; break;
			case rv_rm_rmm: x86_mxcsr_val |= x86_mxcsr_RC_RN; break;
		}
		__builtin_ia32_ldmxcsr(x86_mxcsr_val);
#else
		if (rm == 0b111) return;
		switch (rm) {
			case rv_rm_rne: fesetround(FE_TONEAREST); /* ties to Even */ break;
			case rv_rm_rtz: fesetround(FE_TOWARDZERO); break;
			case rv_rm_rdn: fesetround(FE_DOWNWARD); break;
			case rv_rm_rup: fesetround(FE_UPWARD); break;
			case rv_rm_rmm: fesetround(FE_TONEAREST); /* ties to Max Magnitude */ break;
		}
#endif
	}

	/* convert single or double to signed word (32-bit) */

	template <typename F>
	inline s32 fcvt_w(u32 &fcsr, F f)
	{
		return (((f > std::numeric_limits<s32>::max()) & (s32(f) < 0)) | std::isnan(f))
			? std::numeric_limits<s32>::max()
			: s32(f);
	}

	/* convert single or double to unsigned word (32-bit) */

	template <typename F>
	inline s32 fcvt_wu(u32 &fcsr, F f)
	{
		return (std::isnan(f) | ((f >= 0) & std::isinf(f)))
			? std::numeric_limits<u32>::max()
			: f >= 0
			? s32(u32(f))
			: f > -1
			? (fcsr |= rv_fcsr_NX, s32(0))
			: (fcsr |= rv_fcsr_NV, s32(0));
	}

	/* convert single or double to signed long (64-bit) */

	template <typename F>
	inline s64 fcvt_l(u32 &fcsr, F f)
	{
		return (((f > std::numeric_limits<s64>::max()) & (s64(f) < 0)) | std::isnan(f))
			? std::numeric_limits<s64>::max()
			: s64(f);
	}

	/* convert single or double to unsigned long (64-bit) */

	template <typename F>
	inline s64 fcvt_lu(u32 &fcsr, F f)
	{
		return (std::isnan(f) | ((f >= 0) & std::isinf(f)))
			? std::numeric_limits<u64>::max()
			: f >= 0
			? s64(u64(f))
			: f > -1
			? (fcsr |= rv_fcsr_NX, s64(0))
			: (fcsr |= rv_fcsr_NV, s64(0));
	}

#if defined __GNUC__ && defined __x86_64__

	template <>
	inline s64 fcvt_lu(u32 &fcsr, float f)
	{
		s64 res;
		static const union { f32 f; u32 u; } val = { .u = 0x5f000000 };
		if (std::isnan(f) | ((f >= 0) & std::isinf(f))) return std::numeric_limits<u64>::max();
		if (f <= -1) { fcsr |= rv_fcsr_NV; return 0; }
		if (f < 0) { fcsr |= rv_fcsr_NX; return 0; }
		if (f > val.f) {
			__asm__(
				"subss      %[sub], %[input]\n	"
				"movabsq    $0x8000000000000000, %%rdx\n	"
				"cvttss2siq %[input], %[result]\n	"
				"xorq       %%rdx, %[result]"
			 : [result] "=r"(res)
			 : [sub] "m"(val.f), [input] "x"(f)
			 : "rdx"
			);
		} else {
			__asm__(
				"cvttss2siq %[input], %[result]"
			 : [result] "=r"(res)
			 : [input] "x"(f)
			);
		}
		return res;
	}

	template <>
	inline s64 fcvt_lu(u32 &fcsr, double f)
	{
		s64 res;
		static const union f64_bits { f64 f; u64 u; } val = { .u = 0x43E0000000000000ULL };
		if (std::isnan(f) | ((f >= 0) & std::isinf(f))) return std::numeric_limits<u64>::max();
		if (f <= -1) { fcsr |= rv_fcsr_NV; return 0; }
		if (f < 0) { fcsr |= rv_fcsr_NX; return 0; }
		if (f > val.f) {
			__asm__(
				"subsd      %[sub], %[input]\n	"
				"movabsq    $0x8000000000000000, %%rdx\n	"
				"cvttsd2siq %[input], %[result]\n	"
				"xorq       %%rdx, %[result]"
			 : [result] "=r"(res)
			 : [sub] "m"(val.f), [input] "x"(f)
			 : "rdx"
			);
		} else {
			__asm__(
				"cvttsd2siq %[input], %[result]"
			 : [result] "=r"(res)
			 : [input] "x"(f)
			);
		}
		return res;
	}

#endif

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
			? (neg ? rv_fclass_neg_zero : rv_fclass_pos_zero)
			: (neg ? rv_fclass_neg_subnorm : rv_fclass_pos_subnorm)
			: (exp == F::exp_inf)
			? (man == 0)
			? (neg ? rv_fclass_neg_inf : rv_fclass_pos_inf)
			: (man & 1 ? rv_fclass_signaling_nan : rv_fclass_quiet_nan)
			: (neg ? rv_fclass_neg_norm : rv_fclass_pos_norm);
	}

	inline int f32_classify(f32 f) { return float_classify<f32_type>(f); }
	inline int f64_classify(f64 f) { return float_classify<f64_type>(f); }

}

#endif
