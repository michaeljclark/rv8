//
//  riscv-interp.h
//
//  DANGER - This is machine generated code
//

#ifndef riscv_interp_h
#define riscv_interp_h

/* Execute Instruction RV32 */

template <bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvc, typename T, typename P>
riscv::addr_t exec_inst_rv32(T &dec, P &proc, riscv::addr_t pc_offset)
{
	enum { xlen = 32 };
	typedef int32_t sx;
	typedef uint32_t ux;
	using namespace riscv;

	switch (dec.op) {
		case riscv_op_lui:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = dec.imm;
			};
			break;
		case riscv_op_auipc:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.pc + dec.imm;
			};
			break;
		case riscv_op_jal:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.pc + pc_offset; pc_offset = dec.imm;
			};
			break;
		case riscv_op_jalr:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.pc + pc_offset; pc_offset = (proc.ireg[dec.rs1] + dec.imm - proc.pc) & ~1;
			};
			break;
		case riscv_op_beq:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bne:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_blt:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bge:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bltu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bgeu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_lb:
			if (rvi) {
				s8 t; proc.mmu.template load<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lh:
			if (rvi) {
				s16 t; proc.mmu.template load<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lw:
			if (rvi) {
				s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lbu:
			if (rvi) {
				u8 t; proc.mmu.template load<P,u8>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lhu:
			if (rvi) {
				u16 t; proc.mmu.template load<P,u16>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_sb:
			if (rvi) {
				proc.mmu.template store<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, s8(proc.ireg[dec.rs2]));
			};
			break;
		case riscv_op_sh:
			if (rvi) {
				proc.mmu.template store<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, s16(proc.ireg[dec.rs2]));
			};
			break;
		case riscv_op_sw:
			if (rvi) {
				proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case riscv_op_addi:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val + sx(dec.imm);
			};
			break;
		case riscv_op_slti:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val < sx(dec.imm);
			};
			break;
		case riscv_op_sltiu:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val < ux(dec.imm);
			};
			break;
		case riscv_op_xori:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val ^ ux(dec.imm);
			};
			break;
		case riscv_op_ori:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val | ux(dec.imm);
			};
			break;
		case riscv_op_andi:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val & ux(dec.imm);
			};
			break;
		case riscv_op_slli_rv32i:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val << dec.imm;
			};
			break;
		case riscv_op_srli_rv32i:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val >> dec.imm;
			};
			break;
		case riscv_op_srai_rv32i:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val >> dec.imm;
			};
			break;
		case riscv_op_add:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val + proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_sub:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val - proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_sll:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val << (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case riscv_op_slt:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_sltu:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_xor:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val ^ proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_srl:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case riscv_op_sra:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case riscv_op_or:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val | proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_and:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val & proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_mul:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val * proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_mulh:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = riscv::mulh(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.x.val);
			};
			break;
		case riscv_op_mulhsu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = riscv::mulhsu(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_mulhu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = riscv::mulhu(proc.ireg[dec.rs1].r.xu.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_div:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? std::numeric_limits<sx>::min() : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : proc.ireg[dec.rs1].r.x.val / proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_divu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs2].r.x.val == 0 ? -1 : sx(proc.ireg[dec.rs1].r.xu.val / proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_rem:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : proc.ireg[dec.rs1].r.x.val % proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_remu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : sx(proc.ireg[dec.rs1].r.xu.val % proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_lr_w:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1], t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_sc_w:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val); if (dec.rd > 0) proc.ireg[dec.rd] = res;
			};
			break;
		case riscv_op_amoswap_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoadd_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val + t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoxor_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val ^ t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoor_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val | t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoand_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val & t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomin_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val < t ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomax_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val > t ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amominu_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.wu.val < u32(t) ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomaxu_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.wu.val > u32(t) ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_flw:
			if (rvf) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.wu.val = t;
			};
			break;
		case riscv_op_fsw:
			if (rvf) {
				proc.mmu.template store<P,f32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.s.val);
			};
			break;
		case riscv_op_fmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case riscv_op_fmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case riscv_op_fnmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val);
			};
			break;
		case riscv_op_fnmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val);
			};
			break;
		case riscv_op_fadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val + proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val - proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fmul_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fdiv_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val / proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fsgnj_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case riscv_op_fsgnjn_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (~proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case riscv_op_fsgnjx_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case riscv_op_fmin_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val) | ((proc.freg[dec.rs2].r.wu.val & 0x7fc00000) == 0x7fc00000) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fmax_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val > proc.freg[dec.rs2].r.s.val) | ((proc.freg[dec.rs2].r.wu.val & 0x7fc00000) == 0x7fc00000) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fsqrt_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = riscv::f32_sqrt(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fle_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_flt_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_feq_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fcvt_w_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fcvt_wu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fcvt_s_w:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case riscv_op_fcvt_s_wu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case riscv_op_fmv_x_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = (proc.freg[dec.rs1].r.wu.val & 0x7fc00000) == 0x7fc00000 ? s32(0x7fc00000) : proc.freg[dec.rs1].r.w.val;
			};
			break;
		case riscv_op_fclass_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = f32_classify(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fmv_s_x:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.ireg[dec.rs1].r.wu.val;
			};
			break;
		case riscv_op_fld:
			if (rvd) {
				u64 t; proc.mmu.template load<P,u64>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.lu.val = t;
			};
			break;
		case riscv_op_fsd:
			if (rvd) {
				proc.mmu.template store<P,f64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.d.val);
			};
			break;
		case riscv_op_fmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case riscv_op_fmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case riscv_op_fnmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val);
			};
			break;
		case riscv_op_fnmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val);
			};
			break;
		case riscv_op_fadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val + proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val - proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fmul_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fdiv_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val / proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fsgnj_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case riscv_op_fsgnjn_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (~proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case riscv_op_fsgnjx_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case riscv_op_fmin_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val) | ((proc.freg[dec.rs2].r.lu.val & 0x7ff8000000000000ULL) == 0x7ff8000000000000ULL) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fmax_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val > proc.freg[dec.rs2].r.d.val) | ((proc.freg[dec.rs2].r.lu.val & 0x7ff8000000000000ULL) == 0x7ff8000000000000ULL) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fcvt_s_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_d_s:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fsqrt_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = riscv::f64_sqrt(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fle_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_flt_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_feq_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fcvt_w_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_wu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_d_w:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case riscv_op_fcvt_d_wu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case riscv_op_fclass_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = f64_classify(proc.freg[dec.rs1].r.d.val);
			};
			break;
		default: return 0; /* illegal instruction */
	}
	return pc_offset;
}

/* Execute Instruction RV64 */

template <bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvc, typename T, typename P>
riscv::addr_t exec_inst_rv64(T &dec, P &proc, riscv::addr_t pc_offset)
{
	enum { xlen = 64 };
	typedef int64_t sx;
	typedef uint64_t ux;
	using namespace riscv;

	switch (dec.op) {
		case riscv_op_lui:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = dec.imm;
			};
			break;
		case riscv_op_auipc:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.pc + dec.imm;
			};
			break;
		case riscv_op_jal:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.pc + pc_offset; pc_offset = dec.imm;
			};
			break;
		case riscv_op_jalr:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.pc + pc_offset; pc_offset = (proc.ireg[dec.rs1] + dec.imm - proc.pc) & ~1;
			};
			break;
		case riscv_op_beq:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bne:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_blt:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bge:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bltu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_bgeu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case riscv_op_lb:
			if (rvi) {
				s8 t; proc.mmu.template load<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lh:
			if (rvi) {
				s16 t; proc.mmu.template load<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lw:
			if (rvi) {
				s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lbu:
			if (rvi) {
				u8 t; proc.mmu.template load<P,u8>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lhu:
			if (rvi) {
				u16 t; proc.mmu.template load<P,u16>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_sb:
			if (rvi) {
				proc.mmu.template store<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, s8(proc.ireg[dec.rs2]));
			};
			break;
		case riscv_op_sh:
			if (rvi) {
				proc.mmu.template store<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, s16(proc.ireg[dec.rs2]));
			};
			break;
		case riscv_op_sw:
			if (rvi) {
				proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case riscv_op_addi:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val + sx(dec.imm);
			};
			break;
		case riscv_op_slti:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val < sx(dec.imm);
			};
			break;
		case riscv_op_sltiu:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val < ux(dec.imm);
			};
			break;
		case riscv_op_xori:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val ^ ux(dec.imm);
			};
			break;
		case riscv_op_ori:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val | ux(dec.imm);
			};
			break;
		case riscv_op_andi:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val & ux(dec.imm);
			};
			break;
		case riscv_op_add:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val + proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_sub:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val - proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_sll:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val << (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case riscv_op_slt:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_sltu:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_xor:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val ^ proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_srl:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case riscv_op_sra:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case riscv_op_or:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val | proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_and:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val & proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case riscv_op_lwu:
			if (rvi) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_ld:
			if (rvi) {
				s64 t; proc.mmu.template load<P,s64>(proc, proc.ireg[dec.rs1] + dec.imm, t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_sd:
			if (rvi) {
				proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.l.val);
			};
			break;
		case riscv_op_slli_rv64i:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val << dec.imm;
			};
			break;
		case riscv_op_srli_rv64i:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.xu.val >> dec.imm;
			};
			break;
		case riscv_op_srai_rv64i:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val >> dec.imm;
			};
			break;
		case riscv_op_addiw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.w.val + dec.imm);
			};
			break;
		case riscv_op_slliw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.wu.val << dec.imm);
			};
			break;
		case riscv_op_srliw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.wu.val >> dec.imm);
			};
			break;
		case riscv_op_sraiw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.w.val >> dec.imm;
			};
			break;
		case riscv_op_addw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.w.val + proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case riscv_op_subw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.w.val - proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case riscv_op_sllw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.wu.val << (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case riscv_op_srlw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.wu.val >> (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case riscv_op_sraw:
			if (rvi) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.w.val >> (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case riscv_op_mul:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val * proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_mulh:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = riscv::mulh(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.x.val);
			};
			break;
		case riscv_op_mulhsu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = riscv::mulhsu(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_mulhu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = riscv::mulhu(proc.ireg[dec.rs1].r.xu.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_div:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? std::numeric_limits<sx>::min() : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : proc.ireg[dec.rs1].r.x.val / proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_divu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs2].r.x.val == 0 ? -1 : sx(proc.ireg[dec.rs1].r.xu.val / proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_rem:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : proc.ireg[dec.rs1].r.x.val % proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case riscv_op_remu:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : sx(proc.ireg[dec.rs1].r.xu.val % proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case riscv_op_mulw:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1].r.wu.val * proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case riscv_op_divw:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.w.val == std::numeric_limits<s32>::min() && proc.ireg[dec.rs2].r.w.val == -1 ? std::numeric_limits<s32>::min() : proc.ireg[dec.rs2].r.w.val == 0 ? -1 : proc.ireg[dec.rs1].r.w.val / proc.ireg[dec.rs2].r.w.val;
			};
			break;
		case riscv_op_divuw:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs2].r.w.val == 0 ? -1 : s32(proc.ireg[dec.rs1].r.wu.val / proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case riscv_op_remw:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs1].r.w.val == std::numeric_limits<s32>::min() && proc.ireg[dec.rs2].r.w.val == -1 ? 0 : proc.ireg[dec.rs2].r.w.val == 0 ? proc.ireg[dec.rs1].r.w.val : proc.ireg[dec.rs1].r.w.val % proc.ireg[dec.rs2].r.w.val;
			};
			break;
		case riscv_op_remuw:
			if (rvm) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.ireg[dec.rs2].r.w.val == 0 ? proc.ireg[dec.rs1].r.w.val : s32(proc.ireg[dec.rs1].r.wu.val % proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case riscv_op_lr_w:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1], t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_sc_w:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val); if (dec.rd > 0) proc.ireg[dec.rd] = res;
			};
			break;
		case riscv_op_amoswap_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoadd_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val + t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoxor_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val ^ t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoor_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val | t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoand_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val & t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomin_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val < t ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomax_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val > t ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amominu_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.wu.val < u32(t) ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomaxu_w:
			if (rva) {
				s32 t(*(s32*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.wu.val > u32(t) ? proc.ireg[dec.rs2].r.w.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_lr_d:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s64 t; proc.mmu.template load<P,s64>(proc, proc.ireg[dec.rs1], t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_sc_d:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val); if (dec.rd > 0) proc.ireg[dec.rd] = res;
			};
			break;
		case riscv_op_amoswap_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoadd_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val + t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoxor_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val ^ t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoor_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val | t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amoand_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val & t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomin_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val < t ? proc.ireg[dec.rs2].r.l.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomax_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val > t ? proc.ireg[dec.rs2].r.l.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amominu_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.lu.val < u64(t) ? proc.ireg[dec.rs2].r.l.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_amomaxu_d:
			if (rva) {
				s64 t(*(s64*)proc.ireg[dec.rs1]); proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.lu.val > u64(t) ? proc.ireg[dec.rs2].r.l.val : t); if (dec.rd > 0) proc.ireg[dec.rd] = t;
			};
			break;
		case riscv_op_flw:
			if (rvf) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.wu.val = t;
			};
			break;
		case riscv_op_fsw:
			if (rvf) {
				proc.mmu.template store<P,f32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.s.val);
			};
			break;
		case riscv_op_fmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case riscv_op_fmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case riscv_op_fnmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val);
			};
			break;
		case riscv_op_fnmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = -(proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val);
			};
			break;
		case riscv_op_fadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val + proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val - proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fmul_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fdiv_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val / proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fsgnj_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case riscv_op_fsgnjn_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (~proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case riscv_op_fsgnjx_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case riscv_op_fmin_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val) | ((proc.freg[dec.rs2].r.wu.val & 0x7fc00000) == 0x7fc00000) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fmax_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val > proc.freg[dec.rs2].r.s.val) | ((proc.freg[dec.rs2].r.wu.val & 0x7fc00000) == 0x7fc00000) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fsqrt_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = riscv::f32_sqrt(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fle_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_flt_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_feq_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val;
			};
			break;
		case riscv_op_fcvt_w_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fcvt_wu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fcvt_s_w:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case riscv_op_fcvt_s_wu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case riscv_op_fmv_x_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = (proc.freg[dec.rs1].r.wu.val & 0x7fc00000) == 0x7fc00000 ? s32(0x7fc00000) : proc.freg[dec.rs1].r.w.val;
			};
			break;
		case riscv_op_fclass_s:
			if (rvf) {
				if (dec.rd > 0) proc.ireg[dec.rd] = f32_classify(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fmv_s_x:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.ireg[dec.rs1].r.wu.val;
			};
			break;
		case riscv_op_fcvt_l_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_l(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fcvt_lu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_lu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fcvt_s_l:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.l.val);
			};
			break;
		case riscv_op_fcvt_s_lu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.lu.val);
			};
			break;
		case riscv_op_fld:
			if (rvd) {
				u64 t; proc.mmu.template load<P,u64>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.lu.val = t;
			};
			break;
		case riscv_op_fsd:
			if (rvd) {
				proc.mmu.template store<P,f64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.d.val);
			};
			break;
		case riscv_op_fmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case riscv_op_fmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case riscv_op_fnmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val);
			};
			break;
		case riscv_op_fnmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = -(proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val);
			};
			break;
		case riscv_op_fadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val + proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val - proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fmul_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fdiv_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val / proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fsgnj_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case riscv_op_fsgnjn_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (~proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case riscv_op_fsgnjx_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case riscv_op_fmin_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val) | ((proc.freg[dec.rs2].r.lu.val & 0x7ff8000000000000ULL) == 0x7ff8000000000000ULL) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fmax_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val > proc.freg[dec.rs2].r.d.val) | ((proc.freg[dec.rs2].r.lu.val & 0x7ff8000000000000ULL) == 0x7ff8000000000000ULL) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fcvt_s_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_d_s:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case riscv_op_fsqrt_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = riscv::f64_sqrt(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fle_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_flt_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_feq_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val;
			};
			break;
		case riscv_op_fcvt_w_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_wu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_d_w:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case riscv_op_fcvt_d_wu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case riscv_op_fclass_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = f64_classify(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_l_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_l(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fcvt_lu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); if (dec.rd > 0) proc.ireg[dec.rd] = riscv::fcvt_lu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case riscv_op_fmv_x_d:
			if (rvd) {
				if (dec.rd > 0) proc.ireg[dec.rd] = (proc.freg[dec.rs1].r.lu.val & 0x7ff8000000000000ULL) == 0x7ff8000000000000ULL ? s64(0x7ff8000000000000ULL) : proc.freg[dec.rs1].r.l.val;
			};
			break;
		case riscv_op_fcvt_d_l:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.l.val);
			};
			break;
		case riscv_op_fcvt_d_lu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.lu.val);
			};
			break;
		case riscv_op_fmv_d_x:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.ireg[dec.rs1].r.lu.val;
			};
			break;
		default: return 0; /* illegal instruction */
	}
	return pc_offset;
}

#endif
