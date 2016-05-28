//
//  riscv-format.h
//

#ifndef riscv_format_h
#define riscv_format_h

/* Instruction Argument Formats */

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
	rvf_imm5,
	rvf_imm,
	rvf_disp,
	rvf_csr,
	rvf_rm,
	rvf_aqrl,
};

/* Instruction Format Declarations */

const rvf riscv_fmt_none[] =                  { rvf_z };
const rvf riscv_fmt_disp[] =                  { rvf_disp, rvf_z };
const rvf riscv_fmt_rs1_rs2[] =               { rvf_rs1, rvf_c, rvf_rs2, rvf_z };
const rvf riscv_fmt_rd_imm[] =                { rvf_rd, rvf_c, rvf_imm, rvf_z };
const rvf riscv_fmt_rd_disp[] =               { rvf_rd, rvf_c, rvf_disp, rvf_z };
const rvf riscv_fmt_rd_rs1_rs2[] =            { rvf_rd, rvf_c, rvf_rs1, rvf_c, rvf_rs2, rvf_z };
const rvf riscv_fmt_frd_rs1[] =               { rvf_frd, rvf_c, rvf_rs1, rvf_z };
const rvf riscv_fmt_rd_frs1[] =               { rvf_rd, rvf_c, rvf_frs1, rvf_z };
const rvf riscv_fmt_rd_frs1_frs2[] =          { rvf_frd, rvf_c, rvf_rs1, rvf_c, rvf_frs2, rvf_z };
const rvf riscv_fmt_rm_frd_frs1[] =           { rvf_rm, rvf_c, rvf_frd, rvf_c, rvf_frs1, rvf_z };
const rvf riscv_fmt_rm_frd_rs1[] =            { rvf_rm, rvf_c, rvf_frd, rvf_c, rvf_rs1, rvf_z };
const rvf riscv_fmt_rm_rd_frs1[] =            { rvf_rm, rvf_c, rvf_rd, rvf_c, rvf_frs1, rvf_z };
const rvf riscv_fmt_rm_frd_frs1_frs2[] =      { rvf_rm, rvf_c, rvf_frd, rvf_c, rvf_frs1, rvf_c, rvf_frs2, rvf_z };
const rvf riscv_fmt_rm_frd_frs1_frs2_frs3[] = { rvf_rm, rvf_c, rvf_frd, rvf_c, rvf_frs1, rvf_c, rvf_frs2, rvf_c, rvf_frs3, rvf_z };
const rvf riscv_fmt_rd_rs1_imm[] =            { rvf_rd, rvf_c, rvf_rs1, rvf_c, rvf_imm, rvf_z };
const rvf riscv_fmt_rd_imm_rs1[] =            { rvf_rd, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf riscv_fmt_frd_imm_rs1[] =           { rvf_frd, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf riscv_fmt_rd_csr_rs1[] =            { rvf_rd, rvf_c, rvf_csr, rvf_c, rvf_rs1, rvf_z };
const rvf riscv_fmt_rd_csr_imm5[] =           { rvf_rd, rvf_c, rvf_csr, rvf_c, rvf_imm5, rvf_z };
const rvf riscv_fmt_rs2_imm_rs1[] =           { rvf_rs2, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf riscv_fmt_frs2_imm_rs1[] =          { rvf_frs2, rvf_c, rvf_imm, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf riscv_fmt_rs1_rs2_disp[] =          { rvf_rs1, rvf_c, rvf_rs2, rvf_c, rvf_disp, rvf_z };
const rvf riscv_fmt_aqrl_rd_rs2_b_rs1[] =     { rvf_aqrl, rvf_c, rvf_rd, rvf_c, rvf_rs2, rvf_c, rvf_b, rvf_rs1, rvf_d, rvf_z };
const rvf riscv_fmt_aqrl_rd_b_rs1[] =         { rvf_aqrl, rvf_c, rvf_rd, rvf_c, rvf_b, rvf_rs1, rvf_d, rvf_z };

#endif
