//
//  riscv-disasm.cc
//

#include <cstdio>
#include <map>
#include <vector>
#include <string>

#include "riscv-types.h"
#include "riscv-format.h"
#include "riscv-opcodes.h"
#include "riscv-imm.h"
#include "riscv-regs.h"
#include "riscv-decode.h"
#include "riscv-csr.h"
#include "riscv-processor.h"
#include "riscv-compression.h"
#include "riscv-format.h"
#include "riscv-disasm.h"

const char* riscv_null_symbol_lookup(riscv_ptr) { return nullptr; }
const char* riscv_null_symbol_colorizer(char *buf, size_t buflen, const char *symbol, const char *type) { return symbol; }

void riscv_disasm_instruction(riscv_decode &dec, riscv_proc_state *proc,
	riscv_ptr pc, riscv_ptr next_pc, riscv_ptr pc_offset,
	riscv_symbol_name_fn symlookup, riscv_symbol_colorizer_fn colorizer)
{
	// decompress opcode if compressed
	const riscv_inst_comp_metadata *comp = riscv_lookup_comp_metadata((riscv_op)dec.op);
	if (comp) {
		dec.op = comp->op;
		dec.type = comp->type;
	}

	// print symbol name and address
	const rvf *fmt = riscv_instruction_format[dec.op];
	const uint64_t addr = (pc - pc_offset);
	const char* symbol_name = symlookup((riscv_ptr)addr);
	printf("%30s%s %016tx: \t", symbol_name ? symbol_name : "", symbol_name ? ":" : " ", addr);

	// print instruction bytes
	switch (next_pc - pc) {
		case 2: printf("%04x\t\t", *(riscv_hu*)pc); break;
		case 4: printf("%08x\t", *(riscv_wu*)pc); break;
	}

	// print opcode and arguments
	printf("%s\t", riscv_instruction_name[dec.op]);
	while (*fmt != rvf_z) {
		switch (*fmt) {
			case rvf_b:    printf("("); break;
			case rvf_c:    printf(","); break;
			case rvf_d:    printf(")"); break;
			case rvf_rd:   printf("%s", riscv_i_registers[dec.rd]); break;
			case rvf_rs1:  printf("%s", riscv_i_registers[dec.rs1]); break;
			case rvf_rs2:  printf("%s", riscv_i_registers[dec.rs2]); break;
			case rvf_frd:  printf("%s", riscv_f_registers[dec.rd]); break;
			case rvf_frs1: printf("%s", riscv_f_registers[dec.rs1]); break;
			case rvf_frs2: printf("%s", riscv_f_registers[dec.rs2]); break;
			case rvf_frs3: printf("%s", riscv_f_registers[dec.rs3]); break;
			case rvf_irs1: printf("%d", dec.rs1); break;
			case rvf_imm:  printf("%lld", dec.imm); break;
			case rvf_ipc:
			{
				uint64_t addr = pc - pc_offset + dec.imm;
				const char* symbol_name = symlookup((riscv_ptr)addr);
				printf("%lld\t# 0x%016tx%s%s%s", dec.imm, addr,
					symbol_name ? " <" : "",
					symbol_name ? symbol_name : "",
					symbol_name ? ">" : "");
				break;
			}
			case rvf_csr:
			{
				const riscv_csr_metadata *csr = riscv_lookup_csr_metadata(dec.imm);
				if (csr) printf("%s", csr->csr_name);
				else printf("%llu", dec.imm);
				break;
			}
			case rvf_z: break;
		}
		fmt++;
	}
	printf("\n");
}
