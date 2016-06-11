//
//  riscv-decode.h
//

#ifndef riscv_decode_h
#define riscv_decode_h

/* Decoded Instruction */

struct riscv_decode
{
	riscv_l   imm;
	riscv_hu  op;
	riscv_hu  codec;
	riscv_wu  rd   : 6;
	riscv_wu  rs1  : 6;
	riscv_wu  rs2  : 6;
	riscv_wu  rs3  : 6;
	riscv_wu  rm   : 3;
	riscv_wu  aq   : 1;
	riscv_wu  rl   : 1;
	riscv_wu  pad  : 4;

	riscv_decode()
		: imm(0), op(0), codec(0), rd(0), rs1(0), rs2(0), rs3(0), rm(0), aq(0), rl(0), pad(0) {}
};

/* Disassembled Instruction */

struct riscv_disasm : riscv_decode
{
	riscv_ptr pc;
	riscv_lu  insn;

	riscv_disasm() : riscv_decode(), pc(0), insn(0) {}
};

#include "riscv-decode-args.h"

/* Decode none */
template <typename T>
inline void riscv_decode_none(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode C nop */
template <typename T>
inline void riscv_decode_ci_none(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode CR */
template <typename T>
inline void riscv_decode_cr(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rd::decode(insn);
	dec.rs2 = riscv_arg_crs2::decode(insn);
	dec.imm = 0;
}

/* Decode CR mv */
template <typename T>
inline void riscv_decode_cr_mv(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crd::decode(insn);
	dec.rs1 = riscv_ireg_zero;
	dec.rs2 = riscv_arg_crs2::decode(insn);
	dec.imm = 0;
}

/* Decode CR jalr */
template <typename T>
inline void riscv_decode_cr_jalr(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_ra;
	dec.rs1 = riscv_arg_crs1::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode CR jr */
template <typename T>
inline void riscv_decode_cr_jr(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_arg_crs1::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
}

/* Decode CI */
template <typename T>
inline void riscv_decode_ci(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rd::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmi::decode(insn);
}

/* Decode CI shamt5 */
template <typename T>
inline void riscv_decode_ci_sh5(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rd::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmsh5::decode(insn);
}

/* Decode CI shamt6 */
template <typename T>
inline void riscv_decode_ci_sh6(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rd::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmsh6::decode(insn);
}

/* Decode CI li */
template <typename T>
inline void riscv_decode_ci_li(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crd::decode(insn);
	dec.rs1 = riscv_ireg_zero;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmi::decode(insn);
}

/* Decode CI lui */
template <typename T>
inline void riscv_decode_ci_lui(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crd::decode(insn);
	dec.rs1 = riscv_ireg_zero;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmui::decode(insn);
}

/* Decode CI lwsp */
template <typename T>
inline void riscv_decode_ci_lwsp(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crd::decode(insn);
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmlwsp::decode(insn);
}

/* Decode CI ldsp */
template <typename T>
inline void riscv_decode_ci_ldsp(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crd::decode(insn);
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmldsp::decode(insn);
}

/* Decode CI 16sp */
template <typename T>
inline void riscv_decode_ci_16sp(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_sp;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimm16sp::decode(insn);
}

/* Decode CSS swsp */
template <typename T>
inline void riscv_decode_css_swsp(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_arg_crs2::decode(insn);
	dec.imm = riscv_arg_cimmswsp::decode(insn);
}

/* Decode CSS sdsp */
template <typename T>
inline void riscv_decode_css_sdsp(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_arg_crs2::decode(insn);
	dec.imm = riscv_arg_cimmsdsp::decode(insn);
}

/* Decode CIW 4spn */
template <typename T>
inline void riscv_decode_ciw_4spn(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crdq::decode(insn) + 8;
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimm4spn::decode(insn);
}

/* Decode CL lw */
template <typename T>
inline void riscv_decode_cl_lw(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crdq::decode(insn) + 8;
	dec.rs1 = riscv_arg_crs1q::decode(insn) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmw::decode(insn);
}

/* Decode CL ld */
template <typename T>
inline void riscv_decode_cl_ld(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_crdq::decode(insn) + 8;
	dec.rs1 = riscv_arg_crs1q::decode(insn) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmd::decode(insn);
}

/* Decode CS f */
template <typename T>
inline void riscv_decode_cs(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rdq::decode(insn) + 8;
	dec.rs2 = riscv_arg_crs2q::decode(insn) + 8;
	dec.imm = 0;
}

/* Decode CS sd */
template <typename T>
inline void riscv_decode_cs_sd(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_arg_crs1q::decode(insn) + 8;
	dec.rs2 = riscv_arg_crs2q::decode(insn) + 8;
	dec.imm = riscv_arg_cimmd::decode(insn);
}

/* Decode CS sw */
template <typename T>
inline void riscv_decode_cs_sw(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_arg_crs1q::decode(insn) + 8;
	dec.rs2 = riscv_arg_crs2q::decode(insn) + 8;
	dec.imm = riscv_arg_cimmw::decode(insn);
}

/* Decode CB */
template <typename T>
inline void riscv_decode_cb(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_arg_crs1q::decode(insn) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmb::decode(insn);
}

/* Decode CB imm */
template <typename T>
inline void riscv_decode_cb_imm(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rdq::decode(insn) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmi::decode(insn);
}

/* Decode CB shamt5 */
template <typename T>
inline void riscv_decode_cb_sh5(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rdq::decode(insn) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmsh5::decode(insn);
}

/* Decode CB shamt6 */
template <typename T>
inline void riscv_decode_cb_sh6(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rdq::decode(insn) + 8;
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmsh6::decode(insn);
}


/* Decode CJ - imm */
template <typename T>
inline void riscv_decode_cj(T &dec, riscv_lu insn)
{
	dec.rd = dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmj::decode(insn);
}

/* Decode R */
template <typename T>
inline void riscv_decode_r(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_arg_rs2::decode(insn);
	dec.imm = 0;
}

/* Decode R RM */
template <typename T>
inline void riscv_decode_r_m(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_arg_rs2::decode(insn);
	dec.imm = 0;
	dec.rm = riscv_arg_rm::decode(insn);
}

/* Decode R AMO L */
template <typename T>
inline void riscv_decode_r_l(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = 0;
	dec.aq = riscv_arg_aq::decode(insn);
	dec.rl = riscv_arg_rl::decode(insn);
}

/* Decode R AMO S */
template <typename T>
inline void riscv_decode_r_a(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_arg_rs2::decode(insn);
	dec.imm = 0;
	dec.aq = riscv_arg_aq::decode(insn);
	dec.rl = riscv_arg_rl::decode(insn);
}

/* Decode R 4f */
template <typename T>
inline void riscv_decode_r4_m(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_arg_rs2::decode(insn);
	dec.rs3 = riscv_arg_rs3::decode(insn);
	dec.imm = 0;
	dec.rm = riscv_arg_rm::decode(insn);
}

/* Decode I */
template <typename T>
inline void riscv_decode_i(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_imm12::decode(insn);
}

/* Decode I sh5 */
template <typename T>
inline void riscv_decode_i_sh5(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_shamt5::decode(insn);
}

/* Decode I sh6 */
template <typename T>
inline void riscv_decode_i_sh6(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_shamt6::decode(insn);
}

/* Decode S Store */
template <typename T>
inline void riscv_decode_s(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_arg_rs2::decode(insn);
	dec.imm = riscv_arg_simm12::decode(insn);
}

/* Decode SB Branch */
template <typename T>
inline void riscv_decode_sb(T &dec, riscv_lu insn)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_arg_rs1::decode(insn);
	dec.rs2 = riscv_arg_rs2::decode(insn);
	dec.imm = riscv_arg_sbimm12::decode(insn);
}

/* Decode U */
template <typename T>
inline void riscv_decode_u(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_imm20::decode(insn);
}

/* Decode UJ */
template <typename T>
inline void riscv_decode_uj(T &dec, riscv_lu insn)
{
	dec.rd = riscv_arg_rd::decode(insn);
	dec.rs1 = dec.rs2 = riscv_ireg_zero;
	dec.imm = riscv_arg_jimm20::decode(insn);
}

/* Encode none */
template <typename T>
inline riscv_lu riscv_encode_none(T &dec) { return 0; }

/* Encode C nop */
template <typename T>
inline riscv_lu riscv_encode_ci_none(T &dec)
{
	assert(dec.rd == riscv_ireg_zero);
	assert(dec.rs1 == riscv_ireg_zero);
	assert(dec.rs2 == riscv_ireg_zero);
	return 0;
}

/* Encode CR */
template <typename T>
inline riscv_lu riscv_encode_cr(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rd::encode(dec.rd) |
		riscv_arg_crs2::encode(dec.rs2);
}

/* Encode CR mv */
template <typename T>
inline riscv_lu riscv_encode_cr_mv(T &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_crs2::encode(dec.rs2);
}

/* Encode CR jalr */
template <typename T>
inline riscv_lu riscv_encode_cr_jalr(T &dec)
{
	assert(dec.rd == riscv_ireg_ra);
	return riscv_arg_crs1::encode(dec.rs1);
}

/* Encode CR jr */
template <typename T>
inline riscv_lu riscv_encode_cr_jr(T &dec)
{
	assert(dec.rd == riscv_ireg_zero);
	return riscv_arg_crs1::encode(dec.rs1);
}

/* Encode CI */
template <typename T>
inline riscv_lu riscv_encode_ci(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rd::encode(dec.rs1) |
		riscv_arg_cimmi::encode(dec.imm);
}

/* Encode CI shamt5 */
template <typename T>
inline riscv_lu riscv_encode_ci_sh5(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rd::encode(dec.rs1) |
		riscv_arg_cimmsh5::encode(dec.imm);
}

/* Encode CI shamt6 */
template <typename T>
inline riscv_lu riscv_encode_ci_sh6(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rd::encode(dec.rs1) |
		riscv_arg_cimmsh6::encode(dec.imm);
}

/* Encode CI li */
template <typename T>
inline riscv_lu riscv_encode_ci_li(T &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmi::encode(dec.imm);
}

/* Encode CI lui */
template <typename T>
inline riscv_lu riscv_encode_ci_lui(T &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmui::encode(dec.imm);
}

/* Encode CI lwsp */
template <typename T>
inline riscv_lu riscv_encode_ci_lwsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmlwsp::encode(dec.imm);
}

/* Encode CI ldsp */
template <typename T>
inline riscv_lu riscv_encode_ci_ldsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmldsp::encode(dec.imm);
}

/* Encode CI 16sp */
template <typename T>
inline riscv_lu riscv_encode_ci_16sp(T &dec)
{
	assert(dec.rd == riscv_ireg_sp);
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_cimm16sp::encode(dec.imm);
}

/* Encode CSS swsp */
template <typename T>
inline riscv_lu riscv_encode_css_swsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crs2::encode(dec.rs2) |
		riscv_arg_cimmswsp::encode(dec.imm);
}

/* Encode CSS sdsp */
template <typename T>
inline riscv_lu riscv_encode_css_sdsp(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crs2::encode(dec.rs2) |
		riscv_arg_cimmsdsp::encode(dec.imm);
}

/* Encode CIW 4spn */
template <typename T>
inline riscv_lu riscv_encode_ciw_4spn(T &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_cimm4spn::encode(dec.imm);
}

/* Encode CL lw */
template <typename T>
inline riscv_lu riscv_encode_cl_lw(T &dec)
{
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_cimmw::encode(dec.imm);
}

/* Encode CL ld */
template <typename T>
inline riscv_lu riscv_encode_cl_ld(T &dec)
{
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_cimmd::encode(dec.imm);
}

/* Encode CS f */
template <typename T>
inline riscv_lu riscv_encode_cs(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_crs1q::encode(dec.rs1 + 8);
}

/* Encode CS sd */
template <typename T>
inline riscv_lu riscv_encode_cs_sd(T &dec)
{
	return riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_crs2q::encode(dec.rs2 + 8) |
		riscv_arg_cimmd::encode(dec.imm);
}

/* Encode CS sw */
template <typename T>
inline riscv_lu riscv_encode_cs_sw(T &dec)
{
	return riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_crs2q::encode(dec.rs2 + 8) |
		riscv_arg_cimmw::encode(dec.imm);
}

/* Encode CB */
template <typename T>
inline riscv_lu riscv_encode_cb(T &dec)
{
	return riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_cimmb::encode(dec.imm);
}

/* Encode CB imm */
template <typename T>
inline riscv_lu riscv_encode_cb_imm(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rdq::encode(dec.rs1 + 8)  |
		riscv_arg_cimmi::encode(dec.imm);
}

/* Encode CB shamt5 */
template <typename T>
inline riscv_lu riscv_encode_cb_sh5(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rdq::encode(dec.rs1 + 8)  |
		riscv_arg_cimmsh5::encode(dec.imm);
}

/* Encode CB shamt6 */
template <typename T>
inline riscv_lu riscv_encode_cb_sh6(T &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rdq::encode(dec.rs1 + 8)  |
		riscv_arg_cimmsh6::encode(dec.imm);
}


/* Encode CJ - imm */
template <typename T>
inline riscv_lu riscv_encode_cj(T &dec)
{
	return riscv_arg_cimmj::encode(dec.imm);
}

/* Encode R */
template <typename T>
inline riscv_lu riscv_encode_r(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2);
}

/* Encode R RM */
template <typename T>
inline riscv_lu riscv_encode_r_m(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_rm::encode(dec.rm);
}

/* Encode R AMO Load */
template <typename T>
inline riscv_lu riscv_encode_r_l(T &dec)
{
	assert(dec.rs2 == 0);
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_aq::encode(dec.aq) |
		riscv_arg_rl::encode(dec.rl);
}

/* Encode R AMO */
template <typename T>
inline riscv_lu riscv_encode_r_a(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_aq::encode(dec.aq) |
		riscv_arg_rl::encode(dec.rl);
}

/* Encode R 4f */
template <typename T>
inline riscv_lu riscv_encode_r4_m(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_rs3::encode(dec.rs3) |
		riscv_arg_rm::encode(dec.rm);
}

/* Encode I */
template <typename T>
inline riscv_lu riscv_encode_i(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_imm12::encode(dec.imm);
}

/* Encode I sh5 */
template <typename T>
inline riscv_lu riscv_encode_i_sh5(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_shamt5::encode(dec.imm);
}

/* Encode I sh6 */
template <typename T>
inline riscv_lu riscv_encode_i_sh6(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_shamt6::encode(dec.imm);
}

/* Encode S Store */
template <typename T>
inline riscv_lu riscv_encode_s(T &dec)
{
	return riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_simm12::encode(dec.imm);
}

/* Encode SB Branch */
template <typename T>
inline riscv_lu riscv_encode_sb(T &dec)
{
	return riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_sbimm12::encode(dec.imm);
}

/* Encode U */
template <typename T>
inline riscv_lu riscv_encode_u(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_imm20::encode(dec.imm);
}

/* Encode UJ */
template <typename T>
inline riscv_lu riscv_encode_uj(T &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_jimm20::encode(dec.imm);
}

#include "riscv-decode-codec.h"

/* Instruction length */

inline size_t riscv_get_insn_length(riscv_lu insn)
{
	// instruction length coding

	//      aa - 16 bit aa != 11
	//   bbb11 - 32 bit bbb != 111
	//  011111 - 48 bit
	// 0111111 - 64 bit

	// NOTE: currenttly supports maximum of 64-bit
	return (insn &      0b11) != 0b11      ? 2
		 : (insn &   0b11100) != 0b11100   ? 4
		 : (insn &  0b111111) == 0b011111  ? 6
		 : 8;
}

/* Fetch Instruction */

inline riscv_lu riscv_get_insn(riscv_ptr pc, riscv_ptr *next_pc = nullptr)
{
	// NOTE: currenttly supports maximum of 64-bit
	riscv_lu insn;
	if ((*(uint16_t*)pc & 0b11) != 0b11) {
		insn = htole16(*(uint16_t*)pc);
		if (next_pc) *next_pc = pc + 2;
	} else if ((*(uint16_t*)pc & 0b11100) != 0b11100) {
		insn = htole32(*(uint32_t*)pc);
		if (next_pc) *next_pc = pc + 4;
	} else if ((*(uint16_t*)pc & 0b111111) == 0b011111) {
		insn = uint64_t(htole32(*(uint32_t*)pc)) | uint64_t(htole16(*(uint16_t*)(pc + 4))) << 32;
		if (next_pc) *next_pc = pc + 6;
	} else {
		insn = htole64(*(uint64_t*)pc);
		if (next_pc) *next_pc = pc + 8;
	}
	return insn;
}

/* Decompress Instruction */

template <typename T>
inline void riscv_decode_decompress(T &dec)
{
    int decomp_op = riscv_insn_decomp[dec.op];
    if (decomp_op != riscv_op_unknown) {
        dec.op = decomp_op;
        dec.codec = riscv_insn_codec[decomp_op];
    }
}

/* Decode Instruction */

template <typename T, bool rv32, bool rv64, bool rvi = true, bool rvm = true, bool rva = true, bool rvs = true, bool rvf = true, bool rvd = true, bool rvc = true>
inline void riscv_decode_insn(T &dec, riscv_lu insn)
{
	dec.op = riscv_decode_op<rv32,rv64,rvi,rvm,rva,rvs,rvf,rvd,rvc>(insn);
	riscv_decode_type<T>(dec, insn);
	riscv_decode_decompress<T>(dec);
}

template <typename T>
inline void riscv_decode_rv32(T &dec, riscv_lu insn)
{
	riscv_decode_insn<T,true,false>(dec, insn);
}

template <typename T>
inline void riscv_decode_rv64(T &dec, riscv_lu insn)
{
	riscv_decode_insn<T,false,true>(dec, insn);
}

/* Check constraint */

template <typename T>
inline bool riscv_compress_check(T &dec, const rvc_constraint *c)
{
	auto imm = dec.imm;
	auto rd = dec.rd, rs1 = dec.rs1, rs2 = dec.rs2;
	while (*c != rvc_end) {
		switch (*c) {
			case rvc_imm_6:         if (!(imm <= (imm & 0b111111))) return false; break;
			case rvc_imm_7:         if (!(imm <= (imm & 0b1111111))) return false; break;
			case rvc_imm_8:         if (!(imm <= (imm & 0b11111111))) return false; break;
			case rvc_imm_9:         if (!(imm <= (imm & 0b111111111))) return false; break;
			case rvc_imm_10:        if (!(imm <= (imm & 0b1111111111))) return false; break;
			case rvc_imm_12:        if (!(imm <= (imm & 0b111111111111))) return false; break;
			case rvc_imm_18:        if (!(imm <= (imm & 0b111111111111111111))) return false; break;
			case rvc_imm_nz:        if (!(imm != 0)) return false; break;
			case rvc_imm_x2:        if (!((imm & 0b1) == 0)) return false; break;
			case rvc_imm_x4:        if (!((imm & 0b11) == 0)) return false; break;
			case rvc_imm_x8:        if (!((imm & 0b111) == 0)) return false; break;
			case rvc_rd_b3:         if (!(rd  >= 8 && rd  <= 15)) return false; break;
			case rvc_rs1_b3:        if (!(rs1 >= 8 && rs1 <= 15)) return false; break;
			case rvc_rs2_b3:        if (!(rs2 >= 8 && rs2 <= 15)) return false; break;
			case rvc_rd_eq_rs1:     if (!(rd == rs1)) return false; break;
			case rvc_rd_eq_ra:      if (!(rd == 1)) return false; break;
			case rvc_rd_eq_sp:      if (!(rd == 2)) return false; break;
			case rvc_rd_eq_x0:      if (!(rd == 0)) return false; break;
			case rvc_rs1_eq_sp:     if (!(rs1 == 2)) return false; break;
			case rvc_rs1_eq_x0:     if (!(rs1 == 0)) return false; break;
			case rvc_rs2_eq_x0:     if (!(rs2 == 0)) return false; break;
			case rvc_rd_ne_x0_x2:   if (!(rd != 0 && rd != 2)) return false; break;
			case rvc_rd_ne_x0:      if (!(rd != 0)) return false; break;
			case rvc_rs1_ne_x0:     if (!(rs1 != 0)) return false; break;
			case rvc_rs2_ne_x0:     if (!(rs2 != 0)) return false; break;
			case rvc_end:           break;
		}
		c++;
	}
	return true;
}

/* Compress Instruction */

template <typename T>
inline bool riscv_compress_insn(T &dec)
{
	const riscv_comp_data *comp_data = riscv_insn_comp[dec.op];
	if (!comp_data) return false;
	while (comp_data->constraints) {
		if (riscv_compress_check(dec, comp_data->constraints)) {
			dec.op = comp_data->op;
			dec.codec = riscv_insn_codec[dec.op];
			return true;
		}
		comp_data++;
	}
	return false;
}

#endif
