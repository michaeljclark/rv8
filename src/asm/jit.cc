//
//  jit.cc
//
//  DANGER - This is machine generated code
//

#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <map>
#include <vector>
#include <memory>
#include <string>

#include "types.h"
#include "host-endian.h"
#include "bits.h"
#include "meta.h"
#include "codec.h"
#include "assembler.h"
#include "jit.h"

using namespace riscv;

inst_t riscv::emit_lui(ireg5 rd, simm32 imm20)
{
	decode dec;
	if (!(rd.valid() && imm20.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lui;
	dec.rd = rd;
	dec.imm = imm20;
	return encode_inst(dec);
}

inst_t riscv::emit_auipc(ireg5 rd, offset32 oimm20)
{
	decode dec;
	if (!(rd.valid() && oimm20.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_auipc;
	dec.rd = rd;
	dec.imm = oimm20;
	return encode_inst(dec);
}

inst_t riscv::emit_jal(ireg5 rd, offset21 jimm20)
{
	decode dec;
	if (!(rd.valid() && jimm20.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_jal;
	dec.rd = rd;
	dec.imm = jimm20;
	return encode_inst(dec);
}

inst_t riscv::emit_jalr(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_jalr;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_beq(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_beq;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_bne(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_bne;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_blt(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_blt;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_bge(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_bge;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_bltu(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_bltu;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_bgeu(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_bgeu;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_lb(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lb;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_lh(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lh;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_lw(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_lbu(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lbu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_lhu(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lhu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_sb(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sb;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_sh(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sh;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_sw(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sw;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_addi(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_addi;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_slti(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_slti;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_sltiu(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sltiu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_xori(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_xori;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_ori(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_ori;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_andi(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_andi;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_slli(ireg5 rd, ireg5 rs1, uimm7 shamt7)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt7.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_slli;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt7;
	return encode_inst(dec);
}

inst_t riscv::emit_srli(ireg5 rd, ireg5 rs1, uimm7 shamt7)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt7.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srli;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt7;
	return encode_inst(dec);
}

inst_t riscv::emit_srai(ireg5 rd, ireg5 rs1, uimm7 shamt7)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt7.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srai;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt7;
	return encode_inst(dec);
}

inst_t riscv::emit_add(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_add;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_sub(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sub;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_sll(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sll;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_slt(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_slt;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_sltu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sltu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_xor(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_xor;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_srl(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srl;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_sra(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sra;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_or(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_or;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_and(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_and;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fence(arg4 pred, arg4 succ)
{
	decode dec;
	if (!(pred.valid() && succ.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fence;
	dec.pred = pred;
	dec.succ = succ;
	return encode_inst(dec);
}

inst_t riscv::emit_fence_i()
{
	decode dec;
	dec.op = rv_op_fence_i;
	return encode_inst(dec);
}

inst_t riscv::emit_lwu(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lwu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_ld(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_ld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_sd(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_addiw(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_addiw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_slliw(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_slliw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

inst_t riscv::emit_srliw(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srliw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

inst_t riscv::emit_sraiw(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sraiw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

inst_t riscv::emit_addw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_addw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_subw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_subw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_sllw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sllw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_srlw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srlw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_sraw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sraw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_ldu(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_ldu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_lq(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lq;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_sq(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sq;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_addid(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_addid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

inst_t riscv::emit_sllid(ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sllid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt6;
	return encode_inst(dec);
}

inst_t riscv::emit_srlid(ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srlid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt6;
	return encode_inst(dec);
}

inst_t riscv::emit_sraid(ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sraid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt6;
	return encode_inst(dec);
}

inst_t riscv::emit_addd(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_addd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_subd(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_subd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_slld(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_slld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_srld(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_srad(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_srad;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_mul(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_mul;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_mulh(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_mulh;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_mulhsu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_mulhsu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_mulhu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_mulhu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_div(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_div;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_divu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_divu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_rem(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_rem;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_remu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_remu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_mulw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_mulw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_divw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_divw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_divuw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_divuw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_remw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_remw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_remuw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_remuw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_muld(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_muld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_divd(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_divd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_divud(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_divud;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_remd(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_remd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_remud(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_remud;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_lr_w(ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lr_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_sc_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sc_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoswap_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoswap_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoadd_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoadd_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoxor_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoxor_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoor_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoor_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoand_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoand_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomin_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomin_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomax_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomax_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amominu_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amominu_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomaxu_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomaxu_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_lr_d(ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lr_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_sc_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sc_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoswap_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoswap_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoadd_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoadd_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoxor_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoxor_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoor_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoor_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoand_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoand_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomin_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomin_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomax_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomax_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amominu_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amominu_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomaxu_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomaxu_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_lr_q(ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_lr_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_sc_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sc_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoswap_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoswap_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoadd_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoadd_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoxor_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoxor_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoor_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoor_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amoand_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amoand_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomin_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomin_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomax_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomax_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amominu_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amominu_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_amomaxu_q(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_amomaxu_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	return encode_inst(dec);
}

inst_t riscv::emit_ecall()
{
	decode dec;
	dec.op = rv_op_ecall;
	return encode_inst(dec);
}

inst_t riscv::emit_ebreak()
{
	decode dec;
	dec.op = rv_op_ebreak;
	return encode_inst(dec);
}

inst_t riscv::emit_uret()
{
	decode dec;
	dec.op = rv_op_uret;
	return encode_inst(dec);
}

inst_t riscv::emit_sret()
{
	decode dec;
	dec.op = rv_op_sret;
	return encode_inst(dec);
}

inst_t riscv::emit_hret()
{
	decode dec;
	dec.op = rv_op_hret;
	return encode_inst(dec);
}

inst_t riscv::emit_mret()
{
	decode dec;
	dec.op = rv_op_mret;
	return encode_inst(dec);
}

inst_t riscv::emit_dret()
{
	decode dec;
	dec.op = rv_op_dret;
	return encode_inst(dec);
}

inst_t riscv::emit_sfence_vm(ireg5 rs1)
{
	decode dec;
	if (!(rs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sfence_vm;
	dec.rs1 = rs1;
	return encode_inst(dec);
}

inst_t riscv::emit_sfence_vma(ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_sfence_vma;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	return encode_inst(dec);
}

inst_t riscv::emit_wfi()
{
	decode dec;
	dec.op = rv_op_wfi;
	return encode_inst(dec);
}

inst_t riscv::emit_csrrw(ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_csrrw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = csr12;
	return encode_inst(dec);
}

inst_t riscv::emit_csrrs(ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_csrrs;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = csr12;
	return encode_inst(dec);
}

inst_t riscv::emit_csrrc(ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_csrrc;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = csr12;
	return encode_inst(dec);
}

inst_t riscv::emit_csrrwi(ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_csrrwi;
	dec.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	return encode_inst(dec);
}

inst_t riscv::emit_csrrsi(ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_csrrsi;
	dec.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	return encode_inst(dec);
}

inst_t riscv::emit_csrrci(ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_csrrci;
	dec.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	return encode_inst(dec);
}

inst_t riscv::emit_flw(freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_flw;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_fsw(ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsw;
	dec.rs1 = rs1;
	dec.rs2 = frs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_fmadd_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmadd_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmsub_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmsub_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fnmsub_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fnmsub_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fnmadd_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fnmadd_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fadd_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fadd_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsub_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsub_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmul_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmul_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fdiv_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fdiv_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnj_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnj_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnjn_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnjn_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnjx_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnjx_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fmin_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmin_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fmax_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmax_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fsqrt_s(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsqrt_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fle_s(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fle_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_flt_s(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_flt_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_feq_s(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_feq_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_w_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_w_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_wu_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_wu_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_s_w(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_s_w;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_s_wu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_s_wu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmv_x_s(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmv_x_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	return encode_inst(dec);
}

inst_t riscv::emit_fclass_s(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fclass_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	return encode_inst(dec);
}

inst_t riscv::emit_fmv_s_x(freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmv_s_x;
	dec.rd = frd;
	dec.rs1 = rs1;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_l_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_l_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_lu_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_lu_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_s_l(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_s_l;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_s_lu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_s_lu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fld(freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fld;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_fsd(ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsd;
	dec.rs1 = rs1;
	dec.rs2 = frs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_fmadd_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmadd_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmsub_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmsub_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fnmsub_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fnmsub_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fnmadd_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fnmadd_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fadd_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fadd_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsub_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsub_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmul_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmul_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fdiv_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fdiv_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnj_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnj_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnjn_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnjn_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnjx_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnjx_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fmin_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmin_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fmax_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmax_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_s_d(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_s_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_d_s(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_d_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsqrt_d(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsqrt_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fle_d(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fle_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_flt_d(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_flt_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_feq_d(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_feq_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_w_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_w_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_wu_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_wu_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_d_w(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_d_w;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_d_wu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_d_wu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fclass_d(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fclass_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_l_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_l_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_lu_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_lu_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmv_x_d(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmv_x_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_d_l(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_d_l;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_d_lu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_d_lu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmv_d_x(freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmv_d_x;
	dec.rd = frd;
	dec.rs1 = rs1;
	return encode_inst(dec);
}

inst_t riscv::emit_flq(freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_flq;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

inst_t riscv::emit_fsq(ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsq;
	dec.rs1 = rs1;
	dec.rs2 = frs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

inst_t riscv::emit_fmadd_q(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmadd_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmsub_q(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmsub_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fnmsub_q(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fnmsub_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fnmadd_q(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fnmadd_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fadd_q(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fadd_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsub_q(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsub_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmul_q(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmul_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fdiv_q(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fdiv_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnj_q(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnj_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnjn_q(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnjn_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fsgnjx_q(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsgnjx_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fmin_q(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmin_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fmax_q(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmax_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_s_q(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_s_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_q_s(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_q_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_d_q(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_d_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_q_d(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_q_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fsqrt_q(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fsqrt_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fle_q(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fle_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_flt_q(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_flt_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_feq_q(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_feq_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_w_q(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_w_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_wu_q(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_wu_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_q_w(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_q_w;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_q_wu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_q_wu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fclass_q(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fclass_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_l_q(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_l_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_lu_q(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_lu_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_q_l(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_q_l;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fcvt_q_lu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fcvt_q_lu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	return encode_inst(dec);
}

inst_t riscv::emit_fmv_x_q(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmv_x_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	return encode_inst(dec);
}

inst_t riscv::emit_fmv_q_x(freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return 0; /* illegal instruction */
	dec.op = rv_op_fmv_q_x;
	dec.rd = frd;
	dec.rs1 = rs1;
	return encode_inst(dec);
}


bool riscv::asm_lui(assembler &as, ireg5 rd, simm32 imm20)
{
	decode dec;
	if (!(rd.valid() && imm20.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lui;
	dec.rd = rd;
	dec.imm = imm20;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_auipc(assembler &as, ireg5 rd, offset32 oimm20)
{
	decode dec;
	if (!(rd.valid() && oimm20.valid())) return false; /* illegal instruction */
	dec.op = rv_op_auipc;
	dec.rd = rd;
	dec.imm = oimm20;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_jal(assembler &as, ireg5 rd, offset21 jimm20)
{
	decode dec;
	if (!(rd.valid() && jimm20.valid())) return false; /* illegal instruction */
	dec.op = rv_op_jal;
	dec.rd = rd;
	dec.imm = jimm20;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_jalr(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_jalr;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_beq(assembler &as, ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_beq;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_bne(assembler &as, ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_bne;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_blt(assembler &as, ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_blt;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_bge(assembler &as, ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_bge;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_bltu(assembler &as, ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_bltu;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_bgeu(assembler &as, ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_bgeu;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = sbimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lb(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lb;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lh(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lh;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lw(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lbu(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lbu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lhu(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lhu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sb(assembler &as, ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sb;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sh(assembler &as, ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sh;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sw(assembler &as, ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sw;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_addi(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_addi;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_slti(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_slti;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sltiu(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sltiu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_xori(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_xori;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_ori(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_ori;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_andi(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_andi;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_slli(assembler &as, ireg5 rd, ireg5 rs1, uimm7 shamt7)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt7.valid())) return false; /* illegal instruction */
	dec.op = rv_op_slli;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt7;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srli(assembler &as, ireg5 rd, ireg5 rs1, uimm7 shamt7)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt7.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srli;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt7;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srai(assembler &as, ireg5 rd, ireg5 rs1, uimm7 shamt7)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt7.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srai;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt7;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_add(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_add;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sub(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sub;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sll(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sll;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_slt(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_slt;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sltu(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sltu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_xor(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_xor;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srl(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srl;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sra(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sra;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_or(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_or;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_and(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_and;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fence(assembler &as, arg4 pred, arg4 succ)
{
	decode dec;
	if (!(pred.valid() && succ.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fence;
	dec.pred = pred;
	dec.succ = succ;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fence_i(assembler &as)
{
	decode dec;
	dec.op = rv_op_fence_i;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lwu(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lwu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_ld(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_ld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sd(assembler &as, ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_addiw(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_addiw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_slliw(assembler &as, ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return false; /* illegal instruction */
	dec.op = rv_op_slliw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt5;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srliw(assembler &as, ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srliw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt5;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sraiw(assembler &as, ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sraiw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt5;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_addw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_addw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_subw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_subw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sllw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sllw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srlw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srlw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sraw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sraw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_ldu(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_ldu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lq(assembler &as, ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lq;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sq(assembler &as, ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sq;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_addid(assembler &as, ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_addid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = imm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sllid(assembler &as, ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sllid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt6;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srlid(assembler &as, ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srlid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt6;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sraid(assembler &as, ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sraid;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = shamt6;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_addd(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_addd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_subd(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_subd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_slld(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_slld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srld(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_srad(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_srad;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_mul(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_mul;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_mulh(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_mulh;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_mulhsu(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_mulhsu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_mulhu(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_mulhu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_div(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_div;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_divu(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_divu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_rem(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_rem;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_remu(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_remu;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_mulw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_mulw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_divw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_divw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_divuw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_divuw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_remw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_remw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_remuw(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_remuw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_muld(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_muld;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_divd(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_divd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_divud(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_divud;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_remd(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_remd;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_remud(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_remud;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lr_w(assembler &as, ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lr_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sc_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sc_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoswap_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoswap_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoadd_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoadd_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoxor_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoxor_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoor_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoor_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoand_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoand_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomin_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomin_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomax_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomax_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amominu_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amominu_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomaxu_w(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomaxu_w;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lr_d(assembler &as, ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lr_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sc_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sc_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoswap_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoswap_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoadd_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoadd_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoxor_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoxor_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoor_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoor_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoand_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoand_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomin_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomin_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomax_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomax_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amominu_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amominu_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomaxu_d(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomaxu_d;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_lr_q(assembler &as, ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_lr_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sc_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sc_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoswap_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoswap_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoadd_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoadd_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoxor_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoxor_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoor_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoor_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amoand_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amoand_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomin_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomin_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomax_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomax_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amominu_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amominu_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_amomaxu_q(assembler &as, ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return false; /* illegal instruction */
	dec.op = rv_op_amomaxu_q;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	dec.aq = aq;
	dec.rl = rl;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_ecall(assembler &as)
{
	decode dec;
	dec.op = rv_op_ecall;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_ebreak(assembler &as)
{
	decode dec;
	dec.op = rv_op_ebreak;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_uret(assembler &as)
{
	decode dec;
	dec.op = rv_op_uret;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sret(assembler &as)
{
	decode dec;
	dec.op = rv_op_sret;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_hret(assembler &as)
{
	decode dec;
	dec.op = rv_op_hret;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_mret(assembler &as)
{
	decode dec;
	dec.op = rv_op_mret;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_dret(assembler &as)
{
	decode dec;
	dec.op = rv_op_dret;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sfence_vm(assembler &as, ireg5 rs1)
{
	decode dec;
	if (!(rs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sfence_vm;
	dec.rs1 = rs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_sfence_vma(assembler &as, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_sfence_vma;
	dec.rs1 = rs1;
	dec.rs2 = rs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_wfi(assembler &as)
{
	decode dec;
	dec.op = rv_op_wfi;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_csrrw(assembler &as, ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_csrrw;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = csr12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_csrrs(assembler &as, ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_csrrs;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = csr12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_csrrc(assembler &as, ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_csrrc;
	dec.rd = rd;
	dec.rs1 = rs1;
	dec.imm = csr12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_csrrwi(assembler &as, ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_csrrwi;
	dec.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_csrrsi(assembler &as, ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_csrrsi;
	dec.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_csrrci(assembler &as, ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_csrrci;
	dec.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_flw(assembler &as, freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_flw;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsw(assembler &as, ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsw;
	dec.rs1 = rs1;
	dec.rs2 = frs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmadd_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmadd_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmsub_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmsub_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fnmsub_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fnmsub_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fnmadd_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fnmadd_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fadd_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fadd_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsub_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsub_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmul_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmul_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fdiv_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fdiv_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnj_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnj_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnjn_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnjn_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnjx_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnjx_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmin_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmin_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmax_s(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmax_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsqrt_s(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsqrt_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fle_s(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fle_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_flt_s(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_flt_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_feq_s(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_feq_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_w_s(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_w_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_wu_s(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_wu_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_s_w(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_s_w;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_s_wu(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_s_wu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmv_x_s(assembler &as, ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmv_x_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fclass_s(assembler &as, ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fclass_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmv_s_x(assembler &as, freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmv_s_x;
	dec.rd = frd;
	dec.rs1 = rs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_l_s(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_l_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_lu_s(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_lu_s;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_s_l(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_s_l;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_s_lu(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_s_lu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fld(assembler &as, freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fld;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsd(assembler &as, ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsd;
	dec.rs1 = rs1;
	dec.rs2 = frs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmadd_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmadd_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmsub_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmsub_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fnmsub_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fnmsub_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fnmadd_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fnmadd_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fadd_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fadd_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsub_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsub_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmul_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmul_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fdiv_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fdiv_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnj_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnj_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnjn_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnjn_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnjx_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnjx_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmin_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmin_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmax_d(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmax_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_s_d(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_s_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_d_s(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_d_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsqrt_d(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsqrt_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fle_d(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fle_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_flt_d(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_flt_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_feq_d(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_feq_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_w_d(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_w_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_wu_d(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_wu_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_d_w(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_d_w;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_d_wu(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_d_wu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fclass_d(assembler &as, ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fclass_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_l_d(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_l_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_lu_d(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_lu_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmv_x_d(assembler &as, ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmv_x_d;
	dec.rd = rd;
	dec.rs1 = frs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_d_l(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_d_l;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_d_lu(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_d_lu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmv_d_x(assembler &as, freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmv_d_x;
	dec.rd = frd;
	dec.rs1 = rs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_flq(assembler &as, freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_flq;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.imm = oimm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsq(assembler &as, ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsq;
	dec.rs1 = rs1;
	dec.rs2 = frs2;
	dec.imm = simm12;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmadd_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmadd_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmsub_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmsub_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fnmsub_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fnmsub_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fnmadd_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fnmadd_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rs3 = frs3;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fadd_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fadd_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsub_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsub_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmul_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmul_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fdiv_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fdiv_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnj_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnj_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnjn_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnjn_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsgnjx_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsgnjx_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmin_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmin_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmax_q(assembler &as, freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmax_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_s_q(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_s_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_q_s(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_q_s;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_d_q(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_d_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_q_d(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_q_d;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fsqrt_q(assembler &as, freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fsqrt_q;
	dec.rd = frd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fle_q(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fle_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_flt_q(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_flt_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_feq_q(assembler &as, ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return false; /* illegal instruction */
	dec.op = rv_op_feq_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rs2 = frs2;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_w_q(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_w_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_wu_q(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_wu_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_q_w(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_q_w;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_q_wu(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_q_wu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fclass_q(assembler &as, ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fclass_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_l_q(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_l_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_lu_q(assembler &as, ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_lu_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_q_l(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_q_l;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fcvt_q_lu(assembler &as, freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fcvt_q_lu;
	dec.rd = frd;
	dec.rs1 = rs1;
	dec.rm = rm;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmv_x_q(assembler &as, ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmv_x_q;
	dec.rd = rd;
	dec.rs1 = frs1;
	as.add_inst(encode_inst(dec));
	return true;
}

bool riscv::asm_fmv_q_x(assembler &as, freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return false; /* illegal instruction */
	dec.op = rv_op_fmv_q_x;
	dec.rd = frd;
	dec.rs1 = rs1;
	as.add_inst(encode_inst(dec));
	return true;
}


