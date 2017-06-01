//
//  interp.h
//
//  DANGER - This is machine generated code
//

#ifndef rv_interp_h
#define rv_interp_h

/* Execute Instruction RV32 */

template <bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvq, bool rvc, typename T, typename P>
typename P::ux exec_inst_rv32(T &dec, P &proc, typename P::ux pc_offset)
{
	using namespace riscv;
	enum { xlen = 32 };
	typedef s32 sx;
	typedef u32 ux;

	switch (dec.op) {
		case rv_op_lui:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : dec.imm;
			};
			break;
		case rv_op_auipc:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + dec.imm;
			};
			break;
		case rv_op_jal:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + pc_offset; pc_offset = dec.imm;
			};
			break;
		case rv_op_jalr:
			if (rvi) {
				ux new_offset = (proc.ireg[dec.rs1] + dec.imm - proc.pc) & ~1; proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + pc_offset; pc_offset = new_offset;
			};
			break;
		case rv_op_beq:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bne:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_blt:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bge:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bltu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bgeu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_lb:
			if (rvi) {
				s8 t; proc.mmu.template load<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lh:
			if (rvi) {
				s16 t; proc.mmu.template load<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lw:
			if (rvi) {
				s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lbu:
			if (rvi) {
				u8 t; proc.mmu.template load<P,u8>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lhu:
			if (rvi) {
				u16 t; proc.mmu.template load<P,u16>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sb:
			if (rvi) {
				proc.mmu.template store<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, s8(proc.ireg[dec.rs2]));
			};
			break;
		case rv_op_sh:
			if (rvi) {
				proc.mmu.template store<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, s16(proc.ireg[dec.rs2]));
			};
			break;
		case rv_op_sw:
			if (rvi) {
				proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case rv_op_addi:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val + sx(dec.imm);
			};
			break;
		case rv_op_slti:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val < sx(dec.imm);
			};
			break;
		case rv_op_sltiu:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val < ux(dec.imm);
			};
			break;
		case rv_op_xori:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val ^ ux(dec.imm);
			};
			break;
		case rv_op_ori:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val | ux(dec.imm);
			};
			break;
		case rv_op_andi:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val & ux(dec.imm);
			};
			break;
		case rv_op_slli:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val << dec.imm;
			};
			break;
		case rv_op_srli:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val >> dec.imm;
			};
			break;
		case rv_op_srai:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val >> dec.imm;
			};
			break;
		case rv_op_add:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val + proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sub:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val - proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sll:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val << (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_slt:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sltu:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_xor:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val ^ proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_srl:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_sra:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_or:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val | proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_and:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val & proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_mul:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val * proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_mulh:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulh(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.x.val);
			};
			break;
		case rv_op_mulhsu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulhsu(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_mulhu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulhu(proc.ireg[dec.rs1].r.xu.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_div:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? std::numeric_limits<sx>::min() : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : proc.ireg[dec.rs1].r.x.val / proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_divu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : sx(proc.ireg[dec.rs1].r.xu.val / proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_rem:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : proc.ireg[dec.rs1].r.x.val % proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_remu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : sx(proc.ireg[dec.rs1].r.xu.val % proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_lr_w:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1], t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sc_w:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : res;
			};
			break;
		case rv_op_amoswap_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoswap, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoadd_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoadd, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoxor_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoxor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoor_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoand_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoand, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomin_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomin, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomax_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomax, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amominu_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amominu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomaxu_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomaxu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_flw:
			if (rvf) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.wu.val = t;
			};
			break;
		case rv_op_fsw:
			if (rvf) {
				proc.mmu.template store<P,f32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.s.val);
			};
			break;
		case rv_op_fmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fnmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * -proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fnmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * -proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val + proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val - proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fmul_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fdiv_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val / proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsgnj_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fsgnjn_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (~proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fsgnjx_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fmin_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val) || std::isnan(proc.freg[dec.rs2].r.s.val) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fmax_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val > proc.freg[dec.rs2].r.s.val) || std::isnan(proc.freg[dec.rs2].r.s.val) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsqrt_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = riscv::f32_sqrt(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fle_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_flt_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_feq_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fcvt_w_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_wu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_s_w:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case rv_op_fcvt_s_wu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case rv_op_fmv_x_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : std::isnan(proc.freg[dec.rs1].r.s.val) ? s32(0x7fc00000) : proc.freg[dec.rs1].r.w.val;
			};
			break;
		case rv_op_fclass_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : f32_classify(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fmv_s_x:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.ireg[dec.rs1].r.wu.val;
			};
			break;
		case rv_op_fld:
			if (rvd) {
				u64 t; proc.mmu.template load<P,u64>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.lu.val = t;
			};
			break;
		case rv_op_fsd:
			if (rvd) {
				proc.mmu.template store<P,f64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.d.val);
			};
			break;
		case rv_op_fmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fnmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * -proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fnmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * -proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val + proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val - proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fmul_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fdiv_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val / proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fsgnj_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fsgnjn_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (~proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fsgnjx_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fmin_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val) || std::isnan(proc.freg[dec.rs2].r.d.val) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fmax_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val > proc.freg[dec.rs2].r.d.val) || std::isnan(proc.freg[dec.rs2].r.d.val) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fcvt_s_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_d_s:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fsqrt_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = riscv::f64_sqrt(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fle_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_flt_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_feq_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fcvt_w_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_wu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_d_w:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case rv_op_fcvt_d_wu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case rv_op_fclass_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : f64_classify(proc.freg[dec.rs1].r.d.val);
			};
			break;
		default: return -1; /* illegal instruction */
	}
	return pc_offset;
}

/* Execute Instruction RV64 */

template <bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvq, bool rvc, typename T, typename P>
typename P::ux exec_inst_rv64(T &dec, P &proc, typename P::ux pc_offset)
{
	using namespace riscv;
	enum { xlen = 64 };
	typedef s64 sx;
	typedef u64 ux;

	switch (dec.op) {
		case rv_op_lui:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : dec.imm;
			};
			break;
		case rv_op_auipc:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + dec.imm;
			};
			break;
		case rv_op_jal:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + pc_offset; pc_offset = dec.imm;
			};
			break;
		case rv_op_jalr:
			if (rvi) {
				ux new_offset = (proc.ireg[dec.rs1] + dec.imm - proc.pc) & ~1; proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + pc_offset; pc_offset = new_offset;
			};
			break;
		case rv_op_beq:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bne:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_blt:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bge:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bltu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bgeu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_lb:
			if (rvi) {
				s8 t; proc.mmu.template load<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lh:
			if (rvi) {
				s16 t; proc.mmu.template load<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lw:
			if (rvi) {
				s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lbu:
			if (rvi) {
				u8 t; proc.mmu.template load<P,u8>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lhu:
			if (rvi) {
				u16 t; proc.mmu.template load<P,u16>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sb:
			if (rvi) {
				proc.mmu.template store<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, s8(proc.ireg[dec.rs2]));
			};
			break;
		case rv_op_sh:
			if (rvi) {
				proc.mmu.template store<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, s16(proc.ireg[dec.rs2]));
			};
			break;
		case rv_op_sw:
			if (rvi) {
				proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case rv_op_addi:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val + sx(dec.imm);
			};
			break;
		case rv_op_slti:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val < sx(dec.imm);
			};
			break;
		case rv_op_sltiu:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val < ux(dec.imm);
			};
			break;
		case rv_op_xori:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val ^ ux(dec.imm);
			};
			break;
		case rv_op_ori:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val | ux(dec.imm);
			};
			break;
		case rv_op_andi:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val & ux(dec.imm);
			};
			break;
		case rv_op_add:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val + proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sub:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val - proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sll:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val << (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_slt:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sltu:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_xor:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val ^ proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_srl:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_sra:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_or:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val | proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_and:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val & proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_lwu:
			if (rvi) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_ld:
			if (rvi) {
				s64 t; proc.mmu.template load<P,s64>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sd:
			if (rvi) {
				proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.l.val);
			};
			break;
		case rv_op_slli:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val << dec.imm;
			};
			break;
		case rv_op_srli:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val >> dec.imm;
			};
			break;
		case rv_op_srai:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val >> dec.imm;
			};
			break;
		case rv_op_addiw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val + dec.imm);
			};
			break;
		case rv_op_slliw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val << dec.imm);
			};
			break;
		case rv_op_srliw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val >> dec.imm);
			};
			break;
		case rv_op_sraiw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.w.val >> dec.imm;
			};
			break;
		case rv_op_addw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val + proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case rv_op_subw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val - proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case rv_op_sllw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val << (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case rv_op_srlw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val >> (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case rv_op_sraw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val >> (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case rv_op_mul:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val * proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_mulh:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulh(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.x.val);
			};
			break;
		case rv_op_mulhsu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulhsu(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_mulhu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulhu(proc.ireg[dec.rs1].r.xu.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_div:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? std::numeric_limits<sx>::min() : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : proc.ireg[dec.rs1].r.x.val / proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_divu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : sx(proc.ireg[dec.rs1].r.xu.val / proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_rem:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : proc.ireg[dec.rs1].r.x.val % proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_remu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : sx(proc.ireg[dec.rs1].r.xu.val % proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_mulw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val * proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case rv_op_divw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.w.val == std::numeric_limits<s32>::min() && proc.ireg[dec.rs2].r.w.val == -1 ? std::numeric_limits<s32>::min() : proc.ireg[dec.rs2].r.w.val == 0 ? -1 : proc.ireg[dec.rs1].r.w.val / proc.ireg[dec.rs2].r.w.val;
			};
			break;
		case rv_op_divuw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.w.val == 0 ? -1 : s32(proc.ireg[dec.rs1].r.wu.val / proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case rv_op_remw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.w.val == std::numeric_limits<s32>::min() && proc.ireg[dec.rs2].r.w.val == -1 ? 0 : proc.ireg[dec.rs2].r.w.val == 0 ? proc.ireg[dec.rs1].r.w.val : proc.ireg[dec.rs1].r.w.val % proc.ireg[dec.rs2].r.w.val;
			};
			break;
		case rv_op_remuw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.w.val == 0 ? proc.ireg[dec.rs1].r.w.val : s32(proc.ireg[dec.rs1].r.wu.val % proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case rv_op_lr_w:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1], t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sc_w:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : res;
			};
			break;
		case rv_op_amoswap_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoswap, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoadd_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoadd, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoxor_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoxor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoor_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoand_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoand, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomin_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomin, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomax_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomax, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amominu_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amominu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomaxu_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomaxu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_lr_d:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s64 t; proc.mmu.template load<P,s64>(proc, proc.ireg[dec.rs1], t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sc_d:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : res;
			};
			break;
		case rv_op_amoswap_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoswap, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoadd_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoadd, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoxor_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoxor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoor_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoand_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoand, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomin_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amomin, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomax_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amomax, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amominu_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amominu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomaxu_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amomaxu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_flw:
			if (rvf) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.wu.val = t;
			};
			break;
		case rv_op_fsw:
			if (rvf) {
				proc.mmu.template store<P,f32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.s.val);
			};
			break;
		case rv_op_fmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fnmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * -proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fnmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * -proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val + proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val - proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fmul_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fdiv_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val / proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsgnj_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fsgnjn_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (~proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fsgnjx_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fmin_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val) || std::isnan(proc.freg[dec.rs2].r.s.val) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fmax_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val > proc.freg[dec.rs2].r.s.val) || std::isnan(proc.freg[dec.rs2].r.s.val) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsqrt_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = riscv::f32_sqrt(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fle_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_flt_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_feq_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fcvt_w_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_wu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_s_w:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case rv_op_fcvt_s_wu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case rv_op_fmv_x_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : std::isnan(proc.freg[dec.rs1].r.s.val) ? s32(0x7fc00000) : proc.freg[dec.rs1].r.w.val;
			};
			break;
		case rv_op_fclass_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : f32_classify(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fmv_s_x:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.ireg[dec.rs1].r.wu.val;
			};
			break;
		case rv_op_fcvt_l_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_l(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_lu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_lu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_s_l:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.l.val);
			};
			break;
		case rv_op_fcvt_s_lu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.lu.val);
			};
			break;
		case rv_op_fld:
			if (rvd) {
				u64 t; proc.mmu.template load<P,u64>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.lu.val = t;
			};
			break;
		case rv_op_fsd:
			if (rvd) {
				proc.mmu.template store<P,f64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.d.val);
			};
			break;
		case rv_op_fmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fnmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * -proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fnmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * -proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val + proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val - proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fmul_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fdiv_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val / proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fsgnj_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fsgnjn_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (~proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fsgnjx_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fmin_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val) || std::isnan(proc.freg[dec.rs2].r.d.val) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fmax_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val > proc.freg[dec.rs2].r.d.val) || std::isnan(proc.freg[dec.rs2].r.d.val) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fcvt_s_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_d_s:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fsqrt_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = riscv::f64_sqrt(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fle_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_flt_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_feq_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fcvt_w_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_wu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_d_w:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case rv_op_fcvt_d_wu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case rv_op_fclass_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : f64_classify(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_l_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_l(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_lu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_lu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fmv_x_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : std::isnan(proc.freg[dec.rs1].r.d.val) ? s64(0x7ff8000000000000ULL) : proc.freg[dec.rs1].r.l.val;
			};
			break;
		case rv_op_fcvt_d_l:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.l.val);
			};
			break;
		case rv_op_fcvt_d_lu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.lu.val);
			};
			break;
		case rv_op_fmv_d_x:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.ireg[dec.rs1].r.lu.val;
			};
			break;
		default: return -1; /* illegal instruction */
	}
	return pc_offset;
}

/* Execute Instruction RV128 */

template <bool rvi, bool rvm, bool rva, bool rvs, bool rvf, bool rvd, bool rvq, bool rvc, typename T, typename P>
typename P::ux exec_inst_rv128(T &dec, P &proc, typename P::ux pc_offset)
{
	using namespace riscv;
	enum { xlen = 128 };
	typedef s128 sx;
	typedef u128 ux;

	switch (dec.op) {
		case rv_op_lui:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : dec.imm;
			};
			break;
		case rv_op_auipc:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + dec.imm;
			};
			break;
		case rv_op_jal:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + pc_offset; pc_offset = dec.imm;
			};
			break;
		case rv_op_jalr:
			if (rvi) {
				ux new_offset = (proc.ireg[dec.rs1] + dec.imm - proc.pc) & ~1; proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.pc + pc_offset; pc_offset = new_offset;
			};
			break;
		case rv_op_beq:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val == proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bne:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val != proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_blt:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bge:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.x.val >= proc.ireg[dec.rs2].r.x.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bltu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_bgeu:
			if (rvi) {
				if (proc.ireg[dec.rs1].r.xu.val >= proc.ireg[dec.rs2].r.xu.val) pc_offset = dec.imm;
			};
			break;
		case rv_op_lb:
			if (rvi) {
				s8 t; proc.mmu.template load<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lh:
			if (rvi) {
				s16 t; proc.mmu.template load<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lw:
			if (rvi) {
				s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lbu:
			if (rvi) {
				u8 t; proc.mmu.template load<P,u8>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_lhu:
			if (rvi) {
				u16 t; proc.mmu.template load<P,u16>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sb:
			if (rvi) {
				proc.mmu.template store<P,s8>(proc, proc.ireg[dec.rs1] + dec.imm, s8(proc.ireg[dec.rs2]));
			};
			break;
		case rv_op_sh:
			if (rvi) {
				proc.mmu.template store<P,s16>(proc, proc.ireg[dec.rs1] + dec.imm, s16(proc.ireg[dec.rs2]));
			};
			break;
		case rv_op_sw:
			if (rvi) {
				proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case rv_op_addi:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val + sx(dec.imm);
			};
			break;
		case rv_op_slti:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val < sx(dec.imm);
			};
			break;
		case rv_op_sltiu:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val < ux(dec.imm);
			};
			break;
		case rv_op_xori:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val ^ ux(dec.imm);
			};
			break;
		case rv_op_ori:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val | ux(dec.imm);
			};
			break;
		case rv_op_andi:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val & ux(dec.imm);
			};
			break;
		case rv_op_add:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val + proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sub:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val - proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sll:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val << (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_slt:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val < proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_sltu:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val < proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_xor:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val ^ proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_srl:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_sra:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val >> (proc.ireg[dec.rs2] & 0b1111111);
			};
			break;
		case rv_op_or:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val | proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_and:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val & proc.ireg[dec.rs2].r.xu.val;
			};
			break;
		case rv_op_lwu:
			if (rvi) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_ld:
			if (rvi) {
				s64 t; proc.mmu.template load<P,s64>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sd:
			if (rvi) {
				proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.ireg[dec.rs2].r.l.val);
			};
			break;
		case rv_op_addiw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val + dec.imm);
			};
			break;
		case rv_op_slliw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val << dec.imm);
			};
			break;
		case rv_op_srliw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val >> dec.imm);
			};
			break;
		case rv_op_sraiw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.w.val >> dec.imm;
			};
			break;
		case rv_op_addw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val + proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case rv_op_subw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val - proc.ireg[dec.rs2].r.w.val);
			};
			break;
		case rv_op_sllw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val << (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case rv_op_srlw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val >> (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case rv_op_sraw:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.w.val >> (proc.ireg[dec.rs2] & 0b11111));
			};
			break;
		case rv_op_slli:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val << dec.imm;
			};
			break;
		case rv_op_srli:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.xu.val >> dec.imm;
			};
			break;
		case rv_op_srai:
			if (rvi) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val >> dec.imm;
			};
			break;
		case rv_op_mul:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val * proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_mulh:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulh(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.x.val);
			};
			break;
		case rv_op_mulhsu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulhsu(proc.ireg[dec.rs1].r.x.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_mulhu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::mulhu(proc.ireg[dec.rs1].r.xu.val, proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_div:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? std::numeric_limits<sx>::min() : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : proc.ireg[dec.rs1].r.x.val / proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_divu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? -1 : sx(proc.ireg[dec.rs1].r.xu.val / proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_rem:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.x.val == std::numeric_limits<sx>::min() && proc.ireg[dec.rs2].r.x.val == -1 ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : proc.ireg[dec.rs1].r.x.val % proc.ireg[dec.rs2].r.x.val;
			};
			break;
		case rv_op_remu:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.x.val == 0 ? proc.ireg[dec.rs1].r.x.val : sx(proc.ireg[dec.rs1].r.xu.val % proc.ireg[dec.rs2].r.xu.val);
			};
			break;
		case rv_op_mulw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : s32(proc.ireg[dec.rs1].r.wu.val * proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case rv_op_divw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.w.val == std::numeric_limits<s32>::min() && proc.ireg[dec.rs2].r.w.val == -1 ? std::numeric_limits<s32>::min() : proc.ireg[dec.rs2].r.w.val == 0 ? -1 : proc.ireg[dec.rs1].r.w.val / proc.ireg[dec.rs2].r.w.val;
			};
			break;
		case rv_op_divuw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.w.val == 0 ? -1 : s32(proc.ireg[dec.rs1].r.wu.val / proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case rv_op_remw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs1].r.w.val == std::numeric_limits<s32>::min() && proc.ireg[dec.rs2].r.w.val == -1 ? 0 : proc.ireg[dec.rs2].r.w.val == 0 ? proc.ireg[dec.rs1].r.w.val : proc.ireg[dec.rs1].r.w.val % proc.ireg[dec.rs2].r.w.val;
			};
			break;
		case rv_op_remuw:
			if (rvm) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.ireg[dec.rs2].r.w.val == 0 ? proc.ireg[dec.rs1].r.w.val : s32(proc.ireg[dec.rs1].r.wu.val % proc.ireg[dec.rs2].r.wu.val);
			};
			break;
		case rv_op_lr_w:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s32 t; proc.mmu.template load<P,s32>(proc, proc.ireg[dec.rs1], t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sc_w:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s32>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.w.val); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : res;
			};
			break;
		case rv_op_amoswap_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoswap, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoadd_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoadd, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoxor_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoxor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoor_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoand_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amoand, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomin_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomin, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomax_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomax, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amominu_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amominu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomaxu_w:
			if (rva) {
				s32 t1, t2 = proc.ireg[dec.rs2].r.w.val; proc.mmu.template amo<P,s32>(proc, amomaxu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_lr_d:
			if (rva) {
				proc.lr = proc.ireg[dec.rs1]; s64 t; proc.mmu.template load<P,s64>(proc, proc.ireg[dec.rs1], t); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t;
			};
			break;
		case rv_op_sc_d:
			if (rva) {
				ux res = 0; if (proc.lr != proc.ireg[dec.rs1]) res = 1; else proc.mmu.template store<P,s64>(proc, proc.ireg[dec.rs1], proc.ireg[dec.rs2].r.l.val); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : res;
			};
			break;
		case rv_op_amoswap_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoswap, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoadd_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoadd, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoxor_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoxor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoor_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoor, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amoand_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amoand, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomin_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amomin, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomax_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amomax, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amominu_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amominu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_amomaxu_d:
			if (rva) {
				s64 t1, t2 = proc.ireg[dec.rs2].r.l.val; proc.mmu.template amo<P,s64>(proc, amomaxu, proc.ireg[dec.rs1], t1, t2); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : t1;
			};
			break;
		case rv_op_flw:
			if (rvf) {
				u32 t; proc.mmu.template load<P,u32>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.wu.val = t;
			};
			break;
		case rv_op_fsw:
			if (rvf) {
				proc.mmu.template store<P,f32>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.s.val);
			};
			break;
		case rv_op_fmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fnmsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * -proc.freg[dec.rs2].r.s.val + proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fnmadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * -proc.freg[dec.rs2].r.s.val - proc.freg[dec.rs3].r.s.val;
			};
			break;
		case rv_op_fadd_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val + proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsub_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val - proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fmul_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val * proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fdiv_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = proc.freg[dec.rs1].r.s.val / proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsgnj_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fsgnjn_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = (proc.freg[dec.rs1].r.wu.val & u32(~(1U<<31))) | (~proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fsgnjx_s:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.freg[dec.rs1].r.wu.val ^ (proc.freg[dec.rs2].r.wu.val & u32(1U<<31));
			};
			break;
		case rv_op_fmin_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val) || std::isnan(proc.freg[dec.rs2].r.s.val) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fmax_s:
			if (rvf) {
				proc.freg[dec.rd].r.s.val = (proc.freg[dec.rs1].r.s.val > proc.freg[dec.rs2].r.s.val) || std::isnan(proc.freg[dec.rs2].r.s.val) ? proc.freg[dec.rs1].r.s.val : proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fsqrt_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = riscv::f32_sqrt(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fle_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val <= proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_flt_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val < proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_feq_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.s.val == proc.freg[dec.rs2].r.s.val;
			};
			break;
		case rv_op_fcvt_w_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_wu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_s_w:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case rv_op_fcvt_s_wu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case rv_op_fmv_x_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : std::isnan(proc.freg[dec.rs1].r.s.val) ? s32(0x7fc00000) : proc.freg[dec.rs1].r.w.val;
			};
			break;
		case rv_op_fclass_s:
			if (rvf) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : f32_classify(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fmv_s_x:
			if (rvf) {
				proc.freg[dec.rd].r.wu.val = proc.ireg[dec.rs1].r.wu.val;
			};
			break;
		case rv_op_fcvt_l_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_l(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_lu_s:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_lu(proc.fcsr, proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fcvt_s_l:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.l.val);
			};
			break;
		case rv_op_fcvt_s_lu:
			if (rvf) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.ireg[dec.rs1].r.lu.val);
			};
			break;
		case rv_op_fld:
			if (rvd) {
				u64 t; proc.mmu.template load<P,u64>(proc, proc.ireg[dec.rs1] + dec.imm, t); proc.freg[dec.rd].r.lu.val = t;
			};
			break;
		case rv_op_fsd:
			if (rvd) {
				proc.mmu.template store<P,f64>(proc, proc.ireg[dec.rs1] + dec.imm, proc.freg[dec.rs2].r.d.val);
			};
			break;
		case rv_op_fmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fnmsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * -proc.freg[dec.rs2].r.d.val + proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fnmadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * -proc.freg[dec.rs2].r.d.val - proc.freg[dec.rs3].r.d.val;
			};
			break;
		case rv_op_fadd_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val + proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fsub_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val - proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fmul_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val * proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fdiv_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = proc.freg[dec.rs1].r.d.val / proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fsgnj_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fsgnjn_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = (proc.freg[dec.rs1].r.lu.val & u64(~(1ULL<<63))) | (~proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fsgnjx_d:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.freg[dec.rs1].r.lu.val ^ (proc.freg[dec.rs2].r.lu.val & u64(1ULL<<63));
			};
			break;
		case rv_op_fmin_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val) || std::isnan(proc.freg[dec.rs2].r.d.val) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fmax_d:
			if (rvd) {
				proc.freg[dec.rd].r.d.val = (proc.freg[dec.rs1].r.d.val > proc.freg[dec.rs2].r.d.val) || std::isnan(proc.freg[dec.rs2].r.d.val) ? proc.freg[dec.rs1].r.d.val : proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fcvt_s_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.s.val = f32(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_d_s:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.freg[dec.rs1].r.s.val);
			};
			break;
		case rv_op_fsqrt_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = riscv::f64_sqrt(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fle_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val <= proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_flt_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val < proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_feq_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : proc.freg[dec.rs1].r.d.val == proc.freg[dec.rs2].r.d.val;
			};
			break;
		case rv_op_fcvt_w_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_w(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_wu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_wu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_d_w:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.w.val);
			};
			break;
		case rv_op_fcvt_d_wu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.wu.val);
			};
			break;
		case rv_op_fclass_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : f64_classify(proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_l_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_l(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fcvt_lu_d:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : riscv::fcvt_lu(proc.fcsr, proc.freg[dec.rs1].r.d.val);
			};
			break;
		case rv_op_fmv_x_d:
			if (rvd) {
				proc.ireg[dec.rd] = (dec.rd == 0) ? 0 : std::isnan(proc.freg[dec.rs1].r.d.val) ? s64(0x7ff8000000000000ULL) : proc.freg[dec.rs1].r.l.val;
			};
			break;
		case rv_op_fcvt_d_l:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.l.val);
			};
			break;
		case rv_op_fcvt_d_lu:
			if (rvd) {
				fenv_setrm((proc.fcsr >> 5) & 0b111); proc.freg[dec.rd].r.d.val = f64(proc.ireg[dec.rs1].r.lu.val);
			};
			break;
		case rv_op_fmv_d_x:
			if (rvd) {
				proc.freg[dec.rd].r.lu.val = proc.ireg[dec.rs1].r.lu.val;
			};
			break;
		default: return -1; /* illegal instruction */
	}
	return pc_offset;
}

#endif
