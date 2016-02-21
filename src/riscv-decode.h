//
//  riscv-decode.h
//

#ifndef riscv_decode_h
#define riscv_decode_h

template <typename T, unsigned B>
inline T sign_extend(const T x)
{
	struct {
		T x:B;
	} s;
	return s.x = x;
}

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
	riscv_inst_type_cr,
	riscv_inst_type_ci,
	riscv_inst_type_ci_lui,
	riscv_inst_type_ci_lwsp,
	riscv_inst_type_ci_ldsp,
	riscv_inst_type_ci_addi16sp,
	riscv_inst_type_css_swsp,
	riscv_inst_type_css_sdsp,
	riscv_inst_type_ciw_addi4spn,
	riscv_inst_type_cl_lw,
	riscv_inst_type_cl_ld,
	riscv_inst_type_cs_x,
	riscv_inst_type_cs_sw,
	riscv_inst_type_cs_sd,
	riscv_inst_type_cb,
	riscv_inst_type_cj,
	riscv_inst_type_r,
	riscv_inst_type_r_ff,
	riscv_inst_type_r_rf,
	riscv_inst_type_r_fr,
	riscv_inst_type_r_fff,
	riscv_inst_type_r_rff,
	riscv_inst_type_r_ffff,
	riscv_inst_type_i,
	// riscv_inst_type_i_fence,
	// riscv_inst_type_i_shamt,
	// riscv_inst_type_i_shamtw,
	riscv_inst_type_i_s,
	riscv_inst_type_i_l,
	riscv_inst_type_i_lf,
	riscv_inst_type_i_csr,
	riscv_inst_type_i_csri,
	riscv_inst_type_s,
	riscv_inst_type_sf,
	riscv_inst_type_sb,
	riscv_inst_type_u,
	riscv_inst_type_uj
};

/* Decode funct */

inline riscv_bu riscv_decode_c_funct3(riscv_wu inst) { return (inst >> 13) & 0b111; }
inline riscv_bu riscv_decode_c_funct4(riscv_wu inst) { return (inst >> 12) & 0b1111; }
inline riscv_bu riscv_decode_funct3(riscv_wu inst) { return (inst >> 12) & 0b111; }
inline riscv_bu riscv_decode_funct7(riscv_wu inst) { return (inst >> 25) & 0b1111111; }

/* Decode CR */
inline void riscv_decode_cr(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cr;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.rs2 = (inst >> 2) & 0b11111;
}

/* Decode CI */
inline void riscv_decode_ci(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = sign_extend<riscv_l,6>(
		((inst >> 7) & 0b100000) |
		((inst >> 2) & 0b011111)
	);
}

/* Decode CI lui */
inline void riscv_decode_ci_lui(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_lui;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = sign_extend<riscv_l,18>((
		((inst >> 7) & 0b100000) |
		((inst >> 2) & 0b011111)
	) << 12);
}

/* Decode CI lwsp */
inline void riscv_decode_ci_lwsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_lwsp;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = sign_extend<riscv_l,8>((
		((inst << 2) & 0b110000) |
		((inst >> 9) & 0b001000) |
		((inst >> 4) & 0b000111)
	) << 2);
}

/* Decode CI ldsp */
inline void riscv_decode_ci_ldsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_ldsp;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = sign_extend<riscv_l,9>((
		((inst << 1)  & 0b111000) |
		((inst >> 10) & 0b000100) |
		((inst >> 5)  & 0b000011)
	) << 3);
}

/* Decode CI addi16sp */
inline void riscv_decode_ci_addi16sp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ci_addi16sp;
	dec.rd = dec.rs1 = (inst >> 7) & 0b11111;
	dec.imm = sign_extend<riscv_l,10>((
		((inst >> 7) & 0b100000) |
		((inst << 2) & 0b010000) |
		((inst >> 1) & 0b001000) |
		((inst >> 4) & 0b000110) |
		((inst >> 2) & 0b000001)
	) << 4);
}

/* Decode CSS swsp */
inline void riscv_decode_css_swsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_css_swsp;
	dec.rs2 = (inst >> 2) & 0b11111;
	dec.imm = sign_extend<riscv_l,8>((
		((inst >> 5) & 0b111100) |
		((inst >> 11) & 0b000011)
	) << 2);
}

/* Decode CSS sdsp */
inline void riscv_decode_css_sdsp(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_css_sdsp;
	dec.rs2 = (inst >> 2) & 0b11111;
	dec.imm = sign_extend<riscv_l,9>((
		((inst >> 5) & 0b111000) |
		((inst >> 10) & 0b000111)
	) << 3);
}

/* Decode CIW addi4spn */
inline void riscv_decode_ciw_addi4spn(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_ciw_addi4spn;
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.imm = sign_extend<riscv_l,10>((
		((inst >> 1) & 0b11000000) |
		((inst >> 7) & 0b00111100) |
		((inst >> 4) & 0b00000010) |
		((inst >> 6) & 0b00000001)
	) << 2);
}

/* Decode CL lw */
inline void riscv_decode_cl_lw(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cl_lw;
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = sign_extend<riscv_l,7>((
		((inst >> 1) & 0b10000) |
		((inst >> 9) & 0b01110) |
		((inst >> 6) & 0b00001)
	) << 2);
}

/* Decode CL ld */
inline void riscv_decode_cl_ld(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cl_ld;
	dec.rd = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = sign_extend<riscv_l,8>((
		((inst >> 2) & 0b11000) |
		((inst >> 10) & 0b00111)
	) << 3);
}

/* Decode CX */
inline void riscv_decode_cs_x(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cs_x;
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.rd = ((inst >> 7) & 0b111) + 8;
	dec.imm = sign_extend<riscv_l,5>(
		((inst >> 10) & 0b11100) |
		((inst >> 5)  & 0b00011)
	);
}

/* Decode CS sw */
inline void riscv_decode_cs_sw(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cs_sw;
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = sign_extend<riscv_l,7>((
		((inst >> 1) & 0b10000) |
		((inst >> 9) & 0b01110) |
		((inst >> 6) & 0b00001)
	) << 2);
}

/* Decode CS sd */
inline void riscv_decode_cs_sd(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cs_sd;
	dec.rs2 = ((inst >> 2) & 0b111) + 8;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = sign_extend<riscv_l,8>((
		((inst >> 2) & 0b11000) |
		((inst >> 10) & 0b00111)
	) << 3);
}

/* Decode CB */
inline void riscv_decode_cb(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cb;
	dec.rs1 = ((inst >> 7) & 0b111) + 8;
	dec.imm = sign_extend<riscv_l,9>((
		((inst >> 5) & 0b10000000) |
		((inst >> 0) & 0b01100000) |
		((inst << 4) & 0b00010000) |
		((inst >> 8) & 0b00001100) |
		((inst >> 3) & 0b00000011)
	) << 1);
}

/* Decode CJ - imm */
inline void riscv_decode_cj(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_cj;
	dec.imm = sign_extend<riscv_l,12>((
		((inst >> 2) & 0b10110100000) |
		((inst << 3) & 0b01000000000) |
		((inst >> 0) & 0b00001000000) |
		((inst << 2) & 0b00000010000) |
		((inst >> 8) & 0b00000001000) |
		((inst >> 3) & 0b00000000111)
	) << 1);
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
inline void riscv_decode_r_ffff(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_r_ffff;
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
	dec.imm = sign_extend<riscv_l,12>(
		inst >> 20
	);
}

/* Decode I Special*/
inline void riscv_decode_i_s(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_i_s;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = sign_extend<riscv_l,12>(
		inst >> 20
	);
}

/* Decode I Load */
inline void riscv_decode_i_l(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_i_l;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.imm = sign_extend<riscv_l,12>(
		inst >> 20
	);
}

/* Decode S Store */
inline void riscv_decode_s(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_s;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.imm = sign_extend<riscv_l,12>(
		((inst >> 20) & 0b111111100000) |
		((inst >> 7)  & 0b000000011111)
	);
}

/* Decode SB Branch */
inline void riscv_decode_sb(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_sb;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
	dec.imm = sign_extend<riscv_l,13>((
		((inst >> 20) & 0b100000000000) |
		((inst << 3)  & 0b010000000000) |
		((inst >> 21) & 0b001111110000) |
		((inst >> 8)  & 0b000000001111)
	) << 1);
}

/* Decode U */
inline void riscv_decode_u(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_u;
	dec.rd = (inst >> 7) & 0b11111;
	dec.imm = sign_extend<riscv_l,32>((
		inst >> 12
	) << 12);
}

/* Decode UJ */
inline void riscv_decode_uj(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_uj;
	dec.rd = (inst >> 7) & 0b11111;
	dec.imm = sign_extend<riscv_l,21>((
		((inst >> 12) & 0b10000000000000000000) |
		((inst >> 1)  & 0b01111111100000000000) |
		((inst >> 10) & 0b00000000010000000000) |
		((inst >> 21) & 0b00000000001111111111)
	) << 1);
}

#endif