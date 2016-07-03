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
#include "riscv-jit.h"
#include "riscv-util.h"
#include "riscv-csr.h"
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
	uint32_t  label_target;      /* label target for this instruction */
	uint32_t  label_pair;        /* target of first instruction in pair */
	uint32_t  label_branch;      /* target of jump, jump and link or branch  */
	uint32_t  label_cont;        /* target of continuation following jumps */
	uint32_t  is_abs       : 1;  /* absolute address present */
	uint32_t  is_pcrel     : 1;  /* pc relative address present */
	uint32_t  is_gprel     : 1;  /* gp relative address present */

	riscv_asm() : riscv_disasm(),
		label_target(0), label_pair(0), label_branch(0), label_cont(0),
		is_abs(0), is_pcrel(0), is_gprel(0) {}
};

struct riscv_compress_elf
{
	elf_file elf;
	std::string filename;
	std::string output_filename;
	std::map<uintptr_t,uint32_t> continuations;
	std::map<uintptr_t,uintptr_t> relocations;
	ssize_t continuation_num = 1;

	bool do_print_disassembly = false;
	bool do_print_continuations = false;
	bool do_print_relocations = false;
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

	void print_continuation_disassembly_header()
	{
		debug("\n%-18s %-7s%-7s%-7s%-7s%-12s%-18s%-18s %s",
			"program counter",
			"target",
			"pair",
			"cont",
			"branch",
			"instruction",
			"operands",
			"address",
			"flags"
		);
		debug("%-18s %-7s%-7s%-7s%-7s%-12s%-18s%-18s %s\n",
			"==================",
			"======",
			"======",
			"======",
			"======",
			"===========",
			"=================",
			"==================",
			"====="
		);
	}

	template <typename T>
	void print_continuation_disassembly(T &dec)
	{
		std::string args;
		const char *fmt = riscv_insn_format[dec.op];
		while (*fmt) {
			switch (*fmt) {
				case 'O': args += riscv_insn_name[dec.op]; break;
				case '(': args += "("; break;
				case ',': args += ","; break;
				case ')': args += ")"; break;
				case '0': args += riscv_i_registers[dec.rd]; break;
				case '1': args += riscv_i_registers[dec.rs1]; break;
				case '2': args += riscv_i_registers[dec.rs2]; break;
				case '3': args += riscv_f_registers[dec.rd]; break;
				case '4': args += riscv_f_registers[dec.rs1]; break;
				case '5': args += riscv_f_registers[dec.rs2]; break;
				case '6': args += riscv_f_registers[dec.rs3]; break;
				case '7': args += format_string("%d", dec.rs1); break;
				case 'i': args += format_string("%lld", dec.imm); break;
				case 'o': args += format_string("%lld", dec.imm); break;
				case 'c': {
					auto csr = riscv_lookup_csr_metadata(dec.imm);
					if (csr) args += format_string("%s", csr->csr_name);
					else args += format_string("%llu", dec.imm);
					break;
				}
				case 'r':
					switch(dec.rm) {
						case riscv_rm_rne: args += "rne"; break;
						case riscv_rm_rtz: args += "rtz"; break;
						case riscv_rm_rdn: args += "rdn"; break;
						case riscv_rm_rup: args += "rup"; break;
						case riscv_rm_rmm: args += "rmm"; break;
						default:           args += "unk"; break;
					}
					break;
				case '\t': while (args.length() < 12) args += " "; break;
				case 'A': if (dec.aq) args += ".aq"; break;
				case 'R': if (dec.rl) args += ".rl"; break;
				default:
					break;
			}
			fmt++;
		}
		debug("0x%016tx %-7s%-7s%-7s%-7s%-30s%-18s %s%s%s",
			dec.pc,
			dec.label_target ? format_string("%u", dec.label_target).c_str() : "",
			dec.label_pair ? format_string("%u", dec.label_pair).c_str() : "",
			dec.label_cont ? format_string("%u", dec.label_cont).c_str() : "",
			dec.label_branch ? format_string("%u", dec.label_branch).c_str() : "",
			args.c_str(),
			(dec.is_abs || dec.is_pcrel || dec.is_gprel) ? format_string("0x%016tx", dec.addr).c_str() : "",
			dec.is_abs ? "abs" : "",
			dec.is_pcrel ? "pc" : "",
			dec.is_gprel ? "gp" : ""
		);
	}


	std::map<uintptr_t,uint32_t>::iterator get_continuation(uintptr_t addr)
	{
		auto ci = continuations.find(addr);
		if (ci == continuations.end()) {
			ci = continuations.insert(std::pair<uintptr_t,uint32_t>(addr, continuation_num++)).first;
		}
		return ci;
	}

	// decode address using instruction pair constraints and label continuations for jump and link register
	template <typename T>
	bool decode_pairs(T &dec, uintptr_t start, uintptr_t end,
		typename std::deque<T>::iterator bi,
		typename std::deque<T>::iterator bend,
		std::deque<T> &dec_hist)
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
							uint64_t cont_addr = li->pc;
							auto ci = get_continuation(cont_addr);
							dec.label_pair = ci->second;
							if (dec.addr >= start && dec.addr < end) {
								ci = get_continuation(dec.addr);
							}
							return true;
						}
						case rva_pcrel:
						{
							dec.is_pcrel = true;
							dec.addr = li->pc + li->imm + dec.imm;
							uint64_t cont_addr = li->pc;
							auto ci = get_continuation(cont_addr);
							dec.label_pair = ci->second;
							if (dec.op == riscv_op_jalr) {
								if (bi + 1 != bend) {
									uint64_t cont_addr = (bi + 1)->pc;
									auto ci = get_continuation(cont_addr);
									dec.label_cont = ci->second;
								}
							}
							if (dec.addr >= start && dec.addr < end) {
								ci = get_continuation(dec.addr);
								dec.label_branch = ci->second;
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

	// decode address for branches and label jumps and continuations for jump and link
	template <typename T>
	bool deocde_jumps(T &dec, uintptr_t start, uintptr_t end,
		typename std::deque<T>::iterator bi,
		typename std::deque<T>::iterator bend)
	{
		switch (dec.op) {
			case riscv_op_jal:
			{
				dec.is_pcrel = true;
				dec.addr = dec.pc + dec.imm;
				if (dec.addr >= start && dec.addr < end) {
					auto ci = get_continuation(dec.addr);
					dec.label_branch = ci->second;
				}
			}
			case riscv_op_jalr:
			{
				if (bi + 1 != bend) {
					uint64_t cont_addr = (bi + 1)->pc;
					auto ci = get_continuation(cont_addr);
					dec.label_cont = ci->second;
				}
				return true;
			}
			default:
				break;
		}
		switch (dec.codec) {
			case riscv_codec_sb:
			{
				dec.is_pcrel = true;
				dec.addr = dec.pc + dec.imm;
				if (dec.addr >= start && dec.addr < end) {
					auto ci = get_continuation(dec.addr);
					dec.label_branch = ci->second;
				}
				return true;
			}
			default:
				break;
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
			dec.pc = pc - pc_offset;
			dec.insn = riscv_get_insn(pc, &next_pc);
			riscv_decode_insn_rv64(dec, dec.insn);
			riscv_decompress_insn_rv64(dec);
			pc = next_pc;
		}
	}

	void scan_continuations(std::deque<riscv_asm> &bin, uintptr_t start, uintptr_t end, uintptr_t gp)
	{
		std::deque<riscv_asm> dec_hist;

		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;

			// decode address and label continuations
			bool decoded_address = false;
			if (!decoded_address) decoded_address = decode_pairs(dec, start, end, bi, bin.end(), dec_hist);
			if (!decoded_address) decoded_address = deocde_jumps(dec, start, end, bi, bin.end());
			if (!decoded_address) decoded_address = deocde_gprel(dec, gp);

			// clear instruction history on jump boundaries
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

	void label_contntinuations(std::deque<riscv_asm> &bin)
	{
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			auto ci = continuations.find(dec.pc);
			if (ci == continuations.end()) continue;
			dec.label_target = ci->second;
		}
	}

	std::pair<size_t,size_t> compress(std::deque<riscv_asm> &bin)
	{
		size_t bytes = 0, saving = 0;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (bi != bin.begin()) {
				uintptr_t new_pc = (bi-1)->pc + riscv_insn_length((bi-1)->insn);
				elf.update_sym_addr(dec.pc, new_pc);
				relocations[dec.pc] = new_pc;
				auto ci = continuations.find(dec.pc);
				dec.pc = new_pc;
				if (ci != continuations.end()) {
					continuations.erase(ci);
					ci = continuations.insert(std::pair<uintptr_t,uint32_t>(dec.pc, ci->second)).first;
				}
			}
			if (riscv_insn_length(dec.insn) == 4 && riscv_compress_insn_rv64(dec)) {
				dec.insn = riscv_encode_insn(dec);
				bytes += 2;
				saving += 2;
			} else {
				bytes += 4;
			}
		}
		return std::pair<size_t,size_t>(bytes, saving);
	}

	void relocate(std::deque<riscv_asm> &bin, uintptr_t start, uintptr_t end)
	{
		std::map<uint32_t,intptr_t> label_addr;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (dec.label_target == 0) continue;
			label_addr[dec.label_target] = dec.pc;
		}
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (!dec.is_pcrel) continue; // NOTE: we don't currently relocate absolute references
			if (dec.label_pair > 0) {
				auto rbi = bi;
				if (dec.label_branch) dec.addr = label_addr[dec.label_branch]; // points to code
				while (rbi->label_target != dec.label_pair) rbi--; // find first insn
				dec.imm = sign_extend<int64_t,12>(intptr_t(dec.addr - rbi->pc));
				rbi->imm = sign_extend<int64_t,32>(intptr_t(dec.addr - rbi->pc) & 0xfffff000);
				if (intptr_t(dec.imm + rbi->imm + rbi->pc) < intptr_t(dec.addr)) rbi->imm += 0x1000;
				if (dec.imm + rbi->imm + rbi->pc != dec.addr) {
					panic("unable to relocate instruction pair: %d", dec.label_pair);
					print_continuation_disassembly(dec);
				} else {
					dec.insn = riscv_encode_insn(dec);
					rbi->insn = riscv_encode_insn(*rbi);
				}
			} else if (dec.label_branch) {
				dec.imm = label_addr[dec.label_branch] - intptr_t(dec.pc);
				dec.addr = dec.pc + dec.imm;
				dec.insn = riscv_encode_insn(dec);
			}
		}
	}

	void pad_with_nops(std::deque<riscv_asm> &bin, size_t size)
	{
		while (size > 0) {
			riscv_asm dec;
			dec.insn = emit_addi(riscv_ireg_x0, riscv_ireg_x0, 0);
			riscv_decode_insn_rv64(dec, dec.insn);
			dec.pc = bin.back().pc + riscv_insn_length(bin.back().insn);
			if (size == 2) {
				riscv_compress_insn_rv64(dec);
				dec.insn = riscv_encode_insn(dec);
			}
			size -= riscv_insn_length(dec.insn);
			bin.push_back(dec);
		}
	}

	void reassemble(std::deque<riscv_asm> &bin, uintptr_t start, uintptr_t end, uintptr_t pc_offset)
	{
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			uintptr_t pc = dec.pc + pc_offset;
			if (pc < start || pc > end) {
				panic("pc outside of section range");
			}
			size_t insn_len = riscv_insn_length(dec.insn);
			switch (insn_len) {
				case 2: *((uint16_t*)pc) = htole16(dec.insn); break;
				case 4: *((uint32_t*)pc) = htole32(dec.insn); break;
				default: panic("can only handle 2 or 4 byte insns");
			}
			pc += insn_len;
		}
	}

	void print_continuations(std::deque<riscv_asm> &bin, uintptr_t gp)
	{
		size_t line = 0;
		std::deque<riscv_disasm> dec_hist;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (line % 20 == 0) print_continuation_disassembly_header();
			print_continuation_disassembly(dec);
			line++;
		}
	}

	void print_disassembly(std::deque<riscv_asm> &bin, uintptr_t gp)
	{
		std::deque<riscv_disasm> dec_hist;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			riscv_disasm_insn(dec, dec_hist, dec.pc, dec.pc + riscv_insn_length(dec.insn), 0, gp,
				std::bind(&riscv_compress_elf::symlookup, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&riscv_compress_elf::colorize, this, std::placeholders::_1));
		}
	}

	void relocate_section_array(int sh_type)
	{
		size_t shdr_idx = elf.section_offset_by_type(sh_type);
		if (shdr_idx == 0) return;
		Elf64_Shdr &shdr = elf.shdrs[shdr_idx];
		for (size_t i = 0; i < shdr.sh_size; i += sizeof(Elf64_Addr)) {
			Elf64_Addr *addr = (Elf64_Addr*)elf.offset(shdr.sh_offset + i);
			if (addr == nullptr) continue;
			auto ri = relocations.find(*addr);
			if (ri == relocations.end()) continue;
			if (do_print_relocations) {
				debug("relocate section %s: 0x%016tx -> 0x%016tx", elf.shdr_name(shdr_idx), *addr, ri->second);
			}
			*addr = ri->second;
		}
	}

	void compress()
	{
		ssize_t bytes = 0, saving = 0;
		const Elf64_Sym *gp_sym = elf.sym_by_name("_gp");
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR)
			{
				std::deque<riscv_asm> bin;
				uintptr_t offset = (uintptr_t)elf.sections[i].buf.data();
				disassemble(bin, offset, offset + shdr.sh_size, offset - shdr.sh_addr);
				scan_continuations(bin, shdr.sh_addr, shdr.sh_addr + shdr.sh_size, uintptr_t(gp_sym ? gp_sym->st_value : 0));
				label_contntinuations(bin);
				auto res = compress(bin);
				relocate(bin, offset, offset + shdr.sh_size);

				// TODO - initial prototype pads text section with nop (addi x0,x0,0)
				//      - relocate doesn't relocate absolute references.
				//      - relocate may need to scan data segment for code pointers.

				pad_with_nops(bin, res.second); /* res.second = saving */
				reassemble(bin, offset, offset + shdr.sh_size, offset - shdr.sh_addr);

				if (do_print_continuations) {
					printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf.shdr_name(i), colorize("reset"));
					print_continuations(bin, uintptr_t(gp_sym ? gp_sym->st_value : 0));
				}

				if (do_print_disassembly) {
					printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf.shdr_name(i), colorize("reset"));
					print_disassembly(bin, uintptr_t(gp_sym ? gp_sym->st_value : 0));
				}

				bytes += res.first;
				saving += res.second;
			}
		}

		relocate_section_array(SHT_INIT_ARRAY);
		relocate_section_array(SHT_FINI_ARRAY);

		debug("\nStats: before: %lu after: %lu saving: %lu (%5.2f %%)",
			bytes + saving, bytes, saving, (1.0f - (float)(bytes) / (float)(bytes + saving)) * 100.0f);
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
			{ "-r", "-print-relocations", cmdline_arg_type_none,
				"Print Relocations",
				[&](std::string s) { return (do_print_relocations = true); } },
			{ "-c", "--compress", cmdline_arg_type_none,
				"Compress",
				[&](std::string s) { return (do_compress = true); } },
			{ "-d", "--decompress", cmdline_arg_type_none,
				"Decompress",
				[&](std::string s) { return (do_decompress = true); } },
			{ "-o", "--output", cmdline_arg_type_string,
				"Output filename",
				[&](std::string s) { output_filename = s; return true; } },
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
			if (output_filename.size() > 0) {
				elf.save(output_filename);
			}
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
