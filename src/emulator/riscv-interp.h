//
//  riscv-interp.h
//
//  DANGER - This is machine generated code
//

#ifndef riscv_interp_h
#define riscv_interp_h

template <typename T>
bool rv32_exec(T &dec, riscv_processor_rv32 &proc, uintptr_t inst_length)
{
	enum { xlen = 32 };
	typedef int32_t sx;
	typedef uint32_t ux;

	switch (dec.op) {
		case riscv_op_lui: {
			if (dec.rd != 0) proc.ireg[dec.rd] = dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_auipc: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_jal: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_jalr: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.ireg[dec.rs1] + dec.imm;
			goto x;
		};
		case riscv_op_beq: {
			if (sx(proc.ireg[dec.rs1]) == sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bne: {
			if (sx(proc.ireg[dec.rs1]) != sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_blt: {
			if (sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bge: {
			if (sx(proc.ireg[dec.rs1]) >= sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bltu: {
			if (ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bgeu: {
			if (ux(proc.ireg[dec.rs1]) >= ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lb: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lh: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s32*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lbu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lhu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sb: {
			*((u8*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sh: {
			*((u16*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sw: {
			*((u32*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_addi: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_slti: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(dec.imm) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sltiu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(dec.imm) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_xori: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_ori: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_andi: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_slli_rv32i: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srli_rv32i: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srai_rv32i: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_add: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sub: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) - sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sll: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_slt: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sltu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_xor: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srl: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sra: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_or: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_and: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fence: {
			;;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fence_i: {
			;;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_mul: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_div: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) / sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_divu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) / ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_rem: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) % sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_remu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) % ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lr_w: {
			proc.state.lr = proc.ireg[dec.rs1]; if (dec.rd != 0) proc.ireg[dec.rd] = sx(*((s32*)proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sc_w: {
			if (proc.state.lr == proc.ireg[dec.rs1]) *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]);
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoswap_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]); if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoadd_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) + t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoxor_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) ^ t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoor_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) | t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoand_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) & t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomin_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) < t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomax_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) > t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amominu_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) < u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomaxu_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) > u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_flw: {
			proc.freg[dec.rd].r.s.val = *(f32*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsw: {
			*(f32*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmadd_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmsub_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmsub_s: {
			proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmadd_s: {
			proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fadd_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsub_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmul_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val * proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fdiv_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val / proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnj_s: {
			proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjn_s: {
			proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | ~(proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjx_s: {
			proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmin_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val < proc.freg[dec.rs1].r.s.val ? proc.freg[dec.rs2].r.s.val : proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmax_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val > proc.freg[dec.rs1].r.s.val ? proc.freg[dec.rs2].r.s.val : proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsqrt_s: {
			proc.freg[dec.rd].r.s.val = f32_sqrt(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fle_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_flt_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_feq_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_w_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_wu_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_w: {
			proc.freg[dec.rd].r.s.val = f32(s32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_wu: {
			proc.freg[dec.rd].r.s.val = f32(u32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmv_x_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.wu.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fclass_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = f32_classify(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmv_s_x: {
			proc.freg[dec.rd].r.wu.val = u32(proc.ireg[dec.rs1]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fld: {
			proc.freg[dec.rd].r.d.val = *(f64*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsd: {
			*(f64*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmadd_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmsub_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmsub_d: {
			proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmadd_d: {
			proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fadd_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsub_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmul_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val * proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fdiv_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val / proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnj_d: {
			proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjn_d: {
			proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | ~(proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjx_d: {
			proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmin_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val < proc.freg[dec.rs1].r.d.val ? proc.freg[dec.rs2].r.d.val : proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmax_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val > proc.freg[dec.rs1].r.d.val ? proc.freg[dec.rs2].r.d.val : proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_d: {
			proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_s: {
			proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsqrt_d: {
			proc.freg[dec.rd].r.d.val = f64_sqrt(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fle_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_flt_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_feq_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_w_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_wu_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_w: {
			proc.freg[dec.rd].r.d.val = f64(s32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_wu: {
			proc.freg[dec.rd].r.d.val = f64(u32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fclass_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = f64_classify(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		default:
			break;
	}
	return false; /* illegal instruction */
x:	return true;
}

template <typename T>
bool rv64_exec(T &dec, riscv_processor_rv64 &proc, uintptr_t inst_length)
{
	enum { xlen = 64 };
	typedef int64_t sx;
	typedef uint64_t ux;

	switch (dec.op) {
		case riscv_op_lui: {
			if (dec.rd != 0) proc.ireg[dec.rd] = dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_auipc: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_jal: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_jalr: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.ireg[dec.rs1] + dec.imm;
			goto x;
		};
		case riscv_op_beq: {
			if (sx(proc.ireg[dec.rs1]) == sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bne: {
			if (sx(proc.ireg[dec.rs1]) != sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_blt: {
			if (sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bge: {
			if (sx(proc.ireg[dec.rs1]) >= sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bltu: {
			if (ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_bgeu: {
			if (ux(proc.ireg[dec.rs1]) >= ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lb: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lh: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s32*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lbu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lhu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sb: {
			*((u8*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sh: {
			*((u16*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sw: {
			*((u32*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_addi: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_slti: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(dec.imm) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sltiu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(dec.imm) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_xori: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_ori: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_andi: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_add: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sub: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) - sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sll: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_slt: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sltu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_xor: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srl: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sra: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_or: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_and: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fence: {
			;;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fence_i: {
			;;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lwu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u32*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_ld: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s64*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sd: {
			*(u64*)(proc.ireg[dec.rs1] + dec.imm) = proc.ireg[dec.rs2];
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_slli_rv64i: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srli_rv64i: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srai_rv64i: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_addiw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) + s32(dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_slliw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) << dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srliw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sraiw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_addw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) + s32(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_subw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) - s32(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sllw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_srlw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sraw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_mul: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_div: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) / sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_divu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) / ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_rem: {
			if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) % sx(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_remu: {
			if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) % ux(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_mulw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) * u32(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_divw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) / s32(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_divuw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) / u32(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_remw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) % s32(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_remuw: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) % u32(proc.ireg[dec.rs2]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lr_w: {
			proc.state.lr = proc.ireg[dec.rs1]; if (dec.rd != 0) proc.ireg[dec.rd] = sx(*((s32*)proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sc_w: {
			if (proc.state.lr == proc.ireg[dec.rs1]) *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]);
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoswap_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]); if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoadd_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) + t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoxor_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) ^ t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoor_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) | t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoand_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) & t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomin_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) < t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomax_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) > t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amominu_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) < u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomaxu_w: {
			s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) > u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_lr_d: {
			proc.state.lr = proc.ireg[dec.rs1]; if (dec.rd != 0) proc.ireg[dec.rd] = sx(*((s64*)proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_sc_d: {
			if (proc.state.lr == proc.ireg[dec.rs1]) *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]);
			else proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoswap_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]); if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoadd_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) + t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoxor_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) ^ t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoor_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) | t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amoand_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) & t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomin_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) < t ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomax_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) > t ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amominu_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = u64(proc.ireg[dec.rs2]) < u64(t) ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_amomaxu_d: {
			s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = u64(proc.ireg[dec.rs2]) > u64(t) ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_flw: {
			proc.freg[dec.rd].r.s.val = *(f32*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsw: {
			*(f32*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmadd_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmsub_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmsub_s: {
			proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmadd_s: {
			proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fadd_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsub_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmul_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val * proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fdiv_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val / proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnj_s: {
			proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjn_s: {
			proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | ~(proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjx_s: {
			proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmin_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val < proc.freg[dec.rs1].r.s.val ? proc.freg[dec.rs2].r.s.val : proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmax_s: {
			proc.freg[dec.rd].r.s.val = proc.freg[dec.rs2].r.s.val > proc.freg[dec.rs1].r.s.val ? proc.freg[dec.rs2].r.s.val : proc.freg[dec.rs1].r.s.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsqrt_s: {
			proc.freg[dec.rd].r.s.val = f32_sqrt(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fle_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_flt_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_feq_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_w_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_wu_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_w: {
			proc.freg[dec.rd].r.s.val = f32(s32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_wu: {
			proc.freg[dec.rd].r.s.val = f32(u32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmv_x_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.wu.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fclass_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = f32_classify(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmv_s_x: {
			proc.freg[dec.rd].r.wu.val = u32(proc.ireg[dec.rs1]);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_l_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s64(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_lu_s: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u64(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_l: {
			proc.freg[dec.rd].r.s.val = f32(s64(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_lu: {
			proc.freg[dec.rd].r.s.val = f32(u64(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fld: {
			proc.freg[dec.rd].r.d.val = *(f64*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsd: {
			*(f64*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmadd_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmsub_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmsub_d: {
			proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fnmadd_d: {
			proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fadd_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsub_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmul_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val * proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fdiv_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val / proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnj_d: {
			proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjn_d: {
			proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | ~(proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsgnjx_d: {
			proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmin_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val < proc.freg[dec.rs1].r.d.val ? proc.freg[dec.rs2].r.d.val : proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmax_d: {
			proc.freg[dec.rd].r.d.val = proc.freg[dec.rs2].r.d.val > proc.freg[dec.rs1].r.d.val ? proc.freg[dec.rs2].r.d.val : proc.freg[dec.rs1].r.d.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_s_d: {
			proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_s: {
			proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fsqrt_d: {
			proc.freg[dec.rd].r.d.val = f64_sqrt(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fle_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_flt_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_feq_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val ? 1 : 0;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_w_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_wu_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_w: {
			proc.freg[dec.rd].r.d.val = f64(s32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_wu: {
			proc.freg[dec.rd].r.d.val = f64(u32(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fclass_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = f64_classify(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_l_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = s64(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_lu_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = u64(proc.freg[dec.rs1].r.d.val);
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmv_x_d: {
			if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.lu.val;
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_l: {
			proc.freg[dec.rd].r.d.val = f64(s64(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fcvt_d_lu: {
			proc.freg[dec.rd].r.d.val = f64(u64(proc.ireg[dec.rs1]));
			proc.pc += inst_length;
			goto x;
		};
		case riscv_op_fmv_d_x: {
			proc.freg[dec.rd].r.lu.val = u64(proc.ireg[dec.rs1]);
			proc.pc += inst_length;
			goto x;
		};
		default:
			break;
	}
	return false; /* illegal instruction */
x:	return true;
}

#endif
