//
//  riscv-compression.h
//

#ifndef riscv_compression_h
#define riscv_compression_h

/* Compression Metadata table */

struct riscv_inst_comp_metadata
{
	riscv_op cop;
	riscv_op op;
};

extern const riscv_inst_comp_metadata riscv_comp_table[];

const riscv_inst_comp_metadata* riscv_lookup_comp_metadata(riscv_op op);
const void riscv_decode_decompress(riscv_decode &dec);

#endif