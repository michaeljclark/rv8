//
//  riscv-mc.h
//

std::string format_string(const char* fmt, ...);
extern void log_prefix(const char* prefix, const char* fmt, va_list arg);
extern void log_fatal_exit(const char* fmt, ...);
extern void log_error(const char* fmt, ...);
extern void log_info(const char* fmt, ...);
extern void log_debug(const char* fmt, ...);

template <typename T, unsigned B>
inline T sign_extend(const T x)
{
	struct {
		T x:B;
	} s;
	return s.x = x;
}


/* Types */

typedef unsigned char* riscv_ptr;
typedef signed char riscv_b;
typedef unsigned char riscv_bu;
typedef signed short riscv_h;
typedef unsigned short riscv_hu;
typedef signed int riscv_w;
typedef unsigned int riscv_wu;
typedef signed long long riscv_l;
typedef unsigned long long riscv_lu;
typedef double riscv_d;
typedef float riscv_s;



/* Registers */

union riscv_i_reg
{
	riscv_l l;
	riscv_lu lu;
	riscv_w w;
	riscv_wu wu;
};

union riscv_f_reg
{
	riscv_d d;
	riscv_s s;
};

enum riscv_ireg_name
{
 	riscv_ireg_zero, /* x0 - Hard-wired zero */
	riscv_ireg_ra,   /* x1 - Return address Caller */
	riscv_ireg_sp,   /* x2 - Stack pointer Callee */
	riscv_ireg_gp,   /* x3 - Global pointer */
	riscv_ireg_tp,   /* x4 - Thread pointer Callee */
	riscv_ireg_t0,   /* x5 - Temporaries Caller */
	riscv_ireg_t1,   /* x6 - Temporaries Caller */
	riscv_ireg_t2,   /* x7 - Temporaries Caller */
	riscv_ireg_s0,   /* x8 - Saved register/frame pointer Callee */
	riscv_ireg_s1,   /* x9 - Saved registers Callee */
	riscv_ireg_a0,   /* x10 - Function arguments Caller */
	riscv_ireg_a1,   /* x11 - Function arguments Caller */
	riscv_ireg_a2,   /* x12 - Function arguments Caller */
	riscv_ireg_a3,   /* x13 - Function arguments Caller */
	riscv_ireg_a4,   /* x14 - Function arguments Caller */
	riscv_ireg_a5,   /* x15 - Function arguments Caller */
	riscv_ireg_a6,   /* x16 - Function arguments Caller */
	riscv_ireg_a7,   /* x17 - Function arguments Caller */
	riscv_ireg_s2,   /* x18 - Saved registers Callee */
	riscv_ireg_s3,   /* x19 - Saved registers Callee */
	riscv_ireg_s4,   /* x20 - Saved registers Callee */
	riscv_ireg_s5,   /* x21 - Saved registers Callee */
	riscv_ireg_s6,   /* x22 - Saved registers Callee */
	riscv_ireg_s7,   /* x23 - Saved registers Callee */
	riscv_ireg_s8,   /* x24 - Saved registers Callee */
	riscv_ireg_s9,   /* x25 - Saved registers Callee */
	riscv_ireg_s10,  /* x26 - Saved registers Callee */
	riscv_ireg_s11,  /* x27 - Saved registers Callee */
	riscv_ireg_t3,   /* x28 - Temporaries Caller */
	riscv_ireg_t4,   /* x29 - Temporaries Caller */
	riscv_ireg_t5,   /* x30 - Temporaries Caller */
	riscv_ireg_t6    /* x31 - Temporaries Caller */
};

enum riscv_freg_name
{
	riscv_freg_ft0,  /* f0 - FP temporaries Caller */
	riscv_freg_ft1,  /* f1 - FP temporaries Caller */
	riscv_freg_ft2,  /* f2 - FP temporaries Caller */
	riscv_freg_ft3,  /* f3 - FP temporaries Caller */
	riscv_freg_ft4,  /* f4 - FP temporaries Caller */
	riscv_freg_ft5,  /* f5 - FP temporaries Caller */
	riscv_freg_ft6,  /* f6 - FP temporaries Caller */
	riscv_freg_ft7,  /* f7 - FP temporaries Caller */
	riscv_freg_fs0,  /* f8 - FP saved registers Callee */
	riscv_freg_fs1,  /* f9 - FP saved registers Callee */
	riscv_freg_fa0,  /* f10 - FP arguments Caller */
	riscv_freg_fa1,  /* f11 - FP arguments Caller */
	riscv_freg_fa2,  /* f12 - FP arguments Caller */
	riscv_freg_fa3,  /* f13 - FP arguments Caller */
	riscv_freg_fa4,  /* f14 - FP arguments Caller */
	riscv_freg_fa5,  /* f15 - FP arguments Caller */
	riscv_freg_fa6,  /* f16 - FP arguments Caller */
	riscv_freg_fa7,  /* f17 - FP arguments Caller */
	riscv_freg_fs2,  /* f18 - FP saved registers Callee */
	riscv_freg_fs3,  /* f19 - FP saved registers Callee */
	riscv_freg_fs4,  /* f20 - FP saved registers Callee */
	riscv_freg_fs5,  /* f21 - FP saved registers Callee */
	riscv_freg_fs6,  /* f22 - FP saved registers Callee */
	riscv_freg_fs7,  /* f23 - FP saved registers Callee */
	riscv_freg_fs8,  /* f24 - FP saved registers Callee */
	riscv_freg_fs9,  /* f25 - FP saved registers Callee */
	riscv_freg_fs10, /* f26 - FP saved registers Callee */
	riscv_freg_fs11, /* f27 - FP saved registers Callee */
	riscv_freg_ft8,  /* f28 - FP temporaries Caller */
	riscv_freg_ft9,  /* f29 - FP temporaries Caller */
	riscv_freg_ft10, /* f30 - FP temporaries Caller */
	riscv_freg_ft11  /* f31 - FP temporaries Caller */
};

/* Instruction sets */

enum riscv_proc_type
{
	riscv_proc_type_rv32i,
	riscv_proc_type_rv32e,
	riscv_proc_type_rv64i,
	riscv_proc_type_rv128i
};

/* Processor state */

struct riscv_proc_state
{
	riscv_ptr pc;
	riscv_i_reg i_reg[31];
	riscv_f_reg f_reg[32];
	riscv_proc_type p_type;
};

/* Instructions */

enum riscv_op
{
	riscv_op_unknown,
	riscv_op_nop,
	riscv_op_beq,
	riscv_op_bne,
	riscv_op_blt,
	riscv_op_bge,
	riscv_op_bltu,
	riscv_op_bgeu,
	riscv_op_jalr,
	riscv_op_jal,
	riscv_op_lui,
	riscv_op_auipc,
	riscv_op_addi,
	riscv_op_slli,
	riscv_op_slti,
	riscv_op_sltiu,
	riscv_op_xori,
	riscv_op_srli,
	riscv_op_srai,
	riscv_op_ori,
	riscv_op_andi,
	riscv_op_add,
	riscv_op_sub,
	riscv_op_sll,
	riscv_op_slt,
	riscv_op_sltu,
	riscv_op_xor,
	riscv_op_srl,
	riscv_op_sra,
	riscv_op_or,
	riscv_op_and,
	riscv_op_addiw,
	riscv_op_slliw,
	riscv_op_srliw,
	riscv_op_sraiw,
	riscv_op_addw,
	riscv_op_subw,
	riscv_op_sllw,
	riscv_op_srlw,
	riscv_op_sraw,
	riscv_op_lb,
	riscv_op_lh,
	riscv_op_lw,
	riscv_op_ld,
	riscv_op_lbu,
	riscv_op_lhu,
	riscv_op_lwu,
	riscv_op_sb,
	riscv_op_sh,
	riscv_op_sw,
	riscv_op_sd,
	riscv_op_fence,
	riscv_op_fence_i,
	riscv_op_mul,
	riscv_op_mulh,
	riscv_op_mulhsu,
	riscv_op_mulhu,
	riscv_op_div,
	riscv_op_divu,
	riscv_op_rem,
	riscv_op_remu,
	riscv_op_mulw,
	riscv_op_divw,
	riscv_op_divuw,
	riscv_op_remw,
	riscv_op_remuw,
	riscv_op_amoadd_w,
	riscv_op_amoxor_w,
	riscv_op_amoor_w,
	riscv_op_amoand_w,
	riscv_op_amomin_w,
	riscv_op_amomax_w,
	riscv_op_amominu_w,
	riscv_op_amomaxu_w,
	riscv_op_amoswap_w,
	riscv_op_lr_w,
	riscv_op_sc_w,
	riscv_op_amoadd_d,
	riscv_op_amoxor_d,
	riscv_op_amoor_d,
	riscv_op_amoand_d,
	riscv_op_amomin_d,
	riscv_op_amomax_d,
	riscv_op_amominu_d,
	riscv_op_amomaxu_d,
	riscv_op_amoswap_d,
	riscv_op_lr_d,
	riscv_op_sc_d,
	riscv_op_scall,
	riscv_op_sbreak,
	riscv_op_sret,
	riscv_op_sfence_vm,
	riscv_op_wfi,
	riscv_op_mrth,
	riscv_op_mrts,
	riscv_op_hrts,
	riscv_op_csrrw,
	riscv_op_csrrs,
	riscv_op_csrrc,
	riscv_op_csrrwi,
	riscv_op_csrrsi,
	riscv_op_csrrci,
	riscv_op_fadd_s,
	riscv_op_fsub_s,
	riscv_op_fmul_s,
	riscv_op_fdiv_s,
	riscv_op_fsgnj_s,
	riscv_op_fsgnjn_s,
	riscv_op_fsgnjx_s,
	riscv_op_fmin_s,
	riscv_op_fmax_s,
	riscv_op_fsqrt_s,
	riscv_op_fadd_d,
	riscv_op_fsub_d,
	riscv_op_fmul_d,
	riscv_op_fdiv_d,
	riscv_op_fsgnj_d,
	riscv_op_fsgnjn_d,
	riscv_op_fsgnjx_d,
	riscv_op_fmin_d,
	riscv_op_fmax_d,
	riscv_op_fcvt_s_d,
	riscv_op_fcvt_d_s,
	riscv_op_fsqrt_d,
	riscv_op_fle_s,
	riscv_op_flt_s,
	riscv_op_feq_s,
	riscv_op_fle_d,
	riscv_op_flt_d,
	riscv_op_feq_d,
	riscv_op_fcvt_w_s,
	riscv_op_fcvt_wu_s,
	riscv_op_fcvt_l_s,
	riscv_op_fcvt_lu_s,
	riscv_op_fmv_x_s,
	riscv_op_fclass_s,
	riscv_op_fcvt_w_d,
	riscv_op_fcvt_wu_d,
	riscv_op_fcvt_l_d,
	riscv_op_fcvt_lu_d,
	riscv_op_fmv_x_d,
	riscv_op_fclass_d,
	riscv_op_fcvt_s_w,
	riscv_op_fcvt_s_wu,
	riscv_op_fcvt_s_l,
	riscv_op_fcvt_s_lu,
	riscv_op_fmv_s_x,
	riscv_op_fcvt_d_w,
	riscv_op_fcvt_d_wu,
	riscv_op_fcvt_d_l,
	riscv_op_fcvt_d_lu,
	riscv_op_fmv_d_x,
	riscv_op_flw,
	riscv_op_fld,
	riscv_op_fsw,
	riscv_op_fsd,
	riscv_op_fmadd_s,
	riscv_op_fmsub_s,
	riscv_op_fnmsub_s,
	riscv_op_fnmadd_s,
	riscv_op_fmadd_d,
	riscv_op_fmsub_d,
	riscv_op_fnmsub_d,
	riscv_op_fnmadd_d,
	riscv_op_c_nop,
	riscv_op_c_addi,
	riscv_op_c_lw,
	riscv_op_c_mv,
	riscv_op_c_bnez,
	riscv_op_c_sw,
	riscv_op_c_ld,
	riscv_op_c_swsp,
	riscv_op_c_lwsp,
	riscv_op_c_li,
	riscv_op_c_add,
	riscv_op_c_srli,
	riscv_op_c_jr,
	riscv_op_c_fld,
	riscv_op_c_sdsp,
	riscv_op_c_j,
	riscv_op_c_ldsp,
	riscv_op_c_andi,
	riscv_op_c_addiw,
	riscv_op_c_slli,
	riscv_op_c_sd,
	riscv_op_c_beqz,
	riscv_op_c_and,
	riscv_op_c_srai,
	riscv_op_c_jal,
	riscv_op_c_addi4spn,
	riscv_op_c_fldsp,
	riscv_op_c_addi16sp,
	riscv_op_c_fsd,
	riscv_op_c_fsdsp,
	riscv_op_c_addw,
	riscv_op_c_xor,
	riscv_op_c_or,
	riscv_op_c_sub,
	riscv_op_c_lui,
	riscv_op_c_jalr,
	riscv_op_c_subw,
	riscv_op_c_ebreak,
	riscv_op_c_flw,
	riscv_op_c_flwsp,
	riscv_op_c_fsw,
	riscv_op_c_fswsp
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
};

/* CSR Permissions */

enum riscv_csr_perm
{
	riscv_csr_perm_none,
	riscv_csr_perm_urw,
	riscv_csr_perm_uro,
	riscv_csr_perm_srw,
	riscv_csr_perm_sro,
	riscv_csr_perm_hrw,
	riscv_csr_perm_hro,
	riscv_csr_perm_mrw,
	riscv_csr_perm_mro
};

/* CSR Table */

struct riscv_csr_metadata
{
	riscv_hu csr_value;
	riscv_csr_perm csr_perm;
	const char* csr_name;
	const char* csr_desc;
};

/* Instruction Formattings */

enum rvf
{
	rvf_z,
	rvf_b,
	rvf_c,
	rvf_d,
	rvf_rd,
	rvf_rs1,
	rvf_rs2,
	rvf_frd,
	rvf_frs1,
	rvf_frs2,
	rvf_frs3,
	rvf_srs1,
	rvf_i,
	rvf_ipc,
	rvf_icsr,
};

/* Compressed Instruction Mapping table */

struct riscv_inst_comp_metadata
{
	riscv_op cop;
	riscv_inst_type ctype;
	riscv_op op;
	riscv_inst_type type;
};

/* Instruction Type Table */

struct riscv_inst_type_metadata
{
	riscv_inst_type type;
	const rvf* fmt;
};

/* Instruction Decode State Machine */

enum riscv_dsm
{
	riscv_dsm_break = 0,      /* break with no opcode */
	riscv_dsm_table_brk = 1,  /* jump table, break if no entry */
	riscv_dsm_table_dfl = 2,  /* jump table, last entry is default */
	riscv_dsm_match = 3,      /* match predicate for next entry */
	riscv_dsm_jump = 4,       /* jump and clear match */
	riscv_dsm_select = 5,     /* select opcode and break */
	riscv_dsm_mask_srl0 = 6,  /* load match |= (inst >> 0) & val */
	riscv_dsm_mask_srl1 = 7,  /* load match |= (inst >> 1) & val */
	riscv_dsm_mask_srl2 = 8,
	riscv_dsm_mask_srl3 = 9,
	riscv_dsm_mask_srl4 = 10,
	riscv_dsm_mask_srl5 = 11,
	riscv_dsm_mask_srl6 = 12,
	riscv_dsm_mask_srl7 = 13,
	riscv_dsm_mask_srl8 = 14,
	riscv_dsm_mask_srl9 = 15,
	riscv_dsm_mask_srl10 = 16,
	riscv_dsm_mask_srl11 = 17,
	riscv_dsm_mask_srl12 = 18,
	riscv_dsm_mask_srl13 = 19,
	riscv_dsm_mask_srl14 = 20,
	riscv_dsm_mask_srl15 = 21,
	riscv_dsm_mask_srl16 = 22,
	riscv_dsm_mask_srl17 = 23,
	riscv_dsm_mask_srl18 = 24,
	riscv_dsm_mask_srl19 = 25,
	riscv_dsm_mask_srl20 = 26,
	riscv_dsm_mask_srl21 = 27,
	riscv_dsm_mask_srl22 = 28,
	riscv_dsm_mask_srl23 = 29,
	riscv_dsm_mask_srl24 = 30,
	riscv_dsm_mask_srl25 = 31,
	riscv_dsm_mask_srl26 = 32,
	riscv_dsm_mask_srl27 = 33,
	riscv_dsm_mask_srl28 = 34,
	riscv_dsm_mask_srl29 = 35,
	riscv_dsm_mask_srl30 = 36,
	riscv_dsm_mask_srl31 = 37
};

struct riscv_dsm_entry
{
	riscv_hu dsm_op;
	riscv_wu dsm_val;
};

/* Metadata Tables */

extern const char* riscv_instructions[];
extern const char* riscv_i_registers[];
extern const char* riscv_f_registers[];
extern const riscv_dsm_entry riscv_dsm_table[];
extern const riscv_csr_metadata riscv_csr_table[];
extern const riscv_inst_comp_metadata riscv_comp_table[];
extern const riscv_inst_type_metadata riscv_inst_type_table[];
extern const rvf rvf_none[];

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

/* Decode CJ */
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

/* Decode R Register */
inline void riscv_decode_r(riscv_decode &dec, riscv_wu inst)
{
	dec.type = riscv_inst_type_r;
	dec.rd = (inst >> 7) & 0b11111;
	dec.rs1 = (inst >> 15) & 0b11111;
	dec.rs2 = (inst >> 20) & 0b11111;
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

/* Decode instruction */
void riscv_decode_instruction(riscv_decode &dec, riscv_proc_state *proc)
{
	memset(&dec, 0, sizeof(dec));
	riscv_wu inst = htole16(*(riscv_hu*)proc->pc);
	riscv_wu op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(riscv_hu*)(proc->pc + 2)) << 16;
		proc->pc += (dec.sz = 4);
	} else {
		proc->pc += (dec.sz = 2);
	}
	switch (op1) {
		/* 16-bit compressed instructions */
		case 0x0:
		{
			riscv_wu op2 = riscv_decode_c_funct3(inst);
			switch (op2) {
				case 0x00:
					riscv_decode_ciw_addi4spn(dec, inst);
					dec.op = riscv_op_c_addi4spn;
					dec.rd = riscv_ireg_sp;
					break;
				case 0x01:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_cl_ld(dec, inst);
							dec.op = riscv_op_c_fld;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_cl_lq(dec, inst); BOOM
							//dec.op = riscv_op_c_lq;
							break;
					}
					break;
				case 0x02:
					riscv_decode_cl_lw(dec, inst);
					dec.op = riscv_op_c_lw;
					break;
				case 0x03:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_cl_lw(dec, inst);
							dec.op = riscv_op_c_flw;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_cl_ld(dec, inst);
							dec.op = riscv_op_c_ld;
							break;
					}
					break;
				case 0x04:
					break;
				case 0x05:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_cs_sd(dec, inst);
							dec.op = riscv_op_c_fsd;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_cs_sq(dec, inst); BOOM
							//dec.op = riscv_op_c_sq;
							break;
					}
					break;
				case 0x06:
					riscv_decode_cs_sw(dec, inst);
					dec.op = riscv_op_c_sw;
					break;
				case 0x07:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_cs_sw(dec, inst);
							dec.op = riscv_op_c_fsw;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_cs_sd(dec, inst);
							dec.op = riscv_op_c_sd;
							break;
					}
					break;
			}
			break;
		}
		case 0x1:
		{
			riscv_wu op2 = riscv_decode_c_funct3(inst);
			switch (op2) {
				case 0x00:
					riscv_decode_ci(dec, inst);
					dec.op = riscv_op_c_addi;
					break;
				case 0x01:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_cj(dec, inst);
							dec.op = riscv_op_c_jal;
							dec.rd = riscv_ireg_ra;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_ci(dec, inst);
							dec.op = riscv_op_c_addiw;
							break;
					}
					break;
				case 0x02:
					riscv_decode_ci(dec, inst);
					dec.op = riscv_op_c_li;
					dec.rs1 = riscv_ireg_zero;
					break;
				case 0x03:
					if (dec.rd == 2) {
						riscv_decode_ci_addi16sp(dec, inst);
						dec.op = riscv_op_c_addi16sp;
						dec.rd = riscv_ireg_sp;
					} else {
						riscv_decode_ci_lui(dec, inst);
						dec.op = riscv_op_c_lui;
					}
					break;
				case 0x04:
					switch ((inst >> 10) & 0b111) {
						case 0:
							riscv_decode_cb(dec, inst);
							dec.op = riscv_op_c_srli;
							break;
						case 1:
							riscv_decode_cb(dec, inst);
							dec.op = riscv_op_c_srai;
							break;
						case 2:
							riscv_decode_cb(dec, inst);
							dec.op = riscv_op_c_andi;
							break;
						case 3:
							riscv_decode_cs_x(dec, inst);
							switch (dec.imm) {
								case 0:
									dec.op = riscv_op_c_sub;
									break;
								case 1:
									dec.op = riscv_op_c_xor;
									break;
								case 2:
									dec.op = riscv_op_c_or;
									break;
								case 3:
									dec.op = riscv_op_c_and;
									break;
								case 4:
									switch (proc->p_type) {
										case riscv_proc_type_rv32i:
										case riscv_proc_type_rv32e:
											break;
										case riscv_proc_type_rv64i:
										case riscv_proc_type_rv128i:
											dec.op = riscv_op_c_subw;
											break;
									}
									break;
								case 5:
									switch (proc->p_type) {
										case riscv_proc_type_rv32i:
										case riscv_proc_type_rv32e:
											break;
										case riscv_proc_type_rv64i:
										case riscv_proc_type_rv128i:
											dec.op = riscv_op_c_addw;
											break;
									}
									break;
							}
							break;
					}
					break;
				case 0x05:
					riscv_decode_cj(dec, inst);
					dec.op = riscv_op_c_j;
					break;
				case 0x06:
					riscv_decode_cb(dec, inst);
					dec.op = riscv_op_c_beqz;
					break;
				case 0x07:
					riscv_decode_cb(dec, inst);
					dec.op = riscv_op_c_bnez;
					break;
			}
			break;
		}
		case 0x2:
		{
			riscv_wu op2 = riscv_decode_c_funct3(inst);
			switch (op2) {
				case 0x00:
					riscv_decode_ci(dec, inst);
					dec.op = riscv_op_c_slli;
					break;
				case 0x01:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_ci_lwsp(dec, inst);
							dec.op = riscv_op_c_fldsp;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_ci_lqsp(dec, inst); BOOM
							//dec.op = riscv_op_c_lqsp;
							break;
					}
					break;
				case 0x02:
					riscv_decode_ci_lwsp(dec, inst);
					dec.op = riscv_op_c_lwsp;
					break;
				case 0x03:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_ci_lwsp(dec, inst);
							dec.op = riscv_op_c_flwsp;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_ci_ldsp(dec, inst);
							dec.op = riscv_op_c_ldsp;
							break;
					}
					break;
				case 0x04:
					riscv_decode_cr(dec, inst);
					switch (riscv_decode_c_funct4(inst)) {
						case 8:
							if (dec.rd == 0 && dec.rs2 == 0) {
								//
							} else if (dec.rs2 == 0) {
								dec.op = riscv_op_c_jr;
							} else {
								dec.op = riscv_op_c_mv;
							}
							break;
						case 9:
							if (dec.rd == 0 && dec.rs2 == 0) {
								dec.op = riscv_op_c_ebreak;
							} else if (dec.rs2 == 0) {
								dec.op = riscv_op_c_jalr;
							} else {
								dec.op = riscv_op_c_add;
							}
							break;
					}
					break;
				case 0x05:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
						case riscv_proc_type_rv64i:
							riscv_decode_css_sdsp(dec, inst);
							dec.op = riscv_op_c_fsdsp;
							break;
						case riscv_proc_type_rv128i:
							//riscv_decode_css_sqsp(dec, inst); BOOM
							//dec.op = riscv_op_c_sqsp;
							break;
					}
					break;
				case 0x06:
					riscv_decode_css_swsp(dec, inst);
					dec.op = riscv_op_c_swsp;
					break;
				case 0x07:
					switch (proc->p_type) {
						case riscv_proc_type_rv32i:
						case riscv_proc_type_rv32e:
							riscv_decode_css_swsp(dec, inst);
							dec.op = riscv_op_c_fswsp;
							break;
						case riscv_proc_type_rv64i:
						case riscv_proc_type_rv128i:
							riscv_decode_css_sdsp(dec, inst);
							dec.op = riscv_op_c_sdsp;
							break;
					}
					break;
			}
			break;
		}
		/* 32-bit instruction */
		case 0x3:
		{
			riscv_wu op2 = (inst & 0b1111100) >> 2;
			switch (op2) {
				/* lb, lh, lw, ld, lbu, lhu, lwu */
				case 0x00:
					riscv_decode_i_l(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_lb;  break;
						case 1: dec.op = riscv_op_lh;  break;
						case 2: dec.op = riscv_op_lw;  break;
						case 3: dec.op = riscv_op_ld;  break;
						case 4: dec.op = riscv_op_lbu; break;
						case 5: dec.op = riscv_op_lhu; break;
						case 6: dec.op = riscv_op_lwu; break;
					}
					break;

				case 0x01:
				case 0x02:
					break;

				/* fence, fence.i */
				case 0x03:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_fence;   break;
						case 1: dec.op = riscv_op_fence_i; break;
					}
					break;

				/* addi, slli, slti, sltiu, xori, srli, srai, ori, andi */
				case 0x04:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_addi;  break; // BOOM: nop
						case 1: dec.op = riscv_op_slli;  break;
						case 2: dec.op = riscv_op_slti;  break;
						case 3: dec.op = riscv_op_sltiu; break;
						case 4: dec.op = riscv_op_xori;  break;
						case 5:
							dec.op = (inst & (1 << 30)) ? riscv_op_srai : riscv_op_srli;
							dec.imm &= 0b111111; // mask high bits from shift
							break;
						case 6: dec.op = riscv_op_ori;   break;
						case 7: dec.op = riscv_op_andi;  break;
					}
					break;

				/* auipc */
				case 0x05:
					riscv_decode_u(dec, inst);
					dec.op = riscv_op_auipc;
					break;

				/* addiw, slliw, srliw, sraiw */
				case 0x06:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_addiw; break;
						case 1: dec.op = riscv_op_slliw; break;
						case 5:
							dec.op = (inst & (1 << 30)) ? riscv_op_sraiw : riscv_op_srliw;
							dec.imm &= 0b111111; // mask high bits from shift
							break;
					}
					break;

				case 0x07:
					break;

				/* sb, sh, sw, sd */
				case 0x08:
					riscv_decode_s(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_sb; break;
						case 1: dec.op = riscv_op_sh; break;
						case 2: dec.op = riscv_op_sw; break;
						case 3: dec.op = riscv_op_sd; break;
					}
					break;

				case 0x09:
				case 0x0a:
					break;

				/* RV32A: amoadd.w, amoxor.w, amoor.w, amoand.w, amomin.w, amomax.w, amominu.w, amomaxu.w, amoswap.w, lr.w, sc.w */
				/* RV64A: amoadd.d, amoxor.d, amoor.d, amoand.d, amomin.d, amomax.d, amominu.d, amomaxu.d, amoswap.d, lr.d, sc.d */
				case 0x0b:
					riscv_decode_r(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 2:
							switch (riscv_decode_funct7(inst) >> 2) {
								case 0:  dec.op = riscv_op_amoadd_w;  break;
								case 1:  dec.op = riscv_op_amoswap_w; break;
								case 2:  dec.op = riscv_op_lr_w;      break;
								case 3:  dec.op = riscv_op_sc_w;      break;
								case 4:  dec.op = riscv_op_amoxor_w;  break;
								case 8:  dec.op = riscv_op_amoor_w;   break;
								case 12: dec.op = riscv_op_amoand_w;  break;
								case 16: dec.op = riscv_op_amomin_w;  break;
								case 20: dec.op = riscv_op_amomax_w;  break;
								case 24: dec.op = riscv_op_amominu_w;  break;
								case 28: dec.op = riscv_op_amomaxu_w;  break;
							}
							break;
						case 3:
							switch (riscv_decode_funct7(inst) >> 2) {
								case 0:  dec.op = riscv_op_amoadd_d;  break;
								case 1:  dec.op = riscv_op_amoswap_d; break;
								case 2:  dec.op = riscv_op_lr_d;      break;
								case 3:  dec.op = riscv_op_sc_d;      break;
								case 4:  dec.op = riscv_op_amoxor_d;  break;
								case 8:  dec.op = riscv_op_amoor_d;   break;
								case 12: dec.op = riscv_op_amoand_d;  break;
								case 16: dec.op = riscv_op_amomin_d;  break;
								case 20: dec.op = riscv_op_amomax_d;  break;
								case 24: dec.op = riscv_op_amominu_d;  break;
								case 28: dec.op = riscv_op_amomaxu_d;  break;
							}
							break;
					}
					break;

				/* add, sub, sll, slt, sltu, xor, srl, sra, or, and */
				/* RV32M: mul, mulh, mulhsu, mulhu, div, divu, rem, remu */
				case 0x0c:
					riscv_decode_r(dec, inst);
					switch (riscv_decode_funct7(inst)) {
						case 0:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_add;  break;
								case 1: dec.op = riscv_op_sll;  break;
								case 2: dec.op = riscv_op_slt;  break;
								case 3: dec.op = riscv_op_sltu; break;
								case 4: dec.op = riscv_op_xor;  break;
								case 5: dec.op = riscv_op_srl;  break;
								case 6: dec.op = riscv_op_or;   break;
								case 7: dec.op = riscv_op_and;  break;
							}
							break;
						case 32:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_sub;  break;
								case 5: dec.op = riscv_op_sra;  break;
							}
							break;
					}
					break;

				/* lui */
				case 0x0d:
					riscv_decode_u(dec, inst);
					dec.op = riscv_op_lui;
					break;

				/* addw, subw, sllw, srlw, sraw */
				/* RV64M: mulw, divw, divuw, remw, remuw */
				case 0x0e:
					riscv_decode_r(dec, inst);
					switch (riscv_decode_funct7(inst)) {
						case 0:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_addw;  break;
								case 1: dec.op = riscv_op_sllw;  break;
								case 5: dec.op = riscv_op_srlw;  break;
							}
							break;
						case 1:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_mulw;  break;
								case 4: dec.op = riscv_op_divw;  break;
								case 5: dec.op = riscv_op_divuw; break;
								case 6: dec.op = riscv_op_remw;  break;
								case 7: dec.op = riscv_op_remuw; break;
							}
							break;
						case 32:
							switch (riscv_decode_funct3(inst)) {
								case 0: dec.op = riscv_op_subw;  break;
								case 5: dec.op = riscv_op_sraw;  break;
							}
							break;
					}
					break;

				// BOOM floating point
				case 0x0f:
				case 0x10:
				case 0x11:
				case 0x12:
				case 0x13:
					break;

				case 0x14:
					riscv_decode_r(dec, inst);
					switch ((inst >> 27) & 0b11111) {
						case 0:
							// (inst >> 25) & 0b11) ?
							break;
						case 1:
							// (inst >> 25) & 0b11) ?
							break;
					}
					break;

				case 0x15:
				case 0x16:
				case 0x17:
					break;

				/* beq, bne, blt, bge, bltu, bgeu */
				case 0x18:
					riscv_decode_sb(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_beq;  break;
						case 1: dec.op = riscv_op_bne;  break;
						case 4: dec.op = riscv_op_blt;  break;
						case 5: dec.op = riscv_op_bge;  break;
						case 6: dec.op = riscv_op_bltu; break;
						case 7: dec.op = riscv_op_bgeu; break;
					}
					break;

				/* jalr */
				case 0x19:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0: dec.op = riscv_op_jalr;  break;
					}
					break;

				case 0x1a:
					break;

				/* jal */
				case 0x1b:
					riscv_decode_uj(dec, inst);
					dec.op = riscv_op_jal;
					break;

				/* scall, sbreak, sret, sfence.vm, wfi, mrth, mrts, hrts, csrrw, csrrs, csrrc, csrrwi, csrrsi, csrrci */
				case 0x1c:
					riscv_decode_i(dec, inst);
					switch (riscv_decode_funct3(inst)) {
						case 0:
							switch (dec.imm) {
								case 0x000: dec.op = riscv_op_scall; dec.type = riscv_inst_type_i_s;     break;
								case 0x001: dec.op = riscv_op_sbreak; dec.type = riscv_inst_type_i_s;    break;
								case 0x100: dec.op = riscv_op_sret; dec.type = riscv_inst_type_i_s;      break;
								case 0x101: dec.op = riscv_op_sfence_vm; dec.type = riscv_inst_type_i_s; break;
								case 0x102: dec.op = riscv_op_wfi; dec.type = riscv_inst_type_i_s;       break;
								case 0x306: dec.op = riscv_op_mrth; dec.type = riscv_inst_type_i_s;      break;
								case 0x305: dec.op = riscv_op_mrts; dec.type = riscv_inst_type_i_s;      break;
								case 0x205: dec.op = riscv_op_hrts; dec.type = riscv_inst_type_i_s;      break;
							}
						 	break;
						case 1: dec.op = riscv_op_csrrw;  dec.type = riscv_inst_type_i_csr; break;
						case 2: dec.op = riscv_op_csrrs;  dec.type = riscv_inst_type_i_csr; break;
						case 3: dec.op = riscv_op_csrrc;  dec.type = riscv_inst_type_i_csr; break;
						case 5: dec.op = riscv_op_csrrwi; dec.type = riscv_inst_type_i_csri; break;
						case 6: dec.op = riscv_op_csrrsi; dec.type = riscv_inst_type_i_csri; break;
						case 7: dec.op = riscv_op_csrrci; dec.type = riscv_inst_type_i_csri; break;
					}
					break;

				case 0x1d:
				case 0x1e:
				case 0x1f:
					break;

				/* unknonwn major opcode */
				default:
					break;
			}
			break;
		}
	}
}

void riscv_print_dsm_entry(const riscv_dsm_entry *pc)
{
	std::cout << "/* " << std::left << std::setw(5) << pc << "*/";
	switch (pc->dsm_op) {
		case riscv_dsm_break:
			std::cout << "{ riscv_dsm_break, " << pc->dsm_val << " }," << std::endl;
			break;
		case riscv_dsm_table_brk:
			std::cout << "{ riscv_dsm_table_brk, " << pc->dsm_val << " }," << std::endl;
			break;
		case riscv_dsm_table_dfl:
			std::cout << "{ riscv_dsm_table_dfl, " << pc->dsm_val << " }," << std::endl;
			break;
		case riscv_dsm_match:
			std::cout << "{ riscv_dsm_match, " << pc->dsm_val << " }," << std::endl;
			break;
		case riscv_dsm_jump:
			std::cout << "{ riscv_dsm_jump, " << pc->dsm_val << " }," << std::endl;
			break;
		case riscv_dsm_select:
			std::cout << "{ riscv_dsm_select, " << riscv_instructions[pc->dsm_val] << " }," << std::endl;
			break;
		case riscv_dsm_mask_srl0:
		case riscv_dsm_mask_srl1:
		case riscv_dsm_mask_srl2:
		case riscv_dsm_mask_srl3:
		case riscv_dsm_mask_srl4:
		case riscv_dsm_mask_srl5:
		case riscv_dsm_mask_srl6:
		case riscv_dsm_mask_srl7:
		case riscv_dsm_mask_srl8:
		case riscv_dsm_mask_srl9:
		case riscv_dsm_mask_srl10:
		case riscv_dsm_mask_srl11:
		case riscv_dsm_mask_srl12:
		case riscv_dsm_mask_srl13:
		case riscv_dsm_mask_srl14:
		case riscv_dsm_mask_srl15:
		case riscv_dsm_mask_srl16:
		case riscv_dsm_mask_srl17:
		case riscv_dsm_mask_srl18:
		case riscv_dsm_mask_srl19:
		case riscv_dsm_mask_srl20:
		case riscv_dsm_mask_srl21:
		case riscv_dsm_mask_srl22:
		case riscv_dsm_mask_srl23:
		case riscv_dsm_mask_srl24:
		case riscv_dsm_mask_srl25:
		case riscv_dsm_mask_srl26:
		case riscv_dsm_mask_srl27:
		case riscv_dsm_mask_srl28:
		case riscv_dsm_mask_srl29:
		case riscv_dsm_mask_srl30:
		case riscv_dsm_mask_srl31:
			std::cout << "{ riscv_dsm_mask_srl" << (pc->dsm_op - riscv_dsm_mask_srl0) << ", " << pc->dsm_val << " }," << std::endl;
			break;
	}
}

/* Decode instruction */
void riscv_decode_instruction_dsm(riscv_decode &dec, riscv_proc_state *proc)
{
	memset(&dec, 0, sizeof(dec));
	riscv_wu inst = htole16(*(riscv_hu*)proc->pc);
	riscv_wu op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(riscv_hu*)(proc->pc + 2)) << 16;
		proc->pc += (dec.sz = 4);
	} else {
		proc->pc += (dec.sz = 2);
	}

	riscv_wu match = 0;
	const riscv_dsm_entry *pc = riscv_dsm_table;
	while (true) {

		if (false) riscv_print_dsm_entry(pc);

		switch (pc->dsm_op) {
			case riscv_dsm_break:
				return;
			case riscv_dsm_table_brk:
				if (match < pc->dsm_val) {
					pc += match + 1;
					break;
				}
				return;
			case riscv_dsm_table_dfl:
				pc += (match < pc->dsm_val) ? match + 1 : pc->dsm_val;
				break;
			case riscv_dsm_match:
				pc += (match == pc->dsm_val) ? 1 : 2;
				break;
			case riscv_dsm_jump:
				match = 0;
				pc += pc->dsm_val;
				break;
			case riscv_dsm_select:
				dec.op = pc->dsm_val;
				return;
			case riscv_dsm_mask_srl0:
			case riscv_dsm_mask_srl1:
			case riscv_dsm_mask_srl2:
			case riscv_dsm_mask_srl3:
			case riscv_dsm_mask_srl4:
			case riscv_dsm_mask_srl5:
			case riscv_dsm_mask_srl6:
			case riscv_dsm_mask_srl7:
			case riscv_dsm_mask_srl8:
			case riscv_dsm_mask_srl9:
			case riscv_dsm_mask_srl10:
			case riscv_dsm_mask_srl11:
			case riscv_dsm_mask_srl12:
			case riscv_dsm_mask_srl13:
			case riscv_dsm_mask_srl14:
			case riscv_dsm_mask_srl15:
			case riscv_dsm_mask_srl16:
			case riscv_dsm_mask_srl17:
			case riscv_dsm_mask_srl18:
			case riscv_dsm_mask_srl19:
			case riscv_dsm_mask_srl20:
			case riscv_dsm_mask_srl21:
			case riscv_dsm_mask_srl22:
			case riscv_dsm_mask_srl23:
			case riscv_dsm_mask_srl24:
			case riscv_dsm_mask_srl25:
			case riscv_dsm_mask_srl26:
			case riscv_dsm_mask_srl27:
			case riscv_dsm_mask_srl28:
			case riscv_dsm_mask_srl29:
			case riscv_dsm_mask_srl30:
			case riscv_dsm_mask_srl31:
				match |= ((inst >> (pc->dsm_op - riscv_dsm_mask_srl0)) & pc->dsm_val);
				pc++;
				break;
		}
	}
}

const riscv_inst_type_metadata* riscv_lookup_inst_metadata(riscv_inst_type type)
{
	static std::map<riscv_inst_type,const riscv_inst_type_metadata*> type_map;
	if (type_map.size() == 0) {
		const riscv_inst_type_metadata *ent = riscv_inst_type_table;
		while (ent->fmt) {
			type_map[ent->type] = ent;
			ent++;
		}
	}
	std::map<riscv_inst_type,const riscv_inst_type_metadata*>::iterator type_i = type_map.find(type);
	return (type_i != type_map.end()) ? type_i->second : nullptr;
}

const riscv_inst_comp_metadata* riscv_lookup_comp_metadata(riscv_op op)
{
	static std::map<riscv_op,const riscv_inst_comp_metadata*> comp_map;
	if (comp_map.size() == 0) {
		const riscv_inst_comp_metadata *ent = riscv_comp_table;
		while (ent->cop) {
			comp_map[ent->cop] = ent;
			ent++;
		}
	}
	std::map<riscv_op,const riscv_inst_comp_metadata*>::iterator type_i = comp_map.find(op);
	return (type_i != comp_map.end()) ? type_i->second : nullptr;
}

const riscv_csr_metadata* riscv_lookup_csr_metadata(riscv_hu csr_value)
{
	static std::map<riscv_hu,const riscv_csr_metadata*> csr_map;
	if (csr_map.size() == 0) {
		const riscv_csr_metadata *ent = riscv_csr_table;
		while (ent->csr_value) {
			csr_map[ent->csr_value] = ent;
			ent++;
		}
	}
	std::map<riscv_hu,const riscv_csr_metadata*>::iterator csr_i = csr_map.find(csr_value);
	return (csr_i != csr_map.end()) ? csr_i->second : nullptr;
}

std::string riscv_format_instruction(riscv_decode &dec, riscv_proc_state *proc, riscv_ptr pc, riscv_ptr pc_offset)
{
	std::stringstream ss;
#if 1
	const riscv_inst_comp_metadata *comp = riscv_lookup_comp_metadata((riscv_op)dec.op);
	if (comp) {
		dec.op = comp->op;
		dec.type = comp->type;
	}
#endif
	const riscv_inst_type_metadata *inst_meta = riscv_lookup_inst_metadata((riscv_inst_type)dec.type);
	const rvf *fmt = inst_meta ? inst_meta->fmt : rvf_none;

	ss << format_string("%016tx: \t", pc - pc_offset);
	switch (dec.sz) {
		case 2: ss << format_string("%04x\t\t", *(riscv_hu*)pc); break;
		case 4: ss << format_string("%08x\t", *(riscv_wu*)pc); break;
	}
	ss << riscv_instructions[dec.op] << "\t";

	while (*fmt != rvf_z) {
		switch (*fmt) {
			case rvf_b: ss << "("; break;
			case rvf_c: ss << ","; break;
			case rvf_d: ss << ")"; break;
			case rvf_rd: ss << riscv_i_registers[dec.rd]; break;
			case rvf_rs1: ss << riscv_i_registers[dec.rs1]; break;
			case rvf_rs2: ss << riscv_i_registers[dec.rs2]; break;
			case rvf_frd: ss << riscv_f_registers[dec.rd]; break;
			case rvf_frs1: ss << riscv_f_registers[dec.rs1]; break;
			case rvf_frs2: ss << riscv_f_registers[dec.rs2]; break;
			case rvf_frs3: ss << "BOOM"; break;
			case rvf_srs1: ss << format_string("%d", dec.rs1); break;
			case rvf_i: ss << format_string("%lld", dec.imm, dec.imm); break;
			case rvf_ipc: ss << format_string("%lld \t# 0x%016tx", dec.imm, pc - pc_offset + dec.imm); break;
			case rvf_icsr:
			{
				const riscv_csr_metadata *csr = riscv_lookup_csr_metadata(dec.imm);
				ss << (csr ? csr->csr_name : format_string("%llu", dec.imm));
				break;
			}
			case rvf_z: break;
		}
		fmt++;
	}
	return ss.str();
}
