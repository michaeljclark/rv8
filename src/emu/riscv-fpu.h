//
//  riscv-fpu.h
//

#ifndef riscv_fpu_h
#define riscv_fpu_h

namespace riscv {

	template <typename T>
	inline void fenv_setflags(T &fcsr)
	{
		int flags = fetestexcept(FE_ALL_EXCEPT);
		if (flags & FE_DIVBYZERO) fcsr |= riscv_fcsr_DZ;
		if (flags & FE_INEXACT) fcsr |= riscv_fcsr_NX;
		if (flags & FE_INVALID) fcsr |= riscv_fcsr_NV;
		if (flags & FE_OVERFLOW) fcsr |= riscv_fcsr_OF;
		if (flags & FE_UNDERFLOW) fcsr |= riscv_fcsr_UF;
	}

	inline void fenv_setrm(int rm)
	{
		switch (rm) {
			case riscv_rm_rne: fesetround(FE_TONEAREST); /* ties to Even */ break;
			case riscv_rm_rtz: fesetround(FE_TOWARDZERO); break;
			case riscv_rm_rdn: fesetround(FE_DOWNWARD); break;
			case riscv_rm_rup: fesetround(FE_UPWARD); break;
			case riscv_rm_rmm: fesetround(FE_TONEAREST); /* ties to Max Magnitude */ break;
		}
	}

	inline float f32_sqrt(float a) { return std::sqrt(a); }
	inline double f64_sqrt(double a) { return std::sqrt(a); }

	inline float f32_classify(float a)
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

	inline double f64_classify(double a)
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
