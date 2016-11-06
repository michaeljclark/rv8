/* riscv-soft-float.h */

#ifndef riscv_soft_float_h
#define riscv_soft_float_h

#ifdef __cplusplus
extern "C" {
#endif

/* RV32F */

float __rvabi_fmadd_s(float rs1, float rs2, float rs3, int rm);
float __rvabi_fmsub_s(float rs1, float rs2, float rs3, int rm);
float __rvabi_fnmsub_s(float rs1, float rs2, float rs3, int rm);
float __rvabi_fnmadd_s(float rs1, float rs2, float rs3, int rm);
float __rvabi_fadd_s(float rs1, float rs2, int rm);
float __rvabi_fsub_s(float rs1, float rs2, int rm);
float __rvabi_fmul_s(float rs1, float rs2, int rm);
float __rvabi_fdiv_s(float rs1, float rs2, int rm);
float __rvabi_fsgnj_s(float rs1, float rs2);
float __rvabi_fsgnjn_s(float rs1, float rs2);
float __rvabi_fsgnjx_s(float rs1, float rs2);
float __rvabi_fmin_s(float rs1, float rs2);
float __rvabi_fmax_s(float rs1, float rs2);
float __rvabi_fsqrt_s(float rs1, int rm);
int __rvabi_fle_s(float rs1, float rs2);
int __rvabi_flt_s(float rs1, float rs2);
int __rvabi_feq_s(float rs1, float rs2);
int __rvabi_fcvt_w_s(float rs1, int rm);
unsigned int __rvabi_fcvt_wu_s(float rs1, int rm);
float __rvabi_fcvt_s_w(int rs1, int rm);
float __rvabi_fcvt_s_wu(unsigned int rs1, int rm);
int __rvabi_fclass_s(float rs1);

/* RV64F */

long long __rvabi_fcvt_l_s(float rs1, int rm);
unsigned long long __rvabi_fcvt_lu_s(float rs1, int rm);
float __rvabi_fcvt_s_l(long long rs1, int rm);
float __rvabi_fcvt_s_lu(unsigned long long rs1, int rm);

/* RV32D */

double __rvabi_fmadd_d(double rs1, double rs2, double rs3, int rm);
double __rvabi_fmsub_d(double rs1, double rs2, double rs3, int rm);
double __rvabi_fnmsub_d(double rs1, double rs2, double rs3, int rm);
double __rvabi_fnmadd_d(double rs1, double rs2, double rs3, int rm);
double __rvabi_fadd_d(double rs1, double rs2, int rm);
double __rvabi_fsub_d(double rs1, double rs2, int rm);
double __rvabi_fmul_d(double rs1, double rs2, int rm);
double __rvabi_fdiv_d(double rs1, double rs2, int rm);
double __rvabi_fsgnj_d(double rs1, double rs2);
double __rvabi_fsgnjn_d(double rs1, double rs2);
double __rvabi_fsgnjx_d(double rs1, double rs2);
double __rvabi_fmin_d(double rs1, double rs2);
double __rvabi_fmax_d(double rs1, double rs2);
double __rvabi_fcvt_s_d(double rs1, int rm);
double __rvabi_fcvt_d_s(double rs1, int rm);
double __rvabi_fsqrt_d(double rs1, int rm);
int __rvabi_fle_d(double rs1, double rs2);
int __rvabi_flt_d(double rs1, double rs2);
int __rvabi_feq_d(double rs1, double rs2);
int __rvabi_fcvt_w_d(double rs1, int rm);
unsigned int __rvabi_fcvt_wu_d(double rs1, int rm);
double __rvabi_fcvt_d_w(int rs1, int rm);
double __rvabi_fcvt_d_wu(unsigned int rs1, int rm);
int __rvabi_fclass_d(double rs1);

/* RV64D */

long long __rvabi_fcvt_l_d(double rs1, int rm);
unsigned long long __rvabi_fcvt_lu_d(ouble rs1, int rm);
double __rvabi_fcvt_d_l(long long rs1, int rm);
double __rvabi_fcvt_d_lu(unsigned long long rs1, int rm);

#ifdef __cplusplus
}
#endif

#endif
