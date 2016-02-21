//
//  riscv-compression.h
//

#ifndef riscv_compression_h
#define riscv_compression_h

/* Compressed Instruction Mapping table */

struct riscv_inst_comp_metadata
{
	riscv_op cop;
	riscv_inst_type ctype;
	riscv_op op;
	riscv_inst_type type;
};

extern const riscv_inst_comp_metadata riscv_comp_table[];

#endif