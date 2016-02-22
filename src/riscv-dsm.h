//
//  riscv-dsm.h
//

#ifndef riscv_dsm_h
#define riscv_dsm_h

/* Instruction Decode State Machine */

enum riscv_dsm
{
	riscv_dsm_break = 0,       /* break with no opcode */
	riscv_dsm_table_brk = 1,   /* jump table, break if no entry */
	riscv_dsm_table_dfl = 2,   /* jump table, last entry is default */
	riscv_dsm_match = 3,       /* match predicate for next entry */
	riscv_dsm_jump = 4,        /* jump and clear match */
	riscv_dsm_select = 5,      /* select opcode and break */
	riscv_dsm_mask_srl0 = 6,   /* load match |= (inst >> 0) & val */
	riscv_dsm_mask_srl1 = 7,   /* load match |= (inst >> 1) & val */
	riscv_dsm_mask_srl2 = 8,   /* load match |= (inst >> 2) & val */
	riscv_dsm_mask_srl3 = 9,   /* load match |= (inst >> 3) & val */
	riscv_dsm_mask_srl4 = 10,  /* load match |= (inst >> 4) & val */
	riscv_dsm_mask_srl5 = 11,  /* load match |= (inst >> 5) & val */
	riscv_dsm_mask_srl6 = 12,  /* load match |= (inst >> 6) & val */
	riscv_dsm_mask_srl7 = 13,  /* load match |= (inst >> 7) & val */
	riscv_dsm_mask_srl8 = 14,  /* load match |= (inst >> 8) & val */
	riscv_dsm_mask_srl9 = 15,  /* load match |= (inst >> 9) & val */
	riscv_dsm_mask_srl10 = 16, /* load match |= (inst >> 10) & val */
	riscv_dsm_mask_srl11 = 17, /* load match |= (inst >> 11) & val */
	riscv_dsm_mask_srl12 = 18, /* load match |= (inst >> 12) & val */
	riscv_dsm_mask_srl13 = 19, /* load match |= (inst >> 13) & val */
	riscv_dsm_mask_srl14 = 20, /* load match |= (inst >> 14) & val */
	riscv_dsm_mask_srl15 = 21, /* load match |= (inst >> 15) & val */
	riscv_dsm_mask_srl16 = 22, /* load match |= (inst >> 16) & val */
	riscv_dsm_mask_srl17 = 23, /* load match |= (inst >> 17) & val */
	riscv_dsm_mask_srl18 = 24, /* load match |= (inst >> 18) & val */
	riscv_dsm_mask_srl19 = 25, /* load match |= (inst >> 19) & val */
	riscv_dsm_mask_srl20 = 26, /* load match |= (inst >> 20) & val */
	riscv_dsm_mask_srl21 = 27, /* load match |= (inst >> 21) & val */
	riscv_dsm_mask_srl22 = 28, /* load match |= (inst >> 22) & val */
	riscv_dsm_mask_srl23 = 29, /* load match |= (inst >> 23) & val */
	riscv_dsm_mask_srl24 = 30, /* load match |= (inst >> 24) & val */
	riscv_dsm_mask_srl25 = 31, /* load match |= (inst >> 25) & val */
	riscv_dsm_mask_srl26 = 32, /* load match |= (inst >> 26) & val */
	riscv_dsm_mask_srl27 = 33, /* load match |= (inst >> 27) & val */
	riscv_dsm_mask_srl28 = 34, /* load match |= (inst >> 28) & val */
	riscv_dsm_mask_srl29 = 35, /* load match |= (inst >> 29) & val */
	riscv_dsm_mask_srl30 = 36, /* load match |= (inst >> 30) & val */
	riscv_dsm_mask_srl31 = 37  /* load match |= (inst >> 31) & val */
};

struct riscv_dsm_entry
{
	riscv_hu dsm_op;
	riscv_wu dsm_val;

	riscv_dsm_entry() : dsm_op(0), dsm_val(0) {}
	riscv_dsm_entry(int dsm_op) : dsm_op(dsm_op), dsm_val(0) {}
	riscv_dsm_entry(int dsm_op, int dsm_val) : dsm_op(dsm_op), dsm_val(dsm_val) {}
};

void riscv_print_dsm_entry(const riscv_dsm_entry *pc);

#endif
