//
//  riscv-disasm.cc
//

#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <cassert>
#include <map>
#include <algorithm>
#include <functional>
#include <vector>
#include <deque>
#include <string>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-meta.h"
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
	{ riscv_op_auipc,   riscv_op_ld,       rva_pcrel },
	{ riscv_op_unknown, riscv_op_unknown,  rva_none },
};

const size_t rvx_instruction_buffer_len = 16;

const char* riscv_null_symbol_lookup(riscv_ptr, bool nearest) { return nullptr; }
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
	const char* symbol_name = symlookup((riscv_ptr)addr, true);
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

void riscv_disasm_instruction(riscv_disasm &dec, std::deque<riscv_disasm> &dec_hist,
	riscv_ptr pc, riscv_ptr next_pc, riscv_ptr pc_offset, riscv_ptr gp,
	riscv_symbol_name_fn symlookup, riscv_symbol_colorize_fn colorize)
{
	size_t offset = 0;
	uint64_t addr = pc - pc_offset;
	const char *fmt = riscv_instruction_format[dec.op];
	const char *symbol_name = symlookup((riscv_ptr)addr, false);
	const riscv_csr_metadata *csr = nullptr;

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
	while (*fmt) {
		switch (*fmt) {
			case '(': print_add(offset, "("); break;
			case ',': print_add(offset, ","); break;
			case ')': print_add(offset, ")"); break;
			case '0': print_add(offset, riscv_i_registers[dec.rd]); break;
			case '1': print_add(offset, riscv_i_registers[dec.rs1]); break;
			case '2': print_add(offset, riscv_i_registers[dec.rs2]); break;
			case '3': print_add(offset, riscv_f_registers[dec.rd]); break;
			case '4': print_add(offset, riscv_f_registers[dec.rs1]); break;
			case '5': print_add(offset, riscv_f_registers[dec.rs2]); break;
			case '6': print_add(offset, riscv_f_registers[dec.rs3]); break;
			case '7': print_fmt(offset, "%d", dec.rs1); break;
			case 'i': print_fmt(offset, "%lld", dec.imm); break;
 			case 'd':
				addr = pc - pc_offset + dec.imm;
				print_fmt(offset, "%lld", dec.imm);
				print_addr(offset, addr, symlookup, colorize);
				break;
			case 'c':
				csr = riscv_lookup_csr_metadata(dec.imm);
				if (csr) print_fmt(offset, "%s", csr->csr_name);
				else print_fmt(offset, "%llu", dec.imm);
				break;
			case 'r':
				switch(dec.arg) {
					case riscv_rm_rne: print_add(offset, "rne"); break;
					case riscv_rm_rtz: print_add(offset, "rtz"); break;
					case riscv_rm_rdn: print_add(offset, "rdn"); break;
					case riscv_rm_rup: print_add(offset, "rup"); break;
					case riscv_rm_rmm: print_add(offset, "rmm"); break;
					default:           print_add(offset, "unk"); break;
				}
				break;
			case 'a':
				switch(dec.arg) {
					case riscv_aqrl_relaxed: print_add(offset, "relaxed"); break;
					case riscv_aqrl_acquire: print_add(offset, "acquire"); break;
					case riscv_aqrl_release: print_add(offset, "release"); break;
					case riscv_aqrl_acq_rel: print_add(offset, "acq_rel"); break;
				}
 				break;
			default:
				break;
		}
		fmt++;
	}

	// decode address using instruction pair constraints
	addr = 0;
	const rvx* rvxi = rvx_constraints;
	while(rvxi->addr != rva_none) {
		if (rvxi->op2 == dec.op) {
			for (auto li = dec_hist.rbegin(); li != dec_hist.rend(); li++) {
				if (rvxi->op1 != li->op && dec.rs1 == li->rd) break; // break: another primitive encountered
				if (rvxi->op1 != li->op && dec.rd == li->rd) break;  // break: destination register touched
				if (rvxi->op1 != li->op || dec.rs1 != li->rd) continue; // continue: not the right pair
				switch (rvxi->addr) {
					case rva_abs:
						addr = li->imm + dec.imm;
						goto out;
					case rva_pcrel:
						addr = li->pc - pc_offset + li->imm + dec.imm;
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
	if (addr == 0 && gp && dec.rs1 == riscv_ireg_gp)
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
				addr = int64_t(gp + dec.imm);
			default:
				break;
		}
	}

	// print address if present
	if (addr != 0) {
		print_addr(offset, addr, symlookup, colorize);
	}

	// save instruction in deque
	dec_hist.push_back(dec);
	if (dec_hist.size() > rvx_instruction_buffer_len) {
		dec_hist.pop_front();
	}

	printf("\n");
}
