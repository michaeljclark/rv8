//
//  test-cc.cc
//
//  DANGER - This is machine generated code
//

#include <cstdio>
#include <cstdint>
#include <cfloat>
#include <cfenv>
#include <cmath>
#include <limits>

#include "host-endian.h"
#include "types.h"
#include "meta.h"
#include "fpu.h"

using namespace riscv;

typedef signed long        sx;
typedef unsigned long      ux;

static u32 fcsr = 0;
static s32 rm = 0;

extern "C" f32 fmadd_s(f32 s1, f32 s2, f32 s3)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 * s2 + s3;
	return d;
}

extern "C" f32 fmsub_s(f32 s1, f32 s2, f32 s3)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 * s2 - s3;
	return d;
}

extern "C" f32 fnmsub_s(f32 s1, f32 s2, f32 s3)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 * -s2 + s3;
	return d;
}

extern "C" f32 fnmadd_s(f32 s1, f32 s2, f32 s3)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 * -s2 - s3;
	return d;
}

extern "C" f32 fadd_s(f32 s1, f32 s2)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 + s2;
	return d;
}

extern "C" f32 fsub_s(f32 s1, f32 s2)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 - s2;
	return d;
}

extern "C" f32 fmul_s(f32 s1, f32 s2)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 * s2;
	return d;
}

extern "C" f32 fdiv_s(f32 s1, f32 s2)
{
	f32 d = 0;
	fenv_setrm(rm); d = s1 / s2;
	return d;
}

extern "C" f32 fmin_s(f32 s1, f32 s2)
{
	f32 d = 0;
	d = (s1 < s2) || std::isnan(s2) ? s1 : s2;
	return d;
}

extern "C" f32 fmax_s(f32 s1, f32 s2)
{
	f32 d = 0;
	d = (s1 > s2) || std::isnan(s2) ? s1 : s2;
	return d;
}

extern "C" f32 fsqrt_s(f32 s1)
{
	f32 d = 0;
	fenv_setrm(rm); d = riscv::f32_sqrt(s1);
	return d;
}

extern "C" sx fle_s(f32 s1, f32 s2)
{
	sx d = 0;
	d = s1 <= s2;
	return d;
}

extern "C" sx flt_s(f32 s1, f32 s2)
{
	sx d = 0;
	d = s1 < s2;
	return d;
}

extern "C" sx feq_s(f32 s1, f32 s2)
{
	sx d = 0;
	d = s1 == s2;
	return d;
}

extern "C" s32 fcvt_w_s(f32 s1)
{
	s32 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_w(fcsr, s1);
	return d;
}

extern "C" u32 fcvt_wu_s(f32 s1)
{
	u32 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_wu(fcsr, s1);
	return d;
}

extern "C" f32 fcvt_s_w(s32 s1)
{
	f32 d = 0;
	fenv_setrm(rm); d = f32(s32(s1));
	return d;
}

extern "C" f32 fcvt_s_wu(u32 s1)
{
	f32 d = 0;
	fenv_setrm(rm); d = f32(u32(s1));
	return d;
}

extern "C" sx fmv_x_s(f32 s1)
{
	sx d = 0;
	d = std::isnan(s1) ? s32(0x7fc00000) : s1;
	return d;
}

extern "C" sx fclass_s(f32 s1)
{
	sx d = 0;
	d = f32_classify(s1);
	return d;
}

extern "C" f32 fmv_s_x(sx s1)
{
	f32 d = 0;
	d = u32(s1);
	return d;
}

#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" s64 fcvt_l_s(f32 s1)
{
	s64 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_l(fcsr, s1);
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" u64 fcvt_lu_s(f32 s1)
{
	u64 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_lu(fcsr, s1);
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" f32 fcvt_s_l(s64 s1)
{
	f32 d = 0;
	fenv_setrm(rm); d = f32(s64(s1));
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" f32 fcvt_s_lu(u64 s1)
{
	f32 d = 0;
	fenv_setrm(rm); d = f32(u64(s1));
	return d;
}

#endif
extern "C" f64 fmadd_d(f64 s1, f64 s2, f64 s3)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 * s2 + s3;
	return d;
}

extern "C" f64 fmsub_d(f64 s1, f64 s2, f64 s3)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 * s2 - s3;
	return d;
}

extern "C" f64 fnmsub_d(f64 s1, f64 s2, f64 s3)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 * -s2 + s3;
	return d;
}

extern "C" f64 fnmadd_d(f64 s1, f64 s2, f64 s3)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 * -s2 - s3;
	return d;
}

extern "C" f64 fadd_d(f64 s1, f64 s2)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 + s2;
	return d;
}

extern "C" f64 fsub_d(f64 s1, f64 s2)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 - s2;
	return d;
}

extern "C" f64 fmul_d(f64 s1, f64 s2)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 * s2;
	return d;
}

extern "C" f64 fdiv_d(f64 s1, f64 s2)
{
	f64 d = 0;
	fenv_setrm(rm); d = s1 / s2;
	return d;
}

extern "C" f64 fmin_d(f64 s1, f64 s2)
{
	f64 d = 0;
	d = (s1 < s2) || std::isnan(s2) ? s1 : s2;
	return d;
}

extern "C" f64 fmax_d(f64 s1, f64 s2)
{
	f64 d = 0;
	d = (s1 > s2) || std::isnan(s2) ? s1 : s2;
	return d;
}

extern "C" f32 fcvt_s_d(f64 s1)
{
	f32 d = 0;
	fenv_setrm(rm); d = f32(s1);
	return d;
}

extern "C" f64 fcvt_d_s(f32 s1)
{
	f64 d = 0;
	fenv_setrm(rm); d = f64(s1);
	return d;
}

extern "C" f64 fsqrt_d(f64 s1)
{
	f64 d = 0;
	fenv_setrm(rm); d = riscv::f64_sqrt(s1);
	return d;
}

extern "C" sx fle_d(f64 s1, f64 s2)
{
	sx d = 0;
	d = s1 <= s2;
	return d;
}

extern "C" sx flt_d(f64 s1, f64 s2)
{
	sx d = 0;
	d = s1 < s2;
	return d;
}

extern "C" sx feq_d(f64 s1, f64 s2)
{
	sx d = 0;
	d = s1 == s2;
	return d;
}

extern "C" s32 fcvt_w_d(f64 s1)
{
	s32 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_w(fcsr, s1);
	return d;
}

extern "C" u32 fcvt_wu_d(f64 s1)
{
	u32 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_wu(fcsr, s1);
	return d;
}

extern "C" f64 fcvt_d_w(s32 s1)
{
	f64 d = 0;
	fenv_setrm(rm); d = f64(s32(s1));
	return d;
}

extern "C" f64 fcvt_d_wu(u32 s1)
{
	f64 d = 0;
	fenv_setrm(rm); d = f64(u32(s1));
	return d;
}

extern "C" sx fclass_d(f64 s1)
{
	sx d = 0;
	d = f64_classify(s1);
	return d;
}

#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" s64 fcvt_l_d(f64 s1)
{
	s64 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_l(fcsr, s1);
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" u64 fcvt_lu_d(f64 s1)
{
	u64 d = 0;
	fenv_setrm(rm); d = riscv::fcvt_lu(fcsr, s1);
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" sx fmv_x_d(f64 s1)
{
	sx d = 0;
	d = std::isnan(s1) ? s64(0x7ff8000000000000ULL) : s1;
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" f64 fcvt_d_l(s64 s1)
{
	f64 d = 0;
	fenv_setrm(rm); d = f64(s64(s1));
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" f64 fcvt_d_lu(u64 s1)
{
	f64 d = 0;
	fenv_setrm(rm); d = f64(u64(s1));
	return d;
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
extern "C" f64 fmv_d_x(sx s1)
{
	f64 d = 0;
	d = u64(s1);
	return d;
}

#endif
void test_fmadd_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_7()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_8()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_9()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_10()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_11()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_12()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_13()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_14()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_15()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_16()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_17()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_18()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_19()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_20()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_21()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_22()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_23()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_24()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_25()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_26()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_27()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_28()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_29()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_30()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_31()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_32()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_33()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_34()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_35()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_36()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_37()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_38()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_39()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_40()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_41()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_42()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_43()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_44()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_45()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_46()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_47()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_48()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_49()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_50()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_51()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_52()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_53()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_54()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_55()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_56()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_57()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_58()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_59()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_60()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_61()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_62()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_63()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_64()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_65()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_66()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_67()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_68()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_69()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_70()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_71()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_72()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_73()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_74()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_75()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_76()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_77()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_78()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_79()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_80()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_81()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_82()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_83()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_84()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_85()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_86()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_87()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_88()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_89()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_90()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_91()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_92()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_93()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_94()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_95()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_96()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_97()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_98()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_99()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_100()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_101()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_102()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_103()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_104()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_105()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_106()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_107()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_108()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_109()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_110()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_111()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_112()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_113()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_114()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_115()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_116()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_117()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_118()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_119()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_120()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_121()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_122()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_123()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_124()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_125()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_126()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_127()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_128()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_129()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_130()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_131()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_132()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_133()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_134()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_135()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_136()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_137()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_138()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_139()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_140()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_141()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_142()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_143()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_144()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_145()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_146()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_147()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_148()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_149()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_150()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_151()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_152()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_153()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_154()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_155()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_156()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_157()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_158()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_159()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_160()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_161()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_162()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_163()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_164()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_165()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_166()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_167()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_168()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_169()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_170()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_171()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_172()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_173()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_174()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_175()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_176()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_177()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_178()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_179()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_180()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_181()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_182()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_183()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_184()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_185()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_186()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_187()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_188()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_189()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_190()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_191()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_192()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_193()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_194()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_195()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_196()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_197()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_198()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_199()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_200()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_201()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_202()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_203()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_204()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_205()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_206()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_207()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_208()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_209()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_210()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_211()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_212()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_213()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_214()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_215()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmadd_s_216()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_7()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_8()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_9()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_10()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_11()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_12()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_13()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_14()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_15()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_16()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_17()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_18()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_19()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_20()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_21()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_22()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_23()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_24()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_25()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_26()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_27()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_28()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_29()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_30()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_31()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_32()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_33()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_34()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_35()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_36()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_37()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_38()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_39()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_40()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_41()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_42()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_43()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_44()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_45()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_46()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_47()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_48()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_49()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_50()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_51()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_52()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_53()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_54()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_55()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_56()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_57()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_58()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_59()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_60()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_61()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_62()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_63()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_64()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_65()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_66()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_67()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_68()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_69()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_70()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_71()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_72()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_73()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_74()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_75()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_76()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_77()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_78()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_79()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_80()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_81()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_82()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_83()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_84()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_85()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_86()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_87()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_88()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_89()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_90()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_91()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_92()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_93()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_94()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_95()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_96()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_97()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_98()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_99()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_100()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_101()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_102()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_103()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_104()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_105()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_106()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_107()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_108()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_109()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_110()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_111()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_112()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_113()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_114()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_115()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_116()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_117()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_118()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_119()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_120()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_121()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_122()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_123()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_124()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_125()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_126()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_127()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_128()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_129()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_130()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_131()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_132()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_133()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_134()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_135()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_136()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_137()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_138()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_139()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_140()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_141()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_142()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_143()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_144()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_145()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_146()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_147()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_148()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_149()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_150()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_151()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_152()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_153()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_154()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_155()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_156()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_157()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_158()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_159()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_160()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_161()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_162()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_163()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_164()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_165()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_166()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_167()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_168()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_169()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_170()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_171()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_172()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_173()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_174()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_175()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_176()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_177()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_178()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_179()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_180()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_181()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_182()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_183()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_184()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_185()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_186()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_187()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_188()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_189()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_190()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_191()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_192()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_193()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_194()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_195()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_196()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_197()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_198()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_199()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_200()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_201()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_202()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_203()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_204()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_205()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_206()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_207()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_208()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_209()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_210()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_211()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_212()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_213()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_214()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_215()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fmsub_s_216()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_7()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_8()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_9()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_10()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_11()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_12()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_13()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_14()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_15()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_16()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_17()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_18()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_19()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_20()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_21()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_22()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_23()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_24()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_25()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_26()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_27()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_28()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_29()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_30()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_31()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_32()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_33()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_34()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_35()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_36()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_37()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_38()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_39()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_40()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_41()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_42()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_43()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_44()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_45()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_46()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_47()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_48()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_49()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_50()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_51()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_52()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_53()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_54()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_55()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_56()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_57()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_58()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_59()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_60()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_61()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_62()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_63()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_64()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_65()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_66()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_67()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_68()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_69()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_70()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_71()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_72()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_73()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_74()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_75()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_76()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_77()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_78()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_79()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_80()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_81()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_82()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_83()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_84()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_85()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_86()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_87()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_88()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_89()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_90()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_91()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_92()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_93()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_94()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_95()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_96()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_97()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_98()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_99()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_100()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_101()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_102()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_103()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_104()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_105()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_106()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_107()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_108()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_109()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_110()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_111()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_112()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_113()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_114()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_115()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_116()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_117()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_118()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_119()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_120()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_121()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_122()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_123()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_124()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_125()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_126()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_127()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_128()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_129()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_130()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_131()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_132()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_133()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_134()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_135()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_136()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_137()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_138()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_139()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_140()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_141()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_142()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_143()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_144()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_145()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_146()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_147()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_148()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_149()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_150()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_151()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_152()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_153()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_154()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_155()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_156()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_157()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_158()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_159()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_160()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_161()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_162()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_163()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_164()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_165()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_166()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_167()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_168()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_169()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_170()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_171()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_172()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_173()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_174()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_175()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_176()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_177()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_178()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_179()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_180()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_181()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_182()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_183()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_184()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_185()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_186()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_187()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_188()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_189()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_190()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_191()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_192()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_193()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_194()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_195()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_196()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_197()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_198()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_199()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_200()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_201()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_202()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_203()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_204()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_205()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_206()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_207()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_208()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_209()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_210()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_211()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_212()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_213()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_214()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_215()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmsub_s_216()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmsub_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_7()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_8()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_9()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_10()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_11()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_12()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_13()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_14()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_15()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_16()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_17()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_18()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_19()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_20()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_21()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_22()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_23()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_24()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_25()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_26()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_27()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_28()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_29()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_30()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_31()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_32()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_33()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_34()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_35()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_36()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_37()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_38()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_39()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_40()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_41()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_42()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_43()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_44()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_45()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_46()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_47()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_48()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_49()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_50()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_51()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_52()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_53()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_54()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_55()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_56()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_57()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_58()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_59()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_60()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_61()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_62()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_63()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_64()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_65()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_66()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_67()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_68()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_69()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_70()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_71()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_72()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_73()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_74()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_75()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_76()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_77()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_78()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_79()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_80()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_81()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_82()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_83()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_84()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_85()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_86()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_87()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_88()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_89()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_90()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_91()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_92()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_93()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_94()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_95()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_96()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_97()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_98()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_99()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_100()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_101()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_102()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_103()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_104()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_105()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_106()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_107()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_108()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_109()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_110()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_111()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_112()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_113()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_114()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_115()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_116()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_117()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_118()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_119()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_120()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_121()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_122()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_123()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_124()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_125()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_126()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_127()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_128()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_129()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_130()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_131()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_132()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_133()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_134()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_135()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_136()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_137()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_138()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_139()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_140()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_141()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_142()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_143()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_144()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_145()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_146()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_147()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_148()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_149()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_150()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_151()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_152()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_153()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_154()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_155()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_156()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_157()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_158()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_159()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_160()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_161()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_162()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_163()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_164()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_165()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_166()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_167()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_168()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_169()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_170()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_171()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_172()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_173()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_174()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_175()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_176()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_177()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_178()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_179()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_180()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_181()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_182()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_183()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_184()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_185()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_186()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_187()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_188()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_189()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_190()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_191()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_192()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_193()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_194()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_195()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_196()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_197()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_198()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_199()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_200()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_201()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_202()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_203()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_204()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_205()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_206()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_207()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_208()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_209()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_210()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_211()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = -2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_212()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.000000000e+00f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_213()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 2.500000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_214()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = 5.000000000e-01f;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_215()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = INFINITY;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fnmadd_s_216()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 s3 = NAN;
	f32 d = fnmadd_s(s1, s2, s3);
	printf("d=%.9e\n", d);
}

void test_fadd_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fadd_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 d = fadd_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsub_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 d = fsub_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmul_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 d = fmul_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fdiv_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 d = fdiv_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmin_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 d = fmin_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fmax_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	f32 d = fmax_s(s1, s2);
	printf("d=%.9e\n", d);
}

void test_fsqrt_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 d = fsqrt_s(s1);
	printf("d=%.9e\n", d);
}

void test_fsqrt_s_2()
{
	f32 s1 = 2.000000000e+00f;
	f32 d = fsqrt_s(s1);
	printf("d=%.9e\n", d);
}

void test_fsqrt_s_3()
{
	f32 s1 = 2.500000000e-01f;
	f32 d = fsqrt_s(s1);
	printf("d=%.9e\n", d);
}

void test_fsqrt_s_4()
{
	f32 s1 = 5.000000000e-01f;
	f32 d = fsqrt_s(s1);
	printf("d=%.9e\n", d);
}

void test_fsqrt_s_5()
{
	f32 s1 = INFINITY;
	f32 d = fsqrt_s(s1);
	printf("d=%.9e\n", d);
}

void test_fsqrt_s_6()
{
	f32 s1 = NAN;
	f32 d = fsqrt_s(s1);
	printf("d=%.9e\n", d);
}

void test_fle_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	sx d = fle_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	sx d = flt_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_2()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_3()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_4()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_5()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = INFINITY;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_6()
{
	f32 s1 = -2.000000000e+00f;
	f32 s2 = NAN;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_7()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = -2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_8()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_9()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 2.500000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_10()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = 5.000000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_11()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = INFINITY;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_12()
{
	f32 s1 = 2.000000000e+00f;
	f32 s2 = NAN;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_13()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = -2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_14()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_15()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 2.500000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_16()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = 5.000000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_17()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = INFINITY;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_18()
{
	f32 s1 = 2.500000000e-01f;
	f32 s2 = NAN;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_19()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = -2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_20()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_21()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 2.500000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_22()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = 5.000000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_23()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = INFINITY;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_24()
{
	f32 s1 = 5.000000000e-01f;
	f32 s2 = NAN;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_25()
{
	f32 s1 = INFINITY;
	f32 s2 = -2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_26()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_27()
{
	f32 s1 = INFINITY;
	f32 s2 = 2.500000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_28()
{
	f32 s1 = INFINITY;
	f32 s2 = 5.000000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_29()
{
	f32 s1 = INFINITY;
	f32 s2 = INFINITY;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_30()
{
	f32 s1 = INFINITY;
	f32 s2 = NAN;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_31()
{
	f32 s1 = NAN;
	f32 s2 = -2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_32()
{
	f32 s1 = NAN;
	f32 s2 = 2.000000000e+00f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_33()
{
	f32 s1 = NAN;
	f32 s2 = 2.500000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_34()
{
	f32 s1 = NAN;
	f32 s2 = 5.000000000e-01f;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_35()
{
	f32 s1 = NAN;
	f32 s2 = INFINITY;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_s_36()
{
	f32 s1 = NAN;
	f32 s2 = NAN;
	sx d = feq_s(s1, s2);
	printf("d=%ld\n", d);
}

void test_fcvt_w_s_1()
{
	f32 s1 = -2.000000000e+00f;
	s32 d = fcvt_w_s(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_s_2()
{
	f32 s1 = 2.000000000e+00f;
	s32 d = fcvt_w_s(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_s_3()
{
	f32 s1 = 2.500000000e-01f;
	s32 d = fcvt_w_s(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_s_4()
{
	f32 s1 = 5.000000000e-01f;
	s32 d = fcvt_w_s(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_s_5()
{
	f32 s1 = INFINITY;
	s32 d = fcvt_w_s(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_s_6()
{
	f32 s1 = NAN;
	s32 d = fcvt_w_s(s1);
	printf("d=%d\n", d);
}

void test_fcvt_wu_s_1()
{
	f32 s1 = -2.000000000e+00f;
	u32 d = fcvt_wu_s(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_s_2()
{
	f32 s1 = 2.000000000e+00f;
	u32 d = fcvt_wu_s(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_s_3()
{
	f32 s1 = 2.500000000e-01f;
	u32 d = fcvt_wu_s(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_s_4()
{
	f32 s1 = 5.000000000e-01f;
	u32 d = fcvt_wu_s(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_s_5()
{
	f32 s1 = INFINITY;
	u32 d = fcvt_wu_s(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_s_6()
{
	f32 s1 = NAN;
	u32 d = fcvt_wu_s(s1);
	printf("d=%u\n", d);
}

void test_fcvt_s_w_1()
{
	s32 s1 = -2;
	f32 d = fcvt_s_w(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_w_2()
{
	s32 s1 = 2;
	f32 d = fcvt_s_w(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_wu_1()
{
	u32 s1 = 2u;
	f32 d = fcvt_s_wu(s1);
	printf("d=%.9e\n", d);
}

void test_fmv_x_s_1()
{
	f32 s1 = -2.000000000e+00f;
	sx d = fmv_x_s(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_s_2()
{
	f32 s1 = 2.000000000e+00f;
	sx d = fmv_x_s(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_s_3()
{
	f32 s1 = 2.500000000e-01f;
	sx d = fmv_x_s(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_s_4()
{
	f32 s1 = 5.000000000e-01f;
	sx d = fmv_x_s(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_s_5()
{
	f32 s1 = INFINITY;
	sx d = fmv_x_s(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_s_6()
{
	f32 s1 = NAN;
	sx d = fmv_x_s(s1);
	printf("d=%ld\n", d);
}

void test_fclass_s_1()
{
	f32 s1 = -2.000000000e+00f;
	sx d = fclass_s(s1);
	printf("d=%ld\n", d);
}

void test_fclass_s_2()
{
	f32 s1 = 2.000000000e+00f;
	sx d = fclass_s(s1);
	printf("d=%ld\n", d);
}

void test_fclass_s_3()
{
	f32 s1 = 2.500000000e-01f;
	sx d = fclass_s(s1);
	printf("d=%ld\n", d);
}

void test_fclass_s_4()
{
	f32 s1 = 5.000000000e-01f;
	sx d = fclass_s(s1);
	printf("d=%ld\n", d);
}

void test_fclass_s_5()
{
	f32 s1 = INFINITY;
	sx d = fclass_s(s1);
	printf("d=%ld\n", d);
}

void test_fclass_s_6()
{
	f32 s1 = NAN;
	sx d = fclass_s(s1);
	printf("d=%ld\n", d);
}

void test_fmv_s_x_1()
{
	sx s1 = -2l;
	f32 d = fmv_s_x(s1);
	printf("d=%.9e\n", d);
}

void test_fmv_s_x_2()
{
	sx s1 = 2l;
	f32 d = fmv_s_x(s1);
	printf("d=%.9e\n", d);
}

#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_l_s_1()
{
	f32 s1 = -2.000000000e+00f;
	s64 d = fcvt_l_s(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_s_2()
{
	f32 s1 = 2.000000000e+00f;
	s64 d = fcvt_l_s(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_s_3()
{
	f32 s1 = 2.500000000e-01f;
	s64 d = fcvt_l_s(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_s_4()
{
	f32 s1 = 5.000000000e-01f;
	s64 d = fcvt_l_s(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_s_5()
{
	f32 s1 = INFINITY;
	s64 d = fcvt_l_s(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_s_6()
{
	f32 s1 = NAN;
	s64 d = fcvt_l_s(s1);
	printf("d=%lld\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_lu_s_1()
{
	f32 s1 = -2.000000000e+00f;
	u64 d = fcvt_lu_s(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_s_2()
{
	f32 s1 = 2.000000000e+00f;
	u64 d = fcvt_lu_s(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_s_3()
{
	f32 s1 = 2.500000000e-01f;
	u64 d = fcvt_lu_s(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_s_4()
{
	f32 s1 = 5.000000000e-01f;
	u64 d = fcvt_lu_s(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_s_5()
{
	f32 s1 = INFINITY;
	u64 d = fcvt_lu_s(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_s_6()
{
	f32 s1 = NAN;
	u64 d = fcvt_lu_s(s1);
	printf("d=%llu\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_s_l_1()
{
	s64 s1 = -2ll;
	f32 d = fcvt_s_l(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_l_2()
{
	s64 s1 = 2ll;
	f32 d = fcvt_s_l(s1);
	printf("d=%.9e\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_s_lu_1()
{
	u64 s1 = 2ull;
	f32 d = fcvt_s_lu(s1);
	printf("d=%.9e\n", d);
}

#endif
void test_fmadd_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_7()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_8()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_9()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_10()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_11()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_12()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_13()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_14()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_15()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_16()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_17()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_18()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_19()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_20()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_21()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_22()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_23()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_24()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_25()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_26()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_27()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_28()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_29()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_30()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_31()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_32()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_33()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_34()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_35()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_36()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_37()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_38()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_39()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_40()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_41()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_42()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_43()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_44()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_45()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_46()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_47()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_48()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_49()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_50()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_51()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_52()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_53()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_54()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_55()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_56()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_57()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_58()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_59()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_60()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_61()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_62()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_63()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_64()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_65()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_66()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_67()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_68()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_69()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_70()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_71()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_72()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_73()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_74()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_75()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_76()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_77()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_78()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_79()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_80()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_81()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_82()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_83()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_84()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_85()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_86()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_87()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_88()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_89()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_90()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_91()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_92()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_93()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_94()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_95()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_96()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_97()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_98()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_99()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_100()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_101()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_102()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_103()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_104()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_105()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_106()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_107()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_108()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_109()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_110()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_111()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_112()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_113()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_114()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_115()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_116()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_117()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_118()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_119()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_120()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_121()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_122()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_123()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_124()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_125()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_126()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_127()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_128()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_129()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_130()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_131()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_132()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_133()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_134()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_135()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_136()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_137()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_138()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_139()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_140()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_141()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_142()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_143()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_144()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_145()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_146()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_147()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_148()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_149()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_150()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_151()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_152()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_153()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_154()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_155()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_156()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_157()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_158()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_159()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_160()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_161()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_162()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_163()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_164()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_165()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_166()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_167()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_168()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_169()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_170()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_171()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_172()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_173()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_174()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_175()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_176()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_177()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_178()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_179()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_180()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_181()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_182()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_183()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_184()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_185()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_186()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_187()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_188()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_189()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_190()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_191()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_192()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_193()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_194()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_195()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_196()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_197()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_198()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_199()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_200()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_201()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_202()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_203()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_204()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_205()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_206()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_207()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_208()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_209()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_210()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_211()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_212()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_213()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_214()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_215()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmadd_d_216()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_7()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_8()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_9()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_10()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_11()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_12()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_13()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_14()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_15()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_16()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_17()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_18()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_19()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_20()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_21()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_22()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_23()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_24()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_25()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_26()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_27()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_28()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_29()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_30()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_31()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_32()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_33()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_34()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_35()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_36()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_37()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_38()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_39()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_40()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_41()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_42()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_43()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_44()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_45()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_46()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_47()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_48()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_49()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_50()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_51()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_52()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_53()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_54()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_55()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_56()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_57()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_58()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_59()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_60()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_61()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_62()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_63()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_64()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_65()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_66()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_67()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_68()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_69()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_70()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_71()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_72()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_73()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_74()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_75()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_76()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_77()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_78()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_79()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_80()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_81()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_82()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_83()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_84()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_85()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_86()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_87()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_88()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_89()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_90()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_91()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_92()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_93()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_94()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_95()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_96()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_97()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_98()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_99()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_100()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_101()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_102()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_103()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_104()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_105()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_106()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_107()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_108()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_109()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_110()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_111()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_112()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_113()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_114()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_115()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_116()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_117()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_118()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_119()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_120()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_121()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_122()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_123()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_124()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_125()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_126()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_127()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_128()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_129()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_130()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_131()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_132()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_133()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_134()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_135()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_136()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_137()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_138()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_139()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_140()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_141()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_142()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_143()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_144()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_145()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_146()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_147()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_148()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_149()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_150()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_151()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_152()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_153()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_154()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_155()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_156()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_157()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_158()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_159()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_160()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_161()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_162()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_163()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_164()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_165()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_166()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_167()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_168()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_169()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_170()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_171()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_172()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_173()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_174()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_175()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_176()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_177()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_178()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_179()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_180()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_181()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_182()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_183()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_184()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_185()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_186()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_187()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_188()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_189()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_190()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_191()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_192()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_193()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_194()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_195()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_196()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_197()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_198()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_199()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_200()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_201()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_202()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_203()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_204()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_205()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_206()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_207()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_208()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_209()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_210()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_211()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_212()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_213()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_214()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_215()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fmsub_d_216()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_7()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_8()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_9()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_10()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_11()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_12()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_13()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_14()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_15()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_16()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_17()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_18()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_19()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_20()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_21()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_22()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_23()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_24()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_25()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_26()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_27()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_28()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_29()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_30()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_31()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_32()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_33()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_34()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_35()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_36()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_37()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_38()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_39()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_40()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_41()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_42()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_43()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_44()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_45()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_46()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_47()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_48()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_49()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_50()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_51()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_52()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_53()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_54()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_55()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_56()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_57()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_58()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_59()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_60()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_61()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_62()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_63()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_64()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_65()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_66()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_67()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_68()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_69()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_70()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_71()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_72()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_73()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_74()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_75()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_76()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_77()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_78()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_79()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_80()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_81()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_82()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_83()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_84()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_85()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_86()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_87()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_88()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_89()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_90()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_91()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_92()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_93()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_94()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_95()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_96()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_97()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_98()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_99()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_100()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_101()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_102()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_103()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_104()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_105()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_106()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_107()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_108()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_109()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_110()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_111()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_112()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_113()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_114()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_115()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_116()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_117()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_118()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_119()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_120()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_121()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_122()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_123()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_124()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_125()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_126()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_127()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_128()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_129()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_130()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_131()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_132()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_133()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_134()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_135()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_136()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_137()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_138()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_139()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_140()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_141()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_142()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_143()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_144()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_145()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_146()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_147()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_148()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_149()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_150()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_151()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_152()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_153()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_154()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_155()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_156()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_157()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_158()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_159()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_160()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_161()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_162()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_163()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_164()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_165()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_166()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_167()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_168()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_169()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_170()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_171()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_172()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_173()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_174()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_175()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_176()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_177()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_178()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_179()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_180()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_181()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_182()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_183()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_184()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_185()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_186()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_187()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_188()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_189()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_190()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_191()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_192()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_193()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_194()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_195()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_196()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_197()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_198()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_199()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_200()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_201()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_202()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_203()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_204()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_205()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_206()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_207()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_208()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_209()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_210()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_211()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_212()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_213()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_214()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_215()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmsub_d_216()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmsub_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_7()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_8()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_9()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_10()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_11()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_12()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_13()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_14()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_15()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_16()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_17()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_18()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_19()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_20()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_21()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_22()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_23()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_24()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_25()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_26()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_27()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_28()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_29()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_30()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_31()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_32()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_33()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_34()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_35()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_36()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_37()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_38()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_39()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_40()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_41()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_42()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_43()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_44()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_45()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_46()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_47()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_48()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_49()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_50()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_51()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_52()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_53()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_54()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_55()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_56()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_57()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_58()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_59()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_60()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_61()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_62()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_63()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_64()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_65()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_66()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_67()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_68()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_69()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_70()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_71()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_72()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_73()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_74()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_75()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_76()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_77()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_78()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_79()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_80()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_81()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_82()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_83()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_84()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_85()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_86()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_87()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_88()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_89()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_90()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_91()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_92()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_93()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_94()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_95()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_96()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_97()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_98()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_99()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_100()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_101()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_102()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_103()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_104()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_105()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_106()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_107()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_108()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_109()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_110()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_111()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_112()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_113()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_114()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_115()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_116()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_117()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_118()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_119()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_120()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_121()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_122()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_123()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_124()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_125()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_126()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_127()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_128()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_129()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_130()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_131()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_132()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_133()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_134()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_135()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_136()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_137()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_138()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_139()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_140()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_141()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_142()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_143()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_144()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_145()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_146()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_147()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_148()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_149()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_150()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_151()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_152()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_153()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_154()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_155()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_156()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_157()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_158()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_159()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_160()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_161()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_162()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_163()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_164()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_165()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_166()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_167()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_168()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_169()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_170()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_171()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_172()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_173()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_174()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_175()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_176()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_177()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_178()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_179()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_180()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_181()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_182()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_183()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_184()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_185()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_186()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_187()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_188()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_189()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_190()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_191()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_192()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_193()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_194()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_195()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_196()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_197()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_198()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_199()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_200()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_201()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_202()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_203()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_204()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_205()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_206()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_207()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_208()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_209()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_210()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_211()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = -2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_212()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.00000000000000000e+00;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_213()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 2.50000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_214()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = 5.00000000000000000e-01;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_215()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = INFINITY;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fnmadd_d_216()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 s3 = NAN;
	f64 d = fnmadd_d(s1, s2, s3);
	printf("d=%.17e\n", d);
}

void test_fadd_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fadd_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 d = fadd_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fsub_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 d = fsub_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmul_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 d = fmul_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fdiv_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 d = fdiv_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmin_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 d = fmin_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fmax_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	f64 d = fmax_d(s1, s2);
	printf("d=%.17e\n", d);
}

void test_fcvt_s_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f32 d = fcvt_s_d(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	f32 d = fcvt_s_d(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	f32 d = fcvt_s_d(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	f32 d = fcvt_s_d(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_d_5()
{
	f64 s1 = INFINITY;
	f32 d = fcvt_s_d(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_s_d_6()
{
	f64 s1 = NAN;
	f32 d = fcvt_s_d(s1);
	printf("d=%.9e\n", d);
}

void test_fcvt_d_s_1()
{
	f32 s1 = -2.000000000e+00f;
	f64 d = fcvt_d_s(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_s_2()
{
	f32 s1 = 2.000000000e+00f;
	f64 d = fcvt_d_s(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_s_3()
{
	f32 s1 = 2.500000000e-01f;
	f64 d = fcvt_d_s(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_s_4()
{
	f32 s1 = 5.000000000e-01f;
	f64 d = fcvt_d_s(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_s_5()
{
	f32 s1 = INFINITY;
	f64 d = fcvt_d_s(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_s_6()
{
	f32 s1 = NAN;
	f64 d = fcvt_d_s(s1);
	printf("d=%.17e\n", d);
}

void test_fsqrt_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 d = fsqrt_d(s1);
	printf("d=%.17e\n", d);
}

void test_fsqrt_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 d = fsqrt_d(s1);
	printf("d=%.17e\n", d);
}

void test_fsqrt_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 d = fsqrt_d(s1);
	printf("d=%.17e\n", d);
}

void test_fsqrt_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 d = fsqrt_d(s1);
	printf("d=%.17e\n", d);
}

void test_fsqrt_d_5()
{
	f64 s1 = INFINITY;
	f64 d = fsqrt_d(s1);
	printf("d=%.17e\n", d);
}

void test_fsqrt_d_6()
{
	f64 s1 = NAN;
	f64 d = fsqrt_d(s1);
	printf("d=%.17e\n", d);
}

void test_fle_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fle_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	sx d = fle_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_flt_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	sx d = flt_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_2()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_3()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_4()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_5()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = INFINITY;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_6()
{
	f64 s1 = -2.00000000000000000e+00;
	f64 s2 = NAN;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_7()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = -2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_8()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_9()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 2.50000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_10()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = 5.00000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_11()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = INFINITY;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_12()
{
	f64 s1 = 2.00000000000000000e+00;
	f64 s2 = NAN;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_13()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_14()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_15()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_16()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_17()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = INFINITY;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_18()
{
	f64 s1 = 2.50000000000000000e-01;
	f64 s2 = NAN;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_19()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = -2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_20()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_21()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 2.50000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_22()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = 5.00000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_23()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = INFINITY;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_24()
{
	f64 s1 = 5.00000000000000000e-01;
	f64 s2 = NAN;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_25()
{
	f64 s1 = INFINITY;
	f64 s2 = -2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_26()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_27()
{
	f64 s1 = INFINITY;
	f64 s2 = 2.50000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_28()
{
	f64 s1 = INFINITY;
	f64 s2 = 5.00000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_29()
{
	f64 s1 = INFINITY;
	f64 s2 = INFINITY;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_30()
{
	f64 s1 = INFINITY;
	f64 s2 = NAN;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_31()
{
	f64 s1 = NAN;
	f64 s2 = -2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_32()
{
	f64 s1 = NAN;
	f64 s2 = 2.00000000000000000e+00;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_33()
{
	f64 s1 = NAN;
	f64 s2 = 2.50000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_34()
{
	f64 s1 = NAN;
	f64 s2 = 5.00000000000000000e-01;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_35()
{
	f64 s1 = NAN;
	f64 s2 = INFINITY;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_feq_d_36()
{
	f64 s1 = NAN;
	f64 s2 = NAN;
	sx d = feq_d(s1, s2);
	printf("d=%ld\n", d);
}

void test_fcvt_w_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	s32 d = fcvt_w_d(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	s32 d = fcvt_w_d(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	s32 d = fcvt_w_d(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	s32 d = fcvt_w_d(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_d_5()
{
	f64 s1 = INFINITY;
	s32 d = fcvt_w_d(s1);
	printf("d=%d\n", d);
}

void test_fcvt_w_d_6()
{
	f64 s1 = NAN;
	s32 d = fcvt_w_d(s1);
	printf("d=%d\n", d);
}

void test_fcvt_wu_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	u32 d = fcvt_wu_d(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	u32 d = fcvt_wu_d(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	u32 d = fcvt_wu_d(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	u32 d = fcvt_wu_d(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_d_5()
{
	f64 s1 = INFINITY;
	u32 d = fcvt_wu_d(s1);
	printf("d=%u\n", d);
}

void test_fcvt_wu_d_6()
{
	f64 s1 = NAN;
	u32 d = fcvt_wu_d(s1);
	printf("d=%u\n", d);
}

void test_fcvt_d_w_1()
{
	s32 s1 = -2;
	f64 d = fcvt_d_w(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_w_2()
{
	s32 s1 = 2;
	f64 d = fcvt_d_w(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_wu_1()
{
	u32 s1 = 2u;
	f64 d = fcvt_d_wu(s1);
	printf("d=%.17e\n", d);
}

void test_fclass_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	sx d = fclass_d(s1);
	printf("d=%ld\n", d);
}

void test_fclass_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	sx d = fclass_d(s1);
	printf("d=%ld\n", d);
}

void test_fclass_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	sx d = fclass_d(s1);
	printf("d=%ld\n", d);
}

void test_fclass_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	sx d = fclass_d(s1);
	printf("d=%ld\n", d);
}

void test_fclass_d_5()
{
	f64 s1 = INFINITY;
	sx d = fclass_d(s1);
	printf("d=%ld\n", d);
}

void test_fclass_d_6()
{
	f64 s1 = NAN;
	sx d = fclass_d(s1);
	printf("d=%ld\n", d);
}

#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_l_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	s64 d = fcvt_l_d(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	s64 d = fcvt_l_d(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	s64 d = fcvt_l_d(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	s64 d = fcvt_l_d(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_d_5()
{
	f64 s1 = INFINITY;
	s64 d = fcvt_l_d(s1);
	printf("d=%lld\n", d);
}

void test_fcvt_l_d_6()
{
	f64 s1 = NAN;
	s64 d = fcvt_l_d(s1);
	printf("d=%lld\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_lu_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	u64 d = fcvt_lu_d(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	u64 d = fcvt_lu_d(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	u64 d = fcvt_lu_d(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	u64 d = fcvt_lu_d(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_d_5()
{
	f64 s1 = INFINITY;
	u64 d = fcvt_lu_d(s1);
	printf("d=%llu\n", d);
}

void test_fcvt_lu_d_6()
{
	f64 s1 = NAN;
	u64 d = fcvt_lu_d(s1);
	printf("d=%llu\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fmv_x_d_1()
{
	f64 s1 = -2.00000000000000000e+00;
	sx d = fmv_x_d(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_d_2()
{
	f64 s1 = 2.00000000000000000e+00;
	sx d = fmv_x_d(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_d_3()
{
	f64 s1 = 2.50000000000000000e-01;
	sx d = fmv_x_d(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_d_4()
{
	f64 s1 = 5.00000000000000000e-01;
	sx d = fmv_x_d(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_d_5()
{
	f64 s1 = INFINITY;
	sx d = fmv_x_d(s1);
	printf("d=%ld\n", d);
}

void test_fmv_x_d_6()
{
	f64 s1 = NAN;
	sx d = fmv_x_d(s1);
	printf("d=%ld\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_d_l_1()
{
	s64 s1 = -2ll;
	f64 d = fcvt_d_l(s1);
	printf("d=%.17e\n", d);
}

void test_fcvt_d_l_2()
{
	s64 s1 = 2ll;
	f64 d = fcvt_d_l(s1);
	printf("d=%.17e\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fcvt_d_lu_1()
{
	u64 s1 = 2ull;
	f64 d = fcvt_d_lu(s1);
	printf("d=%.17e\n", d);
}

#endif
#if _RISCV_SZPTR != _RISCV_SZINT
void test_fmv_d_x_1()
{
	sx s1 = -2l;
	f64 d = fmv_d_x(s1);
	printf("d=%.17e\n", d);
}

void test_fmv_d_x_2()
{
	sx s1 = 2l;
	f64 d = fmv_d_x(s1);
	printf("d=%.17e\n", d);
}

#endif
int main()
{
	test_fmadd_s_1();
	test_fmadd_s_2();
	test_fmadd_s_3();
	test_fmadd_s_4();
	test_fmadd_s_5();
	test_fmadd_s_6();
	test_fmadd_s_7();
	test_fmadd_s_8();
	test_fmadd_s_9();
	test_fmadd_s_10();
	test_fmadd_s_11();
	test_fmadd_s_12();
	test_fmadd_s_13();
	test_fmadd_s_14();
	test_fmadd_s_15();
	test_fmadd_s_16();
	test_fmadd_s_17();
	test_fmadd_s_18();
	test_fmadd_s_19();
	test_fmadd_s_20();
	test_fmadd_s_21();
	test_fmadd_s_22();
	test_fmadd_s_23();
	test_fmadd_s_24();
	test_fmadd_s_25();
	test_fmadd_s_26();
	test_fmadd_s_27();
	test_fmadd_s_28();
	test_fmadd_s_29();
	test_fmadd_s_30();
	test_fmadd_s_31();
	test_fmadd_s_32();
	test_fmadd_s_33();
	test_fmadd_s_34();
	test_fmadd_s_35();
	test_fmadd_s_36();
	test_fmadd_s_37();
	test_fmadd_s_38();
	test_fmadd_s_39();
	test_fmadd_s_40();
	test_fmadd_s_41();
	test_fmadd_s_42();
	test_fmadd_s_43();
	test_fmadd_s_44();
	test_fmadd_s_45();
	test_fmadd_s_46();
	test_fmadd_s_47();
	test_fmadd_s_48();
	test_fmadd_s_49();
	test_fmadd_s_50();
	test_fmadd_s_51();
	test_fmadd_s_52();
	test_fmadd_s_53();
	test_fmadd_s_54();
	test_fmadd_s_55();
	test_fmadd_s_56();
	test_fmadd_s_57();
	test_fmadd_s_58();
	test_fmadd_s_59();
	test_fmadd_s_60();
	test_fmadd_s_61();
	test_fmadd_s_62();
	test_fmadd_s_63();
	test_fmadd_s_64();
	test_fmadd_s_65();
	test_fmadd_s_66();
	test_fmadd_s_67();
	test_fmadd_s_68();
	test_fmadd_s_69();
	test_fmadd_s_70();
	test_fmadd_s_71();
	test_fmadd_s_72();
	test_fmadd_s_73();
	test_fmadd_s_74();
	test_fmadd_s_75();
	test_fmadd_s_76();
	test_fmadd_s_77();
	test_fmadd_s_78();
	test_fmadd_s_79();
	test_fmadd_s_80();
	test_fmadd_s_81();
	test_fmadd_s_82();
	test_fmadd_s_83();
	test_fmadd_s_84();
	test_fmadd_s_85();
	test_fmadd_s_86();
	test_fmadd_s_87();
	test_fmadd_s_88();
	test_fmadd_s_89();
	test_fmadd_s_90();
	test_fmadd_s_91();
	test_fmadd_s_92();
	test_fmadd_s_93();
	test_fmadd_s_94();
	test_fmadd_s_95();
	test_fmadd_s_96();
	test_fmadd_s_97();
	test_fmadd_s_98();
	test_fmadd_s_99();
	test_fmadd_s_100();
	test_fmadd_s_101();
	test_fmadd_s_102();
	test_fmadd_s_103();
	test_fmadd_s_104();
	test_fmadd_s_105();
	test_fmadd_s_106();
	test_fmadd_s_107();
	test_fmadd_s_108();
	test_fmadd_s_109();
	test_fmadd_s_110();
	test_fmadd_s_111();
	test_fmadd_s_112();
	test_fmadd_s_113();
	test_fmadd_s_114();
	test_fmadd_s_115();
	test_fmadd_s_116();
	test_fmadd_s_117();
	test_fmadd_s_118();
	test_fmadd_s_119();
	test_fmadd_s_120();
	test_fmadd_s_121();
	test_fmadd_s_122();
	test_fmadd_s_123();
	test_fmadd_s_124();
	test_fmadd_s_125();
	test_fmadd_s_126();
	test_fmadd_s_127();
	test_fmadd_s_128();
	test_fmadd_s_129();
	test_fmadd_s_130();
	test_fmadd_s_131();
	test_fmadd_s_132();
	test_fmadd_s_133();
	test_fmadd_s_134();
	test_fmadd_s_135();
	test_fmadd_s_136();
	test_fmadd_s_137();
	test_fmadd_s_138();
	test_fmadd_s_139();
	test_fmadd_s_140();
	test_fmadd_s_141();
	test_fmadd_s_142();
	test_fmadd_s_143();
	test_fmadd_s_144();
	test_fmadd_s_145();
	test_fmadd_s_146();
	test_fmadd_s_147();
	test_fmadd_s_148();
	test_fmadd_s_149();
	test_fmadd_s_150();
	test_fmadd_s_151();
	test_fmadd_s_152();
	test_fmadd_s_153();
	test_fmadd_s_154();
	test_fmadd_s_155();
	test_fmadd_s_156();
	test_fmadd_s_157();
	test_fmadd_s_158();
	test_fmadd_s_159();
	test_fmadd_s_160();
	test_fmadd_s_161();
	test_fmadd_s_162();
	test_fmadd_s_163();
	test_fmadd_s_164();
	test_fmadd_s_165();
	test_fmadd_s_166();
	test_fmadd_s_167();
	test_fmadd_s_168();
	test_fmadd_s_169();
	test_fmadd_s_170();
	test_fmadd_s_171();
	test_fmadd_s_172();
	test_fmadd_s_173();
	test_fmadd_s_174();
	test_fmadd_s_175();
	test_fmadd_s_176();
	test_fmadd_s_177();
	test_fmadd_s_178();
	test_fmadd_s_179();
	test_fmadd_s_180();
	test_fmadd_s_181();
	test_fmadd_s_182();
	test_fmadd_s_183();
	test_fmadd_s_184();
	test_fmadd_s_185();
	test_fmadd_s_186();
	test_fmadd_s_187();
	test_fmadd_s_188();
	test_fmadd_s_189();
	test_fmadd_s_190();
	test_fmadd_s_191();
	test_fmadd_s_192();
	test_fmadd_s_193();
	test_fmadd_s_194();
	test_fmadd_s_195();
	test_fmadd_s_196();
	test_fmadd_s_197();
	test_fmadd_s_198();
	test_fmadd_s_199();
	test_fmadd_s_200();
	test_fmadd_s_201();
	test_fmadd_s_202();
	test_fmadd_s_203();
	test_fmadd_s_204();
	test_fmadd_s_205();
	test_fmadd_s_206();
	test_fmadd_s_207();
	test_fmadd_s_208();
	test_fmadd_s_209();
	test_fmadd_s_210();
	test_fmadd_s_211();
	test_fmadd_s_212();
	test_fmadd_s_213();
	test_fmadd_s_214();
	test_fmadd_s_215();
	test_fmadd_s_216();
	test_fmsub_s_1();
	test_fmsub_s_2();
	test_fmsub_s_3();
	test_fmsub_s_4();
	test_fmsub_s_5();
	test_fmsub_s_6();
	test_fmsub_s_7();
	test_fmsub_s_8();
	test_fmsub_s_9();
	test_fmsub_s_10();
	test_fmsub_s_11();
	test_fmsub_s_12();
	test_fmsub_s_13();
	test_fmsub_s_14();
	test_fmsub_s_15();
	test_fmsub_s_16();
	test_fmsub_s_17();
	test_fmsub_s_18();
	test_fmsub_s_19();
	test_fmsub_s_20();
	test_fmsub_s_21();
	test_fmsub_s_22();
	test_fmsub_s_23();
	test_fmsub_s_24();
	test_fmsub_s_25();
	test_fmsub_s_26();
	test_fmsub_s_27();
	test_fmsub_s_28();
	test_fmsub_s_29();
	test_fmsub_s_30();
	test_fmsub_s_31();
	test_fmsub_s_32();
	test_fmsub_s_33();
	test_fmsub_s_34();
	test_fmsub_s_35();
	test_fmsub_s_36();
	test_fmsub_s_37();
	test_fmsub_s_38();
	test_fmsub_s_39();
	test_fmsub_s_40();
	test_fmsub_s_41();
	test_fmsub_s_42();
	test_fmsub_s_43();
	test_fmsub_s_44();
	test_fmsub_s_45();
	test_fmsub_s_46();
	test_fmsub_s_47();
	test_fmsub_s_48();
	test_fmsub_s_49();
	test_fmsub_s_50();
	test_fmsub_s_51();
	test_fmsub_s_52();
	test_fmsub_s_53();
	test_fmsub_s_54();
	test_fmsub_s_55();
	test_fmsub_s_56();
	test_fmsub_s_57();
	test_fmsub_s_58();
	test_fmsub_s_59();
	test_fmsub_s_60();
	test_fmsub_s_61();
	test_fmsub_s_62();
	test_fmsub_s_63();
	test_fmsub_s_64();
	test_fmsub_s_65();
	test_fmsub_s_66();
	test_fmsub_s_67();
	test_fmsub_s_68();
	test_fmsub_s_69();
	test_fmsub_s_70();
	test_fmsub_s_71();
	test_fmsub_s_72();
	test_fmsub_s_73();
	test_fmsub_s_74();
	test_fmsub_s_75();
	test_fmsub_s_76();
	test_fmsub_s_77();
	test_fmsub_s_78();
	test_fmsub_s_79();
	test_fmsub_s_80();
	test_fmsub_s_81();
	test_fmsub_s_82();
	test_fmsub_s_83();
	test_fmsub_s_84();
	test_fmsub_s_85();
	test_fmsub_s_86();
	test_fmsub_s_87();
	test_fmsub_s_88();
	test_fmsub_s_89();
	test_fmsub_s_90();
	test_fmsub_s_91();
	test_fmsub_s_92();
	test_fmsub_s_93();
	test_fmsub_s_94();
	test_fmsub_s_95();
	test_fmsub_s_96();
	test_fmsub_s_97();
	test_fmsub_s_98();
	test_fmsub_s_99();
	test_fmsub_s_100();
	test_fmsub_s_101();
	test_fmsub_s_102();
	test_fmsub_s_103();
	test_fmsub_s_104();
	test_fmsub_s_105();
	test_fmsub_s_106();
	test_fmsub_s_107();
	test_fmsub_s_108();
	test_fmsub_s_109();
	test_fmsub_s_110();
	test_fmsub_s_111();
	test_fmsub_s_112();
	test_fmsub_s_113();
	test_fmsub_s_114();
	test_fmsub_s_115();
	test_fmsub_s_116();
	test_fmsub_s_117();
	test_fmsub_s_118();
	test_fmsub_s_119();
	test_fmsub_s_120();
	test_fmsub_s_121();
	test_fmsub_s_122();
	test_fmsub_s_123();
	test_fmsub_s_124();
	test_fmsub_s_125();
	test_fmsub_s_126();
	test_fmsub_s_127();
	test_fmsub_s_128();
	test_fmsub_s_129();
	test_fmsub_s_130();
	test_fmsub_s_131();
	test_fmsub_s_132();
	test_fmsub_s_133();
	test_fmsub_s_134();
	test_fmsub_s_135();
	test_fmsub_s_136();
	test_fmsub_s_137();
	test_fmsub_s_138();
	test_fmsub_s_139();
	test_fmsub_s_140();
	test_fmsub_s_141();
	test_fmsub_s_142();
	test_fmsub_s_143();
	test_fmsub_s_144();
	test_fmsub_s_145();
	test_fmsub_s_146();
	test_fmsub_s_147();
	test_fmsub_s_148();
	test_fmsub_s_149();
	test_fmsub_s_150();
	test_fmsub_s_151();
	test_fmsub_s_152();
	test_fmsub_s_153();
	test_fmsub_s_154();
	test_fmsub_s_155();
	test_fmsub_s_156();
	test_fmsub_s_157();
	test_fmsub_s_158();
	test_fmsub_s_159();
	test_fmsub_s_160();
	test_fmsub_s_161();
	test_fmsub_s_162();
	test_fmsub_s_163();
	test_fmsub_s_164();
	test_fmsub_s_165();
	test_fmsub_s_166();
	test_fmsub_s_167();
	test_fmsub_s_168();
	test_fmsub_s_169();
	test_fmsub_s_170();
	test_fmsub_s_171();
	test_fmsub_s_172();
	test_fmsub_s_173();
	test_fmsub_s_174();
	test_fmsub_s_175();
	test_fmsub_s_176();
	test_fmsub_s_177();
	test_fmsub_s_178();
	test_fmsub_s_179();
	test_fmsub_s_180();
	test_fmsub_s_181();
	test_fmsub_s_182();
	test_fmsub_s_183();
	test_fmsub_s_184();
	test_fmsub_s_185();
	test_fmsub_s_186();
	test_fmsub_s_187();
	test_fmsub_s_188();
	test_fmsub_s_189();
	test_fmsub_s_190();
	test_fmsub_s_191();
	test_fmsub_s_192();
	test_fmsub_s_193();
	test_fmsub_s_194();
	test_fmsub_s_195();
	test_fmsub_s_196();
	test_fmsub_s_197();
	test_fmsub_s_198();
	test_fmsub_s_199();
	test_fmsub_s_200();
	test_fmsub_s_201();
	test_fmsub_s_202();
	test_fmsub_s_203();
	test_fmsub_s_204();
	test_fmsub_s_205();
	test_fmsub_s_206();
	test_fmsub_s_207();
	test_fmsub_s_208();
	test_fmsub_s_209();
	test_fmsub_s_210();
	test_fmsub_s_211();
	test_fmsub_s_212();
	test_fmsub_s_213();
	test_fmsub_s_214();
	test_fmsub_s_215();
	test_fmsub_s_216();
	test_fnmsub_s_1();
	test_fnmsub_s_2();
	test_fnmsub_s_3();
	test_fnmsub_s_4();
	test_fnmsub_s_5();
	test_fnmsub_s_6();
	test_fnmsub_s_7();
	test_fnmsub_s_8();
	test_fnmsub_s_9();
	test_fnmsub_s_10();
	test_fnmsub_s_11();
	test_fnmsub_s_12();
	test_fnmsub_s_13();
	test_fnmsub_s_14();
	test_fnmsub_s_15();
	test_fnmsub_s_16();
	test_fnmsub_s_17();
	test_fnmsub_s_18();
	test_fnmsub_s_19();
	test_fnmsub_s_20();
	test_fnmsub_s_21();
	test_fnmsub_s_22();
	test_fnmsub_s_23();
	test_fnmsub_s_24();
	test_fnmsub_s_25();
	test_fnmsub_s_26();
	test_fnmsub_s_27();
	test_fnmsub_s_28();
	test_fnmsub_s_29();
	test_fnmsub_s_30();
	test_fnmsub_s_31();
	test_fnmsub_s_32();
	test_fnmsub_s_33();
	test_fnmsub_s_34();
	test_fnmsub_s_35();
	test_fnmsub_s_36();
	test_fnmsub_s_37();
	test_fnmsub_s_38();
	test_fnmsub_s_39();
	test_fnmsub_s_40();
	test_fnmsub_s_41();
	test_fnmsub_s_42();
	test_fnmsub_s_43();
	test_fnmsub_s_44();
	test_fnmsub_s_45();
	test_fnmsub_s_46();
	test_fnmsub_s_47();
	test_fnmsub_s_48();
	test_fnmsub_s_49();
	test_fnmsub_s_50();
	test_fnmsub_s_51();
	test_fnmsub_s_52();
	test_fnmsub_s_53();
	test_fnmsub_s_54();
	test_fnmsub_s_55();
	test_fnmsub_s_56();
	test_fnmsub_s_57();
	test_fnmsub_s_58();
	test_fnmsub_s_59();
	test_fnmsub_s_60();
	test_fnmsub_s_61();
	test_fnmsub_s_62();
	test_fnmsub_s_63();
	test_fnmsub_s_64();
	test_fnmsub_s_65();
	test_fnmsub_s_66();
	test_fnmsub_s_67();
	test_fnmsub_s_68();
	test_fnmsub_s_69();
	test_fnmsub_s_70();
	test_fnmsub_s_71();
	test_fnmsub_s_72();
	test_fnmsub_s_73();
	test_fnmsub_s_74();
	test_fnmsub_s_75();
	test_fnmsub_s_76();
	test_fnmsub_s_77();
	test_fnmsub_s_78();
	test_fnmsub_s_79();
	test_fnmsub_s_80();
	test_fnmsub_s_81();
	test_fnmsub_s_82();
	test_fnmsub_s_83();
	test_fnmsub_s_84();
	test_fnmsub_s_85();
	test_fnmsub_s_86();
	test_fnmsub_s_87();
	test_fnmsub_s_88();
	test_fnmsub_s_89();
	test_fnmsub_s_90();
	test_fnmsub_s_91();
	test_fnmsub_s_92();
	test_fnmsub_s_93();
	test_fnmsub_s_94();
	test_fnmsub_s_95();
	test_fnmsub_s_96();
	test_fnmsub_s_97();
	test_fnmsub_s_98();
	test_fnmsub_s_99();
	test_fnmsub_s_100();
	test_fnmsub_s_101();
	test_fnmsub_s_102();
	test_fnmsub_s_103();
	test_fnmsub_s_104();
	test_fnmsub_s_105();
	test_fnmsub_s_106();
	test_fnmsub_s_107();
	test_fnmsub_s_108();
	test_fnmsub_s_109();
	test_fnmsub_s_110();
	test_fnmsub_s_111();
	test_fnmsub_s_112();
	test_fnmsub_s_113();
	test_fnmsub_s_114();
	test_fnmsub_s_115();
	test_fnmsub_s_116();
	test_fnmsub_s_117();
	test_fnmsub_s_118();
	test_fnmsub_s_119();
	test_fnmsub_s_120();
	test_fnmsub_s_121();
	test_fnmsub_s_122();
	test_fnmsub_s_123();
	test_fnmsub_s_124();
	test_fnmsub_s_125();
	test_fnmsub_s_126();
	test_fnmsub_s_127();
	test_fnmsub_s_128();
	test_fnmsub_s_129();
	test_fnmsub_s_130();
	test_fnmsub_s_131();
	test_fnmsub_s_132();
	test_fnmsub_s_133();
	test_fnmsub_s_134();
	test_fnmsub_s_135();
	test_fnmsub_s_136();
	test_fnmsub_s_137();
	test_fnmsub_s_138();
	test_fnmsub_s_139();
	test_fnmsub_s_140();
	test_fnmsub_s_141();
	test_fnmsub_s_142();
	test_fnmsub_s_143();
	test_fnmsub_s_144();
	test_fnmsub_s_145();
	test_fnmsub_s_146();
	test_fnmsub_s_147();
	test_fnmsub_s_148();
	test_fnmsub_s_149();
	test_fnmsub_s_150();
	test_fnmsub_s_151();
	test_fnmsub_s_152();
	test_fnmsub_s_153();
	test_fnmsub_s_154();
	test_fnmsub_s_155();
	test_fnmsub_s_156();
	test_fnmsub_s_157();
	test_fnmsub_s_158();
	test_fnmsub_s_159();
	test_fnmsub_s_160();
	test_fnmsub_s_161();
	test_fnmsub_s_162();
	test_fnmsub_s_163();
	test_fnmsub_s_164();
	test_fnmsub_s_165();
	test_fnmsub_s_166();
	test_fnmsub_s_167();
	test_fnmsub_s_168();
	test_fnmsub_s_169();
	test_fnmsub_s_170();
	test_fnmsub_s_171();
	test_fnmsub_s_172();
	test_fnmsub_s_173();
	test_fnmsub_s_174();
	test_fnmsub_s_175();
	test_fnmsub_s_176();
	test_fnmsub_s_177();
	test_fnmsub_s_178();
	test_fnmsub_s_179();
	test_fnmsub_s_180();
	test_fnmsub_s_181();
	test_fnmsub_s_182();
	test_fnmsub_s_183();
	test_fnmsub_s_184();
	test_fnmsub_s_185();
	test_fnmsub_s_186();
	test_fnmsub_s_187();
	test_fnmsub_s_188();
	test_fnmsub_s_189();
	test_fnmsub_s_190();
	test_fnmsub_s_191();
	test_fnmsub_s_192();
	test_fnmsub_s_193();
	test_fnmsub_s_194();
	test_fnmsub_s_195();
	test_fnmsub_s_196();
	test_fnmsub_s_197();
	test_fnmsub_s_198();
	test_fnmsub_s_199();
	test_fnmsub_s_200();
	test_fnmsub_s_201();
	test_fnmsub_s_202();
	test_fnmsub_s_203();
	test_fnmsub_s_204();
	test_fnmsub_s_205();
	test_fnmsub_s_206();
	test_fnmsub_s_207();
	test_fnmsub_s_208();
	test_fnmsub_s_209();
	test_fnmsub_s_210();
	test_fnmsub_s_211();
	test_fnmsub_s_212();
	test_fnmsub_s_213();
	test_fnmsub_s_214();
	test_fnmsub_s_215();
	test_fnmsub_s_216();
	test_fnmadd_s_1();
	test_fnmadd_s_2();
	test_fnmadd_s_3();
	test_fnmadd_s_4();
	test_fnmadd_s_5();
	test_fnmadd_s_6();
	test_fnmadd_s_7();
	test_fnmadd_s_8();
	test_fnmadd_s_9();
	test_fnmadd_s_10();
	test_fnmadd_s_11();
	test_fnmadd_s_12();
	test_fnmadd_s_13();
	test_fnmadd_s_14();
	test_fnmadd_s_15();
	test_fnmadd_s_16();
	test_fnmadd_s_17();
	test_fnmadd_s_18();
	test_fnmadd_s_19();
	test_fnmadd_s_20();
	test_fnmadd_s_21();
	test_fnmadd_s_22();
	test_fnmadd_s_23();
	test_fnmadd_s_24();
	test_fnmadd_s_25();
	test_fnmadd_s_26();
	test_fnmadd_s_27();
	test_fnmadd_s_28();
	test_fnmadd_s_29();
	test_fnmadd_s_30();
	test_fnmadd_s_31();
	test_fnmadd_s_32();
	test_fnmadd_s_33();
	test_fnmadd_s_34();
	test_fnmadd_s_35();
	test_fnmadd_s_36();
	test_fnmadd_s_37();
	test_fnmadd_s_38();
	test_fnmadd_s_39();
	test_fnmadd_s_40();
	test_fnmadd_s_41();
	test_fnmadd_s_42();
	test_fnmadd_s_43();
	test_fnmadd_s_44();
	test_fnmadd_s_45();
	test_fnmadd_s_46();
	test_fnmadd_s_47();
	test_fnmadd_s_48();
	test_fnmadd_s_49();
	test_fnmadd_s_50();
	test_fnmadd_s_51();
	test_fnmadd_s_52();
	test_fnmadd_s_53();
	test_fnmadd_s_54();
	test_fnmadd_s_55();
	test_fnmadd_s_56();
	test_fnmadd_s_57();
	test_fnmadd_s_58();
	test_fnmadd_s_59();
	test_fnmadd_s_60();
	test_fnmadd_s_61();
	test_fnmadd_s_62();
	test_fnmadd_s_63();
	test_fnmadd_s_64();
	test_fnmadd_s_65();
	test_fnmadd_s_66();
	test_fnmadd_s_67();
	test_fnmadd_s_68();
	test_fnmadd_s_69();
	test_fnmadd_s_70();
	test_fnmadd_s_71();
	test_fnmadd_s_72();
	test_fnmadd_s_73();
	test_fnmadd_s_74();
	test_fnmadd_s_75();
	test_fnmadd_s_76();
	test_fnmadd_s_77();
	test_fnmadd_s_78();
	test_fnmadd_s_79();
	test_fnmadd_s_80();
	test_fnmadd_s_81();
	test_fnmadd_s_82();
	test_fnmadd_s_83();
	test_fnmadd_s_84();
	test_fnmadd_s_85();
	test_fnmadd_s_86();
	test_fnmadd_s_87();
	test_fnmadd_s_88();
	test_fnmadd_s_89();
	test_fnmadd_s_90();
	test_fnmadd_s_91();
	test_fnmadd_s_92();
	test_fnmadd_s_93();
	test_fnmadd_s_94();
	test_fnmadd_s_95();
	test_fnmadd_s_96();
	test_fnmadd_s_97();
	test_fnmadd_s_98();
	test_fnmadd_s_99();
	test_fnmadd_s_100();
	test_fnmadd_s_101();
	test_fnmadd_s_102();
	test_fnmadd_s_103();
	test_fnmadd_s_104();
	test_fnmadd_s_105();
	test_fnmadd_s_106();
	test_fnmadd_s_107();
	test_fnmadd_s_108();
	test_fnmadd_s_109();
	test_fnmadd_s_110();
	test_fnmadd_s_111();
	test_fnmadd_s_112();
	test_fnmadd_s_113();
	test_fnmadd_s_114();
	test_fnmadd_s_115();
	test_fnmadd_s_116();
	test_fnmadd_s_117();
	test_fnmadd_s_118();
	test_fnmadd_s_119();
	test_fnmadd_s_120();
	test_fnmadd_s_121();
	test_fnmadd_s_122();
	test_fnmadd_s_123();
	test_fnmadd_s_124();
	test_fnmadd_s_125();
	test_fnmadd_s_126();
	test_fnmadd_s_127();
	test_fnmadd_s_128();
	test_fnmadd_s_129();
	test_fnmadd_s_130();
	test_fnmadd_s_131();
	test_fnmadd_s_132();
	test_fnmadd_s_133();
	test_fnmadd_s_134();
	test_fnmadd_s_135();
	test_fnmadd_s_136();
	test_fnmadd_s_137();
	test_fnmadd_s_138();
	test_fnmadd_s_139();
	test_fnmadd_s_140();
	test_fnmadd_s_141();
	test_fnmadd_s_142();
	test_fnmadd_s_143();
	test_fnmadd_s_144();
	test_fnmadd_s_145();
	test_fnmadd_s_146();
	test_fnmadd_s_147();
	test_fnmadd_s_148();
	test_fnmadd_s_149();
	test_fnmadd_s_150();
	test_fnmadd_s_151();
	test_fnmadd_s_152();
	test_fnmadd_s_153();
	test_fnmadd_s_154();
	test_fnmadd_s_155();
	test_fnmadd_s_156();
	test_fnmadd_s_157();
	test_fnmadd_s_158();
	test_fnmadd_s_159();
	test_fnmadd_s_160();
	test_fnmadd_s_161();
	test_fnmadd_s_162();
	test_fnmadd_s_163();
	test_fnmadd_s_164();
	test_fnmadd_s_165();
	test_fnmadd_s_166();
	test_fnmadd_s_167();
	test_fnmadd_s_168();
	test_fnmadd_s_169();
	test_fnmadd_s_170();
	test_fnmadd_s_171();
	test_fnmadd_s_172();
	test_fnmadd_s_173();
	test_fnmadd_s_174();
	test_fnmadd_s_175();
	test_fnmadd_s_176();
	test_fnmadd_s_177();
	test_fnmadd_s_178();
	test_fnmadd_s_179();
	test_fnmadd_s_180();
	test_fnmadd_s_181();
	test_fnmadd_s_182();
	test_fnmadd_s_183();
	test_fnmadd_s_184();
	test_fnmadd_s_185();
	test_fnmadd_s_186();
	test_fnmadd_s_187();
	test_fnmadd_s_188();
	test_fnmadd_s_189();
	test_fnmadd_s_190();
	test_fnmadd_s_191();
	test_fnmadd_s_192();
	test_fnmadd_s_193();
	test_fnmadd_s_194();
	test_fnmadd_s_195();
	test_fnmadd_s_196();
	test_fnmadd_s_197();
	test_fnmadd_s_198();
	test_fnmadd_s_199();
	test_fnmadd_s_200();
	test_fnmadd_s_201();
	test_fnmadd_s_202();
	test_fnmadd_s_203();
	test_fnmadd_s_204();
	test_fnmadd_s_205();
	test_fnmadd_s_206();
	test_fnmadd_s_207();
	test_fnmadd_s_208();
	test_fnmadd_s_209();
	test_fnmadd_s_210();
	test_fnmadd_s_211();
	test_fnmadd_s_212();
	test_fnmadd_s_213();
	test_fnmadd_s_214();
	test_fnmadd_s_215();
	test_fnmadd_s_216();
	test_fadd_s_1();
	test_fadd_s_2();
	test_fadd_s_3();
	test_fadd_s_4();
	test_fadd_s_5();
	test_fadd_s_6();
	test_fadd_s_7();
	test_fadd_s_8();
	test_fadd_s_9();
	test_fadd_s_10();
	test_fadd_s_11();
	test_fadd_s_12();
	test_fadd_s_13();
	test_fadd_s_14();
	test_fadd_s_15();
	test_fadd_s_16();
	test_fadd_s_17();
	test_fadd_s_18();
	test_fadd_s_19();
	test_fadd_s_20();
	test_fadd_s_21();
	test_fadd_s_22();
	test_fadd_s_23();
	test_fadd_s_24();
	test_fadd_s_25();
	test_fadd_s_26();
	test_fadd_s_27();
	test_fadd_s_28();
	test_fadd_s_29();
	test_fadd_s_30();
	test_fadd_s_31();
	test_fadd_s_32();
	test_fadd_s_33();
	test_fadd_s_34();
	test_fadd_s_35();
	test_fadd_s_36();
	test_fsub_s_1();
	test_fsub_s_2();
	test_fsub_s_3();
	test_fsub_s_4();
	test_fsub_s_5();
	test_fsub_s_6();
	test_fsub_s_7();
	test_fsub_s_8();
	test_fsub_s_9();
	test_fsub_s_10();
	test_fsub_s_11();
	test_fsub_s_12();
	test_fsub_s_13();
	test_fsub_s_14();
	test_fsub_s_15();
	test_fsub_s_16();
	test_fsub_s_17();
	test_fsub_s_18();
	test_fsub_s_19();
	test_fsub_s_20();
	test_fsub_s_21();
	test_fsub_s_22();
	test_fsub_s_23();
	test_fsub_s_24();
	test_fsub_s_25();
	test_fsub_s_26();
	test_fsub_s_27();
	test_fsub_s_28();
	test_fsub_s_29();
	test_fsub_s_30();
	test_fsub_s_31();
	test_fsub_s_32();
	test_fsub_s_33();
	test_fsub_s_34();
	test_fsub_s_35();
	test_fsub_s_36();
	test_fmul_s_1();
	test_fmul_s_2();
	test_fmul_s_3();
	test_fmul_s_4();
	test_fmul_s_5();
	test_fmul_s_6();
	test_fmul_s_7();
	test_fmul_s_8();
	test_fmul_s_9();
	test_fmul_s_10();
	test_fmul_s_11();
	test_fmul_s_12();
	test_fmul_s_13();
	test_fmul_s_14();
	test_fmul_s_15();
	test_fmul_s_16();
	test_fmul_s_17();
	test_fmul_s_18();
	test_fmul_s_19();
	test_fmul_s_20();
	test_fmul_s_21();
	test_fmul_s_22();
	test_fmul_s_23();
	test_fmul_s_24();
	test_fmul_s_25();
	test_fmul_s_26();
	test_fmul_s_27();
	test_fmul_s_28();
	test_fmul_s_29();
	test_fmul_s_30();
	test_fmul_s_31();
	test_fmul_s_32();
	test_fmul_s_33();
	test_fmul_s_34();
	test_fmul_s_35();
	test_fmul_s_36();
	test_fdiv_s_1();
	test_fdiv_s_2();
	test_fdiv_s_3();
	test_fdiv_s_4();
	test_fdiv_s_5();
	test_fdiv_s_6();
	test_fdiv_s_7();
	test_fdiv_s_8();
	test_fdiv_s_9();
	test_fdiv_s_10();
	test_fdiv_s_11();
	test_fdiv_s_12();
	test_fdiv_s_13();
	test_fdiv_s_14();
	test_fdiv_s_15();
	test_fdiv_s_16();
	test_fdiv_s_17();
	test_fdiv_s_18();
	test_fdiv_s_19();
	test_fdiv_s_20();
	test_fdiv_s_21();
	test_fdiv_s_22();
	test_fdiv_s_23();
	test_fdiv_s_24();
	test_fdiv_s_25();
	test_fdiv_s_26();
	test_fdiv_s_27();
	test_fdiv_s_28();
	test_fdiv_s_29();
	test_fdiv_s_30();
	test_fdiv_s_31();
	test_fdiv_s_32();
	test_fdiv_s_33();
	test_fdiv_s_34();
	test_fdiv_s_35();
	test_fdiv_s_36();
	test_fmin_s_1();
	test_fmin_s_2();
	test_fmin_s_3();
	test_fmin_s_4();
	test_fmin_s_5();
	test_fmin_s_6();
	test_fmin_s_7();
	test_fmin_s_8();
	test_fmin_s_9();
	test_fmin_s_10();
	test_fmin_s_11();
	test_fmin_s_12();
	test_fmin_s_13();
	test_fmin_s_14();
	test_fmin_s_15();
	test_fmin_s_16();
	test_fmin_s_17();
	test_fmin_s_18();
	test_fmin_s_19();
	test_fmin_s_20();
	test_fmin_s_21();
	test_fmin_s_22();
	test_fmin_s_23();
	test_fmin_s_24();
	test_fmin_s_25();
	test_fmin_s_26();
	test_fmin_s_27();
	test_fmin_s_28();
	test_fmin_s_29();
	test_fmin_s_30();
	test_fmin_s_31();
	test_fmin_s_32();
	test_fmin_s_33();
	test_fmin_s_34();
	test_fmin_s_35();
	test_fmin_s_36();
	test_fmax_s_1();
	test_fmax_s_2();
	test_fmax_s_3();
	test_fmax_s_4();
	test_fmax_s_5();
	test_fmax_s_6();
	test_fmax_s_7();
	test_fmax_s_8();
	test_fmax_s_9();
	test_fmax_s_10();
	test_fmax_s_11();
	test_fmax_s_12();
	test_fmax_s_13();
	test_fmax_s_14();
	test_fmax_s_15();
	test_fmax_s_16();
	test_fmax_s_17();
	test_fmax_s_18();
	test_fmax_s_19();
	test_fmax_s_20();
	test_fmax_s_21();
	test_fmax_s_22();
	test_fmax_s_23();
	test_fmax_s_24();
	test_fmax_s_25();
	test_fmax_s_26();
	test_fmax_s_27();
	test_fmax_s_28();
	test_fmax_s_29();
	test_fmax_s_30();
	test_fmax_s_31();
	test_fmax_s_32();
	test_fmax_s_33();
	test_fmax_s_34();
	test_fmax_s_35();
	test_fmax_s_36();
	test_fsqrt_s_1();
	test_fsqrt_s_2();
	test_fsqrt_s_3();
	test_fsqrt_s_4();
	test_fsqrt_s_5();
	test_fsqrt_s_6();
	test_fle_s_1();
	test_fle_s_2();
	test_fle_s_3();
	test_fle_s_4();
	test_fle_s_5();
	test_fle_s_6();
	test_fle_s_7();
	test_fle_s_8();
	test_fle_s_9();
	test_fle_s_10();
	test_fle_s_11();
	test_fle_s_12();
	test_fle_s_13();
	test_fle_s_14();
	test_fle_s_15();
	test_fle_s_16();
	test_fle_s_17();
	test_fle_s_18();
	test_fle_s_19();
	test_fle_s_20();
	test_fle_s_21();
	test_fle_s_22();
	test_fle_s_23();
	test_fle_s_24();
	test_fle_s_25();
	test_fle_s_26();
	test_fle_s_27();
	test_fle_s_28();
	test_fle_s_29();
	test_fle_s_30();
	test_fle_s_31();
	test_fle_s_32();
	test_fle_s_33();
	test_fle_s_34();
	test_fle_s_35();
	test_fle_s_36();
	test_flt_s_1();
	test_flt_s_2();
	test_flt_s_3();
	test_flt_s_4();
	test_flt_s_5();
	test_flt_s_6();
	test_flt_s_7();
	test_flt_s_8();
	test_flt_s_9();
	test_flt_s_10();
	test_flt_s_11();
	test_flt_s_12();
	test_flt_s_13();
	test_flt_s_14();
	test_flt_s_15();
	test_flt_s_16();
	test_flt_s_17();
	test_flt_s_18();
	test_flt_s_19();
	test_flt_s_20();
	test_flt_s_21();
	test_flt_s_22();
	test_flt_s_23();
	test_flt_s_24();
	test_flt_s_25();
	test_flt_s_26();
	test_flt_s_27();
	test_flt_s_28();
	test_flt_s_29();
	test_flt_s_30();
	test_flt_s_31();
	test_flt_s_32();
	test_flt_s_33();
	test_flt_s_34();
	test_flt_s_35();
	test_flt_s_36();
	test_feq_s_1();
	test_feq_s_2();
	test_feq_s_3();
	test_feq_s_4();
	test_feq_s_5();
	test_feq_s_6();
	test_feq_s_7();
	test_feq_s_8();
	test_feq_s_9();
	test_feq_s_10();
	test_feq_s_11();
	test_feq_s_12();
	test_feq_s_13();
	test_feq_s_14();
	test_feq_s_15();
	test_feq_s_16();
	test_feq_s_17();
	test_feq_s_18();
	test_feq_s_19();
	test_feq_s_20();
	test_feq_s_21();
	test_feq_s_22();
	test_feq_s_23();
	test_feq_s_24();
	test_feq_s_25();
	test_feq_s_26();
	test_feq_s_27();
	test_feq_s_28();
	test_feq_s_29();
	test_feq_s_30();
	test_feq_s_31();
	test_feq_s_32();
	test_feq_s_33();
	test_feq_s_34();
	test_feq_s_35();
	test_feq_s_36();
	test_fcvt_w_s_1();
	test_fcvt_w_s_2();
	test_fcvt_w_s_3();
	test_fcvt_w_s_4();
	test_fcvt_w_s_5();
	test_fcvt_w_s_6();
	test_fcvt_wu_s_1();
	test_fcvt_wu_s_2();
	test_fcvt_wu_s_3();
	test_fcvt_wu_s_4();
	test_fcvt_wu_s_5();
	test_fcvt_wu_s_6();
	test_fcvt_s_w_1();
	test_fcvt_s_w_2();
	test_fcvt_s_wu_1();
	test_fmv_x_s_1();
	test_fmv_x_s_2();
	test_fmv_x_s_3();
	test_fmv_x_s_4();
	test_fmv_x_s_5();
	test_fmv_x_s_6();
	test_fclass_s_1();
	test_fclass_s_2();
	test_fclass_s_3();
	test_fclass_s_4();
	test_fclass_s_5();
	test_fclass_s_6();
	test_fmv_s_x_1();
	test_fmv_s_x_2();
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_s_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_s_2();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_s_3();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_s_4();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_s_5();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_s_6();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_s_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_s_2();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_s_3();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_s_4();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_s_5();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_s_6();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_s_l_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_s_l_2();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_s_lu_1();
#endif
	test_fmadd_d_1();
	test_fmadd_d_2();
	test_fmadd_d_3();
	test_fmadd_d_4();
	test_fmadd_d_5();
	test_fmadd_d_6();
	test_fmadd_d_7();
	test_fmadd_d_8();
	test_fmadd_d_9();
	test_fmadd_d_10();
	test_fmadd_d_11();
	test_fmadd_d_12();
	test_fmadd_d_13();
	test_fmadd_d_14();
	test_fmadd_d_15();
	test_fmadd_d_16();
	test_fmadd_d_17();
	test_fmadd_d_18();
	test_fmadd_d_19();
	test_fmadd_d_20();
	test_fmadd_d_21();
	test_fmadd_d_22();
	test_fmadd_d_23();
	test_fmadd_d_24();
	test_fmadd_d_25();
	test_fmadd_d_26();
	test_fmadd_d_27();
	test_fmadd_d_28();
	test_fmadd_d_29();
	test_fmadd_d_30();
	test_fmadd_d_31();
	test_fmadd_d_32();
	test_fmadd_d_33();
	test_fmadd_d_34();
	test_fmadd_d_35();
	test_fmadd_d_36();
	test_fmadd_d_37();
	test_fmadd_d_38();
	test_fmadd_d_39();
	test_fmadd_d_40();
	test_fmadd_d_41();
	test_fmadd_d_42();
	test_fmadd_d_43();
	test_fmadd_d_44();
	test_fmadd_d_45();
	test_fmadd_d_46();
	test_fmadd_d_47();
	test_fmadd_d_48();
	test_fmadd_d_49();
	test_fmadd_d_50();
	test_fmadd_d_51();
	test_fmadd_d_52();
	test_fmadd_d_53();
	test_fmadd_d_54();
	test_fmadd_d_55();
	test_fmadd_d_56();
	test_fmadd_d_57();
	test_fmadd_d_58();
	test_fmadd_d_59();
	test_fmadd_d_60();
	test_fmadd_d_61();
	test_fmadd_d_62();
	test_fmadd_d_63();
	test_fmadd_d_64();
	test_fmadd_d_65();
	test_fmadd_d_66();
	test_fmadd_d_67();
	test_fmadd_d_68();
	test_fmadd_d_69();
	test_fmadd_d_70();
	test_fmadd_d_71();
	test_fmadd_d_72();
	test_fmadd_d_73();
	test_fmadd_d_74();
	test_fmadd_d_75();
	test_fmadd_d_76();
	test_fmadd_d_77();
	test_fmadd_d_78();
	test_fmadd_d_79();
	test_fmadd_d_80();
	test_fmadd_d_81();
	test_fmadd_d_82();
	test_fmadd_d_83();
	test_fmadd_d_84();
	test_fmadd_d_85();
	test_fmadd_d_86();
	test_fmadd_d_87();
	test_fmadd_d_88();
	test_fmadd_d_89();
	test_fmadd_d_90();
	test_fmadd_d_91();
	test_fmadd_d_92();
	test_fmadd_d_93();
	test_fmadd_d_94();
	test_fmadd_d_95();
	test_fmadd_d_96();
	test_fmadd_d_97();
	test_fmadd_d_98();
	test_fmadd_d_99();
	test_fmadd_d_100();
	test_fmadd_d_101();
	test_fmadd_d_102();
	test_fmadd_d_103();
	test_fmadd_d_104();
	test_fmadd_d_105();
	test_fmadd_d_106();
	test_fmadd_d_107();
	test_fmadd_d_108();
	test_fmadd_d_109();
	test_fmadd_d_110();
	test_fmadd_d_111();
	test_fmadd_d_112();
	test_fmadd_d_113();
	test_fmadd_d_114();
	test_fmadd_d_115();
	test_fmadd_d_116();
	test_fmadd_d_117();
	test_fmadd_d_118();
	test_fmadd_d_119();
	test_fmadd_d_120();
	test_fmadd_d_121();
	test_fmadd_d_122();
	test_fmadd_d_123();
	test_fmadd_d_124();
	test_fmadd_d_125();
	test_fmadd_d_126();
	test_fmadd_d_127();
	test_fmadd_d_128();
	test_fmadd_d_129();
	test_fmadd_d_130();
	test_fmadd_d_131();
	test_fmadd_d_132();
	test_fmadd_d_133();
	test_fmadd_d_134();
	test_fmadd_d_135();
	test_fmadd_d_136();
	test_fmadd_d_137();
	test_fmadd_d_138();
	test_fmadd_d_139();
	test_fmadd_d_140();
	test_fmadd_d_141();
	test_fmadd_d_142();
	test_fmadd_d_143();
	test_fmadd_d_144();
	test_fmadd_d_145();
	test_fmadd_d_146();
	test_fmadd_d_147();
	test_fmadd_d_148();
	test_fmadd_d_149();
	test_fmadd_d_150();
	test_fmadd_d_151();
	test_fmadd_d_152();
	test_fmadd_d_153();
	test_fmadd_d_154();
	test_fmadd_d_155();
	test_fmadd_d_156();
	test_fmadd_d_157();
	test_fmadd_d_158();
	test_fmadd_d_159();
	test_fmadd_d_160();
	test_fmadd_d_161();
	test_fmadd_d_162();
	test_fmadd_d_163();
	test_fmadd_d_164();
	test_fmadd_d_165();
	test_fmadd_d_166();
	test_fmadd_d_167();
	test_fmadd_d_168();
	test_fmadd_d_169();
	test_fmadd_d_170();
	test_fmadd_d_171();
	test_fmadd_d_172();
	test_fmadd_d_173();
	test_fmadd_d_174();
	test_fmadd_d_175();
	test_fmadd_d_176();
	test_fmadd_d_177();
	test_fmadd_d_178();
	test_fmadd_d_179();
	test_fmadd_d_180();
	test_fmadd_d_181();
	test_fmadd_d_182();
	test_fmadd_d_183();
	test_fmadd_d_184();
	test_fmadd_d_185();
	test_fmadd_d_186();
	test_fmadd_d_187();
	test_fmadd_d_188();
	test_fmadd_d_189();
	test_fmadd_d_190();
	test_fmadd_d_191();
	test_fmadd_d_192();
	test_fmadd_d_193();
	test_fmadd_d_194();
	test_fmadd_d_195();
	test_fmadd_d_196();
	test_fmadd_d_197();
	test_fmadd_d_198();
	test_fmadd_d_199();
	test_fmadd_d_200();
	test_fmadd_d_201();
	test_fmadd_d_202();
	test_fmadd_d_203();
	test_fmadd_d_204();
	test_fmadd_d_205();
	test_fmadd_d_206();
	test_fmadd_d_207();
	test_fmadd_d_208();
	test_fmadd_d_209();
	test_fmadd_d_210();
	test_fmadd_d_211();
	test_fmadd_d_212();
	test_fmadd_d_213();
	test_fmadd_d_214();
	test_fmadd_d_215();
	test_fmadd_d_216();
	test_fmsub_d_1();
	test_fmsub_d_2();
	test_fmsub_d_3();
	test_fmsub_d_4();
	test_fmsub_d_5();
	test_fmsub_d_6();
	test_fmsub_d_7();
	test_fmsub_d_8();
	test_fmsub_d_9();
	test_fmsub_d_10();
	test_fmsub_d_11();
	test_fmsub_d_12();
	test_fmsub_d_13();
	test_fmsub_d_14();
	test_fmsub_d_15();
	test_fmsub_d_16();
	test_fmsub_d_17();
	test_fmsub_d_18();
	test_fmsub_d_19();
	test_fmsub_d_20();
	test_fmsub_d_21();
	test_fmsub_d_22();
	test_fmsub_d_23();
	test_fmsub_d_24();
	test_fmsub_d_25();
	test_fmsub_d_26();
	test_fmsub_d_27();
	test_fmsub_d_28();
	test_fmsub_d_29();
	test_fmsub_d_30();
	test_fmsub_d_31();
	test_fmsub_d_32();
	test_fmsub_d_33();
	test_fmsub_d_34();
	test_fmsub_d_35();
	test_fmsub_d_36();
	test_fmsub_d_37();
	test_fmsub_d_38();
	test_fmsub_d_39();
	test_fmsub_d_40();
	test_fmsub_d_41();
	test_fmsub_d_42();
	test_fmsub_d_43();
	test_fmsub_d_44();
	test_fmsub_d_45();
	test_fmsub_d_46();
	test_fmsub_d_47();
	test_fmsub_d_48();
	test_fmsub_d_49();
	test_fmsub_d_50();
	test_fmsub_d_51();
	test_fmsub_d_52();
	test_fmsub_d_53();
	test_fmsub_d_54();
	test_fmsub_d_55();
	test_fmsub_d_56();
	test_fmsub_d_57();
	test_fmsub_d_58();
	test_fmsub_d_59();
	test_fmsub_d_60();
	test_fmsub_d_61();
	test_fmsub_d_62();
	test_fmsub_d_63();
	test_fmsub_d_64();
	test_fmsub_d_65();
	test_fmsub_d_66();
	test_fmsub_d_67();
	test_fmsub_d_68();
	test_fmsub_d_69();
	test_fmsub_d_70();
	test_fmsub_d_71();
	test_fmsub_d_72();
	test_fmsub_d_73();
	test_fmsub_d_74();
	test_fmsub_d_75();
	test_fmsub_d_76();
	test_fmsub_d_77();
	test_fmsub_d_78();
	test_fmsub_d_79();
	test_fmsub_d_80();
	test_fmsub_d_81();
	test_fmsub_d_82();
	test_fmsub_d_83();
	test_fmsub_d_84();
	test_fmsub_d_85();
	test_fmsub_d_86();
	test_fmsub_d_87();
	test_fmsub_d_88();
	test_fmsub_d_89();
	test_fmsub_d_90();
	test_fmsub_d_91();
	test_fmsub_d_92();
	test_fmsub_d_93();
	test_fmsub_d_94();
	test_fmsub_d_95();
	test_fmsub_d_96();
	test_fmsub_d_97();
	test_fmsub_d_98();
	test_fmsub_d_99();
	test_fmsub_d_100();
	test_fmsub_d_101();
	test_fmsub_d_102();
	test_fmsub_d_103();
	test_fmsub_d_104();
	test_fmsub_d_105();
	test_fmsub_d_106();
	test_fmsub_d_107();
	test_fmsub_d_108();
	test_fmsub_d_109();
	test_fmsub_d_110();
	test_fmsub_d_111();
	test_fmsub_d_112();
	test_fmsub_d_113();
	test_fmsub_d_114();
	test_fmsub_d_115();
	test_fmsub_d_116();
	test_fmsub_d_117();
	test_fmsub_d_118();
	test_fmsub_d_119();
	test_fmsub_d_120();
	test_fmsub_d_121();
	test_fmsub_d_122();
	test_fmsub_d_123();
	test_fmsub_d_124();
	test_fmsub_d_125();
	test_fmsub_d_126();
	test_fmsub_d_127();
	test_fmsub_d_128();
	test_fmsub_d_129();
	test_fmsub_d_130();
	test_fmsub_d_131();
	test_fmsub_d_132();
	test_fmsub_d_133();
	test_fmsub_d_134();
	test_fmsub_d_135();
	test_fmsub_d_136();
	test_fmsub_d_137();
	test_fmsub_d_138();
	test_fmsub_d_139();
	test_fmsub_d_140();
	test_fmsub_d_141();
	test_fmsub_d_142();
	test_fmsub_d_143();
	test_fmsub_d_144();
	test_fmsub_d_145();
	test_fmsub_d_146();
	test_fmsub_d_147();
	test_fmsub_d_148();
	test_fmsub_d_149();
	test_fmsub_d_150();
	test_fmsub_d_151();
	test_fmsub_d_152();
	test_fmsub_d_153();
	test_fmsub_d_154();
	test_fmsub_d_155();
	test_fmsub_d_156();
	test_fmsub_d_157();
	test_fmsub_d_158();
	test_fmsub_d_159();
	test_fmsub_d_160();
	test_fmsub_d_161();
	test_fmsub_d_162();
	test_fmsub_d_163();
	test_fmsub_d_164();
	test_fmsub_d_165();
	test_fmsub_d_166();
	test_fmsub_d_167();
	test_fmsub_d_168();
	test_fmsub_d_169();
	test_fmsub_d_170();
	test_fmsub_d_171();
	test_fmsub_d_172();
	test_fmsub_d_173();
	test_fmsub_d_174();
	test_fmsub_d_175();
	test_fmsub_d_176();
	test_fmsub_d_177();
	test_fmsub_d_178();
	test_fmsub_d_179();
	test_fmsub_d_180();
	test_fmsub_d_181();
	test_fmsub_d_182();
	test_fmsub_d_183();
	test_fmsub_d_184();
	test_fmsub_d_185();
	test_fmsub_d_186();
	test_fmsub_d_187();
	test_fmsub_d_188();
	test_fmsub_d_189();
	test_fmsub_d_190();
	test_fmsub_d_191();
	test_fmsub_d_192();
	test_fmsub_d_193();
	test_fmsub_d_194();
	test_fmsub_d_195();
	test_fmsub_d_196();
	test_fmsub_d_197();
	test_fmsub_d_198();
	test_fmsub_d_199();
	test_fmsub_d_200();
	test_fmsub_d_201();
	test_fmsub_d_202();
	test_fmsub_d_203();
	test_fmsub_d_204();
	test_fmsub_d_205();
	test_fmsub_d_206();
	test_fmsub_d_207();
	test_fmsub_d_208();
	test_fmsub_d_209();
	test_fmsub_d_210();
	test_fmsub_d_211();
	test_fmsub_d_212();
	test_fmsub_d_213();
	test_fmsub_d_214();
	test_fmsub_d_215();
	test_fmsub_d_216();
	test_fnmsub_d_1();
	test_fnmsub_d_2();
	test_fnmsub_d_3();
	test_fnmsub_d_4();
	test_fnmsub_d_5();
	test_fnmsub_d_6();
	test_fnmsub_d_7();
	test_fnmsub_d_8();
	test_fnmsub_d_9();
	test_fnmsub_d_10();
	test_fnmsub_d_11();
	test_fnmsub_d_12();
	test_fnmsub_d_13();
	test_fnmsub_d_14();
	test_fnmsub_d_15();
	test_fnmsub_d_16();
	test_fnmsub_d_17();
	test_fnmsub_d_18();
	test_fnmsub_d_19();
	test_fnmsub_d_20();
	test_fnmsub_d_21();
	test_fnmsub_d_22();
	test_fnmsub_d_23();
	test_fnmsub_d_24();
	test_fnmsub_d_25();
	test_fnmsub_d_26();
	test_fnmsub_d_27();
	test_fnmsub_d_28();
	test_fnmsub_d_29();
	test_fnmsub_d_30();
	test_fnmsub_d_31();
	test_fnmsub_d_32();
	test_fnmsub_d_33();
	test_fnmsub_d_34();
	test_fnmsub_d_35();
	test_fnmsub_d_36();
	test_fnmsub_d_37();
	test_fnmsub_d_38();
	test_fnmsub_d_39();
	test_fnmsub_d_40();
	test_fnmsub_d_41();
	test_fnmsub_d_42();
	test_fnmsub_d_43();
	test_fnmsub_d_44();
	test_fnmsub_d_45();
	test_fnmsub_d_46();
	test_fnmsub_d_47();
	test_fnmsub_d_48();
	test_fnmsub_d_49();
	test_fnmsub_d_50();
	test_fnmsub_d_51();
	test_fnmsub_d_52();
	test_fnmsub_d_53();
	test_fnmsub_d_54();
	test_fnmsub_d_55();
	test_fnmsub_d_56();
	test_fnmsub_d_57();
	test_fnmsub_d_58();
	test_fnmsub_d_59();
	test_fnmsub_d_60();
	test_fnmsub_d_61();
	test_fnmsub_d_62();
	test_fnmsub_d_63();
	test_fnmsub_d_64();
	test_fnmsub_d_65();
	test_fnmsub_d_66();
	test_fnmsub_d_67();
	test_fnmsub_d_68();
	test_fnmsub_d_69();
	test_fnmsub_d_70();
	test_fnmsub_d_71();
	test_fnmsub_d_72();
	test_fnmsub_d_73();
	test_fnmsub_d_74();
	test_fnmsub_d_75();
	test_fnmsub_d_76();
	test_fnmsub_d_77();
	test_fnmsub_d_78();
	test_fnmsub_d_79();
	test_fnmsub_d_80();
	test_fnmsub_d_81();
	test_fnmsub_d_82();
	test_fnmsub_d_83();
	test_fnmsub_d_84();
	test_fnmsub_d_85();
	test_fnmsub_d_86();
	test_fnmsub_d_87();
	test_fnmsub_d_88();
	test_fnmsub_d_89();
	test_fnmsub_d_90();
	test_fnmsub_d_91();
	test_fnmsub_d_92();
	test_fnmsub_d_93();
	test_fnmsub_d_94();
	test_fnmsub_d_95();
	test_fnmsub_d_96();
	test_fnmsub_d_97();
	test_fnmsub_d_98();
	test_fnmsub_d_99();
	test_fnmsub_d_100();
	test_fnmsub_d_101();
	test_fnmsub_d_102();
	test_fnmsub_d_103();
	test_fnmsub_d_104();
	test_fnmsub_d_105();
	test_fnmsub_d_106();
	test_fnmsub_d_107();
	test_fnmsub_d_108();
	test_fnmsub_d_109();
	test_fnmsub_d_110();
	test_fnmsub_d_111();
	test_fnmsub_d_112();
	test_fnmsub_d_113();
	test_fnmsub_d_114();
	test_fnmsub_d_115();
	test_fnmsub_d_116();
	test_fnmsub_d_117();
	test_fnmsub_d_118();
	test_fnmsub_d_119();
	test_fnmsub_d_120();
	test_fnmsub_d_121();
	test_fnmsub_d_122();
	test_fnmsub_d_123();
	test_fnmsub_d_124();
	test_fnmsub_d_125();
	test_fnmsub_d_126();
	test_fnmsub_d_127();
	test_fnmsub_d_128();
	test_fnmsub_d_129();
	test_fnmsub_d_130();
	test_fnmsub_d_131();
	test_fnmsub_d_132();
	test_fnmsub_d_133();
	test_fnmsub_d_134();
	test_fnmsub_d_135();
	test_fnmsub_d_136();
	test_fnmsub_d_137();
	test_fnmsub_d_138();
	test_fnmsub_d_139();
	test_fnmsub_d_140();
	test_fnmsub_d_141();
	test_fnmsub_d_142();
	test_fnmsub_d_143();
	test_fnmsub_d_144();
	test_fnmsub_d_145();
	test_fnmsub_d_146();
	test_fnmsub_d_147();
	test_fnmsub_d_148();
	test_fnmsub_d_149();
	test_fnmsub_d_150();
	test_fnmsub_d_151();
	test_fnmsub_d_152();
	test_fnmsub_d_153();
	test_fnmsub_d_154();
	test_fnmsub_d_155();
	test_fnmsub_d_156();
	test_fnmsub_d_157();
	test_fnmsub_d_158();
	test_fnmsub_d_159();
	test_fnmsub_d_160();
	test_fnmsub_d_161();
	test_fnmsub_d_162();
	test_fnmsub_d_163();
	test_fnmsub_d_164();
	test_fnmsub_d_165();
	test_fnmsub_d_166();
	test_fnmsub_d_167();
	test_fnmsub_d_168();
	test_fnmsub_d_169();
	test_fnmsub_d_170();
	test_fnmsub_d_171();
	test_fnmsub_d_172();
	test_fnmsub_d_173();
	test_fnmsub_d_174();
	test_fnmsub_d_175();
	test_fnmsub_d_176();
	test_fnmsub_d_177();
	test_fnmsub_d_178();
	test_fnmsub_d_179();
	test_fnmsub_d_180();
	test_fnmsub_d_181();
	test_fnmsub_d_182();
	test_fnmsub_d_183();
	test_fnmsub_d_184();
	test_fnmsub_d_185();
	test_fnmsub_d_186();
	test_fnmsub_d_187();
	test_fnmsub_d_188();
	test_fnmsub_d_189();
	test_fnmsub_d_190();
	test_fnmsub_d_191();
	test_fnmsub_d_192();
	test_fnmsub_d_193();
	test_fnmsub_d_194();
	test_fnmsub_d_195();
	test_fnmsub_d_196();
	test_fnmsub_d_197();
	test_fnmsub_d_198();
	test_fnmsub_d_199();
	test_fnmsub_d_200();
	test_fnmsub_d_201();
	test_fnmsub_d_202();
	test_fnmsub_d_203();
	test_fnmsub_d_204();
	test_fnmsub_d_205();
	test_fnmsub_d_206();
	test_fnmsub_d_207();
	test_fnmsub_d_208();
	test_fnmsub_d_209();
	test_fnmsub_d_210();
	test_fnmsub_d_211();
	test_fnmsub_d_212();
	test_fnmsub_d_213();
	test_fnmsub_d_214();
	test_fnmsub_d_215();
	test_fnmsub_d_216();
	test_fnmadd_d_1();
	test_fnmadd_d_2();
	test_fnmadd_d_3();
	test_fnmadd_d_4();
	test_fnmadd_d_5();
	test_fnmadd_d_6();
	test_fnmadd_d_7();
	test_fnmadd_d_8();
	test_fnmadd_d_9();
	test_fnmadd_d_10();
	test_fnmadd_d_11();
	test_fnmadd_d_12();
	test_fnmadd_d_13();
	test_fnmadd_d_14();
	test_fnmadd_d_15();
	test_fnmadd_d_16();
	test_fnmadd_d_17();
	test_fnmadd_d_18();
	test_fnmadd_d_19();
	test_fnmadd_d_20();
	test_fnmadd_d_21();
	test_fnmadd_d_22();
	test_fnmadd_d_23();
	test_fnmadd_d_24();
	test_fnmadd_d_25();
	test_fnmadd_d_26();
	test_fnmadd_d_27();
	test_fnmadd_d_28();
	test_fnmadd_d_29();
	test_fnmadd_d_30();
	test_fnmadd_d_31();
	test_fnmadd_d_32();
	test_fnmadd_d_33();
	test_fnmadd_d_34();
	test_fnmadd_d_35();
	test_fnmadd_d_36();
	test_fnmadd_d_37();
	test_fnmadd_d_38();
	test_fnmadd_d_39();
	test_fnmadd_d_40();
	test_fnmadd_d_41();
	test_fnmadd_d_42();
	test_fnmadd_d_43();
	test_fnmadd_d_44();
	test_fnmadd_d_45();
	test_fnmadd_d_46();
	test_fnmadd_d_47();
	test_fnmadd_d_48();
	test_fnmadd_d_49();
	test_fnmadd_d_50();
	test_fnmadd_d_51();
	test_fnmadd_d_52();
	test_fnmadd_d_53();
	test_fnmadd_d_54();
	test_fnmadd_d_55();
	test_fnmadd_d_56();
	test_fnmadd_d_57();
	test_fnmadd_d_58();
	test_fnmadd_d_59();
	test_fnmadd_d_60();
	test_fnmadd_d_61();
	test_fnmadd_d_62();
	test_fnmadd_d_63();
	test_fnmadd_d_64();
	test_fnmadd_d_65();
	test_fnmadd_d_66();
	test_fnmadd_d_67();
	test_fnmadd_d_68();
	test_fnmadd_d_69();
	test_fnmadd_d_70();
	test_fnmadd_d_71();
	test_fnmadd_d_72();
	test_fnmadd_d_73();
	test_fnmadd_d_74();
	test_fnmadd_d_75();
	test_fnmadd_d_76();
	test_fnmadd_d_77();
	test_fnmadd_d_78();
	test_fnmadd_d_79();
	test_fnmadd_d_80();
	test_fnmadd_d_81();
	test_fnmadd_d_82();
	test_fnmadd_d_83();
	test_fnmadd_d_84();
	test_fnmadd_d_85();
	test_fnmadd_d_86();
	test_fnmadd_d_87();
	test_fnmadd_d_88();
	test_fnmadd_d_89();
	test_fnmadd_d_90();
	test_fnmadd_d_91();
	test_fnmadd_d_92();
	test_fnmadd_d_93();
	test_fnmadd_d_94();
	test_fnmadd_d_95();
	test_fnmadd_d_96();
	test_fnmadd_d_97();
	test_fnmadd_d_98();
	test_fnmadd_d_99();
	test_fnmadd_d_100();
	test_fnmadd_d_101();
	test_fnmadd_d_102();
	test_fnmadd_d_103();
	test_fnmadd_d_104();
	test_fnmadd_d_105();
	test_fnmadd_d_106();
	test_fnmadd_d_107();
	test_fnmadd_d_108();
	test_fnmadd_d_109();
	test_fnmadd_d_110();
	test_fnmadd_d_111();
	test_fnmadd_d_112();
	test_fnmadd_d_113();
	test_fnmadd_d_114();
	test_fnmadd_d_115();
	test_fnmadd_d_116();
	test_fnmadd_d_117();
	test_fnmadd_d_118();
	test_fnmadd_d_119();
	test_fnmadd_d_120();
	test_fnmadd_d_121();
	test_fnmadd_d_122();
	test_fnmadd_d_123();
	test_fnmadd_d_124();
	test_fnmadd_d_125();
	test_fnmadd_d_126();
	test_fnmadd_d_127();
	test_fnmadd_d_128();
	test_fnmadd_d_129();
	test_fnmadd_d_130();
	test_fnmadd_d_131();
	test_fnmadd_d_132();
	test_fnmadd_d_133();
	test_fnmadd_d_134();
	test_fnmadd_d_135();
	test_fnmadd_d_136();
	test_fnmadd_d_137();
	test_fnmadd_d_138();
	test_fnmadd_d_139();
	test_fnmadd_d_140();
	test_fnmadd_d_141();
	test_fnmadd_d_142();
	test_fnmadd_d_143();
	test_fnmadd_d_144();
	test_fnmadd_d_145();
	test_fnmadd_d_146();
	test_fnmadd_d_147();
	test_fnmadd_d_148();
	test_fnmadd_d_149();
	test_fnmadd_d_150();
	test_fnmadd_d_151();
	test_fnmadd_d_152();
	test_fnmadd_d_153();
	test_fnmadd_d_154();
	test_fnmadd_d_155();
	test_fnmadd_d_156();
	test_fnmadd_d_157();
	test_fnmadd_d_158();
	test_fnmadd_d_159();
	test_fnmadd_d_160();
	test_fnmadd_d_161();
	test_fnmadd_d_162();
	test_fnmadd_d_163();
	test_fnmadd_d_164();
	test_fnmadd_d_165();
	test_fnmadd_d_166();
	test_fnmadd_d_167();
	test_fnmadd_d_168();
	test_fnmadd_d_169();
	test_fnmadd_d_170();
	test_fnmadd_d_171();
	test_fnmadd_d_172();
	test_fnmadd_d_173();
	test_fnmadd_d_174();
	test_fnmadd_d_175();
	test_fnmadd_d_176();
	test_fnmadd_d_177();
	test_fnmadd_d_178();
	test_fnmadd_d_179();
	test_fnmadd_d_180();
	test_fnmadd_d_181();
	test_fnmadd_d_182();
	test_fnmadd_d_183();
	test_fnmadd_d_184();
	test_fnmadd_d_185();
	test_fnmadd_d_186();
	test_fnmadd_d_187();
	test_fnmadd_d_188();
	test_fnmadd_d_189();
	test_fnmadd_d_190();
	test_fnmadd_d_191();
	test_fnmadd_d_192();
	test_fnmadd_d_193();
	test_fnmadd_d_194();
	test_fnmadd_d_195();
	test_fnmadd_d_196();
	test_fnmadd_d_197();
	test_fnmadd_d_198();
	test_fnmadd_d_199();
	test_fnmadd_d_200();
	test_fnmadd_d_201();
	test_fnmadd_d_202();
	test_fnmadd_d_203();
	test_fnmadd_d_204();
	test_fnmadd_d_205();
	test_fnmadd_d_206();
	test_fnmadd_d_207();
	test_fnmadd_d_208();
	test_fnmadd_d_209();
	test_fnmadd_d_210();
	test_fnmadd_d_211();
	test_fnmadd_d_212();
	test_fnmadd_d_213();
	test_fnmadd_d_214();
	test_fnmadd_d_215();
	test_fnmadd_d_216();
	test_fadd_d_1();
	test_fadd_d_2();
	test_fadd_d_3();
	test_fadd_d_4();
	test_fadd_d_5();
	test_fadd_d_6();
	test_fadd_d_7();
	test_fadd_d_8();
	test_fadd_d_9();
	test_fadd_d_10();
	test_fadd_d_11();
	test_fadd_d_12();
	test_fadd_d_13();
	test_fadd_d_14();
	test_fadd_d_15();
	test_fadd_d_16();
	test_fadd_d_17();
	test_fadd_d_18();
	test_fadd_d_19();
	test_fadd_d_20();
	test_fadd_d_21();
	test_fadd_d_22();
	test_fadd_d_23();
	test_fadd_d_24();
	test_fadd_d_25();
	test_fadd_d_26();
	test_fadd_d_27();
	test_fadd_d_28();
	test_fadd_d_29();
	test_fadd_d_30();
	test_fadd_d_31();
	test_fadd_d_32();
	test_fadd_d_33();
	test_fadd_d_34();
	test_fadd_d_35();
	test_fadd_d_36();
	test_fsub_d_1();
	test_fsub_d_2();
	test_fsub_d_3();
	test_fsub_d_4();
	test_fsub_d_5();
	test_fsub_d_6();
	test_fsub_d_7();
	test_fsub_d_8();
	test_fsub_d_9();
	test_fsub_d_10();
	test_fsub_d_11();
	test_fsub_d_12();
	test_fsub_d_13();
	test_fsub_d_14();
	test_fsub_d_15();
	test_fsub_d_16();
	test_fsub_d_17();
	test_fsub_d_18();
	test_fsub_d_19();
	test_fsub_d_20();
	test_fsub_d_21();
	test_fsub_d_22();
	test_fsub_d_23();
	test_fsub_d_24();
	test_fsub_d_25();
	test_fsub_d_26();
	test_fsub_d_27();
	test_fsub_d_28();
	test_fsub_d_29();
	test_fsub_d_30();
	test_fsub_d_31();
	test_fsub_d_32();
	test_fsub_d_33();
	test_fsub_d_34();
	test_fsub_d_35();
	test_fsub_d_36();
	test_fmul_d_1();
	test_fmul_d_2();
	test_fmul_d_3();
	test_fmul_d_4();
	test_fmul_d_5();
	test_fmul_d_6();
	test_fmul_d_7();
	test_fmul_d_8();
	test_fmul_d_9();
	test_fmul_d_10();
	test_fmul_d_11();
	test_fmul_d_12();
	test_fmul_d_13();
	test_fmul_d_14();
	test_fmul_d_15();
	test_fmul_d_16();
	test_fmul_d_17();
	test_fmul_d_18();
	test_fmul_d_19();
	test_fmul_d_20();
	test_fmul_d_21();
	test_fmul_d_22();
	test_fmul_d_23();
	test_fmul_d_24();
	test_fmul_d_25();
	test_fmul_d_26();
	test_fmul_d_27();
	test_fmul_d_28();
	test_fmul_d_29();
	test_fmul_d_30();
	test_fmul_d_31();
	test_fmul_d_32();
	test_fmul_d_33();
	test_fmul_d_34();
	test_fmul_d_35();
	test_fmul_d_36();
	test_fdiv_d_1();
	test_fdiv_d_2();
	test_fdiv_d_3();
	test_fdiv_d_4();
	test_fdiv_d_5();
	test_fdiv_d_6();
	test_fdiv_d_7();
	test_fdiv_d_8();
	test_fdiv_d_9();
	test_fdiv_d_10();
	test_fdiv_d_11();
	test_fdiv_d_12();
	test_fdiv_d_13();
	test_fdiv_d_14();
	test_fdiv_d_15();
	test_fdiv_d_16();
	test_fdiv_d_17();
	test_fdiv_d_18();
	test_fdiv_d_19();
	test_fdiv_d_20();
	test_fdiv_d_21();
	test_fdiv_d_22();
	test_fdiv_d_23();
	test_fdiv_d_24();
	test_fdiv_d_25();
	test_fdiv_d_26();
	test_fdiv_d_27();
	test_fdiv_d_28();
	test_fdiv_d_29();
	test_fdiv_d_30();
	test_fdiv_d_31();
	test_fdiv_d_32();
	test_fdiv_d_33();
	test_fdiv_d_34();
	test_fdiv_d_35();
	test_fdiv_d_36();
	test_fmin_d_1();
	test_fmin_d_2();
	test_fmin_d_3();
	test_fmin_d_4();
	test_fmin_d_5();
	test_fmin_d_6();
	test_fmin_d_7();
	test_fmin_d_8();
	test_fmin_d_9();
	test_fmin_d_10();
	test_fmin_d_11();
	test_fmin_d_12();
	test_fmin_d_13();
	test_fmin_d_14();
	test_fmin_d_15();
	test_fmin_d_16();
	test_fmin_d_17();
	test_fmin_d_18();
	test_fmin_d_19();
	test_fmin_d_20();
	test_fmin_d_21();
	test_fmin_d_22();
	test_fmin_d_23();
	test_fmin_d_24();
	test_fmin_d_25();
	test_fmin_d_26();
	test_fmin_d_27();
	test_fmin_d_28();
	test_fmin_d_29();
	test_fmin_d_30();
	test_fmin_d_31();
	test_fmin_d_32();
	test_fmin_d_33();
	test_fmin_d_34();
	test_fmin_d_35();
	test_fmin_d_36();
	test_fmax_d_1();
	test_fmax_d_2();
	test_fmax_d_3();
	test_fmax_d_4();
	test_fmax_d_5();
	test_fmax_d_6();
	test_fmax_d_7();
	test_fmax_d_8();
	test_fmax_d_9();
	test_fmax_d_10();
	test_fmax_d_11();
	test_fmax_d_12();
	test_fmax_d_13();
	test_fmax_d_14();
	test_fmax_d_15();
	test_fmax_d_16();
	test_fmax_d_17();
	test_fmax_d_18();
	test_fmax_d_19();
	test_fmax_d_20();
	test_fmax_d_21();
	test_fmax_d_22();
	test_fmax_d_23();
	test_fmax_d_24();
	test_fmax_d_25();
	test_fmax_d_26();
	test_fmax_d_27();
	test_fmax_d_28();
	test_fmax_d_29();
	test_fmax_d_30();
	test_fmax_d_31();
	test_fmax_d_32();
	test_fmax_d_33();
	test_fmax_d_34();
	test_fmax_d_35();
	test_fmax_d_36();
	test_fcvt_s_d_1();
	test_fcvt_s_d_2();
	test_fcvt_s_d_3();
	test_fcvt_s_d_4();
	test_fcvt_s_d_5();
	test_fcvt_s_d_6();
	test_fcvt_d_s_1();
	test_fcvt_d_s_2();
	test_fcvt_d_s_3();
	test_fcvt_d_s_4();
	test_fcvt_d_s_5();
	test_fcvt_d_s_6();
	test_fsqrt_d_1();
	test_fsqrt_d_2();
	test_fsqrt_d_3();
	test_fsqrt_d_4();
	test_fsqrt_d_5();
	test_fsqrt_d_6();
	test_fle_d_1();
	test_fle_d_2();
	test_fle_d_3();
	test_fle_d_4();
	test_fle_d_5();
	test_fle_d_6();
	test_fle_d_7();
	test_fle_d_8();
	test_fle_d_9();
	test_fle_d_10();
	test_fle_d_11();
	test_fle_d_12();
	test_fle_d_13();
	test_fle_d_14();
	test_fle_d_15();
	test_fle_d_16();
	test_fle_d_17();
	test_fle_d_18();
	test_fle_d_19();
	test_fle_d_20();
	test_fle_d_21();
	test_fle_d_22();
	test_fle_d_23();
	test_fle_d_24();
	test_fle_d_25();
	test_fle_d_26();
	test_fle_d_27();
	test_fle_d_28();
	test_fle_d_29();
	test_fle_d_30();
	test_fle_d_31();
	test_fle_d_32();
	test_fle_d_33();
	test_fle_d_34();
	test_fle_d_35();
	test_fle_d_36();
	test_flt_d_1();
	test_flt_d_2();
	test_flt_d_3();
	test_flt_d_4();
	test_flt_d_5();
	test_flt_d_6();
	test_flt_d_7();
	test_flt_d_8();
	test_flt_d_9();
	test_flt_d_10();
	test_flt_d_11();
	test_flt_d_12();
	test_flt_d_13();
	test_flt_d_14();
	test_flt_d_15();
	test_flt_d_16();
	test_flt_d_17();
	test_flt_d_18();
	test_flt_d_19();
	test_flt_d_20();
	test_flt_d_21();
	test_flt_d_22();
	test_flt_d_23();
	test_flt_d_24();
	test_flt_d_25();
	test_flt_d_26();
	test_flt_d_27();
	test_flt_d_28();
	test_flt_d_29();
	test_flt_d_30();
	test_flt_d_31();
	test_flt_d_32();
	test_flt_d_33();
	test_flt_d_34();
	test_flt_d_35();
	test_flt_d_36();
	test_feq_d_1();
	test_feq_d_2();
	test_feq_d_3();
	test_feq_d_4();
	test_feq_d_5();
	test_feq_d_6();
	test_feq_d_7();
	test_feq_d_8();
	test_feq_d_9();
	test_feq_d_10();
	test_feq_d_11();
	test_feq_d_12();
	test_feq_d_13();
	test_feq_d_14();
	test_feq_d_15();
	test_feq_d_16();
	test_feq_d_17();
	test_feq_d_18();
	test_feq_d_19();
	test_feq_d_20();
	test_feq_d_21();
	test_feq_d_22();
	test_feq_d_23();
	test_feq_d_24();
	test_feq_d_25();
	test_feq_d_26();
	test_feq_d_27();
	test_feq_d_28();
	test_feq_d_29();
	test_feq_d_30();
	test_feq_d_31();
	test_feq_d_32();
	test_feq_d_33();
	test_feq_d_34();
	test_feq_d_35();
	test_feq_d_36();
	test_fcvt_w_d_1();
	test_fcvt_w_d_2();
	test_fcvt_w_d_3();
	test_fcvt_w_d_4();
	test_fcvt_w_d_5();
	test_fcvt_w_d_6();
	test_fcvt_wu_d_1();
	test_fcvt_wu_d_2();
	test_fcvt_wu_d_3();
	test_fcvt_wu_d_4();
	test_fcvt_wu_d_5();
	test_fcvt_wu_d_6();
	test_fcvt_d_w_1();
	test_fcvt_d_w_2();
	test_fcvt_d_wu_1();
	test_fclass_d_1();
	test_fclass_d_2();
	test_fclass_d_3();
	test_fclass_d_4();
	test_fclass_d_5();
	test_fclass_d_6();
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_d_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_d_2();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_d_3();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_d_4();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_d_5();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_l_d_6();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_d_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_d_2();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_d_3();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_d_4();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_d_5();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_lu_d_6();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_x_d_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_x_d_2();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_x_d_3();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_x_d_4();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_x_d_5();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_x_d_6();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_d_l_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_d_l_2();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fcvt_d_lu_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_d_x_1();
#endif
#if _RISCV_SZPTR != _RISCV_SZINT
	test_fmv_d_x_2();
#endif
}
