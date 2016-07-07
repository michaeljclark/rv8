//
//  riscv-csr.h
//

#ifndef riscv_csr_h
#define riscv_csr_h

/* CSR Metadata Table */

struct riscv_csr_metadata
{
	uint16_t csr_value;
	riscv_csr_perm csr_perm;
	const char* csr_name;
	const char* csr_desc;
};

extern const riscv_csr_metadata riscv_csr_table[];

const riscv_csr_metadata* riscv_lookup_csr_metadata(uint16_t csr_value);

#endif
