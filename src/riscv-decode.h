//
//  riscv-decode.h
//

#ifndef riscv_decode_h
#define riscv_decode_h

/* Decoded Instruction */

struct riscv_decode
{
	riscv_ptr pc;
	riscv_lu  inst;
	riscv_l   imm;
	riscv_hu  op;
	riscv_hu  codec;
	riscv_wu  rd   : 6;
	riscv_wu  rs1  : 6;
	riscv_wu  rs2  : 6;
	riscv_wu  rs3  : 6;
	riscv_wu  arg  : 8;
};

/* Decode arg */

typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_rd;
typedef uimm_t<5, S<19,15, B<4,0>>>                                          riscv_arg_rs1;
typedef uimm_t<5, S<24,20, B<4,0>>>                                          riscv_arg_rs2;
typedef uimm_t<5, S<31,27, B<4,0>>>                                          riscv_arg_rs3;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_frd;
typedef uimm_t<5, S<19,15, B<4,0>>>                                          riscv_arg_frs1;
typedef uimm_t<5, S<24,20, B<4,0>>>                                          riscv_arg_frs2;
typedef uimm_t<5, S<31,27, B<4,0>>>                                          riscv_arg_frs3;
typedef uimm_t<2, S<26,25, B<1,0>>>                                          riscv_arg_aqrl;
typedef uimm_t<4, S<27,24, B<3,0>>>                                          riscv_arg_pred;
typedef uimm_t<4, S<23,20, B<3,0>>>                                          riscv_arg_succ;
typedef uimm_t<3, S<14,12, B<2,0>>>                                          riscv_arg_rm;
typedef simm_t<32, S<31,12, B<31,12>>>                                       riscv_arg_imm20;
typedef simm_t<21, S<31,12, B<20>,B<10,1>,B<11>,B<19,12>>>                   riscv_arg_jimm20;
typedef simm_t<12, S<31,20, B<11,0>>>                                        riscv_arg_imm12;
typedef simm_t<12, S<31,25, B<11,5>>, S<11,7, B<4,0>>>                       riscv_arg_simm12;
typedef simm_t<13, S<31,25, B<12>,B<10,5>>, S<11,7, B<4,1>,B<11>>>           riscv_arg_sbimm12;
typedef simm_t<5, S<19,15, B<4,0>>>                                          riscv_arg_zimm;
typedef uimm_t<5, S<24,20, B<4,0>>>                                          riscv_arg_shamt5;
typedef uimm_t<6, S<25,20, B<5,0>>>                                          riscv_arg_shamt6;
typedef uimm_t<3, S<4,2, B<2,0>>>                                            riscv_arg_crdq;
typedef uimm_t<3, S<9,7, B<2,0>>>                                            riscv_arg_crs1q;
typedef uimm_t<3, S<9,7, B<2,0>>>                                            riscv_arg_crs1rdq;
typedef uimm_t<3, S<4,2, B<2,0>>>                                            riscv_arg_crs2q;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_crd;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_crs1;
typedef uimm_t<5, S<11,7, B<4,0>>>                                           riscv_arg_crs1rd;
typedef uimm_t<5, S<6,2, B<4,0>>>                                            riscv_arg_crs2;
typedef uimm_t<6, S<12,12, B<5>>, S<6,2, B<4,0>>>                            riscv_arg_cimmsh;
typedef simm_t<6, S<12,12, B<5>>, S<6,2, B<4,0>>>                            riscv_arg_cimmi;
typedef simm_t<18, S<12,12, B<17>>, S<6,2, B<16,12>>>                        riscv_arg_cimmui;
typedef simm_t<8, S<12,12, B<5>>, S<6,2, B<4,2>,B<7,6>>>                     riscv_arg_cimmlwsp;
typedef simm_t<9, S<12,12, B<5>>, S<6,2, B<4,3>,B<8,6>>>                     riscv_arg_cimmldsp;
typedef simm_t<10, S<12,12, B<9>>, S<6,2, B<4>,B<6>,B<8,7>,B<5>>>            riscv_arg_cimm16sp;
typedef simm_t<12, S<12,2, B<11>,B<4>,B<9,8>,B<10>,B<6>,B<7>,B<3,1>,B<5>>>   riscv_arg_cimmj;
typedef simm_t<9, S<12,10, B<8>,B<4,3>>, S<6,2, B<7,6>,B<2,1>,B<5>>>         riscv_arg_cimmb;
typedef simm_t<8, S<12,7, B<5,2>,B<7,6>>>                                    riscv_arg_cimmswsp;
typedef simm_t<9, S<12,7, B<5,3>,B<8,6>>>                                    riscv_arg_cimmsdsp;
typedef simm_t<10, S<12,7, B<5,4>,B<9,6>>>                                   riscv_arg_cimmsqsp;
typedef simm_t<10, S<12,5, B<5,4>,B<9,6>,B<2>,B<3>>>                         riscv_arg_cimm4spn;
typedef simm_t<7, S<12,10, B<5,3>>, S<6,5, B<2>,B<6>>>                       riscv_arg_cimmw;
typedef simm_t<8, S<12,10, B<5,3>>, S<6,5, B<7,6>>>                          riscv_arg_cimmd;
typedef simm_t<9, S<12,10, B<5,4>,B<8>>, S<6,5, B<7,6>>>                     riscv_arg_cimmq;

/* Decode none */
inline void riscv_decode_none(riscv_decode &dec, riscv_lu inst) {}

/* Decode C nop */
inline void riscv_decode_ci_nop(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = dec.rs1 = dec.rs2 = riscv_ireg_zero;
}

/* Decode CR */
inline void riscv_decode_cr(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rd::decode(inst);
	dec.rs2 = riscv_arg_crs2::decode(inst);
}

/* Decode CR mv */
inline void riscv_decode_cr_mv(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_zero;
	dec.rs2 = riscv_arg_crs2::decode(inst);
}

/* Decode CR jalr */
inline void riscv_decode_cr_jalr(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_ireg_ra;
	dec.rs1 = riscv_arg_crs1::decode(inst);
}

/* Decode CR jr */
inline void riscv_decode_cr_jr(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = riscv_arg_crs1::decode(inst);
}

/* Decode CI */
inline void riscv_decode_ci(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rd::decode(inst);
	dec.imm = riscv_arg_cimmi::decode(inst);
}

/* Decode CI shamt5 */
inline void riscv_decode_ci_sh5(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = dec.rs1 = riscv_arg_crs1rd::decode(inst);
	dec.imm = riscv_arg_cimmsh::decode(inst);
}

/* Decode CI li */
inline void riscv_decode_ci_li(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmi::decode(inst);
}

/* Decode CI lui */
inline void riscv_decode_ci_lui(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_zero;
	dec.imm = riscv_arg_cimmui::decode(inst);
}

/* Decode CI lwsp */
inline void riscv_decode_ci_lwsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_sp;
	dec.imm = riscv_arg_cimmlwsp::decode(inst);
}

/* Decode CI ldsp */
inline void riscv_decode_ci_ldsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crd::decode(inst);
	dec.rs1 = riscv_ireg_sp;
	dec.imm = riscv_arg_cimmldsp::decode(inst);
}

/* Decode CI 16sp */
inline void riscv_decode_ci_16sp(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_ireg_sp;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = riscv_arg_cimm16sp::decode(inst);
}

/* Decode CSS swsp */
inline void riscv_decode_css_swsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_arg_crs2::decode(inst);
	dec.imm = riscv_arg_cimmswsp::decode(inst);
}

/* Decode CSS sdsp */
inline void riscv_decode_css_sdsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = riscv_arg_crs2::decode(inst);
	dec.imm = riscv_arg_cimmsdsp::decode(inst);
}

/* Decode CIW 4spn */
inline void riscv_decode_ciw_4spn(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crdq::decode(inst) + 8;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = riscv_arg_cimm4spn::decode(inst);
}

/* Decode CL lw */
inline void riscv_decode_cl_lw(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crdq::decode(inst) + 8;
	dec.rs1 = riscv_arg_crs1q::decode(inst) + 8;
	dec.imm = riscv_arg_cimmw::decode(inst);
}

/* Decode CL ld */
inline void riscv_decode_cl_ld(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_crdq::decode(inst) + 8;
	dec.rs1 = riscv_arg_crs1q::decode(inst) + 8;
	dec.imm = riscv_arg_cimmd::decode(inst);
}

/* Decode CS f */
inline void riscv_decode_cs(riscv_decode &dec, riscv_lu inst)
{
	dec.rs2 = riscv_arg_crs2q::decode(inst) + 8;
	dec.rd = dec.rs1 = riscv_arg_crs1rdq::decode(inst) + 8;
}

/* Decode CS sd */
inline void riscv_decode_cs_sd(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_arg_crs1q::decode(inst) + 8;
	dec.rs2 = riscv_arg_crs2q::decode(inst) + 8;
	dec.imm = riscv_arg_cimmd::decode(inst);
}

/* Decode CS sw */
inline void riscv_decode_cs_sw(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_arg_crs1q::decode(inst) + 8;
	dec.rs2 = riscv_arg_crs2q::decode(inst) + 8;
	dec.imm = riscv_arg_cimmw::decode(inst);
}

/* Decode CB */
inline void riscv_decode_cb(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_arg_crs1q::decode(inst) + 8;
	dec.imm = riscv_arg_cimmb::decode(inst);
}

/* Decode CB shamt5 */
inline void riscv_decode_cb_sh5(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_arg_crs1q::decode(inst) + 8;
	dec.imm = riscv_arg_cimmsh::decode(inst);
}

/* Decode CJ - imm */
inline void riscv_decode_cj(riscv_decode &dec, riscv_lu inst)
{
	dec.imm = riscv_arg_cimmj::decode(inst);
}

/* Decode R */
inline void riscv_decode_r(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.rs2 = riscv_arg_rs2::decode(inst);
}

/* Decode R RM */
inline void riscv_decode_r_m(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.rs2 = riscv_arg_rs2::decode(inst);
	dec.arg = riscv_arg_rm::decode(inst);
}

/* Decode R AMO L */
inline void riscv_decode_r_l(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.arg = riscv_arg_aqrl::decode(inst);
}

/* Decode R AMO S */
inline void riscv_decode_r_a(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.rs2 = riscv_arg_rs2::decode(inst);
	dec.arg = riscv_arg_aqrl::decode(inst);
}

/* Decode R 4f */
inline void riscv_decode_r_4(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.rs2 = riscv_arg_rs2::decode(inst);
	dec.rs3 = riscv_arg_rs3::decode(inst);
	dec.arg = riscv_arg_rm::decode(inst);
}

/* Decode I */
inline void riscv_decode_i(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.imm = riscv_arg_imm12::decode(inst);
}

/* Decode I sh5 */
inline void riscv_decode_i_sh5(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.imm = riscv_arg_shamt5::decode(inst);
}

/* Decode I sh6 */
inline void riscv_decode_i_sh6(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.imm = riscv_arg_shamt6::decode(inst);
}

/* Decode S Store */
inline void riscv_decode_s(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.rs2 = riscv_arg_rs2::decode(inst);
	dec.imm = riscv_arg_simm12::decode(inst);
}

/* Decode SB Branch */
inline void riscv_decode_sb(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_arg_rs1::decode(inst);
	dec.rs2 = riscv_arg_rs2::decode(inst);
	dec.imm = riscv_arg_sbimm12::decode(inst);
}

/* Decode U */
inline void riscv_decode_u(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.imm = riscv_arg_imm20::decode(inst);
}

/* Decode UJ */
inline void riscv_decode_uj(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_arg_rd::decode(inst);
	dec.imm = riscv_arg_jimm20::decode(inst);
}

/* Encode none */
inline riscv_lu riscv_encode_none(riscv_decode &dec) { return 0; }

/* Encode C nop */
inline riscv_lu riscv_encode_ci_nop(riscv_decode &dec)
{
	assert(dec.rd == riscv_ireg_zero);
	assert(dec.rs1 == riscv_ireg_zero);
	assert(dec.rs2 == riscv_ireg_zero);
	return 0;
}

/* Encode CR */
inline riscv_lu riscv_encode_cr(riscv_decode &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rd::encode(dec.rd) |
		riscv_arg_crs2::encode(dec.rs2);
}

/* Encode CR mv */
inline riscv_lu riscv_encode_cr_mv(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_crs2::encode(dec.rs2);
}

/* Encode CR jalr */
inline riscv_lu riscv_encode_cr_jalr(riscv_decode &dec)
{
	assert(dec.rd == riscv_ireg_ra);
	return riscv_arg_crs1::encode(dec.rs1);
}

/* Encode CR jr */
inline riscv_lu riscv_encode_cr_jr(riscv_decode &dec)
{
	assert(dec.rd == riscv_ireg_zero);
	return riscv_arg_crs1::encode(dec.rs1);
}

/* Encode CI */
inline riscv_lu riscv_encode_ci(riscv_decode &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rd::encode(dec.rs1) |
		riscv_arg_cimmi::encode(dec.imm);
}

/* Encode CI shamt5 */
inline riscv_lu riscv_encode_ci_sh5(riscv_decode &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crs1rd::encode(dec.rs1) |
		riscv_arg_cimmsh::encode(dec.imm);
}

/* Encode CI li */
inline riscv_lu riscv_encode_ci_li(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmi::encode(dec.imm);
}

/* Encode CI lui */
inline riscv_lu riscv_encode_ci_lui(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_zero);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmui::encode(dec.imm);
}

/* Encode CI lwsp */
inline riscv_lu riscv_encode_ci_lwsp(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmlwsp::encode(dec.imm);
}

/* Encode CI ldsp */
inline riscv_lu riscv_encode_ci_ldsp(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crd::encode(dec.rd) |
		riscv_arg_cimmldsp::encode(dec.imm);
}

/* Encode CI 16sp */
inline riscv_lu riscv_encode_ci_16sp(riscv_decode &dec)
{
	assert(dec.rd == riscv_ireg_sp);
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_cimm16sp::encode(dec.imm);
}

/* Encode CSS swsp */
inline riscv_lu riscv_encode_css_swsp(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crs2::encode(dec.rs2) |
		riscv_arg_cimmswsp::encode(dec.imm);
}

/* Encode CSS sdsp */
inline riscv_lu riscv_encode_css_sdsp(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crs2::encode(dec.rs2) |
		riscv_arg_cimmsdsp::encode(dec.imm);
}

/* Encode CIW 4spn */
inline riscv_lu riscv_encode_ciw_4spn(riscv_decode &dec)
{
	assert(dec.rs1 == riscv_ireg_sp);
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_cimm4spn::encode(dec.imm);
}

/* Encode CL lw */
inline riscv_lu riscv_encode_cl_lw(riscv_decode &dec)
{
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_cimmw::encode(dec.imm);
}

/* Encode CL ld */
inline riscv_lu riscv_encode_cl_ld(riscv_decode &dec)
{
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_cimmd::encode(dec.imm);
}

/* Encode CS f */
inline riscv_lu riscv_encode_cs(riscv_decode &dec)
{
	assert(dec.rd == dec.rs1);
	return riscv_arg_crdq::encode(dec.rd + 8) |
		riscv_arg_crs1q::encode(dec.rs1 + 8);
}

/* Encode CS sd */
inline riscv_lu riscv_encode_cs_sd(riscv_decode &dec)
{
	return riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_crs2q::encode(dec.rs2 + 8) |
		riscv_arg_cimmd::encode(dec.imm);
}

/* Encode CS sw */
inline riscv_lu riscv_encode_cs_sw(riscv_decode &dec)
{
	return riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_crs2q::encode(dec.rs2 + 8) |
		riscv_arg_cimmw::encode(dec.imm);
}

/* Encode CB */
inline riscv_lu riscv_encode_cb(riscv_decode &dec)
{
	return riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_cimmb::encode(dec.imm);
}

/* Encode CB shamt5 */
inline riscv_lu riscv_encode_cb_sh5(riscv_decode &dec)
{
	return riscv_arg_crs1q::encode(dec.rs1 + 8)  |
		riscv_arg_cimmsh::encode(dec.imm);
}

/* Encode CJ - imm */
inline riscv_lu riscv_encode_cj(riscv_decode &dec)
{
	return riscv_arg_cimmj::encode(dec.imm);
}

/* Encode R */
inline riscv_lu riscv_encode_r(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2);
}

/* Encode R RM */
inline riscv_lu riscv_encode_r_m(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_rm::encode(dec.arg);
}

/* Encode R AMO Load */
inline riscv_lu riscv_encode_r_l(riscv_decode &dec)
{
	assert(dec.rs2 == 0);
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_aqrl::encode(dec.arg);
}

/* Encode R AMO */
inline riscv_lu riscv_encode_r_a(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_aqrl::encode(dec.arg);
}

/* Encode R 4f */
inline riscv_lu riscv_encode_r_4(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_rs3::encode(dec.rs3) |
		riscv_arg_rm::encode(dec.arg);
}

/* Encode I */
inline riscv_lu riscv_encode_i(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_imm12::encode(dec.imm);
}

/* Encode I sh5 */
inline riscv_lu riscv_encode_i_sh5(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_shamt5::encode(dec.imm);
}

/* Encode I sh6 */
inline riscv_lu riscv_encode_i_sh6(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_shamt6::encode(dec.imm);
}

/* Encode S Store */
inline riscv_lu riscv_encode_s(riscv_decode &dec)
{
	return riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_simm12::encode(dec.imm);
}

/* Encode SB Branch */
inline riscv_lu riscv_encode_sb(riscv_decode &dec)
{
	return riscv_arg_rs1::encode(dec.rs1) |
		riscv_arg_rs2::encode(dec.rs2) |
		riscv_arg_sbimm12::encode(dec.imm);
}

/* Encode U */
inline riscv_lu riscv_encode_u(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_imm20::encode(dec.imm);
}

/* Encode UJ */
inline riscv_lu riscv_encode_uj(riscv_decode &dec)
{
	return riscv_arg_rd::encode(dec.rd) |
		riscv_arg_jimm20::encode(dec.imm);
}

/* Decode Instruction Opcode */

template <bool rv32 = false, bool rv64 = true, bool rvi = true, bool rvm = true, bool rva = true, bool rvs = true, bool rvf = true, bool rvd = true, bool rvc = true>
void riscv_decode_opcode(riscv_decode &dec, riscv_lu inst)
{
	switch (((inst >> 0) & 0b11) /* inst[1:0] */) {
		case 0:
			// c.addi4spn c.fld c.lw c.flw c.fsd c.sw c.fsw c.ld c.sd
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0: if (rvc) dec.op = riscv_op_c_addi4spn; break;
				case 1: if (rvc) dec.op = riscv_op_c_fld; break;
				case 2: if (rvc) dec.op = riscv_op_c_lw; break;
				case 3: 
					if (rvc && rv32) dec.op = riscv_op_c_flw;
					else if (rvc && rv64) dec.op = riscv_op_c_ld;
					break;
				case 5: if (rvc) dec.op = riscv_op_c_fsd; break;
				case 6: if (rvc) dec.op = riscv_op_c_sw; break;
				case 7: 
					if (rvc && rv32) dec.op = riscv_op_c_fsw;
					else if (rvc && rv64) dec.op = riscv_op_c_sd;
					break;
			}
			break;
		case 1:
			// c.nop c.addi c.jal c.li c.lui c.addi16sp c.srli c.srai c.andi c.sub c.xor c.or ...
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0:
					// c.nop c.addi
					switch (((inst >> 2) & 0b11111111111) /* inst[12:2] */) {
						case 0: if (rvc) dec.op = riscv_op_c_nop; break;
						default: if (rvc) dec.op = riscv_op_c_addi; break;
					}
					break;
				case 1: 
					if (rvc && rv32) dec.op = riscv_op_c_jal;
					else if (rvc && rv64) dec.op = riscv_op_c_addiw;
					break;
				case 2: if (rvc) dec.op = riscv_op_c_li; break;
				case 3:
					// c.lui c.addi16sp
					switch (((inst >> 7) & 0b11111) /* inst[11:7] */) {
						case 2: if (rvc) dec.op = riscv_op_c_addi16sp; break;
						default: if (rvc) dec.op = riscv_op_c_lui; break;
					}
					break;
				case 4:
					// c.srli c.srai c.andi c.sub c.xor c.or c.and c.subw c.addw
					switch (((inst >> 10) & 0b11) /* inst[11:10] */) {
						case 0: if (rvc) dec.op = riscv_op_c_srli; break;
						case 1: if (rvc) dec.op = riscv_op_c_srai; break;
						case 2: if (rvc) dec.op = riscv_op_c_andi; break;
						case 3:
							// c.sub c.xor c.or c.and c.subw c.addw
							switch (((inst >> 10) & 0b100) | ((inst >> 5) & 0b011) /* inst[12|6:5] */) {
								case 0: if (rvc) dec.op = riscv_op_c_sub; break;
								case 1: if (rvc) dec.op = riscv_op_c_xor; break;
								case 2: if (rvc) dec.op = riscv_op_c_or; break;
								case 3: if (rvc) dec.op = riscv_op_c_and; break;
								case 4: if (rvc) dec.op = riscv_op_c_subw; break;
								case 5: if (rvc) dec.op = riscv_op_c_addw; break;
							}
							break;
					}
					break;
				case 5: if (rvc) dec.op = riscv_op_c_j; break;
				case 6: if (rvc) dec.op = riscv_op_c_beqz; break;
				case 7: if (rvc) dec.op = riscv_op_c_bnez; break;
			}
			break;
		case 2:
			// c.slli c.fldsp c.lwsp c.flwsp c.jr c.mv c.ebreak c.jalr c.add c.fsdsp c.swsp c.fswsp ...
			switch (((inst >> 13) & 0b111) /* inst[15:13] */) {
				case 0: if (rvc) dec.op = riscv_op_c_slli; break;
				case 1: if (rvc) dec.op = riscv_op_c_fldsp; break;
				case 2: if (rvc) dec.op = riscv_op_c_lwsp; break;
				case 3: 
					if (rvc && rv32) dec.op = riscv_op_c_flwsp;
					else if (rvc && rv64) dec.op = riscv_op_c_ldsp;
					break;
				case 4:
					// c.jr c.mv c.ebreak c.jalr c.add
					switch (((inst >> 12) & 0b1) /* inst[12] */) {
						case 0:
							// c.jr c.mv
							switch (((inst >> 2) & 0b11111) /* inst[6:2] */) {
								case 0: if (rvc) dec.op = riscv_op_c_jr; break;
								default: if (rvc) dec.op = riscv_op_c_mv; break;
							}
							break;
						case 1:
							// c.ebreak c.jalr c.add
							switch (((inst >> 2) & 0b11111) /* inst[6:2] */) {
								case 0:
									// c.ebreak c.jalr
									switch (((inst >> 7) & 0b11111) /* inst[11:7] */) {
										case 0: if (rvc) dec.op = riscv_op_c_ebreak; break;
										default: if (rvc) dec.op = riscv_op_c_jalr; break;
									}
									break;
								default: if (rvc) dec.op = riscv_op_c_add; break;
							}
							break;
					}
					break;
				case 5: if (rvc) dec.op = riscv_op_c_fsdsp; break;
				case 6: if (rvc) dec.op = riscv_op_c_swsp; break;
				case 7: 
					if (rvc && rv32) dec.op = riscv_op_c_fswsp;
					else if (rvc && rv64) dec.op = riscv_op_c_sdsp;
					break;
			}
			break;
		case 3:
			// lui auipc jal jalr beq bne blt bge bltu bgeu lb lh ...
			switch (((inst >> 2) & 0b11111) /* inst[6:2] */) {
				case 0:
					// lb lh lw lbu lhu lwu ld
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_lb; break;
						case 1: if (rvi) dec.op = riscv_op_lh; break;
						case 2: if (rvi) dec.op = riscv_op_lw; break;
						case 3: if (rvi && rv64) dec.op = riscv_op_ld; break;
						case 4: if (rvi) dec.op = riscv_op_lbu; break;
						case 5: if (rvi) dec.op = riscv_op_lhu; break;
						case 6: if (rvi && rv64) dec.op = riscv_op_lwu; break;
					}
					break;
				case 1:
					// flw fld
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 2: if (rvf) dec.op = riscv_op_flw; break;
						case 3: if (rvd) dec.op = riscv_op_fld; break;
					}
					break;
				case 3:
					// fence fence.i
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_fence; break;
						case 1: if (rvi) dec.op = riscv_op_fence_i; break;
					}
					break;
				case 4:
					// addi slti sltiu xori ori andi slli srli srai slli srli srai
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_addi; break;
						case 1:
							// slli slli
							switch (((inst >> 26) & 0b111111) /* inst[31:26] */) {
								case 0: 
									if (rvi && rv32) dec.op = riscv_op_slli_rv32i;
									else if (rvi && rv64) dec.op = riscv_op_slli_rv64i;
									break;
							}
							break;
						case 2: if (rvi) dec.op = riscv_op_slti; break;
						case 3: if (rvi) dec.op = riscv_op_sltiu; break;
						case 4: if (rvi) dec.op = riscv_op_xori; break;
						case 5:
							// srli srai srli srai
							switch (((inst >> 26) & 0b111111) /* inst[31:26] */) {
								case 0: 
									if (rvi && rv32) dec.op = riscv_op_srli_rv32i;
									else if (rvi && rv64) dec.op = riscv_op_srli_rv64i;
									break;
								case 16: 
									if (rvi && rv32) dec.op = riscv_op_srai_rv32i;
									else if (rvi && rv64) dec.op = riscv_op_srai_rv64i;
									break;
							}
							break;
						case 6: if (rvi) dec.op = riscv_op_ori; break;
						case 7: if (rvi) dec.op = riscv_op_andi; break;
					}
					break;
				case 5: if (rvi) dec.op = riscv_op_auipc; break;
				case 6:
					// addiw slliw srliw sraiw
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi && rv64) dec.op = riscv_op_addiw; break;
						case 1:
							// slliw
							switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
								case 0: if (rvi && rv64) dec.op = riscv_op_slliw; break;
							}
							break;
						case 5:
							// srliw sraiw
							switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
								case 0: if (rvi && rv64) dec.op = riscv_op_srliw; break;
								case 32: if (rvi && rv64) dec.op = riscv_op_sraiw; break;
							}
							break;
					}
					break;
				case 8:
					// sb sh sw sd
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_sb; break;
						case 1: if (rvi) dec.op = riscv_op_sh; break;
						case 2: if (rvi) dec.op = riscv_op_sw; break;
						case 3: if (rvi && rv64) dec.op = riscv_op_sd; break;
					}
					break;
				case 9:
					// fsw fsd
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 2: if (rvf) dec.op = riscv_op_fsw; break;
						case 3: if (rvd) dec.op = riscv_op_fsd; break;
					}
					break;
				case 11:
					// lr.w sc.w amoswap.w amoadd.w amoxor.w amoor.w amoand.w amomin.w amomax.w amominu.w amomaxu.w lr.d ...
					switch (((inst >> 24) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[31:27|14:12] */) {
						case 2: if (rva) dec.op = riscv_op_amoadd_w; break;
						case 3: if (rva && rv64) dec.op = riscv_op_amoadd_d; break;
						case 10: if (rva) dec.op = riscv_op_amoswap_w; break;
						case 11: if (rva && rv64) dec.op = riscv_op_amoswap_d; break;
						case 18:
							// lr.w
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rva) dec.op = riscv_op_lr_w; break;
							}
							break;
						case 19:
							// lr.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rva && rv64) dec.op = riscv_op_lr_d; break;
							}
							break;
						case 26: if (rva) dec.op = riscv_op_sc_w; break;
						case 27: if (rva && rv64) dec.op = riscv_op_sc_d; break;
						case 34: if (rva) dec.op = riscv_op_amoxor_w; break;
						case 35: if (rva && rv64) dec.op = riscv_op_amoxor_d; break;
						case 66: if (rva) dec.op = riscv_op_amoor_w; break;
						case 67: if (rva && rv64) dec.op = riscv_op_amoor_d; break;
						case 98: if (rva) dec.op = riscv_op_amoand_w; break;
						case 99: if (rva && rv64) dec.op = riscv_op_amoand_d; break;
						case 130: if (rva) dec.op = riscv_op_amomin_w; break;
						case 131: if (rva && rv64) dec.op = riscv_op_amomin_d; break;
						case 162: if (rva) dec.op = riscv_op_amomax_w; break;
						case 163: if (rva && rv64) dec.op = riscv_op_amomax_d; break;
						case 194: if (rva) dec.op = riscv_op_amominu_w; break;
						case 195: if (rva && rv64) dec.op = riscv_op_amominu_d; break;
						case 226: if (rva) dec.op = riscv_op_amomaxu_w; break;
						case 227: if (rva && rv64) dec.op = riscv_op_amomaxu_d; break;
					}
					break;
				case 12:
					// add sub sll slt sltu xor srl sra or and mul mulh ...
					switch (((inst >> 22) & 0b1111111000) | ((inst >> 12) & 0b0000000111) /* inst[31:25|14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_add; break;
						case 1: if (rvi) dec.op = riscv_op_sll; break;
						case 2: if (rvi) dec.op = riscv_op_slt; break;
						case 3: if (rvi) dec.op = riscv_op_sltu; break;
						case 4: if (rvi) dec.op = riscv_op_xor; break;
						case 5: if (rvi) dec.op = riscv_op_srl; break;
						case 6: if (rvi) dec.op = riscv_op_or; break;
						case 7: if (rvi) dec.op = riscv_op_and; break;
						case 8: if (rvm) dec.op = riscv_op_mul; break;
						case 9: if (rvm) dec.op = riscv_op_mulh; break;
						case 10: if (rvm) dec.op = riscv_op_mulhsu; break;
						case 11: if (rvm) dec.op = riscv_op_mulhu; break;
						case 12: if (rvm) dec.op = riscv_op_div; break;
						case 13: if (rvm) dec.op = riscv_op_divu; break;
						case 14: if (rvm) dec.op = riscv_op_rem; break;
						case 15: if (rvm) dec.op = riscv_op_remu; break;
						case 256: if (rvi) dec.op = riscv_op_sub; break;
						case 261: if (rvi) dec.op = riscv_op_sra; break;
					}
					break;
				case 13: if (rvi) dec.op = riscv_op_lui; break;
				case 14:
					// addw subw sllw srlw sraw mulw divw divuw remw remuw
					switch (((inst >> 22) & 0b1111111000) | ((inst >> 12) & 0b0000000111) /* inst[31:25|14:12] */) {
						case 0: if (rvi && rv64) dec.op = riscv_op_addw; break;
						case 1: if (rvi && rv64) dec.op = riscv_op_sllw; break;
						case 5: if (rvi && rv64) dec.op = riscv_op_srlw; break;
						case 8: if (rvm && rv64) dec.op = riscv_op_mulw; break;
						case 12: if (rvm && rv64) dec.op = riscv_op_divw; break;
						case 13: if (rvm && rv64) dec.op = riscv_op_divuw; break;
						case 14: if (rvm && rv64) dec.op = riscv_op_remw; break;
						case 15: if (rvm && rv64) dec.op = riscv_op_remuw; break;
						case 256: if (rvi && rv64) dec.op = riscv_op_subw; break;
						case 261: if (rvi && rv64) dec.op = riscv_op_sraw; break;
					}
					break;
				case 16:
					// fmadd.s fmadd.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fmadd_s; break;
						case 1: if (rvd) dec.op = riscv_op_fmadd_d; break;
					}
					break;
				case 17:
					// fmsub.s fmsub.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fmsub_s; break;
						case 1: if (rvd) dec.op = riscv_op_fmsub_d; break;
					}
					break;
				case 18:
					// fnmsub.s fnmsub.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fnmsub_s; break;
						case 1: if (rvd) dec.op = riscv_op_fnmsub_d; break;
					}
					break;
				case 19:
					// fnmadd.s fnmadd.d
					switch (((inst >> 25) & 0b11) /* inst[26:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fnmadd_s; break;
						case 1: if (rvd) dec.op = riscv_op_fnmadd_d; break;
					}
					break;
				case 20:
					// fadd.s fsub.s fmul.s fdiv.s fsgnj.s fsgnjn.s fsgnjx.s fmin.s fmax.s fsqrt.s fle.s flt.s ...
					switch (((inst >> 25) & 0b1111111) /* inst[31:25] */) {
						case 0: if (rvf) dec.op = riscv_op_fadd_s; break;
						case 1: if (rvd) dec.op = riscv_op_fadd_d; break;
						case 4: if (rvf) dec.op = riscv_op_fsub_s; break;
						case 5: if (rvd) dec.op = riscv_op_fsub_d; break;
						case 8: if (rvf) dec.op = riscv_op_fmul_s; break;
						case 9: if (rvd) dec.op = riscv_op_fmul_d; break;
						case 12: if (rvf) dec.op = riscv_op_fdiv_s; break;
						case 13: if (rvd) dec.op = riscv_op_fdiv_d; break;
						case 16:
							// fsgnj.s fsgnjn.s fsgnjx.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fsgnj_s; break;
								case 1: if (rvf) dec.op = riscv_op_fsgnjn_s; break;
								case 2: if (rvf) dec.op = riscv_op_fsgnjx_s; break;
							}
							break;
						case 17:
							// fsgnj.d fsgnjn.d fsgnjx.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fsgnj_d; break;
								case 1: if (rvd) dec.op = riscv_op_fsgnjn_d; break;
								case 2: if (rvd) dec.op = riscv_op_fsgnjx_d; break;
							}
							break;
						case 20:
							// fmin.s fmax.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fmin_s; break;
								case 1: if (rvf) dec.op = riscv_op_fmax_s; break;
							}
							break;
						case 21:
							// fmin.d fmax.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fmin_d; break;
								case 1: if (rvd) dec.op = riscv_op_fmax_d; break;
							}
							break;
						case 32:
							// fcvt.s.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 1: if (rvd) dec.op = riscv_op_fcvt_s_d; break;
							}
							break;
						case 33:
							// fcvt.d.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fcvt_d_s; break;
							}
							break;
						case 44:
							// fsqrt.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvf) dec.op = riscv_op_fsqrt_s; break;
							}
							break;
						case 45:
							// fsqrt.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fsqrt_d; break;
							}
							break;
						case 80:
							// fle.s flt.s feq.s
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fle_s; break;
								case 1: if (rvf) dec.op = riscv_op_flt_s; break;
								case 2: if (rvf) dec.op = riscv_op_feq_s; break;
							}
							break;
						case 81:
							// fle.d flt.d feq.d
							switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
								case 0: if (rvd) dec.op = riscv_op_fle_d; break;
								case 1: if (rvd) dec.op = riscv_op_flt_d; break;
								case 2: if (rvd) dec.op = riscv_op_feq_d; break;
							}
							break;
						case 96:
							// fcvt.w.s fcvt.wu.s fcvt.l.s fcvt.lu.s
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvf) dec.op = riscv_op_fcvt_w_s; break;
								case 1: if (rvf) dec.op = riscv_op_fcvt_wu_s; break;
								case 2: if (rvf && rv64) dec.op = riscv_op_fcvt_l_s; break;
								case 3: if (rvf && rv64) dec.op = riscv_op_fcvt_lu_s; break;
							}
							break;
						case 97:
							// fcvt.w.d fcvt.wu.d fcvt.l.d fcvt.lu.d
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fcvt_w_d; break;
								case 1: if (rvd) dec.op = riscv_op_fcvt_wu_d; break;
								case 2: if (rvd && rv64) dec.op = riscv_op_fcvt_l_d; break;
								case 3: if (rvd && rv64) dec.op = riscv_op_fcvt_lu_d; break;
							}
							break;
						case 104:
							// fcvt.s.w fcvt.s.wu fcvt.s.l fcvt.s.lu
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvf) dec.op = riscv_op_fcvt_s_w; break;
								case 1: if (rvf) dec.op = riscv_op_fcvt_s_wu; break;
								case 2: if (rvf && rv64) dec.op = riscv_op_fcvt_s_l; break;
								case 3: if (rvf && rv64) dec.op = riscv_op_fcvt_s_lu; break;
							}
							break;
						case 105:
							// fcvt.d.w fcvt.d.wu fcvt.d.l fcvt.d.lu
							switch (((inst >> 20) & 0b11111) /* inst[24:20] */) {
								case 0: if (rvd) dec.op = riscv_op_fcvt_d_w; break;
								case 1: if (rvd) dec.op = riscv_op_fcvt_d_wu; break;
								case 2: if (rvd && rv64) dec.op = riscv_op_fcvt_d_l; break;
								case 3: if (rvd && rv64) dec.op = riscv_op_fcvt_d_lu; break;
							}
							break;
						case 112:
							// fmv.x.s fclass.s
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fmv_x_s; break;
								case 1: if (rvf) dec.op = riscv_op_fclass_s; break;
							}
							break;
						case 113:
							// fclass.d fmv.x.d
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvd && rv64) dec.op = riscv_op_fmv_x_d; break;
								case 1: if (rvd) dec.op = riscv_op_fclass_d; break;
							}
							break;
						case 120:
							// fmv.s.x
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvf) dec.op = riscv_op_fmv_s_x; break;
							}
							break;
						case 121:
							// fmv.d.x
							switch (((inst >> 17) & 0b11111000) | ((inst >> 12) & 0b00000111) /* inst[24:20|14:12] */) {
								case 0: if (rvd && rv64) dec.op = riscv_op_fmv_d_x; break;
							}
							break;
					}
					break;
				case 24:
					// beq bne blt bge bltu bgeu
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_beq; break;
						case 1: if (rvi) dec.op = riscv_op_bne; break;
						case 4: if (rvi) dec.op = riscv_op_blt; break;
						case 5: if (rvi) dec.op = riscv_op_bge; break;
						case 6: if (rvi) dec.op = riscv_op_bltu; break;
						case 7: if (rvi) dec.op = riscv_op_bgeu; break;
					}
					break;
				case 25:
					// jalr
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0: if (rvi) dec.op = riscv_op_jalr; break;
					}
					break;
				case 27: if (rvi) dec.op = riscv_op_jal; break;
				case 28:
					// scall sbreak sret sfence.vm wfi mrth mrts hrts rdcycle rdtime rdinstret rdcycleh ...
					switch (((inst >> 12) & 0b111) /* inst[14:12] */) {
						case 0:
							// scall sbreak sret sfence.vm wfi mrth mrts hrts
							switch (((inst >> 15) & 0b11111111111100000) | ((inst >> 7) & 0b00000000000011111) /* inst[31:20|11:7] */) {
								case 0:
									// scall
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_scall; break;
									}
									break;
								case 32:
									// sbreak
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_sbreak; break;
									}
									break;
								case 8192:
									// sret
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_sret; break;
									}
									break;
								case 8224: if (rvs) dec.op = riscv_op_sfence_vm; break;
								case 8256:
									// wfi
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_wfi; break;
									}
									break;
								case 16544:
									// hrts
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_hrts; break;
									}
									break;
								case 24736:
									// mrts
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_mrts; break;
									}
									break;
								case 24768:
									// mrth
									switch (((inst >> 15) & 0b11111) /* inst[19:15] */) {
										case 0: if (rvs) dec.op = riscv_op_mrth; break;
									}
									break;
							}
							break;
						case 1:
							// csrrw fscsr fsrm fsflags
							switch (((inst >> 20) & 0b111111111111) /* inst[31:20] */) {
								case 1: if (rvf) dec.op = riscv_op_fsflags; break;
								case 2: if (rvf) dec.op = riscv_op_fsrm; break;
								case 3: if (rvf) dec.op = riscv_op_fscsr; break;
								default: if (rvs) dec.op = riscv_op_csrrw; break;
							}
							break;
						case 2:
							// rdcycle rdtime rdinstret rdcycleh rdtimeh rdinstreth csrrs frcsr frrm frflags
							switch (((inst >> 15) & 0b11111111111111111) /* inst[31:15] */) {
								case 32: if (rvf) dec.op = riscv_op_frflags; break;
								case 64: if (rvf) dec.op = riscv_op_frrm; break;
								case 96: if (rvf) dec.op = riscv_op_frcsr; break;
								case 98304: if (rvs) dec.op = riscv_op_rdcycle; break;
								case 98336: if (rvs) dec.op = riscv_op_rdtime; break;
								case 98368: if (rvs) dec.op = riscv_op_rdinstret; break;
								case 102400: if (rvs && rv32) dec.op = riscv_op_rdcycleh; break;
								case 102432: if (rvs && rv32) dec.op = riscv_op_rdtimeh; break;
								case 102464: if (rvs && rv32) dec.op = riscv_op_rdinstreth; break;
								default: if (rvs) dec.op = riscv_op_csrrs; break;
							}
							break;
						case 3: if (rvs) dec.op = riscv_op_csrrc; break;
						case 5:
							// csrrwi fsrmi fsflagsi
							switch (((inst >> 20) & 0b111111111111) /* inst[31:20] */) {
								case 1: if (rvf) dec.op = riscv_op_fsflagsi; break;
								case 2: if (rvf) dec.op = riscv_op_fsrmi; break;
								default: if (rvs) dec.op = riscv_op_csrrwi; break;
							}
							break;
						case 6: if (rvs) dec.op = riscv_op_csrrsi; break;
						case 7: if (rvs) dec.op = riscv_op_csrrci; break;
					}
					break;
			}
			break;
	}
}

/* Decode Instruction Type */

inline void riscv_decode_type(riscv_decode &dec, riscv_lu inst)
{
	dec.codec = riscv_instruction_codec[dec.op];
	switch (dec.codec) {
		case riscv_codec_none:          riscv_decode_none(dec, inst);            break;
		case riscv_codec_cb:            riscv_decode_cb(dec, inst);              break;
		case riscv_codec_cb_sh5:        riscv_decode_cb_sh5(dec, inst);          break;
		case riscv_codec_ci:            riscv_decode_ci(dec, inst);              break;
		case riscv_codec_ci_sh5:        riscv_decode_ci_sh5(dec, inst);          break;
		case riscv_codec_ci_16sp:       riscv_decode_ci_16sp(dec, inst);         break;
		case riscv_codec_ci_lwsp:       riscv_decode_ci_lwsp(dec, inst);         break;
		case riscv_codec_ci_ldsp:       riscv_decode_ci_ldsp(dec, inst);         break;
		case riscv_codec_ci_li:         riscv_decode_ci_li(dec, inst);           break;
		case riscv_codec_ci_lui:        riscv_decode_ci_lui(dec, inst);          break;
		case riscv_codec_ci_nop:        riscv_decode_ci_nop(dec, inst);          break;
		case riscv_codec_ciw_4spn:      riscv_decode_ciw_4spn(dec, inst);        break;
		case riscv_codec_cj:            riscv_decode_cj(dec, inst);              break;
		case riscv_codec_cl_lw:         riscv_decode_cl_lw(dec, inst);           break;
		case riscv_codec_cl_ld:         riscv_decode_cl_ld(dec, inst);           break;
		case riscv_codec_cr:            riscv_decode_cr(dec, inst);              break;
		case riscv_codec_cr_mv:         riscv_decode_cr_mv(dec, inst);           break;
		case riscv_codec_cr_jalr:       riscv_decode_cr_jalr(dec, inst);         break;
		case riscv_codec_cr_jr:         riscv_decode_cr_jr(dec, inst);           break;
		case riscv_codec_cs:            riscv_decode_cs(dec, inst);              break;
		case riscv_codec_cs_sw:         riscv_decode_cs_sw(dec, inst);           break;
		case riscv_codec_cs_sd:         riscv_decode_cs_sd(dec, inst);           break;
		case riscv_codec_css_swsp:      riscv_decode_css_swsp(dec, inst);        break;
		case riscv_codec_css_sdsp:      riscv_decode_css_sdsp(dec, inst);        break;
		case riscv_codec_i:             riscv_decode_i(dec, inst);               break;
		case riscv_codec_i_sh5:         riscv_decode_i_sh5(dec, inst);           break;
		case riscv_codec_i_sh6:         riscv_decode_i_sh6(dec, inst);           break;
		case riscv_codec_r:             riscv_decode_r(dec, inst);               break;
		case riscv_codec_r_m:           riscv_decode_r_m(dec, inst);             break;
		case riscv_codec_r_4:           riscv_decode_r_4(dec, inst);             break;
		case riscv_codec_r_a:           riscv_decode_r_a(dec, inst);             break;
		case riscv_codec_r_l:           riscv_decode_r_l(dec, inst);             break;
		case riscv_codec_s:             riscv_decode_s(dec, inst);               break;
		case riscv_codec_sb:            riscv_decode_sb(dec, inst);              break;
		case riscv_codec_u:             riscv_decode_u(dec, inst);               break;
		case riscv_codec_uj:            riscv_decode_uj(dec, inst);              break;
	};
}

/* Encode Instruction */

inline riscv_lu riscv_encode(riscv_decode &dec)
{
	dec.codec = riscv_instruction_codec[dec.op];
	riscv_lu inst = riscv_instruction_match[dec.op];
	switch (dec.codec) {
		case riscv_codec_none:          return inst |= riscv_encode_none(dec);            break;
		case riscv_codec_ci_nop:        return inst |= riscv_encode_ci_nop(dec);          break;
		case riscv_codec_cb:            return inst |= riscv_encode_cb(dec);              break;
		case riscv_codec_cb_sh5:        return inst |= riscv_encode_cb_sh5(dec);          break;
		case riscv_codec_ci:            return inst |= riscv_encode_ci(dec);              break;
		case riscv_codec_ci_sh5:        return inst |= riscv_encode_ci_sh5(dec);          break;
		case riscv_codec_ci_16sp:       return inst |= riscv_encode_ci_16sp(dec);         break;
		case riscv_codec_ci_ldsp:       return inst |= riscv_encode_ci_ldsp(dec);         break;
		case riscv_codec_ci_lwsp:       return inst |= riscv_encode_ci_lwsp(dec);         break;
		case riscv_codec_ci_li:         return inst |= riscv_encode_ci_li(dec);           break;
		case riscv_codec_ci_lui:        return inst |= riscv_encode_ci_lui(dec);          break;
		case riscv_codec_ciw_4spn:      return inst |= riscv_encode_ciw_4spn(dec);        break;
		case riscv_codec_cj:            return inst |= riscv_encode_cj(dec);              break;
		case riscv_codec_cl_ld:         return inst |= riscv_encode_cl_ld(dec);           break;
		case riscv_codec_cl_lw:         return inst |= riscv_encode_cl_lw(dec);           break;
		case riscv_codec_cr:            return inst |= riscv_encode_cr(dec);              break;
		case riscv_codec_cr_mv:         return inst |= riscv_encode_cr_mv(dec);           break;
		case riscv_codec_cr_jalr:       return inst |= riscv_encode_cr_jalr(dec);         break;
		case riscv_codec_cr_jr:         return inst |= riscv_encode_cr_jr(dec);           break;
		case riscv_codec_cs:            return inst |= riscv_encode_cs(dec);              break;
		case riscv_codec_cs_sd:         return inst |= riscv_encode_cs_sd(dec);           break;
		case riscv_codec_cs_sw:         return inst |= riscv_encode_cs_sw(dec);           break;
		case riscv_codec_css_sdsp:      return inst |= riscv_encode_css_sdsp(dec);        break;
		case riscv_codec_css_swsp:      return inst |= riscv_encode_css_swsp(dec);        break;
		case riscv_codec_i:             return inst |= riscv_encode_i(dec);               break;
		case riscv_codec_i_sh5:         return inst |= riscv_encode_i_sh5(dec);           break;
		case riscv_codec_i_sh6:         return inst |= riscv_encode_i_sh6(dec);           break;
		case riscv_codec_r:             return inst |= riscv_encode_r(dec);               break;
		case riscv_codec_r_m:           return inst |= riscv_encode_r_m(dec);             break;
		case riscv_codec_r_4:           return inst |= riscv_encode_r_4(dec);             break;
		case riscv_codec_r_a:           return inst |= riscv_encode_r_a(dec);             break;
		case riscv_codec_r_l:           return inst |= riscv_encode_r_l(dec);             break;
		case riscv_codec_s:             return inst |= riscv_encode_s(dec);               break;
		case riscv_codec_sb:            return inst |= riscv_encode_sb(dec);              break;
		case riscv_codec_u:             return inst |= riscv_encode_u(dec);               break;
		case riscv_codec_uj:            return inst |= riscv_encode_uj(dec);              break;
	};
	return inst;
}

/* Instruction length */

inline size_t riscv_get_instruction_length(riscv_lu inst)
{
	// instruction length coding

	//      aa - 16 bit aa != 11
	//   bbb11 - 32 bit bbb != 111
	//  011111 - 48 bit
	// 0111111 - 64 bit

	// NOTE: currenttly supports maximum of 64-bit
	return (inst &      0b11) != 0b11      ? 2
		 : (inst &   0b11100) != 0b11100   ? 4
		 : (inst &  0b111111) == 0b011111  ? 6
		 : 8;
}

/* Fetch Instruction */

inline riscv_lu riscv_get_instruction(unsigned char *pc, unsigned char **next_pc = nullptr)
{
	// NOTE: currenttly supports maximum of 64-bit
	riscv_lu inst;
	if ((*(uint8_t*)pc & 0b11) != 0b11) {
		inst = htole16(*(uint16_t*)pc);
		if (next_pc) *next_pc = pc + 2;
	} else if ((*(uint8_t*)pc & 0b11100) != 0b11100) {
		inst = htole32(*(uint32_t*)pc);
		if (next_pc) *next_pc = pc + 4;
	} else if ((*(uint8_t*)pc & 0b111111) == 0b011111) {
		inst = uint64_t(htole32(*(uint32_t*)pc)) | uint64_t(htole16(*(uint16_t*)(pc + 4))) << 32;
		if (next_pc) *next_pc = pc + 6;
	} else {
		inst = htole64(*(uint64_t*)pc);
		if (next_pc) *next_pc = pc + 8;
	}
	return inst;
}

/* Decode Instruction */

template <bool rv32 = false, bool rv64 = true, bool rvi = true, bool rvm = true, bool rva = true, bool rvs = true, bool rvf = true, bool rvd = true, bool rvc = true>
riscv_ptr riscv_decode_instruction(riscv_decode &dec, riscv_ptr pc)
{
	riscv_ptr next_pc;
	memset(&dec, 0, sizeof(dec));
	dec.pc = pc;
	dec.inst = riscv_get_instruction(pc, &next_pc);
	riscv_decode_opcode<rv32,rv64,rvi,rvm,rva,rvs,rvf,rvd,rvc>(dec, dec.inst);
	riscv_decode_type(dec, dec.inst);
	return next_pc;
}

/* Decompress Instruction */

inline void riscv_decode_decompress(riscv_decode &dec)
{
    int decomp_op = riscv_instruction_decomp[dec.op];
    if (decomp_op != riscv_op_unknown) {
        dec.op = decomp_op;
        dec.codec = riscv_instruction_codec[decomp_op];
    }
}

/* Check constraint */

inline bool riscv_decode_decompress_check(riscv_decode &dec, const rvc_constraint *c)
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
			case rvc_imm_not_zero:  if (!(imm != 0)) return false; break;
			case rvc_imm_scale_2:   if (!((imm & 0b1) == 0)) return false; break;
			case rvc_imm_scale_4:   if (!((imm & 0b11) == 0)) return false; break;
			case rvc_imm_scale_8:   if (!((imm & 0b111) == 0)) return false; break;
			case rvc_rd_comp:       if (!(rd  >= 8 && rd  <= 15)) return false; break;
			case rvc_rs1_comp:      if (!(rs1 >= 8 && rs1 <= 15)) return false; break;
			case rvc_rs2_comp:      if (!(rs2 >= 8 && rs2 <= 15)) return false; break;
			case rvc_rd_eq_rs1:     if (!(rd == rs1)) return false; break;
			case rvc_rd_eq_ra:      if (!(rd == 1)) return false; break;
			case rvc_rd_eq_sp:      if (!(rd == 2)) return false; break;
			case rvc_rd_eq_zero:    if (!(rd == 0)) return false; break;
			case rvc_rs1_eq_sp:     if (!(rs1 == 2)) return false; break;
			case rvc_rs1_eq_zero:   if (!(rs1 == 0)) return false; break;
			case rvc_rs2_eq_zero:   if (!(rs2 == 0)) return false; break;
			case rvc_rd_not_sp:     if (!(rd != 2)) return false; break;
			case rvc_rd_not_zero:   if (!(rd != 0)) return false; break;
			case rvc_rs2_not_zero:  if (!(rs2 != 0)) return false; break;
			case rvc_end:           break;
		}
		c++;
	}
	return true;
}

/* Compress Instruction */

inline bool riscv_decode_compress(riscv_decode &dec)
{
	const riscv_comp_data *comp_data = riscv_instruction_comp[dec.op];
	if (!comp_data) return false;
	while (comp_data->constraints) {
		if (riscv_decode_decompress_check(dec, comp_data->constraints)) {
			dec.op = comp_data->op;
			dec.codec = riscv_instruction_codec[dec.op];
			return true;
		}
		comp_data++;
	}
	return false;
}

#endif
