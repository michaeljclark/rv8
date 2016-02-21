//
//  riscv-decode-dsm.cc
//

#include <cstring>

#include "endian.h"

#include "riscv-opcodes.h"
#include "riscv-types.h"
#include "riscv-regs.h"
#include "riscv-processor.h"
#include "riscv-dsm.h"
#include "riscv-dsm-table.h"
#include "riscv-decode.h"

/* Decode instruction */

void riscv_decode_instruction_dsm(riscv_decode &dec, riscv_proc_state *proc)
{
	memset(&dec, 0, sizeof(dec));
	riscv_wu inst = htole16(*(riscv_hu*)proc->pc);
	riscv_wu op1 = inst & 0b11;
	if (op1 == 3) {
		inst |= htole16(*(riscv_hu*)(proc->pc + 2)) << 16;
		proc->pc += (dec.sz = 4);
	} else {
		proc->pc += (dec.sz = 2);
	}

	riscv_wu match = 0;
	const riscv_dsm_entry *pc = riscv_dsm_table;
	while (true) {

		if (false) riscv_print_dsm_entry(pc);

		switch (pc->dsm_op) {
			case riscv_dsm_break:
				return;
			case riscv_dsm_table_brk:
				if (match < pc->dsm_val) {
					pc += match + 1;
					break;
				}
				return;
			case riscv_dsm_table_dfl:
				pc += (match < pc->dsm_val) ? match + 1 : pc->dsm_val;
				break;
			case riscv_dsm_match:
				pc += (match == pc->dsm_val) ? 1 : 2;
				break;
			case riscv_dsm_jump:
				match = 0;
				pc += pc->dsm_val;
				break;
			case riscv_dsm_select:
				dec.op = pc->dsm_val;
				return;
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
				match |= ((inst >> (pc->dsm_op - riscv_dsm_mask_srl0)) & pc->dsm_val);
				pc++;
				break;
		}
	}
}
