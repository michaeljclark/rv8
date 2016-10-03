//
//  riscv-fpu.h
//

#ifndef riscv_fpu_h
#define riscv_fpu_h

namespace riscv {

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

	template <typename T, typename F>
	inline s32 fcvt_w(T &fcsr, F f)
	{
		return (f > std::numeric_limits<s32>::max()) & (s32(f) < 0) || std::isnan(f) ? std::numeric_limits<s32>::max() : s32(f);
	}

	template <typename T, typename F>
	inline s32 fcvt_wu(T &fcsr, F f)
	{
		return f >= 0 ? s32(u32(f)) : std::isnan(f) ? std::numeric_limits<u32>::max() : (f > -1 ? (fcsr |= riscv_fcsr_NX, s32(0)) : (fcsr |= riscv_fcsr_NV, s32(0)));
	}

	template <typename T, typename F>
	inline s64 fcvt_l(T &fcsr, F f)
	{
		return (f > std::numeric_limits<s64>::max()) & (s64(f) < 0) || std::isnan(f) ? std::numeric_limits<s64>::max() : s64(f);
	}

	template <typename T, typename F>
	inline s64 fcvt_lu(T &fcsr, F f)
	{
		return f >= 0 ? s64(u64(f)) : std::isnan(f) ? std::numeric_limits<u64>::max() : (f > -1 ? (fcsr |= riscv_fcsr_NX, s64(0)) : (fcsr |= riscv_fcsr_NV, s64(0)));
	}

	inline float f32_sqrt(float a) { return std::sqrt(a); }
	inline double f64_sqrt(double a) { return std::sqrt(a); }

	inline int f32_classify(float a)
	{
		union { uint32_t wu; float f; } v = { .f = a };
		uint32_t neg = v.wu >> 31, exp = (v.wu >> 23) & ((1<<8)-1), man = v.wu & ((1<<23)-1);
		bool inf = exp == ((1<<8)-1);
		bool denorm = exp == 0;
		if (denorm) {
			if (man == 0) return neg ? riscv_fclass_neg_zero : riscv_fclass_pos_zero;
			else return neg ? riscv_fclass_neg_subnorm : riscv_fclass_pos_subnorm;
		} else if (inf) {
			if (man == 0) return neg ? riscv_fclass_neg_inf : riscv_fclass_pos_inf;
			else return man & 1 ? riscv_fclass_signaling_nan : riscv_fclass_quiet_nan;
		} else {
			return neg ? riscv_fclass_neg_norm : riscv_fclass_pos_norm;
		}
	}

	inline int f64_classify(double a)
	{
		union { uint64_t lu; double d; } v = { .d = a };
		uint64_t neg = v.lu >> 63, exp = (v.lu >> 52) & ((1ULL<<11)-1), man = v.lu & ((1ULL<<52)-1);
		bool inf = exp == ((1ULL<<11)-1);
		bool denorm = exp == 0;
		if (denorm) {
			if (man == 0) return neg ? riscv_fclass_neg_zero : riscv_fclass_pos_zero;
			else return neg ? riscv_fclass_neg_subnorm : riscv_fclass_pos_subnorm;
		} else if (inf) {
			if (man == 0) return neg ? riscv_fclass_neg_inf : riscv_fclass_pos_inf;
			else return man & 1 ? riscv_fclass_signaling_nan : riscv_fclass_quiet_nan;
		} else {
			return neg ? riscv_fclass_neg_norm : riscv_fclass_pos_norm;
		}
	}

}

#endif
