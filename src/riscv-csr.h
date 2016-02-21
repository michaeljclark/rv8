//
//  riscv-csr.h
//

#ifndef riscv_csr_h
#define riscv_csr_h

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

extern const riscv_csr_metadata riscv_csr_table[];

#endif