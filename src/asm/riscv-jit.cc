//
//  riscv-jit.cc
//
//  DANGER - This is machine generated code
//

#include <cstdint>
#include <cstdlib>
#include <cassert>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-jit.h"
#include "riscv-meta.h"
#include "riscv-codec.h"

using namespace riscv;

uint64_t riscv::emit_lui(ireg5 rd, simm32 imm20)
{
	decode dec;
	if (!(rd.valid() && imm20.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lui;
	dec.rv.r.rd = rd;
	dec.imm = imm20;
	return encode_inst(dec);
}

uint64_t riscv::emit_auipc(ireg5 rd, offset32 oimm20)
{
	decode dec;
	if (!(rd.valid() && oimm20.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_auipc;
	dec.rv.r.rd = rd;
	dec.imm = oimm20;
	return encode_inst(dec);
}

uint64_t riscv::emit_jal(ireg5 rd, offset21 jimm20)
{
	decode dec;
	if (!(rd.valid() && jimm20.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_jal;
	dec.rv.r.rd = rd;
	dec.imm = jimm20;
	return encode_inst(dec);
}

uint64_t riscv::emit_jalr(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_jalr;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_beq(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_beq;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_bne(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_bne;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_blt(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_blt;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_bge(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_bge;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_bltu(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_bltu;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_bgeu(ireg5 rs1, ireg5 rs2, offset13 sbimm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && sbimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_bgeu;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = sbimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_lb(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lb;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_lh(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lh;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_lw(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_lbu(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lbu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_lhu(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lhu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_sb(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sb;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_sh(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sh;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_sw(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sw;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_addi(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_addi;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_slti(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_slti;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_sltiu(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sltiu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_xori(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_xori;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_ori(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_ori;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_andi(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_andi;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_slli_rv32i(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_slli_rv32i;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

uint64_t riscv::emit_srli_rv32i(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_srli_rv32i;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

uint64_t riscv::emit_srai_rv32i(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_srai_rv32i;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

uint64_t riscv::emit_add(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_add;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_sub(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sub;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_sll(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sll;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_slt(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_slt;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_sltu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sltu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_xor(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_xor;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_srl(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_srl;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_sra(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sra;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_or(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_or;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_and(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_and;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fence(arg4 pred, arg4 succ)
{
	decode dec;
	if (!(pred.valid() && succ.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fence;
	dec.rv.fence.pred = pred;
	dec.rv.fence.succ = succ;
	return encode_inst(dec);
}

uint64_t riscv::emit_fence_i()
{
	decode dec;
	dec.op = riscv_op_fence_i;
	return encode_inst(dec);
}

uint64_t riscv::emit_lwu(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lwu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_ld(ireg5 rd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_ld;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_sd(ireg5 rs1, ireg5 rs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && rs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_slli_rv64i(ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_slli_rv64i;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt6;
	return encode_inst(dec);
}

uint64_t riscv::emit_srli_rv64i(ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_srli_rv64i;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt6;
	return encode_inst(dec);
}

uint64_t riscv::emit_srai_rv64i(ireg5 rd, ireg5 rs1, uimm6 shamt6)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt6.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_srai_rv64i;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt6;
	return encode_inst(dec);
}

uint64_t riscv::emit_addiw(ireg5 rd, ireg5 rs1, simm12 imm12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && imm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_addiw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = imm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_slliw(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_slliw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

uint64_t riscv::emit_srliw(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_srliw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

uint64_t riscv::emit_sraiw(ireg5 rd, ireg5 rs1, uimm5 shamt5)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && shamt5.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sraiw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = shamt5;
	return encode_inst(dec);
}

uint64_t riscv::emit_addw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_addw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_subw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_subw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_sllw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sllw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_srlw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_srlw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_sraw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sraw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_mul(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_mul;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_mulh(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_mulh;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_mulhsu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_mulhsu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_mulhu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_mulhu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_div(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_div;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_divu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_divu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_rem(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_rem;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_remu(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_remu;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_mulw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_mulw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_divw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_divw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_divuw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_divuw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_remw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_remw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_remuw(ireg5 rd, ireg5 rs1, ireg5 rs2)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_remuw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_lr_w(ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lr_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_sc_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sc_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoswap_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoswap_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoadd_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoadd_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoxor_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoxor_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoor_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoor_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoand_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoand_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amomin_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amomin_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amomax_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amomax_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amominu_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amominu_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amomaxu_w(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amomaxu_w;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_lr_d(ireg5 rd, ireg5 rs1, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_lr_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_sc_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sc_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoswap_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoswap_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoadd_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoadd_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoxor_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoxor_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoor_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoor_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amoand_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amoand_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amomin_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amomin_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amomax_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amomax_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amominu_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amominu_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_amomaxu_d(ireg5 rd, ireg5 rs1, ireg5 rs2, arg1 aq, arg1 rl)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && rs2.valid() && aq.valid() && rl.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_amomaxu_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = rs2;
	dec.rv.amo.aq = aq;
	dec.rv.amo.rl = rl;
	return encode_inst(dec);
}

uint64_t riscv::emit_ecall()
{
	decode dec;
	dec.op = riscv_op_ecall;
	return encode_inst(dec);
}

uint64_t riscv::emit_ebreak()
{
	decode dec;
	dec.op = riscv_op_ebreak;
	return encode_inst(dec);
}

uint64_t riscv::emit_uret()
{
	decode dec;
	dec.op = riscv_op_uret;
	return encode_inst(dec);
}

uint64_t riscv::emit_sret()
{
	decode dec;
	dec.op = riscv_op_sret;
	return encode_inst(dec);
}

uint64_t riscv::emit_hret()
{
	decode dec;
	dec.op = riscv_op_hret;
	return encode_inst(dec);
}

uint64_t riscv::emit_mret()
{
	decode dec;
	dec.op = riscv_op_mret;
	return encode_inst(dec);
}

uint64_t riscv::emit_dret()
{
	decode dec;
	dec.op = riscv_op_dret;
	return encode_inst(dec);
}

uint64_t riscv::emit_sfence_vm(ireg5 rs1)
{
	decode dec;
	if (!(rs1.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_sfence_vm;
	dec.rv.r.rs1 = rs1;
	return encode_inst(dec);
}

uint64_t riscv::emit_wfi()
{
	decode dec;
	dec.op = riscv_op_wfi;
	return encode_inst(dec);
}

uint64_t riscv::emit_csrrw(ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_csrrw;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = csr12;
	return encode_inst(dec);
}

uint64_t riscv::emit_csrrs(ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_csrrs;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = csr12;
	return encode_inst(dec);
}

uint64_t riscv::emit_csrrc(ireg5 rd, ireg5 rs1, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && rs1.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_csrrc;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = rs1;
	dec.imm = csr12;
	return encode_inst(dec);
}

uint64_t riscv::emit_csrrwi(ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_csrrwi;
	dec.rv.r.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	return encode_inst(dec);
}

uint64_t riscv::emit_csrrsi(ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_csrrsi;
	dec.rv.r.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	return encode_inst(dec);
}

uint64_t riscv::emit_csrrci(ireg5 rd, uimm5 zimm, uimm12 csr12)
{
	decode dec;
	if (!(rd.valid() && zimm.valid() && csr12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_csrrci;
	dec.rv.r.rd = rd;
	dec.imm = zimm;
	dec.imm = csr12;
	return encode_inst(dec);
}

uint64_t riscv::emit_flw(freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_flw;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsw(ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsw;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = frs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmadd_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmadd_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmsub_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmsub_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fnmsub_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fnmsub_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fnmadd_s(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fnmadd_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fadd_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fadd_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsub_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsub_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmul_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmul_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fdiv_s(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fdiv_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsgnj_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsgnj_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsgnjn_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsgnjn_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsgnjx_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsgnjx_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmin_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmin_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmax_s(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmax_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsqrt_s(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsqrt_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fle_s(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fle_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_flt_s(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_flt_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_feq_s(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_feq_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_w_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_w_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_wu_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_wu_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_s_w(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_s_w;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_s_wu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_s_wu;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmv_x_s(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmv_x_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	return encode_inst(dec);
}

uint64_t riscv::emit_fclass_s(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fclass_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmv_s_x(freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmv_s_x;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_l_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_l_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_lu_s(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_lu_s;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_s_l(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_s_l;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_s_lu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_s_lu;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fld(freg5 frd, ireg5 rs1, offset12 oimm12)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && oimm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fld;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.imm = oimm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsd(ireg5 rs1, freg5 frs2, offset12 simm12)
{
	decode dec;
	if (!(rs1.valid() && frs2.valid() && simm12.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rs2 = frs2;
	dec.imm = simm12;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmadd_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmadd_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmsub_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmsub_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fnmsub_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fnmsub_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fnmadd_d(freg5 frd, freg5 frs1, freg5 frs2, freg5 frs3, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && frs3.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fnmadd_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rs3 = frs3;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fadd_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fadd_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsub_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsub_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmul_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmul_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fdiv_d(freg5 frd, freg5 frs1, freg5 frs2, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fdiv_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsgnj_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsgnj_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsgnjn_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsgnjn_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsgnjx_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsgnjx_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmin_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmin_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmax_d(freg5 frd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmax_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_s_d(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_s_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_d_s(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_d_s;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fsqrt_d(freg5 frd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fsqrt_d;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fle_d(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fle_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_flt_d(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_flt_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_feq_d(ireg5 rd, freg5 frs1, freg5 frs2)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && frs2.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_feq_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rs2 = frs2;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_w_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_w_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_wu_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_wu_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_d_w(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_d_w;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_d_wu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_d_wu;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fclass_d(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fclass_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_l_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_l_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_lu_d(ireg5 rd, freg5 frs1, arg3 rm)
{
	decode dec;
	if (!(rd.valid() && frs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_lu_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmv_x_d(ireg5 rd, freg5 frs1)
{
	decode dec;
	if (!(rd.valid() && frs1.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmv_x_d;
	dec.rv.r.rd = rd;
	dec.rv.r.rs1 = frs1;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_d_l(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_d_l;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fcvt_d_lu(freg5 frd, ireg5 rs1, arg3 rm)
{
	decode dec;
	if (!(frd.valid() && rs1.valid() && rm.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fcvt_d_lu;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	dec.rv.r.rm = rm;
	return encode_inst(dec);
}

uint64_t riscv::emit_fmv_d_x(freg5 frd, ireg5 rs1)
{
	decode dec;
	if (!(frd.valid() && rs1.valid())) return 0; /* illegal instruction */
	dec.op = riscv_op_fmv_d_x;
	dec.rv.r.rd = frd;
	dec.rv.r.rs1 = rs1;
	return encode_inst(dec);
}


