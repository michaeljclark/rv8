//
//  riscv-encode.h
//

#ifndef riscv_encode_h
#define riscv_encode_h

/* Instruction encoders */

/* Encode none */
template <typename T> inline uint64_t encode_none(T &dec) { return 0; }

/* Encode C nop */
template <typename T> inline uint64_t encode_ci_none(T &dec)
{
	assert(dec.rv.r.rd == riscv_ireg_zero);
	assert(dec.rv.r.rs1 == riscv_ireg_zero);
	assert(dec.rv.r.rs2 == riscv_ireg_zero);
	return 0;
}

/* Encode CR */
template <typename T> inline uint64_t encode_cr(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crs1rd::encode(dec.rv.r.rd) | operand_crs2::encode(dec.rv.r.rs2);
}

/* Encode CR mv */
template <typename T> inline uint64_t encode_cr_mv(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_zero);
	return operand_crd::encode(dec.rv.r.rd) | operand_crs2::encode(dec.rv.r.rs2);
}

/* Encode CR jalr */
template <typename T> inline uint64_t encode_cr_jalr(T &dec)
{
	assert(dec.rv.r.rd == riscv_ireg_ra);
	return operand_crs1::encode(dec.rv.r.rs1);
}

/* Encode CR jr */
template <typename T> inline uint64_t encode_cr_jr(T &dec)
{
	assert(dec.rv.r.rd == riscv_ireg_zero);
	return operand_crs1::encode(dec.rv.r.rs1);
}

/* Encode CI */
template <typename T> inline uint64_t encode_ci(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crs1rd::encode(dec.rv.r.rs1) | operand_cimmi::encode(dec.imm);
}

/* Encode CI shamt5 */
template <typename T> inline uint64_t encode_ci_sh5(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crs1rd::encode(dec.rv.r.rs1) | operand_cimmsh5::encode(dec.imm);
}

/* Encode CI shamt6 */
template <typename T> inline uint64_t encode_ci_sh6(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crs1rd::encode(dec.rv.r.rs1) | operand_cimmsh6::encode(dec.imm);
}

/* Encode CI li */
template <typename T> inline uint64_t encode_ci_li(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_zero);
	return operand_crd::encode(dec.rv.r.rd) | operand_cimmi::encode(dec.imm);
}

/* Encode CI lui */
template <typename T> inline uint64_t encode_ci_lui(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_zero);
	return operand_crd::encode(dec.rv.r.rd) | operand_cimmui::encode(dec.imm);
}

/* Encode CI lwsp */
template <typename T> inline uint64_t encode_ci_lwsp(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_sp);
	return operand_crd::encode(dec.rv.r.rd) | operand_cimmlwsp::encode(dec.imm);
}

/* Encode CI ldsp */
template <typename T> inline uint64_t encode_ci_ldsp(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_sp);
	return operand_crd::encode(dec.rv.r.rd) | operand_cimmldsp::encode(dec.imm);
}

/* Encode CI 16sp */
template <typename T> inline uint64_t encode_ci_16sp(T &dec)
{
	assert(dec.rv.r.rd == riscv_ireg_sp);
	assert(dec.rv.r.rs1 == riscv_ireg_sp);
	return operand_cimm16sp::encode(dec.imm);
}

/* Encode CSS swsp */
template <typename T> inline uint64_t encode_css_swsp(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_sp);
	return operand_crs2::encode(dec.rv.r.rs2) | operand_cimmswsp::encode(dec.imm);
}

/* Encode CSS sdsp */
template <typename T> inline uint64_t encode_css_sdsp(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_sp);
	return operand_crs2::encode(dec.rv.r.rs2) | operand_cimmsdsp::encode(dec.imm);
}

/* Encode CIW 4spn */
template <typename T> inline uint64_t encode_ciw_4spn(T &dec)
{
	assert(dec.rv.r.rs1 == riscv_ireg_sp);
	return operand_crdq::encode(dec.rv.r.rd + 8) | operand_cimm4spn::encode(dec.imm);
}

/* Encode CL lw */
template <typename T> inline uint64_t encode_cl_lw(T &dec)
{
	return operand_crdq::encode(dec.rv.r.rd + 8) | operand_crs1q::encode(dec.rv.r.rs1 + 8)  | operand_cimmw::encode(dec.imm);
}

/* Encode CL ld */
template <typename T> inline uint64_t encode_cl_ld(T &dec)
{
	return operand_crdq::encode(dec.rv.r.rd + 8) | operand_crs1q::encode(dec.rv.r.rs1 + 8)  | operand_cimmd::encode(dec.imm);
}

/* Encode CS f */
template <typename T> inline uint64_t encode_cs(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crdq::encode(dec.rv.r.rd + 8) | operand_crs1q::encode(dec.rv.r.rs1 + 8);
}

/* Encode CS sd */
template <typename T> inline uint64_t encode_cs_sd(T &dec)
{
	return operand_crs1q::encode(dec.rv.r.rs1 + 8) | operand_crs2q::encode(dec.rv.r.rs2 + 8) | operand_cimmd::encode(dec.imm);
}

/* Encode CS sw */
template <typename T> inline uint64_t encode_cs_sw(T &dec)
{
	return operand_crs1q::encode(dec.rv.r.rs1 + 8) | operand_crs2q::encode(dec.rv.r.rs2 + 8) | operand_cimmw::encode(dec.imm);
}

/* Encode CB */
template <typename T> inline uint64_t encode_cb(T &dec)
{
	return operand_crs1q::encode(dec.rv.r.rs1 + 8) | operand_cimmb::encode(dec.imm);
}

/* Encode CB imm */
template <typename T> inline uint64_t encode_cb_imm(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crs1rdq::encode(dec.rv.r.rs1 + 8) | operand_cimmi::encode(dec.imm);
}

/* Encode CB shamt5 */
template <typename T> inline uint64_t encode_cb_sh5(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crs1rdq::encode(dec.rv.r.rs1 + 8) | operand_cimmsh5::encode(dec.imm);
}

/* Encode CB shamt6 */
template <typename T> inline uint64_t encode_cb_sh6(T &dec)
{
	assert(dec.rv.r.rd == dec.rv.r.rs1);
	return operand_crs1rdq::encode(dec.rv.r.rs1 + 8) | operand_cimmsh6::encode(dec.imm);
}

/* Encode CJ */
template <typename T> inline uint64_t encode_cj(T &dec)
{
	return operand_cimmj::encode(dec.imm);
}

/* Encode CJ jal */
template <typename T> inline uint64_t encode_cj_jal(T &dec)
{
	return operand_cimmj::encode(dec.imm);
}

/* Encode R */
template <typename T> inline uint64_t encode_r(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_rs2::encode(dec.rv.r.rs2);
}

/* Encode R RM */
template <typename T> inline uint64_t encode_r_m(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_rs2::encode(dec.rv.r.rs2) | operand_rm::encode(dec.rv.r.rm);
}

/* Encode R AMO Load */
template <typename T> inline uint64_t encode_r_l(T &dec)
{
	assert(dec.rv.r.rs2 == 0);
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_aq::encode(dec.rv.amo.aq) | operand_rl::encode(dec.rv.amo.rl);
}

/* Encode R AMO */
template <typename T> inline uint64_t encode_r_a(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_rs2::encode(dec.rv.r.rs2) | operand_aq::encode(dec.rv.amo.aq) | operand_rl::encode(dec.rv.amo.rl);
}

/* Encode R 4f */
template <typename T> inline uint64_t encode_r4_m(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_rs2::encode(dec.rv.r.rs2) | operand_rs3::encode(dec.rv.r.rs3) | operand_rm::encode(dec.rv.r.rm);
}

/* Encode R fence */
template <typename T> inline uint64_t encode_r_f(T &dec)
{
	return operand_pred::encode(dec.rv.fence.pred) | operand_succ::encode(dec.rv.fence.succ);
}

/* Encode I */
template <typename T> inline uint64_t encode_i(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_imm12::encode(dec.imm);
}

/* Encode I sh5 */
template <typename T> inline uint64_t encode_i_sh5(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_shamt5::encode(dec.imm);
}

/* Encode I sh6 */
template <typename T> inline uint64_t encode_i_sh6(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_rs1::encode(dec.rv.r.rs1) | operand_shamt6::encode(dec.imm);
}

/* Encode S Store */
template <typename T> inline uint64_t encode_s(T &dec)
{
	return operand_rs1::encode(dec.rv.r.rs1) | operand_rs2::encode(dec.rv.r.rs2) | operand_simm12::encode(dec.imm);
}

/* Encode SB Branch */
template <typename T> inline uint64_t encode_sb(T &dec)
{
	return operand_rs1::encode(dec.rv.r.rs1) | operand_rs2::encode(dec.rv.r.rs2) | operand_sbimm12::encode(dec.imm);
}

/* Encode U */
template <typename T> inline uint64_t encode_u(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_imm20::encode(dec.imm);
}

/* Encode UJ */
template <typename T> inline uint64_t encode_uj(T &dec)
{
	return operand_rd::encode(dec.rv.r.rd) | operand_jimm20::encode(dec.imm);
}

#endif
