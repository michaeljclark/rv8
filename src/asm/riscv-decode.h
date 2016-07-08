//
//  riscv-decode.h
//

#ifndef riscv_decode_h
#define riscv_decode_h

/* Instruction decoders */

/* Decode none */
template <typename T> inline void decode_none(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode C nop */
template <typename T> inline void decode_ci_none(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode CR */
template <typename T> inline void decode_cr(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rd::decode(inst);
	dec.rs2 = arg_crs2::decode(inst);
	dec.imm = 0;
}

/* Decode CR mv */
template <typename T> inline void decode_cr_mv(T &dec, uint64_t inst)
{
	dec.rd = arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_zero;
	dec.rs2 = arg_crs2::decode(inst);
	dec.imm = 0;
}

/* Decode CR jalr */
template <typename T> inline void decode_cr_jalr(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_ra;
	dec.rs1 = arg_crs1::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode CR jr */
template <typename T> inline void decode_cr_jr(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = arg_crs1::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode CI */
template <typename T> inline void decode_ci(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rd::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmi::decode(inst);
}

/* Decode CI shamt5 */
template <typename T> inline void decode_ci_sh5(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rd::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmsh5::decode(inst);
}

/* Decode CI shamt6 */
template <typename T> inline void decode_ci_sh6(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rd::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmsh6::decode(inst);
}

/* Decode CI li */
template <typename T> inline void decode_ci_li(T &dec, uint64_t inst)
{
	dec.rd = arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_zero;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmi::decode(inst);
}

/* Decode CI lui */
template <typename T> inline void decode_ci_lui(T &dec, uint64_t inst)
{
	dec.rd = arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_zero;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmui::decode(inst);
}

/* Decode CI lwsp */
template <typename T> inline void decode_ci_lwsp(T &dec, uint64_t inst)
{
	dec.rd = arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmlwsp::decode(inst);
}

/* Decode CI ldsp */
template <typename T> inline void decode_ci_ldsp(T &dec, uint64_t inst)
{
	dec.rd = arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmldsp::decode(inst);
}

/* Decode CI 16sp */
template <typename T> inline void decode_ci_16sp(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_sp;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimm16sp::decode(inst);
}

/* Decode CSS swsp */
template <typename T> inline void decode_css_swsp(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = arg_crs2::decode(inst);
	dec.imm = arg_cimmswsp::decode(inst);
}

/* Decode CSS sdsp */
template <typename T> inline void decode_css_sdsp(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = arg_crs2::decode(inst);
	dec.imm = arg_cimmsdsp::decode(inst);
}

/* Decode CIW 4spn */
template <typename T> inline void decode_ciw_4spn(T &dec, uint64_t inst)
{
	dec.rd = arg_crdq::decode(inst) + 8;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimm4spn::decode(inst);
}

/* Decode CL lw */
template <typename T> inline void decode_cl_lw(T &dec, uint64_t inst)
{
	dec.rd = arg_crdq::decode(inst) + 8;
	dec.rs1 = arg_crs1q::decode(inst) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmw::decode(inst);
}

/* Decode CL ld */
template <typename T> inline void decode_cl_ld(T &dec, uint64_t inst)
{
	dec.rd = arg_crdq::decode(inst) + 8;
	dec.rs1 = arg_crs1q::decode(inst) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmd::decode(inst);
}

/* Decode CS f */
template <typename T> inline void decode_cs(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rdq::decode(inst) + 8;
	dec.rs2 = arg_crs2q::decode(inst) + 8;
	dec.imm = 0;
}

/* Decode CS sd */
template <typename T> inline void decode_cs_sd(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = arg_crs1q::decode(inst) + 8;
	dec.rs2 = arg_crs2q::decode(inst) + 8;
	dec.imm = arg_cimmd::decode(inst);
}

/* Decode CS sw */
template <typename T> inline void decode_cs_sw(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = arg_crs1q::decode(inst) + 8;
	dec.rs2 = arg_crs2q::decode(inst) + 8;
	dec.imm = arg_cimmw::decode(inst);
}

/* Decode CB */
template <typename T> inline void decode_cb(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = arg_crs1q::decode(inst) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmb::decode(inst);
}

/* Decode CB imm */
template <typename T> inline void decode_cb_imm(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rdq::decode(inst) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmi::decode(inst);
}

/* Decode CB shamt5 */
template <typename T> inline void decode_cb_sh5(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rdq::decode(inst) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmsh5::decode(inst);
}

/* Decode CB shamt6 */
template <typename T> inline void decode_cb_sh6(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = arg_crs1rdq::decode(inst) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmsh6::decode(inst);
}

/* Decode CJ - imm */
template <typename T> inline void decode_cj(T &dec, uint64_t inst)
{
	dec.rd = dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_cimmj::decode(inst);
}

/* Decode R */
template <typename T> inline void decode_r(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = arg_rs2::decode(inst);
	dec.imm = 0;
}

/* Decode R RM */
template <typename T> inline void decode_r_m(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = arg_rs2::decode(inst);
	dec.imm = 0;
	dec.rm = arg_rm::decode(inst);
}

/* Decode R AMO L */
template <typename T> inline void decode_r_l(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
	dec.aq = arg_aq::decode(inst);
	dec.rl = arg_rl::decode(inst);
}

/* Decode R AMO S */
template <typename T> inline void decode_r_a(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = arg_rs2::decode(inst);
	dec.imm = 0;
	dec.aq = arg_aq::decode(inst);
	dec.rl = arg_rl::decode(inst);
}

/* Decode R 4f */
template <typename T> inline void decode_r4_m(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = arg_rs2::decode(inst);
	dec.rs3 = arg_rs3::decode(inst);
	dec.imm = 0;
	dec.rm = arg_rm::decode(inst);
}

/* Decode I */
template <typename T> inline void decode_i(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_imm12::decode(inst);
}

/* Decode I sh5 */
template <typename T> inline void decode_i_sh5(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_shamt5::decode(inst);
}

/* Decode I sh6 */
template <typename T> inline void decode_i_sh6(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_shamt6::decode(inst);
}

/* Decode S Store */
template <typename T> inline void decode_s(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = arg_rs2::decode(inst);
	dec.imm = arg_simm12::decode(inst);
}

/* Decode SB Branch */
template <typename T> inline void decode_sb(T &dec, uint64_t inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = arg_rs1::decode(inst);
	dec.rs2 = arg_rs2::decode(inst);
	dec.imm = arg_sbimm12::decode(inst);
}

/* Decode U */
template <typename T> inline void decode_u(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_imm20::decode(inst);
}

/* Decode UJ */
template <typename T> inline void decode_uj(T &dec, uint64_t inst)
{
	dec.rd = arg_rd::decode(inst);
	dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = arg_jimm20::decode(inst);
}

#endif