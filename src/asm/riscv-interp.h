//
//  riscv-interp.h
//
//  DANGER - This is machine generated code
//

#ifndef riscv_interp_h
#define riscv_interp_h

template <typename T>
bool rv32_exec(T &dec, riscv_proc_rv32 &proc, uintptr_t next_pc)
{
	enum { xlen = 32 };
	typedef int32_t sx;
	typedef uint32_t ux;

	switch (dec.op) {
		case riscv_op_lui: {
			proc.ireg[dec.rd] = dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_auipc: {
			proc.ireg[dec.rd] = proc.pc + dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_jal: {
			proc.ireg[dec.rd] = proc.pc + 4; proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_jalr: {
			proc.ireg[dec.rd] = proc.pc + 4; proc.pc = proc.ireg[dec.rs1] + dec.imm;
			goto x;
		};
		case riscv_op_beq: {
			if (sx(proc.ireg[dec.rs1]) == sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bne: {
			if (sx(proc.ireg[dec.rs1]) != sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_blt: {
			if (sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bge: {
			if (sx(proc.ireg[dec.rs1]) >= sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bltu: {
			if (ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bgeu: {
			if (ux(proc.ireg[dec.rs1]) >= ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_lb: {
			proc.ireg[dec.rd] = sx(*(s8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lh: {
			proc.ireg[dec.rd] = sx(*(s16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lw: {
			proc.ireg[dec.rd] = sx(*(s32*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lbu: {
			proc.ireg[dec.rd] = ux(*(u8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lhu: {
			proc.ireg[dec.rd] = ux(*(u16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sb: {
			*((u8*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sh: {
			*((u16*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sw: {
			*((u32*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_addi: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_slti: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(dec.imm) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sltiu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(dec.imm) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_xori: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_ori: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_andi: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_slli_rv32i: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srli_rv32i: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srai_rv32i: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_add: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sub: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) - sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sll: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_slt: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sltu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_xor: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srl: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sra: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_or: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_and: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mul: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mulh: {
			proc.ireg[dec.rd] = (sx(proc.ireg[dec.rs1]) * sx(proc.ireg[dec.rs2])) >> xlen;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mulhsu: {
			proc.ireg[dec.rd] = (sx(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2])) >> xlen;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mulhu: {
			proc.ireg[dec.rd] = (ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2])) >> xlen;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_div: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) / sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_divu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) / ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_rem: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) % sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_remu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) % ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lr_w: {
			proc.state.lr = proc.ireg[dec.rs1]; proc.ireg[dec.rd] = sx(*((s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sc_w: {
			if (proc.state.lr == proc.ireg[dec.rs1]) *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoswap_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]); proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoadd_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = u32(proc.ireg[dec.rs2]) + t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoxor_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) ^ t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoor_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) | t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoand_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) & t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomin_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) < t ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomax_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) > t ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amominu_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = u32(proc.ireg[dec.rs2]) < u32(t) ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomaxu_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = u32(proc.ireg[dec.rs2]) > u32(t) ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_flw: {
			proc.freg[dec.rd] = *(f32*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsw: {
			*(f32*)(proc.ireg[dec.rs1] + dec.imm) = f32(proc.freg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmadd_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) + f32(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmsub_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) - f32(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmsub_s: {
			proc.freg[dec.rd] = -(f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) + f32(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmadd_s: {
			proc.freg[dec.rd] = -(f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) - f32(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fadd_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) + f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsub_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) - f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmul_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) * f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fdiv_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) / f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnj_s: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u32(~(1U<<31))) | (proc.freg[dec.rs2] & u32(1U<<31));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjn_s: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u32(~(1U<<31))) | ~(proc.freg[dec.rs2] & u32(1U<<31));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjx_s: {
			proc.freg[dec.rd] = proc.freg[dec.rs1] ^ (proc.freg[dec.rs2] & u32(1U<<31));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmin_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) < f32(proc.freg[dec.rs1]) ? f32(proc.freg[dec.rs2]) : f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmax_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) > f32(proc.freg[dec.rs1]) ? f32(proc.freg[dec.rs2]) : f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsqrt_s: {
			proc.freg[dec.rd] = f32_sqrt(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fle_s: {
			proc.ireg[dec.rd] = f32(proc.freg[dec.rs1]) <= f32(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_flt_s: {
			proc.ireg[dec.rd] = f32(proc.freg[dec.rs1]) < f32(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_feq_s: {
			proc.ireg[dec.rd] = f32(proc.freg[dec.rs1]) == f32(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_w_s: {
			proc.ireg[dec.rd] = s32(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_wu_s: {
			proc.ireg[dec.rd] = u32(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_w: {
			proc.freg[dec.rd] = f32(s32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_wu: {
			proc.freg[dec.rd] = f32(u32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmv_x_s: {
			proc.ireg[dec.rd] = u32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fclass_s: {
			proc.ireg[dec.rd] = f32_classify(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmv_s_x: {
			proc.freg[dec.rd] = s32(proc.ireg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fld: {
			proc.freg[dec.rd] = *(f64*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsd: {
			*(f64*)(proc.ireg[dec.rs1] + dec.imm) = f64(proc.freg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmadd_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) + f64(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmsub_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) - f64(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmsub_d: {
			proc.freg[dec.rd] = -(f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) + f64(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmadd_d: {
			proc.freg[dec.rd] = -(f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) - f64(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fadd_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) + f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsub_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) - f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmul_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) * f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fdiv_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) / f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnj_d: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u64(~(1ULL<<63))) | (proc.freg[dec.rs2] & u64(1ULL<<63));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjn_d: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u64(~(1ULL<<63))) | ~(proc.freg[dec.rs2] & u64(1ULL<<63));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjx_d: {
			proc.freg[dec.rd] = proc.freg[dec.rs1] ^ (proc.freg[dec.rs2] & u64(1ULL<<63));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmin_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) < f64(proc.freg[dec.rs1]) ? f64(proc.freg[dec.rs2]) : f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmax_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) > f64(proc.freg[dec.rs1]) ? f64(proc.freg[dec.rs2]) : f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_d: {
			proc.freg[dec.rd] = f32(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_s: {
			proc.freg[dec.rd] = f64(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsqrt_d: {
			proc.freg[dec.rd] = f64_sqrt(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fle_d: {
			proc.ireg[dec.rd] = f64(proc.freg[dec.rs1]) <= f64(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_flt_d: {
			proc.ireg[dec.rd] = f64(proc.freg[dec.rs1]) < f64(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_feq_d: {
			proc.ireg[dec.rd] = f64(proc.freg[dec.rs1]) == f64(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_w_d: {
			proc.ireg[dec.rd] = s32(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_wu_d: {
			proc.ireg[dec.rd] = u32(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_w: {
			proc.freg[dec.rd] = f64(s32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_wu: {
			proc.freg[dec.rd] = f64(u32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fclass_d: {
			proc.ireg[dec.rd] = f64_classify(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		default:
			break;
	}
	return false; /* illegal instruction */
x:	return true;
}

template <typename T>
bool rv64_exec(T &dec, riscv_proc_rv64 &proc, uintptr_t next_pc)
{
	enum { xlen = 64 };
	typedef int64_t sx;
	typedef uint64_t ux;

	switch (dec.op) {
		case riscv_op_lui: {
			proc.ireg[dec.rd] = dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_auipc: {
			proc.ireg[dec.rd] = proc.pc + dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_jal: {
			proc.ireg[dec.rd] = proc.pc + 4; proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_jalr: {
			proc.ireg[dec.rd] = proc.pc + 4; proc.pc = proc.ireg[dec.rs1] + dec.imm;
			goto x;
		};
		case riscv_op_beq: {
			if (sx(proc.ireg[dec.rs1]) == sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bne: {
			if (sx(proc.ireg[dec.rs1]) != sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_blt: {
			if (sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bge: {
			if (sx(proc.ireg[dec.rs1]) >= sx(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bltu: {
			if (ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_bgeu: {
			if (ux(proc.ireg[dec.rs1]) >= ux(proc.ireg[dec.rs2])) proc.pc = proc.pc + dec.imm;
			goto x;
		};
		case riscv_op_lb: {
			proc.ireg[dec.rd] = sx(*(s8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lh: {
			proc.ireg[dec.rd] = sx(*(s16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lw: {
			proc.ireg[dec.rd] = sx(*(s32*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lbu: {
			proc.ireg[dec.rd] = ux(*(u8*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lhu: {
			proc.ireg[dec.rd] = ux(*(u16*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sb: {
			*((u8*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sh: {
			*((u16*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sw: {
			*((u32*)(proc.ireg[dec.rs1] + dec.imm)) = proc.ireg[dec.rs2];
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_addi: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_slti: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(dec.imm) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sltiu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(dec.imm) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_xori: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_ori: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_andi: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_add: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) + sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sub: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) - sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sll: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_slt: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) < sx(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sltu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) < ux(proc.ireg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_xor: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) ^ ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srl: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sra: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_or: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) | ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_and: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) & ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lwu: {
			proc.ireg[dec.rd] = ux(*(u32*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_ld: {
			proc.ireg[dec.rd] = sx(*(s64*)(proc.ireg[dec.rs1] + dec.imm));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sd: {
			*(u64*)(proc.ireg[dec.rs1] + dec.imm) = proc.ireg[dec.rs2];
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_slli_rv64i: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) << dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srli_rv64i: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srai_rv64i: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_addiw: {
			proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) + s32(dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_slliw: {
			proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) << dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srliw: {
			proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sraiw: {
			proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) >> dec.imm;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_addw: {
			proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) + s32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_subw: {
			proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) - s32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sllw: {
			proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) << (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_srlw: {
			proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sraw: {
			proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) >> (proc.ireg[dec.rs2] & 0b111111);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mul: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mulh: {
			proc.ireg[dec.rd] = (sx(proc.ireg[dec.rs1]) * sx(proc.ireg[dec.rs2])) >> xlen;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mulhsu: {
			proc.ireg[dec.rd] = (sx(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2])) >> xlen;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mulhu: {
			proc.ireg[dec.rd] = (ux(proc.ireg[dec.rs1]) * ux(proc.ireg[dec.rs2])) >> xlen;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_div: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) / sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_divu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) / ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_rem: {
			proc.ireg[dec.rd] = sx(proc.ireg[dec.rs1]) % sx(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_remu: {
			proc.ireg[dec.rd] = ux(proc.ireg[dec.rs1]) % ux(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_mulw: {
			proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) * u32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_divw: {
			proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) / s32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_divuw: {
			proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) / u32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_remw: {
			proc.ireg[dec.rd] = s32(proc.ireg[dec.rs1]) % s32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_remuw: {
			proc.ireg[dec.rd] = u32(proc.ireg[dec.rs1]) % u32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lr_w: {
			proc.state.lr = proc.ireg[dec.rs1]; proc.ireg[dec.rd] = sx(*((s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sc_w: {
			if (proc.state.lr == proc.ireg[dec.rs1]) *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoswap_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]); proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoadd_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = u32(proc.ireg[dec.rs2]) + t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoxor_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) ^ t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoor_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) | t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoand_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) & t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomin_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) < t ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomax_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = s32(proc.ireg[dec.rs2]) > t ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amominu_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = u32(proc.ireg[dec.rs2]) < u32(t) ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomaxu_w: {
			s32 t(*(s32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))); *((u32*)(uintptr_t(proc.ireg[dec.rs1].r.wu.val))) = u32(proc.ireg[dec.rs2]) > u32(t) ? s32(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_lr_d: {
			proc.state.lr = proc.ireg[dec.rs1]; proc.ireg[dec.rd] = sx(*((s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_sc_d: {
			if (proc.state.lr == proc.ireg[dec.rs1]) *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoswap_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]); proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoadd_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]) + t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoxor_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]) ^ t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoor_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]) | t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amoand_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]) & t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomin_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]) < t ? s64(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomax_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = s64(proc.ireg[dec.rs2]) > t ? s64(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amominu_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = u64(proc.ireg[dec.rs2]) < u64(t) ? s64(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_amomaxu_d: {
			s64 t(*(s64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))); *((u64*)(uintptr_t(proc.ireg[dec.rs1].r.lu.val))) = u64(proc.ireg[dec.rs2]) > u64(t) ? s64(proc.ireg[dec.rs2]) : t; proc.ireg[dec.rd] = t;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_flw: {
			proc.freg[dec.rd] = *(f32*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsw: {
			*(f32*)(proc.ireg[dec.rs1] + dec.imm) = f32(proc.freg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmadd_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) + f32(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmsub_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) - f32(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmsub_s: {
			proc.freg[dec.rd] = -(f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) + f32(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmadd_s: {
			proc.freg[dec.rd] = -(f32(proc.freg[dec.rs1]) * f32(proc.freg[dec.rs2]) - f32(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fadd_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) + f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsub_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) - f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmul_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) * f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fdiv_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) / f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnj_s: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u32(~(1U<<31))) | (proc.freg[dec.rs2] & u32(1U<<31));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjn_s: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u32(~(1U<<31))) | ~(proc.freg[dec.rs2] & u32(1U<<31));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjx_s: {
			proc.freg[dec.rd] = proc.freg[dec.rs1] ^ (proc.freg[dec.rs2] & u32(1U<<31));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmin_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) < f32(proc.freg[dec.rs1]) ? f32(proc.freg[dec.rs2]) : f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmax_s: {
			proc.freg[dec.rd] = f32(proc.freg[dec.rs2]) > f32(proc.freg[dec.rs1]) ? f32(proc.freg[dec.rs2]) : f32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsqrt_s: {
			proc.freg[dec.rd] = f32_sqrt(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fle_s: {
			proc.ireg[dec.rd] = f32(proc.freg[dec.rs1]) <= f32(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_flt_s: {
			proc.ireg[dec.rd] = f32(proc.freg[dec.rs1]) < f32(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_feq_s: {
			proc.ireg[dec.rd] = f32(proc.freg[dec.rs1]) == f32(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_w_s: {
			proc.ireg[dec.rd] = s32(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_wu_s: {
			proc.ireg[dec.rd] = u32(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_w: {
			proc.freg[dec.rd] = f32(s32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_wu: {
			proc.freg[dec.rd] = f32(u32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmv_x_s: {
			proc.ireg[dec.rd] = u32(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fclass_s: {
			proc.ireg[dec.rd] = f32_classify(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmv_s_x: {
			proc.freg[dec.rd] = s32(proc.ireg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_l_s: {
			proc.ireg[dec.rd] = s64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_lu_s: {
			proc.ireg[dec.rd] = u64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_l: {
			proc.freg[dec.rd] = f32(s64(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_lu: {
			proc.freg[dec.rd] = f32(u64(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fld: {
			proc.freg[dec.rd] = *(f64*)(proc.ireg[dec.rs1] + dec.imm);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsd: {
			*(f64*)(proc.ireg[dec.rs1] + dec.imm) = f64(proc.freg[dec.rs2]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmadd_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) + f64(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmsub_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) - f64(proc.freg[dec.rs3]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmsub_d: {
			proc.freg[dec.rd] = -(f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) + f64(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fnmadd_d: {
			proc.freg[dec.rd] = -(f64(proc.freg[dec.rs1]) * f64(proc.freg[dec.rs2]) - f64(proc.freg[dec.rs3]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fadd_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) + f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsub_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) - f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmul_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) * f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fdiv_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) / f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnj_d: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u64(~(1ULL<<63))) | (proc.freg[dec.rs2] & u64(1ULL<<63));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjn_d: {
			proc.freg[dec.rd] = (proc.freg[dec.rs1] & u64(~(1ULL<<63))) | ~(proc.freg[dec.rs2] & u64(1ULL<<63));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsgnjx_d: {
			proc.freg[dec.rd] = proc.freg[dec.rs1] ^ (proc.freg[dec.rs2] & u64(1ULL<<63));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmin_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) < f64(proc.freg[dec.rs1]) ? f64(proc.freg[dec.rs2]) : f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmax_d: {
			proc.freg[dec.rd] = f64(proc.freg[dec.rs2]) > f64(proc.freg[dec.rs1]) ? f64(proc.freg[dec.rs2]) : f64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_s_d: {
			proc.freg[dec.rd] = f32(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_s: {
			proc.freg[dec.rd] = f64(f32(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fsqrt_d: {
			proc.freg[dec.rd] = f64_sqrt(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fle_d: {
			proc.ireg[dec.rd] = f64(proc.freg[dec.rs1]) <= f64(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_flt_d: {
			proc.ireg[dec.rd] = f64(proc.freg[dec.rs1]) < f64(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_feq_d: {
			proc.ireg[dec.rd] = f64(proc.freg[dec.rs1]) == f64(proc.freg[dec.rs2]) ? 1 : 0;
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_w_d: {
			proc.ireg[dec.rd] = s32(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_wu_d: {
			proc.ireg[dec.rd] = u32(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_w: {
			proc.freg[dec.rd] = f64(s32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_wu: {
			proc.freg[dec.rd] = f64(u32(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fclass_d: {
			proc.ireg[dec.rd] = f64_classify(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_l_d: {
			proc.ireg[dec.rd] = s64(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_lu_d: {
			proc.ireg[dec.rd] = u64(f64(proc.freg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmv_x_d: {
			proc.ireg[dec.rd] = u64(proc.freg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_l: {
			proc.freg[dec.rd] = f64(s64(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fcvt_d_lu: {
			proc.freg[dec.rd] = f64(u64(proc.ireg[dec.rs1]));
			proc.pc = next_pc;
			goto x;
		};
		case riscv_op_fmv_d_x: {
			proc.freg[dec.rd] = s64(proc.ireg[dec.rs1]);
			proc.pc = next_pc;
			goto x;
		};
		default:
			break;
	}
	return false; /* illegal instruction */
x:	return true;
}

#endif
