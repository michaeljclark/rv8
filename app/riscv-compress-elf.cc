#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <algorithm>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include <unistd.h>

#include "riscv-types.h"
#include "riscv-endian.h"
#include "riscv-format.h"
#include "riscv-meta.h"
#include "riscv-util.h"
#include "riscv-cmdline.h"
#include "riscv-color.h"
#include "riscv-decode.h"
#include "riscv-disasm.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

using namespace riscv;

struct riscv_asm : riscv_disasm
{
	uint32_t  label_in;      /* target label for this insn */
	uint32_t  label_br;      /* branch, jump or jump and link  */
	uint32_t  label_pr;      /* link to first instruction in pair */
	uint32_t  label_co;      /* continuation after jump */
	uint32_t  is_abs   : 1;  /* absolute address present */
	uint32_t  is_pcrel : 1;  /* pc relative address present */
	uint32_t  is_gprel : 1;  /* gp relative address present */

	riscv_asm() : riscv_disasm(), label_in(0), label_br(0), label_pr(0), label_co(0),
		is_abs(0), is_pcrel(0), is_gprel(0) {}
};

struct riscv_compress_elf
{
	elf_file elf;
	std::string filename;
	std::map<uintptr_t,uint32_t> continuations;
	ssize_t continuation_num = 1;

	bool do_print_disassembly = false;
	bool do_print_continuations = false;
	bool do_compress = false;
	bool do_decompress = false;
	bool help_or_error = false;

	const char* colorize(const char *type)
	{
		return "";
	}

	const char* symlookup(uintptr_t addr, bool nearest)
	{
		static char symbol_tmpname[256];
		auto sym = elf.sym_by_addr((Elf64_Addr)addr);
		auto bli = continuations.find(addr);
		if (sym && bli != continuations.end()) {
				snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"LOC_%06" PRIu32 ":<%s>", bli->second, elf.sym_name(sym));
			return symbol_tmpname;
		}
		if (sym) {
			snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"<%s>", elf.sym_name(sym));
			return symbol_tmpname;
		}
		if (bli != continuations.end()) {
			snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"LOC_%06" PRIu32, bli->second);
			return symbol_tmpname;
		}
		if (nearest) {
			sym = elf.sym_by_nearest_addr((Elf64_Addr)addr);
			if (sym) {
				int64_t offset = int64_t(addr) - sym->st_value;
				snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"<%s%s0x%" PRIx64 ">", elf.sym_name(sym),
					offset < 0 ? "-" : "+", offset < 0 ? -offset : offset);
				return symbol_tmpname;
			}
		}
		return nullptr;
	}

	// decode address using instruction pair constraints
	template <typename T>
	bool deocde_pair(T &dec, std::deque<T> &dec_hist, uintptr_t pc_offset)
	{
		const rvx* rvxi = rvx_constraints;
		while(rvxi->addr != rva_none) {
			if (rvxi->op2 == dec.op) {
				for (auto li = dec_hist.rbegin(); li != dec_hist.rend(); li++) {
					if (rvxi->op1 != li->op && dec.rs1 == li->rd) break; // break: another primitive encountered
					if (rvxi->op1 != li->op || dec.rs1 != li->rd) continue; // continue: not the right pair
					switch (rvxi->addr) {
						case rva_abs:
						{
							dec.is_abs = true;
							dec.addr = li->imm + dec.imm;
							uint64_t cont_addr = li->pc - pc_offset;
							auto ci = continuations.find(cont_addr);
							if (ci == continuations.end()) {
								ci = continuations.insert(std::pair<uintptr_t,uint32_t>(cont_addr, continuation_num++)).first;
							}
							dec.label_pr = ci->second;
							return true;
						}
						case rva_pcrel:
						{
							dec.is_pcrel = true;
							dec.addr = li->pc - pc_offset + li->imm + dec.imm;
							uint64_t cont_addr = li->pc - pc_offset;
							auto ci = continuations.find(cont_addr);
							if (ci == continuations.end()) {
								ci = continuations.insert(std::pair<uintptr_t,uint32_t>(cont_addr, continuation_num++)).first;
							}
							dec.label_pr = ci->second;
							if (dec.op == riscv_op_jalr) {
								auto ci = continuations.find(dec.addr);
								if (ci == continuations.end()) {
									ci = continuations.insert(std::pair<uintptr_t,uint32_t>(dec.addr, continuation_num++)).first;
								}
								dec.label_br = ci->second;
							}
							return true;
						}
						case rva_none:
						default:
							continue;
					}
					break;
				}
			}
			rvxi++;
		}
		return false;
	}

	// decode address for loads and stores from the global pointer
	template <typename T>
	bool deocde_gprel(T &dec, uintptr_t gp)
	{
		if (!gp || dec.rs1 != riscv_ireg_gp) return false;
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
				dec.is_gprel = true;
				dec.addr = int64_t(gp + dec.imm);
				return true;
			default:
				break;
		}
		return false;
	}

	void disassemble(std::deque<riscv_asm> &bin, uintptr_t start, uintptr_t end, uintptr_t pc_offset)
	{
		uintptr_t pc = start, next_pc;
		while (pc < end) {
			bin.resize(bin.size() + 1);
			auto &dec = bin.back();
			dec.pc = pc;
			dec.insn = riscv_get_insn(pc, &next_pc);
			riscv_decode_rv64(dec, dec.insn);
			riscv_decode_decompress(dec);
			pc = next_pc;
		}
	}

	void scan_continuations(std::deque<riscv_asm> &bin, uintptr_t pc_offset, uintptr_t gp)
	{
		std::deque<riscv_asm> dec_hist;

		// label instructions that are the destination of a jump or a continuation
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			switch (dec.op) {
				case riscv_op_jal:
				{
					dec.is_pcrel = true;
					dec.addr = dec.pc - pc_offset + dec.imm;
					auto ci = continuations.find(dec.addr);
					if (ci == continuations.end()) {
						ci = continuations.insert(std::pair<uintptr_t,uint32_t>(dec.addr, continuation_num++)).first;
					}
					dec.label_br = ci->second;
				}
				case riscv_op_jalr:
				{
					if (bi != bin.end()) {
						uint64_t cont_addr = (bi + 1)->pc - pc_offset;
						auto ci = continuations.find(cont_addr);
						if (ci == continuations.end()) {
							ci = continuations.insert(std::pair<uintptr_t,uint32_t>(cont_addr, continuation_num++)).first;
						}
						dec.label_co = ci->second;
					}
					break;
				}
				default:
					break;
			}
			switch (dec.codec) {
				case riscv_codec_sb:
				{
					dec.is_pcrel = true;
					dec.addr = dec.pc - pc_offset + dec.imm;
					auto ci = continuations.find(dec.addr);
					if (ci == continuations.end()) {
						ci = continuations.insert(std::pair<uintptr_t,uint32_t>(dec.addr, continuation_num++)).first;
					}
					dec.label_br = ci->second;
					break;
				}
				default:
					break;
			}

			bool decoded_address = false;

			// decode instruction pair address
			if (!decoded_address) decoded_address = deocde_pair(dec, dec_hist, pc_offset);

			// decode address for loads and stores from the global pointer
			if (!decoded_address) decoded_address = deocde_gprel(dec, gp);

			// clear the instruction history on jump boundaries
			switch(dec.op) {
				case riscv_op_jal:
				case riscv_op_jalr:
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
	}

	void label_continuations(std::deque<riscv_asm> &bin, uintptr_t pc_offset)
	{
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			auto ci = continuations.find(dec.pc - pc_offset);
			if (ci == continuations.end()) continue;
			dec.label_in = ci->second;
		}
	}

	std::pair<size_t,size_t> compress(std::deque<riscv_asm> &bin)
	{
		size_t bytes = 0, saving = 0;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (riscv_get_insn_length(dec.insn) == 4 && riscv_compress_insn(dec)) {
				dec.insn = riscv_encode_insn(dec);
				bytes += 2;
				saving += 2;
			} else {
				bytes += 4;
			}
		}
		return std::pair<size_t,size_t>(bytes, saving);
	}

	void print_continuations(std::deque<riscv_asm> &bin, uintptr_t pc_offset, uintptr_t gp)
	{
		std::deque<riscv_disasm> dec_hist;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			printf("0x%016tx %-9s %-9s %-9s %-9s %-20s %-20s %s%s%s\n",
				dec.pc - pc_offset,
				dec.label_in ? format_string("loc_%u", dec.label_in).c_str() : "",
				dec.label_co ? format_string("cont_%u", dec.label_co).c_str() : "",
				dec.label_pr ? format_string("pair_%u", dec.label_pr).c_str() : "",
				dec.label_br ? format_string("bra_%u", dec.label_br).c_str() : "",
				riscv_insn_name[dec.op],
				(dec.is_abs || dec.is_pcrel || dec.is_gprel) ? format_string("0x%016tx", dec.addr).c_str() : "",
				dec.is_abs ? "abs" : "",
				dec.is_pcrel ? "pc" : "",
				dec.is_gprel ? "gp" : ""
			);
		}
	}

	void print_disassembly(std::deque<riscv_asm> &bin, uintptr_t pc_offset, uintptr_t gp)
	{
		std::deque<riscv_disasm> dec_hist;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			riscv_disasm_insn(dec, dec_hist, dec.pc, dec.pc + riscv_get_insn_length(dec.insn), pc_offset, gp,
				std::bind(&riscv_compress_elf::symlookup, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&riscv_compress_elf::colorize, this, std::placeholders::_1));
		}
	}

	void compress()
	{
		const Elf64_Sym *gp_sym = elf.sym_by_name("_gp");
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				uintptr_t offset = (uintptr_t)elf.sections[i].buf.data();
				std::deque<riscv_asm> bin;
				disassemble(bin, offset, offset + shdr.sh_size, offset - shdr.sh_addr);
				scan_continuations(bin, offset - shdr.sh_addr, uintptr_t(gp_sym ? gp_sym->st_value : 0));
				label_continuations(bin, offset - shdr.sh_addr);
				auto res = compress(bin);

				// TODO - reassign pc based on new insn lengths (and inserted or removed insns)
				//      - relocate jumps, branches using labels
				//      - relocate load store offsets outside the executable section due
				//        shrinkage of the executable section. May need to scan the data
				//        segment for function pointers.

				if (do_print_continuations) {
					printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf.shdr_name(i), colorize("reset"));
					print_continuations(bin, offset- shdr.sh_addr, uintptr_t(gp_sym ? gp_sym->st_value : 0));
				}

				if (do_print_disassembly) {
					printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf.shdr_name(i), colorize("reset"));
					print_disassembly(bin, offset- shdr.sh_addr, uintptr_t(gp_sym ? gp_sym->st_value : 0));
				}

				// TODO - repack and save ELF

				ssize_t bytes = res.first, saving = res.second;
				printf("\nStats: before: %lu after: %lu saving: %lu (%5.2f %%)   // TODO - Relocate and save\n",
					bytes + saving, bytes, saving, (1.0f - (float)(bytes) / (float)(bytes + saving)) * 100.0f);
			}
		}
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-x", "-print-disassembly", cmdline_arg_type_none,
				"Print Disassembly",
				[&](std::string s) { return (do_print_disassembly = true); } },
			{ "-y", "-print-continuations", cmdline_arg_type_none,
				"Print Continuations",
				[&](std::string s) { return (do_print_continuations = true); } },
			{ "-c", "--compress", cmdline_arg_type_none,
				"Compress",
				[&](std::string s) { return (do_compress = true); } },
			{ "-d", "--decompress", cmdline_arg_type_none,
				"Decompress",
				[&](std::string s) { return (do_decompress = true); } },
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() != 1) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if ((help_or_error |= !do_compress && !do_decompress))
		{
			printf("usage: %s [<options>] <elf_file>\n", argv[0]);
			cmdline_option::print_options(options);
			exit(9);
		}

		filename = result.first[0];
	}

	void run()
	{
		elf.load(filename);
		if (do_compress && elf.ehdr.e_machine == EM_RISCV) {
			compress();
		}
		printf("\n");
	}
};

int main(int argc, const char *argv[])
{
	riscv_compress_elf elf_compress;
	elf_compress.parse_commandline(argc, argv);
	elf_compress.run();
	return 0;
}
