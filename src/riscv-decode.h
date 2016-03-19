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
	riscv_bu  sz;
	riscv_bu  type;
	riscv_bu  rd;
	riscv_bu  rs1;
	riscv_bu  rs2;
	riscv_bu  rs3;
};

/* Instruction encodings */

enum riscv_inst_type
{
	riscv_inst_type_unknown,
	riscv_inst_type_c_fld,
	riscv_inst_type_c_none,
	riscv_inst_type_cb,
	riscv_inst_type_ci,
	riscv_inst_type_ci_16sp,
	riscv_inst_type_ci_ldsp,
	riscv_inst_type_ci_lwsp,
	riscv_inst_type_ci_lui,
	riscv_inst_type_ciw_4spn,
	riscv_inst_type_cj,
	riscv_inst_type_cl_d,
	riscv_inst_type_cl_w,
	riscv_inst_type_cr,
	riscv_inst_type_cr_jalr,
	riscv_inst_type_cr_jr,
	riscv_inst_type_cs,
	riscv_inst_type_cs_d,
	riscv_inst_type_cs_w,
	riscv_inst_type_css_sdsp,
	riscv_inst_type_css_swsp,
	riscv_inst_type_i,
	riscv_inst_type_i_csr,
	riscv_inst_type_i_csri,
	riscv_inst_type_i_l,
	riscv_inst_type_i_lf,
	riscv_inst_type_i_none,
	riscv_inst_type_i_sh5,
	riscv_inst_type_i_sh6,
	riscv_inst_type_r,
	riscv_inst_type_r_3f,
	riscv_inst_type_r_4f,
	riscv_inst_type_r_ff,
	riscv_inst_type_r_fr,
	riscv_inst_type_r_rf,
	riscv_inst_type_r_rff,
	riscv_inst_type_s,
	riscv_inst_type_sb,
	riscv_inst_type_sf,
	riscv_inst_type_u,
	riscv_inst_type_uj
};

/* Decode funct */

inline riscv_bu riscv_decode_c_funct3(riscv_wu inst) { return (inst >> 13) & 0b111; }
inline riscv_bu riscv_decode_c_funct4(riscv_wu inst) { return (inst >> 12) & 0b1111; }
inline riscv_bu riscv_decode_funct3(riscv_wu inst) { return (inst >> 12) & 0b111; }
inline riscv_bu riscv_decode_funct7(riscv_wu inst) { return (inst >> 25) & 0b1111111; }

/* Decode immediate */

typedef imm_t<6,  S<12,12, B<5>>,           S<6,2,  B<4,0>>>                IMM_CI;
typedef imm_t<18, S<12,12, B<17>>,          S<6,2,  B<16,12>>>              IMM_CI_lui;
typedef imm_t<8,  S<12,12, B<5>>,           S<6,2,  B<4,2>,B<7,6>>>         IMM_CI_lwsp;
typedef imm_t<9,  S<12,12, B<5>>,           S<6,2,  B<4,3>,B<8,6>>>         IMM_CI_ldsp;
typedef imm_t<10, S<12,12, B<9>>,           S<6,2,  B<4>,B<6>,B<8,7>,B<5>>> IMM_CI_addi16sp;
typedef imm_t<8,  S<12,7,  B<5,2>,B<7,6>>>                                  IMM_CSS_swsp;
typedef imm_t<9,  S<12,7,  B<5,3>,B<8,6>>>                                  IMM_CSS_sdsp;
typedef imm_t<10, S<12,7,  B<5,4>,B<9,6>>>                                  IMM_CSS_sqsp;
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

/* Decode CR */
inline void riscv_decode_cr(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cr;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.rs2 = (inst >> 2) & 0b11111;
}

/* Decode CR jalr */
inline void riscv_decode_cr_jalr(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cr_jalr;
	dec.rd = riscv_ireg_ra;
	dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = IMM_CI_lui::decode(inst);
}

/* Decode CR jr */
inline void riscv_decode_cr_jr(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cr_jr;
	dec.rd = riscv_ireg_zero;
	dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = IMM_CI_lui::decode(inst);
}

/* Decode CI */
inline void riscv_decode_ci(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = IMM_CI::decode(inst);
}

/* Decode CI lui */
inline void riscv_decode_ci_lui(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_lui;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = IMM_CI_lui::decode(inst);
}

/* Decode CI lwsp */
inline void riscv_decode_ci_lwsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_lwsp;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CI_lwsp::decode(inst);
}

/* Decode CI ldsp */
inline void riscv_decode_ci_ldsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_ldsp;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CI_ldsp::decode(inst);
}

/* Decode CI addi16sp */
inline void riscv_decode_ci_addi16sp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_16sp;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CI_addi16sp::decode(inst);
}

/* Decode CSS swsp */
inline void riscv_decode_css_swsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_css_swsp;
	dec.rd = riscv_ireg_sp;
	dec.rs2 = (inst >> 2) & 0b11111;
	dec.imm = IMM_CSS_swsp::decode(inst);
}

/* Decode CSS sdsp */
inline void riscv_decode_css_sdsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_css_sdsp;
	dec.rd = riscv_ireg_sp;
	dec.rs2 = (inst >> 2) & 0b11111;
	dec.imm = IMM_CSS_sdsp::decode(inst);
}

/* Decode CIW addi4spn */
inline void riscv_decode_ciw_addi4spn(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ciw_4spn;
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = riscv_ireg_sp;
	dec.imm = IMM_CSS_sqsp::decode(inst);
}

/* Decode CL lw */
inline void riscv_decode_cl_lw(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cl_w;
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CW::decode(inst);
}

/* Decode CL ld */
inline void riscv_decode_cl_ld(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cl_d;
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CD::decode(inst);
}

/* Decode CS f */
inline void riscv_decode_cs(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cs;
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rd = dec.rs1 = ((inst >> 7) & 0b111) + 8;
}

/* Decode CS sw */
inline void riscv_decode_cs_sw(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cs_w;
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CW::decode(inst);
}

/* Decode CS sd */
inline void riscv_decode_cs_sd(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cs_d;
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CD::decode(inst);
}

/* Decode CB */
inline void riscv_decode_cb(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cb;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = IMM_CB::decode(inst);
}

/* Decode CJ - imm */
inline void riscv_decode_cj(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cj;
	dec.imm = IMM_CJ::decode(inst);
}

/* Decode R Register - rd, rs1, rs2 */
inline void riscv_decode_r(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_r;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
}

/* Decode R Register - rd, rs1, rs2, rs3 */
inline void riscv_decode_r_4f(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_r_4f;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.rs3 = (inst >> 27) & 0b11111;
}

/* Decode I */
inline void riscv_decode_i(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_i;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = IMM_I::decode(inst);
}

/* Decode I None */
inline void riscv_decode_i_none(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_i_none;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = IMM_I::decode(inst);
}

/* Decode I Load */
inline void riscv_decode_i_l(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_i_l;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = IMM_I::decode(inst);
}

/* Decode S Store */
inline void riscv_decode_s(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_s;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.imm = IMM_S::decode(inst);
}

/* Decode SB Branch */
inline void riscv_decode_sb(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_sb;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.imm = IMM_SB::decode(inst);
}

/* Decode U */
inline void riscv_decode_u(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_u;
	dec.rd = (inst >> 7) & 0b11111;
	dec.imm = IMM_U::decode(inst);
}

/* Decode UJ */
inline void riscv_decode_uj(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_uj;
	dec.rd = (inst >> 7) & 0b11111;
	dec.imm = IMM_UJ::decode(inst);
}

#endif
