//
//  riscv-disasm.cc
//

#include <cstdio>
#include <map>
#include <algorithm>
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
const char* riscv_null_symbol_colorize(const char *type) { return ""; }

const void print_add(size_t &offset, const char *str)
{
	printf("%s", str);
	offset += strlen(str);
}

const void print_pad(size_t &offset, size_t pad_to)
{
	static const char *space32 = "                                        ";
	printf("%s", space32 + strlen(space32) - std::max((pad_to - offset), 0UL));
	offset += std::max((pad_to - offset), 0UL);
}

const void print_fmt(size_t &offset, const char* fmt, ...)
{
    std::vector<char> buf(32);
    va_list ap;

    va_start(ap, fmt);
    int len = vsnprintf(buf.data(), buf.capacity(), fmt, ap);
    va_end(ap);

    std::string str;
    if (len >= (int)buf.capacity()) {
        buf.resize(len + 1);
        va_start(ap, fmt);
        vsnprintf(buf.data(), buf.capacity(), fmt, ap);
        va_end(ap);
    }
    printf("%s", buf.data());
    offset += strlen(buf.data());
}

const void print_pad(size_t &offset, size_t pad_to, const char *str)
{
	print_add(offset, str);
	print_pad(offset, pad_to);
}

void riscv_disasm_instruction(riscv_decode &dec, riscv_proc_state *proc,
	riscv_ptr pc, riscv_ptr next_pc, riscv_ptr pc_offset,
	riscv_symbol_name_fn symlookup, riscv_symbol_colorize_fn colorize)
{
	// decompress opcode if compressed
	const riscv_inst_comp_metadata *comp = riscv_lookup_comp_metadata((riscv_op)dec.op);
	if (comp) {
		dec.op = comp->op;
		dec.type = comp->type;
	}

	size_t offset = 0;
	const rvf *fmt = riscv_instruction_format[dec.op];
	const uint64_t addr = (pc - pc_offset);
	const char* symbol_name = symlookup((riscv_ptr)addr);

	// print symbol name if present
	if (symbol_name) {
		printf("%s", colorize("symbol"));
		print_fmt(offset, "%30s", symbol_name);
		print_add(offset, ": ");
		printf("%s", colorize("reset"));
	} else {
		print_fmt(offset, "%30s", "");
		print_add(offset, "  ");
	}

	// print address
	printf("%s", colorize("address"));
	print_fmt(offset, "%016tx:   ", addr);
	printf("%s", colorize("reset"));

	// print instruction bytes
	switch (next_pc - pc) {
		case 2: print_fmt(offset, "%04x", *(riscv_hu*)pc); break;
		case 4: print_fmt(offset, "%08x", *(riscv_wu*)pc); break;
	}
	print_pad(offset, 64);

	// print opcode
	printf("%s", colorize("opcode"));
	print_pad(offset, 74, riscv_instruction_name[dec.op]);
	printf("%s", colorize("reset"));

	// print arguments
	while (*fmt != rvf_z) {
		switch (*fmt) {
			case rvf_b:    print_add(offset, "("); break;
			case rvf_c:    print_add(offset, ","); break;
			case rvf_d:    print_add(offset, ")"); break;
			case rvf_rd:   print_add(offset, riscv_i_registers[dec.rd]); break;
			case rvf_rs1:  print_add(offset, riscv_i_registers[dec.rs1]); break;
			case rvf_rs2:  print_add(offset, riscv_i_registers[dec.rs2]); break;
			case rvf_frd:  print_add(offset, riscv_f_registers[dec.rd]); break;
			case rvf_frs1: print_add(offset, riscv_f_registers[dec.rs1]); break;
			case rvf_frs2: print_add(offset, riscv_f_registers[dec.rs2]); break;
			case rvf_frs3: print_add(offset, riscv_f_registers[dec.rs3]); break;
			case rvf_irs1: print_fmt(offset, "%d", dec.rs1); break;
			case rvf_imm:  print_fmt(offset, "%lld", dec.imm); break;
			case rvf_ipc:
			{
				uint64_t addr = pc - pc_offset + dec.imm;
				print_fmt(offset, "%lld", dec.imm);
				print_pad(offset, 90);
				printf("%s", colorize("address"));
				print_add(offset, "# ");
				print_fmt(offset, "0x%016tx", addr);
				printf("%s", colorize("reset"));
				const char* symbol_name = symlookup((riscv_ptr)addr);
				if (symbol_name) {
					printf("%s", colorize("symbol"));
					print_fmt(offset, " <%s>", symbol_name);
					printf("%s", colorize("reset"));
				}
				break;
			}
			case rvf_csr:
			{
				const riscv_csr_metadata *csr = riscv_lookup_csr_metadata(dec.imm);
				if (csr) print_fmt(offset, "%s", csr->csr_name);
				else print_fmt(offset, "%llu", dec.imm);
				break;
			}
			case rvf_z: break;
		}
		fmt++;
	}
	printf("\n");
}
