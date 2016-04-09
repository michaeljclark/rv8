//
//  riscv-compression.h
//

#ifndef riscv_compression_h
#define riscv_compression_h

/* Compression Metadata table */

struct riscv_inst_comp_metadata
{
	riscv_op cop;
	riscv_inst_type ctype;
	riscv_op op;
	riscv_inst_type type;
};

extern const riscv_inst_comp_metadata riscv_comp_table[];

const riscv_inst_comp_metadata* riscv_lookup_comp_metadata(riscv_op op);

#endif