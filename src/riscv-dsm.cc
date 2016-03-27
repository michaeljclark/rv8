//
//  riscv-dsm.cc
//

#include <cstdio>

#include "riscv-types.h"
#include "riscv-dsm.h"
#include "riscv-opcodes.h"

void riscv_print_dsm_entry(const riscv_dsm_entry *pc)
{
	printf("/* %p */", (void*)pc);
	switch (pc->dsm_op) {
		case riscv_dsm_break: printf("{ riscv_dsm_break, %d },\n", pc->dsm_val); break;
		case riscv_dsm_table_brk: printf("{ riscv_dsm_table_brk, %d },\n", pc->dsm_val); break;
		case riscv_dsm_table_dfl: printf("{ riscv_dsm_table_dfl, %d },\n", pc->dsm_val); break;
		case riscv_dsm_match: printf("{ riscv_dsm_match, %d },\n", pc->dsm_val); break;
		case riscv_dsm_jump: printf("{ riscv_dsm_jump, %d },\n", pc->dsm_val); break;
		case riscv_dsm_select: printf("{ riscv_dsm_select, %s },\n", riscv_instructions[pc->dsm_val].opcode); break;
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
			printf("{ riscv_dsm_mask_srl%d, %d },\n", (pc->dsm_op - riscv_dsm_mask_srl0), pc->dsm_val);
			break;
	}
}
