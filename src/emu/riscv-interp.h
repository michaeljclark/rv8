//
//  riscv-interp.h
//
//  DANGER - This is machine generated code
//

#ifndef riscv_interp_h
#define riscv_interp_h

/* Execute Instruction RV32 */

template <bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvc, typename T, typename P>
bool exec_inst_rv32(T &dec, P &proc, uintptr_t inst_length)
{
	enum { xlen = 32 };
	typedef int32_t sx;
	typedef uint32_t ux;
	using namespace riscv;

	switch (dec.op) {
		case riscv_op_lui: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_auipc: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_jal: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.pc + dec.imm;
				return true;
			};
		};
		case riscv_op_jalr: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.ireg[dec.rs1] + dec.imm;
				return true;
			};
		};
		case riscv_op_beq: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) == sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bne: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) != sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_blt: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bge: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) >= sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bltu: {
			if (rvi) {
				if (ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bgeu: {
			if (rvi) {
				if (ux(proc.ireg[dec.rs1]) >= ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lb: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s8*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lh: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s16*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s32*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lbu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u8*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lhu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u16*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sb: {
			if (rvi) {
				*((u8*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sh: {
			if (rvi) {
				*((u16*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sw: {
			if (rvi) {
				*((u32*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_addi: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_slti: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(dec.imm) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sltiu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(dec.imm) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_xori: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_ori: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_andi: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_slli_rv32i: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srli_rv32i: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srai_rv32i: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_add: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sub: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) - sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sll: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_slt: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2]) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sltu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2]) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_xor: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srl: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sra: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_or: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_and: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fence: {
			if (rvi) {
				;;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fence_i: {
			if (rvi) {
				;;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mul: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mulh: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = mulh(sx(proc.ireg[dec.rs1]), sx(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mulhsu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = mulhsu(sx(proc.ireg[dec.rs1]), ux(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mulhu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = mulhu(ux(proc.ireg[dec.rs1]), ux(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_div: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) / sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_divu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) / ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_rem: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) % sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_remu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) % ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lr_w: {
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; if (dec.rd != 0) proc.ireg[dec.rd] = sx(*((s32*)proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sc_w: {
			if (rva) {
				ux res; if (proc.lr == proc.ireg[dec.rs1]) { *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]); res = 0; } else { res = 1; }; if (dec.rd != 0) proc.ireg[dec.rd] = res;;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoswap_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]); if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoadd_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) + t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoxor_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) ^ t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoor_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) | t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoand_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) & t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomin_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) < t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomax_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) > t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amominu_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) < u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomaxu_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) > u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_flw: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = *(f32*)(proc.ireg[dec.rs1] + dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsw: {
			if (rvf) {
				*(f32*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmadd_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmsub_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmsub_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmadd_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fadd_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val + proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsub_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val - proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmul_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fdiv_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val / proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnj_s: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjn_s: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (~proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjx_s: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmin_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmax_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val > proc.freg[dec.rs2].r.s.val ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsqrt_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32_sqrt(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fle_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_flt_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_feq_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_w_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_wu_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.s.val > 0 ? proc.freg[dec.rs1].r.s.val : 0);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_w: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32(s32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_wu: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32(u32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmv_x_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.w.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fclass_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = f32_classify(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmv_s_x: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = u32(proc.ireg[dec.rs1]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fld: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = *(f64*)(proc.ireg[dec.rs1] + dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsd: {
			if (rvd) {
				*(f64*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmadd_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmsub_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmsub_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmadd_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fadd_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val + proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsub_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val - proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmul_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fdiv_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val / proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnj_d: {
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjn_d: {
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (~proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjx_d: {
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmin_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmax_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val > proc.freg[dec.rs2].r.d.val ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_d: {
			if (rvd) {
				proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_s: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsqrt_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64_sqrt(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fle_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_flt_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_feq_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_w_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_wu_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.d.val > 0 ? proc.freg[dec.rs1].r.d.val : 0);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_w: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(s32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_wu: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(u32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fclass_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = f64_classify(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		default:
			break;
	}
	return false; /* illegal instruction */
}

/* Execute Instruction RV64 */

template <bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvc, typename T, typename P>
bool exec_inst_rv64(T &dec, P &proc, uintptr_t inst_length)
{
	enum { xlen = 64 };
	typedef int64_t sx;
	typedef uint64_t ux;
	using namespace riscv;

	switch (dec.op) {
		case riscv_op_lui: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_auipc: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_jal: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.pc + dec.imm;
				return true;
			};
		};
		case riscv_op_jalr: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.pc + inst_length; proc.pc = proc.ireg[dec.rs1] + dec.imm;
				return true;
			};
		};
		case riscv_op_beq: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) == sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bne: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) != sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_blt: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bge: {
			if (rvi) {
				if (sx(proc.ireg[dec.rs1]) >= sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bltu: {
			if (rvi) {
				if (ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_bgeu: {
			if (rvi) {
				if (ux(proc.ireg[dec.rs1]) >= ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
				else proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lb: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s8*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lh: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s16*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s32*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lbu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u8*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lhu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u16*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sb: {
			if (rvi) {
				*((u8*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sh: {
			if (rvi) {
				*((u16*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sw: {
			if (rvi) {
				*((u32*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_addi: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_slti: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(dec.imm) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sltiu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(dec.imm) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_xori: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_ori: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_andi: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_add: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sub: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) - sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sll: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_slt: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2]) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sltu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2]) ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_xor: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srl: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sra: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_or: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_and: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fence: {
			if (rvi) {
				;;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fence_i: {
			if (rvi) {
				;;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lwu: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(*(u32*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_ld: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(*(s64*)(proc.ireg[dec.rs1] + dec.imm));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sd: {
			if (rvi) {
				*(u64*)(proc.ireg[dec.rs1] + dec.imm) = proc.ireg[dec.rs2];
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_slli_rv64i: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srli_rv64i: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srai_rv64i: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_addiw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) + s32(dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_slliw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(u32(proc.ireg[dec.rs1]) << dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srliw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(u32(proc.ireg[dec.rs1]) >> dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sraiw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) >> dec.imm;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_addw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) + s32(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_subw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) - s32(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sllw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(u32(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_srlw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(u32(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sraw: {
			if (rvi) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mul: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mulh: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = mulh(sx(proc.ireg[dec.rs1]), sx(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mulhsu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = mulhsu(sx(proc.ireg[dec.rs1]), ux(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mulhu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = mulhu(ux(proc.ireg[dec.rs1]), ux(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_div: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) / sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_divu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) / ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_rem: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) % sx(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_remu: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) % ux(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_mulw: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(u32(proc.ireg[dec.rs1]) * u32(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_divw: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) / s32(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_divuw: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(u32(proc.ireg[dec.rs1]) / u32(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_remw: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) % s32(proc.ireg[dec.rs2]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_remuw: {
			if (rvm) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(u32(proc.ireg[dec.rs1]) % u32(proc.ireg[dec.rs2]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lr_w: {
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; if (dec.rd != 0) proc.ireg[dec.rd] = sx(*((s32*)proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sc_w: {
			if (rva) {
				ux res; if (proc.lr == proc.ireg[dec.rs1]) { *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]); res = 0; } else { res = 1; }; if (dec.rd != 0) proc.ireg[dec.rd] = res;;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoswap_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]); if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoadd_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) + t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoxor_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) ^ t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoor_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) | t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoand_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) & t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomin_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) < t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomax_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = s32(proc.ireg[dec.rs2]) > t ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amominu_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) < u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomaxu_w: {
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); *((s32*)proc.ireg[dec.rs1]) = u32(proc.ireg[dec.rs2]) > u32(t) ? s32(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_lr_d: {
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; if (dec.rd != 0) proc.ireg[dec.rd] = sx(*((s64*)proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_sc_d: {
			if (rva) {
				ux res; if (proc.lr == proc.ireg[dec.rs1]) { *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]); res = 0; } else { res = 1; }; if (dec.rd != 0) proc.ireg[dec.rd] = res;;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoswap_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]); if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoadd_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) + t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoxor_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) ^ t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoor_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) | t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amoand_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) & t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomin_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) < t ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomax_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = s64(proc.ireg[dec.rs2]) > t ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amominu_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = u64(proc.ireg[dec.rs2]) < u64(t) ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_amomaxu_d: {
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); *((s64*)proc.ireg[dec.rs1]) = u64(proc.ireg[dec.rs2]) > u64(t) ? s64(proc.ireg[dec.rs2]) : t; if (dec.rd != 0) proc.ireg[dec.rd] = t;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_flw: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = *(f32*)(proc.ireg[dec.rs1] + dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsw: {
			if (rvf) {
				*(f32*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmadd_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmsub_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmsub_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmadd_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fadd_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val + proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsub_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val - proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmul_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fdiv_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val / proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnj_s: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjn_s: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (~proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjx_s: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmin_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmax_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val > proc.freg[dec.rs2].r.s.val ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsqrt_s: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32_sqrt(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fle_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_flt_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_feq_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_w_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_wu_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.s.val > 0 ? proc.freg[dec.rs1].r.s.val : 0);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_w: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32(s32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_wu: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32(u32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmv_x_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.w.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fclass_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = f32_classify(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmv_s_x: {
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = u32(proc.ireg[dec.rs1]);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_l_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s64(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_lu_s: {
			if (rvf) {
				if (dec.rd != 0) proc.ireg[dec.rd] = u64(proc.freg[dec.rs1].r.s.val > 0 ? proc.freg[dec.rs1].r.s.val : 0);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_l: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32(s64(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_lu: {
			if (rvf) {
				proc.freg[dec.rd].r.s.val = f32(u64(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fld: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = *(f64*)(proc.ireg[dec.rs1] + dec.imm);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsd: {
			if (rvd) {
				*(f64*)(proc.ireg[dec.rs1] + dec.imm) = proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmadd_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmsub_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmsub_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fnmadd_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fadd_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val + proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsub_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val - proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmul_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fdiv_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val / proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnj_d: {
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjn_d: {
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (~proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsgnjx_d: {
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmin_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmax_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val > proc.freg[dec.rs2].r.d.val ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_s_d: {
			if (rvd) {
				proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_s: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fsqrt_d: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64_sqrt(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fle_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_flt_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_feq_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val ? 1 : 0;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_w_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s32(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_wu_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = u32(proc.freg[dec.rs1].r.d.val > 0 ? proc.freg[dec.rs1].r.d.val : 0);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_w: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(s32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_wu: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(u32(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fclass_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = f64_classify(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_l_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = s64(proc.freg[dec.rs1].r.d.val);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_lu_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = u64(proc.freg[dec.rs1].r.d.val > 0 ? proc.freg[dec.rs1].r.d.val : 0);
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmv_x_d: {
			if (rvd) {
				if (dec.rd != 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.l.val;
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_l: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(s64(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fcvt_d_lu: {
			if (rvd) {
				proc.freg[dec.rd].r.d.val = f64(u64(proc.ireg[dec.rs1]));
				proc.pc += inst_length;
				return true;
			};
		};
		case riscv_op_fmv_d_x: {
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = u64(proc.ireg[dec.rs1]);
				proc.pc += inst_length;
				return true;
			};
		};
		default:
			break;
	}
	return false; /* illegal instruction */
}

#endif
