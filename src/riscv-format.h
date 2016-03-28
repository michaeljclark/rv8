//
//  riscv-format.h
//

#ifndef riscv_format_h
#define riscv_format_h

/* Instruction Formatting */

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
	rvf_irs1,
	rvf_imm,
	rvf_ipc,
	rvf_csr,
};

/* Instruction Type Table */

struct riscv_inst_type_metadata
{
	riscv_inst_type type;
	const rvf* fmt;
};

extern void riscv_print_instruction(riscv_decode &dec, riscv_proc_state *proc, riscv_ptr pc, riscv_ptr pc_offset);

#endif
