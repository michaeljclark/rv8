//
//  riscv-disasm.cc
//

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <string>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-opcodes.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-csr.h"
#include "riscv-format.h"
#include "riscv-disasm.h"

enum rva {
	rva_none,
	rva_abs,
	rva_pcrel
};

struct rvx {
	riscv_op op1;
	riscv_op op2;
	rva addr;
};

const rvx rvx_constraints[] = {
	{ riscv_op_lui,     riscv_op_addi,     rva_abs   },
	{ riscv_op_auipc,   riscv_op_addi,     rva_pcrel },
	{ riscv_op_auipc,   riscv_op_jalr,     rva_pcrel },
	{ riscv_op_unknown, riscv_op_unknown,  rva_none },
};

const size_t rvx_instruction_buffer_len = 16;

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

const void print_addr(size_t &offset, uint64_t addr,
	riscv_symbol_name_fn symlookup, riscv_symbol_colorize_fn colorize)
{
	print_pad(offset, 75);
	printf("%s", colorize("address"));
	print_add(offset, "# ");
	print_fmt(offset, "0x%016tx", addr);
	printf("%s", colorize("reset"));
	const char* symbol_name = symlookup((riscv_ptr)addr);
	if (symbol_name) {
		if (strncmp(symbol_name, "LOC_", 4) == 0) {
			printf(" %s", colorize("location"));
			print_fmt(offset, "%s", symbol_name);
			printf("%s", colorize("reset"));
		} else {
			printf(" %s", colorize("symbol"));
			print_fmt(offset, "<%s>", symbol_name);
			printf("%s", colorize("reset"));
		}
	}
}

void riscv_disasm_instruction(riscv_decode &dec, std::deque<riscv_decode> &dec_hist,
	riscv_ptr pc, riscv_ptr next_pc, riscv_ptr pc_offset, riscv_ptr gp,
	riscv_symbol_name_fn symlookup, riscv_symbol_colorize_fn colorize,
	bool decompress)
{
	if (decompress) riscv_decode_decompress(dec);

	size_t offset = 0;
	const rvf *fmt = riscv_instruction_format[dec.op];
	const uint64_t addr = pc - pc_offset;
	const char* symbol_name = symlookup((riscv_ptr)addr);

	// print symbol name if present
	if (symbol_name) {
		if (strncmp(symbol_name, "LOC_", 4) == 0) {
			printf("%s", colorize("location"));
			print_fmt(offset, "%s:", symbol_name);
			printf("%s", colorize("reset"));
		} else {
			// clear the instruction history on symbol boundaries
			dec_hist.clear();
			printf("\n%s", colorize("address"));
			print_fmt(offset, "0x%016tx: ", addr);
			printf("%s", colorize("reset"));
			printf("%s", colorize("symbol"));
			print_fmt(offset, "<%s>:", symbol_name);
			printf("%s\n", colorize("reset"));
			offset = 0;
		}
	}
	print_pad(offset, 12);

	// print address
	printf("%s", colorize("address"));
	print_fmt(offset, "%8tx:", addr & 0xffffffff);
	printf("%s", colorize("reset"));
	print_pad(offset, 24);

	// print instruction bytes
	switch (riscv_get_instruction_length(dec.inst)) {
		case 2: print_fmt(offset, "%04llx", dec.inst); break;
		case 4: print_fmt(offset, "%08llx", dec.inst); break;
		case 6: print_fmt(offset, "%012llx", dec.inst); break;
		case 8: print_fmt(offset, "%016llx", dec.inst); break;
	}
	print_pad(offset, 45);

	// print opcode
	printf("%s", colorize("opcode"));
	print_pad(offset, 55, riscv_instruction_name[dec.op]);
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
				print_addr(offset, addr, symlookup, colorize);
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

	// decoded address
	uint64_t sym_addr = 0;

	// decode address using instruction pair constraints
	const rvx* rvxi = rvx_constraints;
	while(rvxi->addr != rva_none) {
		if (rvxi->op2 == dec.op) {
			for (auto li = dec_hist.rbegin(); li != dec_hist.rend(); li++) {
				if (rvxi->op1 != li->op && dec.rs1 == li->rd) break;
				if (rvxi->op1 != li->op) continue;
				if (dec.rs1 != li->rd) continue;
				switch (rvxi->addr) {
					case rva_abs:
						sym_addr = li->imm + dec.imm;
						goto out;
					case rva_pcrel:
						sym_addr = li->pc - pc_offset + li->imm + dec.imm;
						goto out;
					case rva_none:
					default:
						continue;
				}
				break;
			}
		}
		rvxi++;
	}
out:

	// decode address for loads and stores from the global pointer
	if (sym_addr == 0 && gp && dec.rs1 == riscv_ireg_gp)
	{
		switch (dec.op) {
			case riscv_op_addi:
			case riscv_op_lb:
			case riscv_op_lh:
			case riscv_op_lw:
			case riscv_op_ld:
			case riscv_op_lbu:
			case riscv_op_lhu:
			case riscv_op_lwu:
			case riscv_op_flw:
			case riscv_op_fld:
			case riscv_op_sb:
			case riscv_op_sh:
			case riscv_op_sw:
			case riscv_op_sd:
			case riscv_op_fsw:
			case riscv_op_fsd:
				sym_addr = int64_t(gp + dec.imm);
			default:
				break;
		}
	}

	// print address if present
	if (sym_addr != 0) {
		print_addr(offset, sym_addr, symlookup, colorize);
	}

	// save instruction in deque
	dec_hist.push_back(dec);
	if (dec_hist.size() > rvx_instruction_buffer_len) {
		dec_hist.pop_front();
	}

	printf("\n");
}
