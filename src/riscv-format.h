//
//  riscv-format.h
//

#ifndef riscv_format_h
#define riscv_format_h

/* Instruction Formats */

extern "C" {
	extern const char* riscv_fmt_none;
	extern const char* riscv_fmt_disp;
	extern const char* riscv_fmt_rs1_rs2;
	extern const char* riscv_fmt_rd_imm;
	extern const char* riscv_fmt_rd_disp;
	extern const char* riscv_fmt_rd_rs1_rs2;
	extern const char* riscv_fmt_frd_rs1;
	extern const char* riscv_fmt_rd_frs1;
	extern const char* riscv_fmt_rd_frs1_frs2;
	extern const char* riscv_fmt_rm_frd_frs1;
	extern const char* riscv_fmt_rm_frd_rs1;
	extern const char* riscv_fmt_rm_rd_frs1;
	extern const char* riscv_fmt_rm_frd_frs1_frs2;
	extern const char* riscv_fmt_rm_frd_frs1_frs2_frs3;
	extern const char* riscv_fmt_rd_rs1_imm;
	extern const char* riscv_fmt_rd_imm_rs1;
	extern const char* riscv_fmt_frd_imm_rs1;
	extern const char* riscv_fmt_rd_csr_rs1;
	extern const char* riscv_fmt_rd_csr_imm5;
	extern const char* riscv_fmt_rs2_imm_rs1;
	extern const char* riscv_fmt_frs2_imm_rs1;
	extern const char* riscv_fmt_rs1_rs2_disp;
	extern const char* riscv_fmt_aqrl_rd_rs2_rs1;
	extern const char* riscv_fmt_aqrl_rd_rs1;
}

#endif
