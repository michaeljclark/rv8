//
//  disasm.cc
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

#include "types.h"
#include "host-endian.h"
#include "format.h"
#include "meta.h"
#include "codec.h"
#include "format.h"
#include "strings.h"
#include "util.h"
#include "disasm.h"

using namespace riscv;

const char* riscv::null_symbol_lookup(addr_t, bool nearest) { return nullptr; }
const char* riscv::null_symbol_colorize(const char *type) { return ""; }

static const void print_add(size_t &offset, const char *str)
{
	printf("%s", str);
	offset += strlen(str);
}

static const void print_pad(size_t &offset, size_t pad_to)
{
	static const char *space32 = "                                        ";
	if (pad_to < offset) pad_to = offset;
	size_t x = std::min(strlen(space32), std::max((pad_to - offset), 0UL));
	printf("%s", space32 + strlen(space32) - x);
	offset += x;
}

static const void print_fmt(size_t &offset, const char* fmt, ...)
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

static const void print_pad(size_t &offset, size_t pad_to, const char *str)
{
	print_add(offset, str);
	print_pad(offset, pad_to);
}

static const void print_addr(size_t &offset, addr_t addr,
	riscv::symbol_name_fn symlookup, riscv::symbol_colorize_fn colorize)
{
	print_pad(offset, 80);
	printf("%s", colorize("address"));
	print_add(offset, "# ");
	print_fmt(offset, "0x%016llx", addr);
	printf("%s", colorize("reset"));
	const char* symbol_name = symlookup((addr_t)addr, true);
	if (symbol_name) {
		printf(" %s", colorize("label"));
		print_fmt(offset, "%s", symbol_name);
		printf("%s", colorize("reset"));
	}
}

void riscv::disasm_inst_print(disasm &dec, std::deque<disasm> &dec_hist,
	addr_t pc, addr_t pc_bias, addr_t gp,
	riscv::symbol_name_fn symlookup, riscv::symbol_colorize_fn colorize)
{
	size_t offset = 0;
	addr_t addr = pc - pc_bias;
	const char *fmt = rv_inst_format[dec.op];
	const char *symbol_name = symlookup((addr_t)addr, false);
	const char* csr_name = nullptr;

	// print symbol name if present
	if (symbol_name) {
		printf("\n%s", colorize("address"));
		print_fmt(offset, "0x%016llx: ", addr);
		printf("%s", colorize("reset"));
		printf("%s", colorize("label"));
		print_fmt(offset, "%s", symbol_name);
		printf("%s\n", colorize("reset"));
		offset = 0;
	}
	print_pad(offset, 12);

	// print address
	printf("%s", colorize("address"));
	print_fmt(offset, "%8llx:", addr & 0xffffffff);
	printf("%s", colorize("reset"));
	print_pad(offset, 24);

	// print instruction bytes
	switch (inst_length(dec.inst)) {
		case 2: print_fmt(offset, "%04llx", dec.inst); break;
		case 4: print_fmt(offset, "%08llx", dec.inst); break;
		case 6: print_fmt(offset, "%012llx", dec.inst); break;
		case 8: print_fmt(offset, "%016llx", dec.inst); break;
	}
	print_pad(offset, 45);

	// print arguments
	while (*fmt) {
		switch (*fmt) {
			case '(': print_add(offset, "("); break;
			case ',': print_add(offset, ", "); break;
			case ')': print_add(offset, ")"); break;
			case '0': print_add(offset, rv_ireg_name_sym[dec.rd]); break;
			case '1': print_add(offset, rv_ireg_name_sym[dec.rs1]); break;
			case '2': print_add(offset, rv_ireg_name_sym[dec.rs2]); break;
			case '3': print_add(offset, rv_freg_name_sym[dec.rd]); break;
			case '4': print_add(offset, rv_freg_name_sym[dec.rs1]); break;
			case '5': print_add(offset, rv_freg_name_sym[dec.rs2]); break;
			case '6': print_add(offset, rv_freg_name_sym[dec.rs3]); break;
			case '7': print_fmt(offset, "%d", dec.rs1); break;
			case 'i': print_fmt(offset, "%d", dec.imm); break;
			case 'o':
				print_fmt(offset, "pc %c %td",
					intptr_t(dec.imm) < 0 ? '-' : '+',
					intptr_t(dec.imm) < 0 ? -intptr_t(dec.imm) : intptr_t(dec.imm));
				break;
			case 'c':
				csr_name = rv_csr_name_sym[dec.imm & 0xfff];
				if (csr_name) print_fmt(offset, "%s", csr_name);
				else print_fmt(offset, "0x%03x", dec.imm & 0xfff);
				break;
			case 'r':
				switch(dec.rm) {
					case rv_rm_rne: print_add(offset, "rne"); break;
					case rv_rm_rtz: print_add(offset, "rtz"); break;
					case rv_rm_rdn: print_add(offset, "rdn"); break;
					case rv_rm_rup: print_add(offset, "rup"); break;
					case rv_rm_rmm: print_add(offset, "rmm"); break;
					case rv_rm_dyn: print_add(offset, "dyn"); break;
					default:           print_add(offset, "inv"); break;
				}
				break;
			case 'p':
				if (dec.pred & rv_fence_i) print_add(offset, "i");
				if (dec.pred & rv_fence_o) print_add(offset, "o");
				if (dec.pred & rv_fence_r) print_add(offset, "r");
				if (dec.pred & rv_fence_w) print_add(offset, "w");
				break;
			case 's':
				if (dec.succ & rv_fence_i) print_add(offset, "i");
				if (dec.succ & rv_fence_o) print_add(offset, "o");
				if (dec.succ & rv_fence_r) print_add(offset, "r");
				if (dec.succ & rv_fence_w) print_add(offset, "w");
				break;
			case 'O':
				printf("%s", colorize("opcode"));
				print_add(offset, rv_inst_name_sym[dec.op]);
 				break;
			case '\t':
				print_pad(offset, 60, "");
				printf("%s", colorize("reset"));
 				break;
			case 'A':
				if (dec.aq) print_add(offset, ".aq");
				break;
			case 'R':
				if (dec.rl) print_add(offset, ".rl");
				break;
			default:
				break;
		}
		fmt++;
	}

	// decode address
	addr = 0;
	bool decoded_address = false;
	if (!decoded_address) decoded_address = decode_pcrel(dec, addr, pc, pc_bias);
	if (!decoded_address) decoded_address = decode_pairs(dec, addr, dec_hist, pc_bias);
	if (!decoded_address) decoded_address = deocde_gprel(dec, addr, gp);

	// print address if present
	if (decoded_address) print_addr(offset, addr, symlookup, colorize);
	printf("\n");

	// clear the instruction history on jump boundaries
	switch(dec.op) {
		case rv_op_jal:
		case rv_op_jalr:
			dec_hist.clear();
			break;
		default:
			break;
	}

	// save instruction in deque
	dec_hist.push_back(dec);
	if (dec_hist.size() > rvx_instruction_buffer_len) {
		dec_hist.pop_front();
	}
}
