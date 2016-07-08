//
//  riscv-codec-encode.h
//

#ifndef riscv_codec_encode_h
#define riscv_codec_encode_h

/* Instruction encoders */

/* Encode none */
template <typename T> inline uint64_t encode_none(T &dec) { return 0; }

/* Encode C nop */
template <typename T> inline uint64_t encode_ci_none(T &dec)
{
	assert(dec.rd == riscv_ireg_zero);
	assert(dec.rs1 == riscv_ireg_zero);
	assert(dec.rs2 == riscv_ireg_zero);
	return 0;
}

/* Encode CR */
template <typename T> inline uint64_t encode_cr(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crs1rd::encode(dec.rd) | arg_crs2::encode(dec.rs2);
}

/* Encode CR mv */
template <typename T> inline uint64_t encode_cr_mv(T &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return arg_crd::encode(dec.rd) | arg_crs2::encode(dec.rs2);
}

/* Encode CR jalr */
template <typename T> inline uint64_t encode_cr_jalr(T &dec)
{
	assert(dec.rd == riscv_ireg_ra);
	return arg_crs1::encode(dec.rs1);
}

/* Encode CR jr */
template <typename T> inline uint64_t encode_cr_jr(T &dec)
{
	assert(dec.rd == riscv_ireg_zero);
	return arg_crs1::encode(dec.rs1);
}

/* Encode CI */
template <typename T> inline uint64_t encode_ci(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crs1rd::encode(dec.rs1) | arg_cimmi::encode(dec.imm);
}

/* Encode CI shamt5 */
template <typename T> inline uint64_t encode_ci_sh5(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crs1rd::encode(dec.rs1) | arg_cimmsh5::encode(dec.imm);
}

/* Encode CI shamt6 */
template <typename T> inline uint64_t encode_ci_sh6(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crs1rd::encode(dec.rs1) | arg_cimmsh6::encode(dec.imm);
}

/* Encode CI li */
template <typename T> inline uint64_t encode_ci_li(T &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return arg_crd::encode(dec.rd) | arg_cimmi::encode(dec.imm);
}

/* Encode CI lui */
template <typename T> inline uint64_t encode_ci_lui(T &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return arg_crd::encode(dec.rd) | arg_cimmui::encode(dec.imm);
}

/* Encode CI lwsp */
template <typename T> inline uint64_t encode_ci_lwsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return arg_crd::encode(dec.rd) | arg_cimmlwsp::encode(dec.imm);
}

/* Encode CI ldsp */
template <typename T> inline uint64_t encode_ci_ldsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return arg_crd::encode(dec.rd) | arg_cimmldsp::encode(dec.imm);
}

/* Encode CI 16sp */
template <typename T> inline uint64_t encode_ci_16sp(T &dec)
{
	assert(dec.rd == riscv_ireg_sp);
	assert(dec.rs1 == riscv_ireg_sp);
	return arg_cimm16sp::encode(dec.imm);
}

/* Encode CSS swsp */
template <typename T> inline uint64_t encode_css_swsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return arg_crs2::encode(dec.rs2) | arg_cimmswsp::encode(dec.imm);
}

/* Encode CSS sdsp */
template <typename T> inline uint64_t encode_css_sdsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return arg_crs2::encode(dec.rs2) | arg_cimmsdsp::encode(dec.imm);
}

/* Encode CIW 4spn */
template <typename T> inline uint64_t encode_ciw_4spn(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return arg_crdq::encode(dec.rd + 8) | arg_cimm4spn::encode(dec.imm);
}

/* Encode CL lw */
template <typename T> inline uint64_t encode_cl_lw(T &dec)
{
	return arg_crdq::encode(dec.rd + 8) | arg_crs1q::encode(dec.rs1 + 8)  | arg_cimmw::encode(dec.imm);
}

/* Encode CL ld */
template <typename T> inline uint64_t encode_cl_ld(T &dec)
{
	return arg_crdq::encode(dec.rd + 8) | arg_crs1q::encode(dec.rs1 + 8)  | arg_cimmd::encode(dec.imm);
}

/* Encode CS f */
template <typename T> inline uint64_t encode_cs(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crdq::encode(dec.rd + 8) | arg_crs1q::encode(dec.rs1 + 8);
}

/* Encode CS sd */
template <typename T> inline uint64_t encode_cs_sd(T &dec)
{
	return arg_crs1q::encode(dec.rs1 + 8) | arg_crs2q::encode(dec.rs2 + 8) | arg_cimmd::encode(dec.imm);
}

/* Encode CS sw */
template <typename T> inline uint64_t encode_cs_sw(T &dec)
{
	return arg_crs1q::encode(dec.rs1 + 8) | arg_crs2q::encode(dec.rs2 + 8) | arg_cimmw::encode(dec.imm);
}

/* Encode CB */
template <typename T> inline uint64_t encode_cb(T &dec)
{
	return arg_crs1q::encode(dec.rs1 + 8) | arg_cimmb::encode(dec.imm);
}

/* Encode CB imm */
template <typename T> inline uint64_t encode_cb_imm(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crs1rdq::encode(dec.rs1 + 8) | arg_cimmi::encode(dec.imm);
}

/* Encode CB shamt5 */
template <typename T> inline uint64_t encode_cb_sh5(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crs1rdq::encode(dec.rs1 + 8) | arg_cimmsh5::encode(dec.imm);
}

/* Encode CB shamt6 */
template <typename T> inline uint64_t encode_cb_sh6(T &dec)
{
	assert(dec.rd == dec.rs1);
	return arg_crs1rdq::encode(dec.rs1 + 8) | arg_cimmsh6::encode(dec.imm);
}

/* Encode CJ - imm */
template <typename T> inline uint64_t encode_cj(T &dec)
{
	return arg_cimmj::encode(dec.imm);
}

/* Encode R */
template <typename T> inline uint64_t encode_r(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_rs2::encode(dec.rs2);
}

/* Encode R RM */
template <typename T> inline uint64_t encode_r_m(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_rs2::encode(dec.rs2) | arg_rm::encode(dec.rm);
}

/* Encode R AMO Load */
template <typename T> inline uint64_t encode_r_l(T &dec)
{
	assert(dec.rs2 == 0);
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_aq::encode(dec.aq) | arg_rl::encode(dec.rl);
}

/* Encode R AMO */
template <typename T> inline uint64_t encode_r_a(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_rs2::encode(dec.rs2) | arg_aq::encode(dec.aq) | arg_rl::encode(dec.rl);
}

/* Encode R 4f */
template <typename T> inline uint64_t encode_r4_m(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_rs2::encode(dec.rs2) | arg_rs3::encode(dec.rs3) | arg_rm::encode(dec.rm);
}

/* Encode I */
template <typename T> inline uint64_t encode_i(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_imm12::encode(dec.imm);
}

/* Encode I sh5 */
template <typename T> inline uint64_t encode_i_sh5(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_shamt5::encode(dec.imm);
}

/* Encode I sh6 */
template <typename T> inline uint64_t encode_i_sh6(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_rs1::encode(dec.rs1) | arg_shamt6::encode(dec.imm);
}

/* Encode S Store */
template <typename T> inline uint64_t encode_s(T &dec)
{
	return arg_rs1::encode(dec.rs1) | arg_rs2::encode(dec.rs2) | arg_simm12::encode(dec.imm);
}

/* Encode SB Branch */
template <typename T> inline uint64_t encode_sb(T &dec)
{
	return arg_rs1::encode(dec.rs1) | arg_rs2::encode(dec.rs2) | arg_sbimm12::encode(dec.imm);
}

/* Encode U */
template <typename T> inline uint64_t encode_u(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_imm20::encode(dec.imm);
}

/* Encode UJ */
template <typename T> inline uint64_t encode_uj(T &dec)
{
	return arg_rd::encode(dec.rd) | arg_jimm20::encode(dec.imm);
}

#endif