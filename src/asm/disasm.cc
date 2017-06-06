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
#include "fmt.h"

using namespace riscv;

const char* riscv::null_symbol_lookup(addr_t, bool nearest) { return nullptr; }
const char* riscv::null_symbol_colorize(const char *type) { return ""; }

template <typename... Params>
inline void sprintf_fmt(size_t &offset, std::string &buf, std::string fmt, Params&&... params)
{
	size_t sz = buf.size();
	std::array<arg_type, sizeof...(Params)> bt;
	std::array<type_holder, sizeof...(Params)> tb;
	sprintf(buf, fmt, bt, tb, 0, std::forward<Params>(params)...);
	offset += (buf.size() - sz);
}

static const void sprintf_add(size_t &offset, std::string &buf, const char *str)
{
	buf += str;
	offset += strlen(str);
}

static const void sprintf_pad(size_t &offset, std::string &buf, size_t pad_to)
{
	static const char *space32 = "                                        ";
	if (pad_to < offset) pad_to = offset;
	size_t x = std::min(strlen(space32), (size_t)std::max(((ssize_t)pad_to - (ssize_t)offset), 0L));
	sprintf_fmt(offset, buf, "%s", space32 + strlen(space32) - x);
}

static const void sprintf_pad(size_t &offset, std::string &buf, size_t pad_to, const char *str)
{
	sprintf_add(offset, buf, str);
	sprintf_pad(offset, buf, pad_to);
}

static const void sprintf_addr(size_t &offset, std::string &buf, addr_t addr,
	riscv::symbol_name_fn symlookup, riscv::symbol_colorize_fn colorize)
{
	sprintf_pad(offset, buf, 80);
	sprintf(buf, colorize("address"));
	sprintf_add(offset, buf, "# ");
	sprintf_fmt(offset, buf, "0x%016llx", addr);
	buf += colorize("reset");
	const char* symbol_name = symlookup((addr_t)addr, true);
	if (symbol_name) {
		buf += " ";
		buf += colorize("label");
		buf += symbol_name;
		buf += colorize("reset");
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
	std::string buf;
	buf.reserve(256);

	// print symbol name if present
	if (symbol_name) {
		buf += "\n";
		buf += colorize("address");
		sprintf(buf, "0x%016llx: ", addr);
		buf += colorize("reset");
		buf += colorize("label");
		buf += symbol_name;
		buf += colorize("reset");
		buf += "\n";
		offset = 0;
	}
	sprintf_pad(offset, buf, 12);

	// print address
	buf += colorize("address");
	sprintf_fmt(offset, buf, "%8llx:", addr & 0xffffffff);
	buf += colorize("reset");
	sprintf_pad(offset, buf, 24);

	// print instruction bytes
	switch (inst_length(dec.inst)) {
		case 2: sprintf_fmt(offset, buf, "%04llx", dec.inst); break;
		case 4: sprintf_fmt(offset, buf, "%08llx", dec.inst); break;
		case 6: sprintf_fmt(offset, buf, "%012llx", dec.inst); break;
		case 8: sprintf_fmt(offset, buf, "%016llx", dec.inst); break;
	}
	sprintf_pad(offset, buf, 45);

	// print arguments
	while (*fmt) {
		switch (*fmt) {
			case '(': sprintf_add(offset, buf, "("); break;
			case ',': sprintf_add(offset, buf, ", "); break;
			case ')': sprintf_add(offset, buf, ")"); break;
			case '0': sprintf_add(offset, buf, rv_ireg_name_sym[dec.rd]); break;
			case '1': sprintf_add(offset, buf, rv_ireg_name_sym[dec.rs1]); break;
			case '2': sprintf_add(offset, buf, rv_ireg_name_sym[dec.rs2]); break;
			case '3': sprintf_add(offset, buf, rv_freg_name_sym[dec.rd]); break;
			case '4': sprintf_add(offset, buf, rv_freg_name_sym[dec.rs1]); break;
			case '5': sprintf_add(offset, buf, rv_freg_name_sym[dec.rs2]); break;
			case '6': sprintf_add(offset, buf, rv_freg_name_sym[dec.rs3]); break;
			case '7': sprintf_fmt(offset, buf, "%d", dec.rs1); break;
			case 'i': sprintf_fmt(offset, buf, "%d", dec.imm); break;
			case 'o':
				sprintf_fmt(offset, buf, "pc %c %td",
					intptr_t(dec.imm) < 0 ? '-' : '+',
					intptr_t(dec.imm) < 0 ? -intptr_t(dec.imm) : intptr_t(dec.imm));
				break;
			case 'c':
				csr_name = rv_csr_name_sym[dec.imm & 0xfff];
				if (csr_name) sprintf_fmt(offset, buf, "%s", csr_name);
				else sprintf_fmt(offset, buf, "0x%03x", dec.imm & 0xfff);
				break;
			case 'r':
				switch(dec.rm) {
					case rv_rm_rne: sprintf_add(offset, buf, "rne"); break;
					case rv_rm_rtz: sprintf_add(offset, buf, "rtz"); break;
					case rv_rm_rdn: sprintf_add(offset, buf, "rdn"); break;
					case rv_rm_rup: sprintf_add(offset, buf, "rup"); break;
					case rv_rm_rmm: sprintf_add(offset, buf, "rmm"); break;
					case rv_rm_dyn: sprintf_add(offset, buf, "dyn"); break;
					default:        sprintf_add(offset, buf, "inv"); break;
				}
				break;
			case 'p':
				if (dec.pred & rv_fence_i) sprintf_add(offset, buf, "i");
				if (dec.pred & rv_fence_o) sprintf_add(offset, buf, "o");
				if (dec.pred & rv_fence_r) sprintf_add(offset, buf, "r");
				if (dec.pred & rv_fence_w) sprintf_add(offset, buf, "w");
				break;
			case 's':
				if (dec.succ & rv_fence_i) sprintf_add(offset, buf, "i");
				if (dec.succ & rv_fence_o) sprintf_add(offset, buf, "o");
				if (dec.succ & rv_fence_r) sprintf_add(offset, buf, "r");
				if (dec.succ & rv_fence_w) sprintf_add(offset, buf, "w");
				break;
			case 'O':
				buf += colorize("opcode");
				sprintf_add(offset, buf, rv_inst_name_sym[dec.op]);
 				break;
			case '\t':
				sprintf_pad(offset, buf, 60, "");
				buf += colorize("reset");
 				break;
			case 'A':
				if (dec.aq) sprintf_add(offset, buf, ".aq");
				break;
			case 'R':
				if (dec.rl) sprintf_add(offset, buf, ".rl");
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
	if (decoded_address) sprintf_addr(offset, buf, addr, symlookup, colorize);
	printf("%s\n", buf.c_str());

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
