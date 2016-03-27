//
//  riscv-decode-dsm.cc
//

#include <cstring>

#include "riscv-endian.h"
#include "riscv-opcodes.h"
#include "riscv-types.h"
#include "riscv-regs.h"
#include "riscv-processor.h"
#include "riscv-dsm.h"
#include "riscv-dsm-table.h"
#include "riscv-imm.h"
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
				goto next;
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
next:
	dec.type = riscv_instructions[dec.op].type;
	switch (dec.type) {
		case riscv_inst_type_c_none:        riscv_decode_c_none(dec, inst);          break;
		case riscv_inst_type_cb:            riscv_decode_cb(dec, inst);              break;
		case riscv_inst_type_ci:            riscv_decode_ci(dec, inst);              break;
		case riscv_inst_type_ci_16sp:       riscv_decode_ci_16sp(dec, inst);         break;
		case riscv_inst_type_ci_fldsp:      riscv_decode_ci_fldsp(dec, inst);        break;
		case riscv_inst_type_ci_flwsp:      riscv_decode_ci_flwsp(dec, inst);        break;
		case riscv_inst_type_ci_ldsp:       riscv_decode_ci_ldsp(dec, inst);         break;
		case riscv_inst_type_ci_li:         riscv_decode_ci_li(dec, inst);           break;
		case riscv_inst_type_ci_lui:        riscv_decode_ci_lui(dec, inst);          break;
		case riscv_inst_type_ci_lwsp:       riscv_decode_ci_lwsp(dec, inst);         break;
		case riscv_inst_type_ciw_4spn:      riscv_decode_ciw_4spn(dec, inst);        break;
		case riscv_inst_type_cj:            riscv_decode_cj(dec, inst);              break;
		case riscv_inst_type_cl_fld:        riscv_decode_cl_fld(dec, inst);          break;
		case riscv_inst_type_cl_flw:        riscv_decode_cl_flw(dec, inst);          break;
		case riscv_inst_type_cl_ld:         riscv_decode_cl_ld(dec, inst);           break;
		case riscv_inst_type_cl_lw:         riscv_decode_cl_lw(dec, inst);           break;
		case riscv_inst_type_cr:            riscv_decode_cr(dec, inst);              break;
		case riscv_inst_type_cr_jalr:       riscv_decode_cr_jalr(dec, inst);         break;
		case riscv_inst_type_cr_jr:         riscv_decode_cr_jr(dec, inst);           break;
		case riscv_inst_type_cs:            riscv_decode_cs(dec, inst);              break;
		case riscv_inst_type_cs_fsd:        riscv_decode_cs_fsd(dec, inst);          break;
		case riscv_inst_type_cs_fsw:        riscv_decode_cs_fsw(dec, inst);          break;
		case riscv_inst_type_cs_sd:         riscv_decode_cs_sd(dec, inst);           break;
		case riscv_inst_type_cs_sw:         riscv_decode_cs_sw(dec, inst);           break;
		case riscv_inst_type_css_sdsp:      riscv_decode_css_sdsp(dec, inst);        break;
		case riscv_inst_type_css_swsp:      riscv_decode_css_swsp(dec, inst);        break;
		case riscv_inst_type_css_fsdsp:     riscv_decode_css_fsdsp(dec, inst);       break;
		case riscv_inst_type_css_fswsp:     riscv_decode_css_fswsp(dec, inst);       break;
		case riscv_inst_type_i:             riscv_decode_i(dec, inst);               break;
		case riscv_inst_type_i_csr:         riscv_decode_i_csr(dec, inst);           break;
		case riscv_inst_type_i_csri:        riscv_decode_i_csri(dec, inst);          break;
		case riscv_inst_type_i_l:           riscv_decode_i_l(dec, inst);             break;
		case riscv_inst_type_i_lf:          riscv_decode_i_lf(dec, inst);            break;
		case riscv_inst_type_i_none:        riscv_decode_i_none(dec, inst);          break;
		case riscv_inst_type_i_sh5:         riscv_decode_i_sh5(dec, inst);           break;
		case riscv_inst_type_i_sh6:         riscv_decode_i_sh6(dec, inst);           break;
		case riscv_inst_type_r:             riscv_decode_r(dec, inst);               break;
		case riscv_inst_type_r_3f:          riscv_decode_r_3f(dec, inst);            break;
		case riscv_inst_type_r_4f:          riscv_decode_r_4f(dec, inst);            break;
		case riscv_inst_type_r_a:           riscv_decode_r_a(dec, inst);             break;
		case riscv_inst_type_r_ff:          riscv_decode_r_ff(dec, inst);            break;
		case riscv_inst_type_r_fr:          riscv_decode_r_fr(dec, inst);            break;
		case riscv_inst_type_r_l:           riscv_decode_r_l(dec, inst);             break;
		case riscv_inst_type_r_rf:          riscv_decode_r_rf(dec, inst);            break;
		case riscv_inst_type_r_rff:         riscv_decode_r_rff(dec, inst);           break;
		case riscv_inst_type_s:             riscv_decode_s(dec, inst);               break;
		case riscv_inst_type_sb:            riscv_decode_sb(dec, inst);              break;
		case riscv_inst_type_s_f:           riscv_decode_s_f(dec, inst);             break;
		case riscv_inst_type_u:             riscv_decode_u(dec, inst);               break;
		case riscv_inst_type_uj:            riscv_decode_uj(dec, inst);              break;
	};
}
