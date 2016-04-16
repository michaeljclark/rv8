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
	riscv_hu  type;
	riscv_bu  rd;
	riscv_bu  rs1;
	riscv_bu  rs2;
	riscv_bu  rs3;
};

/* Decode immediate */

typedef imm_t<6,  S<12,12, B<5>>,           S<6,2,  B<4,0>>>                IMM_CI;
typedef imm_t<18, S<12,12, B<17>>,          S<6,2,  B<16,12>>>              IMM_CI_lui;
typedef imm_t<8,  S<12,12, B<5>>,           S<6,2,  B<4,2>,B<7,6>>>         IMM_CI_lwsp;
typedef imm_t<9,  S<12,12, B<5>>,           S<6,2,  B<4,3>,B<8,6>>>         IMM_CI_ldsp;
typedef imm_t<10, S<12,12, B<9>>,           S<6,2,  B<4>,B<6>,B<8,7>,B<5>>> IMM_CI_16sp;
typedef imm_t<8,  S<12,7,  B<5,2>,B<7,6>>>                                  IMM_CSS_swsp;
typedef imm_t<9,  S<12,7,  B<5,3>,B<8,6>>>                                  IMM_CSS_sdsp;
typedef imm_t<10, S<12,7,  B<5,4>,B<9,6>>>                                  IMM_CSS_sqsp;
typedef imm_t<10, S<12,5,  B<5,4>,B<9,6>,B<2>,B<3>>>                        IMM_CSS_4spn;
typedef imm_t<7,  S<12,10, B<5,3>>,         S<6,5,  B<2>,B<6>>>             IMM_CW;
typedef imm_t<8,  S<12,10, B<5,3>>,         S<6,5,  B<7,6>>>                IMM_CD;
typedef imm_t<9,  S<12,10, B<5,4>,B<8>>,    S<6,5,  B<7,6>>>                IMM_CQ;
typedef imm_t<9,  S<12,10, B<8>,B<4,3>>,    S<6,2,  B<7,6>,B<2,1>,B<5>>>    IMM_CB;
typedef imm_t<12, S<12,2,  B<11>,B<4>,B<9,8>,B<10>,B<6>,B<7>,B<3,1>,B<5>>>  IMM_CJ;
typedef imm_t<12, S<31,20, B<11,0>>>                                        IMM_I;
typedef imm_t<12, S<31,25, B<11,5>>,        S<11,7, B<4,0>>>                IMM_S;
typedef imm_t<13, S<31,25, B<12>,B<10,5>>,  S<11,7, B<4,1>,B<11>>>          IMM_SB;
typedef imm_t<32, S<31,12, B<31,12>>>                                       IMM_U;
typedef imm_t<21, S<31,12, B<20>,B<10,1>,B<11>,B<19,12>>>                   IMM_UJ;


/* Decode none */
inline void riscv_decode_none(riscv_decode &dec, riscv_lu inst) {}

/* Decode CR */
inline void riscv_decode_cr(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.rs2 = (inst >> 2) & 0b11111;
}

/* Decode CR jalr */
inline void riscv_decode_cr_jalr(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_ireg_ra;
	dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = IMM_CI_lui::decode(inst);
}

/* Decode CR jr */
inline void riscv_decode_cr_jr(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_ireg_zero;
	dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = IMM_CI_lui::decode(inst);
}

/* Decode CI */
inline void riscv_decode_ci(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = IMM_CI::decode(inst);
}

/* Decode CI li */
inline void riscv_decode_ci_li(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = riscv_ireg_zero;
	dec.imm = IMM_CI::decode(inst);
}

/* Decode CI lui */
inline void riscv_decode_ci_lui(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = riscv_ireg_zero;
	dec.imm = IMM_CI_lui::decode(inst);
}

/* Decode CI lwsp */
inline void riscv_decode_ci_lwsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CI_lwsp::decode(inst);
}

/* Decode CI ldsp */
inline void riscv_decode_ci_ldsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CI_ldsp::decode(inst);
}

/* Decode CI 16sp */
inline void riscv_decode_ci_16sp(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = riscv_ireg_sp;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CI_16sp::decode(inst);
}

/* Decode CSS swsp */
inline void riscv_decode_css_swsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = (inst >> 2) & 0b11111;
	dec.imm = IMM_CSS_swsp::decode(inst);
}

/* Decode CSS sdsp */
inline void riscv_decode_css_sdsp(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = riscv_ireg_sp;
	dec.rs2 = (inst >> 2) & 0b11111;
	dec.imm = IMM_CSS_sdsp::decode(inst);
}

/* Decode CIW 4spn */
inline void riscv_decode_ciw_4spn(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CSS_4spn::decode(inst);
}

/* Decode CL lw */
inline void riscv_decode_cl_lw(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CW::decode(inst);
}

/* Decode CL ld */
inline void riscv_decode_cl_ld(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CD::decode(inst);
}

/* Decode CS f */
inline void riscv_decode_cs(riscv_decode &dec, riscv_lu inst)
{
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rd = dec.rs1 = ((inst >> 7) & 0b111) + 8;
}

/* Decode CS sd */
inline void riscv_decode_cs_sd(riscv_decode &dec, riscv_lu inst)
{
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CD::decode(inst);
}

/* Decode CS sw */
inline void riscv_decode_cs_sw(riscv_decode &dec, riscv_lu inst)
{
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CW::decode(inst);
}

/* Decode CB */
inline void riscv_decode_cb(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CB::decode(inst);
}

/* Decode CJ - imm */
inline void riscv_decode_cj(riscv_decode &dec, riscv_lu inst)
{
	dec.imm = IMM_CJ::decode(inst);
}

/* Decode R */
inline void riscv_decode_r(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
}

/* Decode R 4f */
inline void riscv_decode_r_4f(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.rs3 = (inst >> 27) & 0b11111;
}

/* Decode I */
inline void riscv_decode_i(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = IMM_I::decode(inst);
}

/* Decode I sh5 */
inline void riscv_decode_i_sh5(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = (inst >> 20) & 0b11111;
}

/* Decode I sh6 */
inline void riscv_decode_i_sh6(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = (inst >> 20) & 0b111111;
}

/* Decode S Store */
inline void riscv_decode_s(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.imm = IMM_S::decode(inst);
}

/* Decode SB Branch */
inline void riscv_decode_sb(riscv_decode &dec, riscv_lu inst)
{
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.imm = IMM_SB::decode(inst);
}

/* Decode U */
inline void riscv_decode_u(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.imm = IMM_U::decode(inst);
}

/* Decode UJ */
inline void riscv_decode_uj(riscv_decode &dec, riscv_lu inst)
{
	dec.rd = (inst >> 7) & 0b11111;
	dec.imm = IMM_UJ::decode(inst);
}

#endif
