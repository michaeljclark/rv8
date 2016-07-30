//
//  riscv-fpu.h
//

#ifndef riscv_fpu_h
#define riscv_fpu_h

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
		else return man & (1<<22) ? riscv_fclass_signaling_nan : riscv_fclass_quiet_nan;
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
		else return man & (1ULL<<51) ? riscv_fclass_signaling_nan : riscv_fclass_quiet_nan;
	} else {
		return neg ? riscv_fclass_neg_norm : riscv_fclass_pos_norm;
	}
}

#endif
