//
//  rv-compress.cc
//

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

#include "host-endian.h"
#include "types.h"
#include "bits.h"
#include "format.h"
#include "meta.h"
#include "codec.h"
#include "assembler.h"
#include "jit.h"
#include "util.h"
#include "cmdline.h"
#include "color.h"
#include "strings.h"
#include "disasm.h"
#include "elf.h"
#include "elf-file.h"
#include "elf-format.h"

using namespace riscv;

struct spasm : disasm
{
	addr_t  addr;              /* decoded address if present */
	u32     label_target;      /* label target for this instruction */
	u32     label_pair;        /* target of first instruction in pair */
	u32     label_branch;      /* target of jump, jump and link or branch  */
	u32     label_cont;        /* target of continuation following jumps */
	u32     is_abs       : 1;  /* absolute address present */
	u32     is_pcrel     : 1;  /* pc relative address present */
	u32     is_gprel     : 1;  /* gp relative address present */

	spasm() : disasm(),
		label_target(0), label_pair(0), label_branch(0), label_cont(0),
		is_abs(0), is_pcrel(0), is_gprel(0) {}
};

struct rv_compress_elf
{
	elf_file elf;
	std::string filename;
	std::string output_filename;
	std::map<addr_t,label_t> continuations;
	std::map<addr_t,addr_t> relocations;
	ssize_t continuation_num = 1;

	bool do_print_disassembly = false;
	bool do_print_continuations = false;
	bool do_print_relocations = false;
	bool do_print_external = false;
	bool do_compress = false;
	bool do_decompress = false;
	bool help_or_error = false;

	const char* colorize(const char *type)
	{
		return "";
	}

	const char* symlookup(addr_t addr, bool nearest)
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
		debug("\n%-18s %-7s%-7s%-7s%-7s%-12s%-23s%-18s %s",
			"program counter", "target", "pair", "cont",
			"branch", "instruction", "operands",
			"address", "flags"
		);
		debug("%-18s %-7s%-7s%-7s%-7s%-12s%-23s%-18s %s\n",
			"==================", "======", "======", "======",
			"======", "===========", "======================",
			"==================", "====="
		);
	}

	template <typename T>
	void print_continuation_disassembly(T &dec)
	{
		std::string args = disasm_inst_simple(dec);
		debug("0x%016llx %-7s%-7s%-7s%-7s%-35s%-18s %s%s%s",
			dec.pc,
			dec.label_target ? format_string("%u", dec.label_target).c_str() : "",
			dec.label_pair ? format_string("%u", dec.label_pair).c_str() : "",
			dec.label_cont ? format_string("%u", dec.label_cont).c_str() : "",
			dec.label_branch ? format_string("%u", dec.label_branch).c_str() : "",
			args.c_str(),
			(dec.is_abs || dec.is_pcrel || dec.is_gprel) ? format_string("0x%016llx", dec.addr).c_str() : "",
			dec.is_abs ? "abs" : "",
			dec.is_pcrel ? "pc" : "",
			dec.is_gprel ? "gp" : ""
		);
	}

	// helper for creating continiation address entries
	std::map<addr_t,label_t>::iterator get_continuation(addr_t addr)
	{
		auto ci = continuations.find(addr);
		if (ci == continuations.end()) {
			ci = continuations.insert(std::pair<addr_t,label_t>(addr, continuation_num++)).first;
		}
		return ci;
	}

	// decode address using instruction pair constraints and label continuations for jump and link register
	template <typename T>
	bool decode_pairs(T &dec, addr_t start, addr_t end,
		typename std::deque<T>::iterator bi,
		typename std::deque<T>::iterator bend,
		std::deque<T> &dec_hist)
	{
		const rvx* rvxi = rvx_constraints;
		while(rvxi->addr != rva_none) {
			if (rvxi->op2 == dec.op) {
				for (auto li = dec_hist.rbegin(); li != dec_hist.rend(); li++) {

					// break if another primitive encountered on the register
					if (rvxi->op1 != li->op && dec.rs1 == li->rd) break;

					// continue if until reaching the paired instruction
					if (rvxi->op1 != li->op || dec.rs1 != li->rd) continue;

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
							if (dec.op == rv_op_jalr) {
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
	bool deocde_jumps(T &dec, addr_t start, addr_t end,
		typename std::deque<T>::iterator bi,
		typename std::deque<T>::iterator bend)
	{
		switch (dec.op) {
			case rv_op_jal:
			{
				dec.is_pcrel = true;
				dec.addr = dec.pc + dec.imm;
				if (dec.addr >= start && dec.addr < end) {
					auto ci = get_continuation(dec.addr);
					dec.label_branch = ci->second;
				}
			}
			case rv_op_jalr:
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
			case rv_codec_sb:
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
	bool deocde_gprel(T &dec, addr_t gp)
	{
		if (!gp || dec.rs1 != rv_ireg_gp) return false;
		switch (dec.op) {
			case rv_op_addi:
			case rv_op_lb:
			case rv_op_lh:
			case rv_op_lw:
			case rv_op_ld:
			case rv_op_lbu:
			case rv_op_lhu:
			case rv_op_lwu:
			case rv_op_flw:
			case rv_op_fld:
			case rv_op_sb:
			case rv_op_sh:
			case rv_op_sw:
			case rv_op_sd:
			case rv_op_fsw:
			case rv_op_fsd:
				dec.is_gprel = true;
				dec.addr = int64_t(gp + dec.imm);
				return true;
			default:
				break;
		}
		return false;
	}

	void disassemble(std::deque<spasm> &bin, addr_t start, addr_t end, addr_t pc_bias)
	{
		addr_t pc_offset;
		addr_t pc = start;
		while (pc < end) {
			bin.resize(bin.size() + 1);
			auto &dec = bin.back();
			dec.pc = pc - pc_bias;
			dec.inst = inst_fetch(pc, pc_offset);
			decode_inst_rv64(dec, dec.inst);
			decompress_inst_rv64(dec);
			pc += pc_offset;
		}
	}

	void scan_continuations(std::deque<spasm> &bin, addr_t start, addr_t end, addr_t gp)
	{
		std::deque<spasm> dec_hist;

		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;

			// decode address and label continuations
			bool decoded_address = false;
			if (!decoded_address) decoded_address = decode_pairs(dec, start, end, bi, bin.end(), dec_hist);
			if (!decoded_address) decoded_address = deocde_jumps(dec, start, end, bi, bin.end());
			if (!decoded_address) decoded_address = deocde_gprel(dec, gp);

			// clear instruction history on jump boundaries
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
	}

	void label_contntinuations(std::deque<spasm> &bin)
	{
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			auto ci = continuations.find(dec.pc);
			if (ci == continuations.end()) continue;
			dec.label_target = ci->second;
		}
	}

	std::pair<size_t,size_t> compress(std::deque<spasm> &bin)
	{
		size_t bytes = 0, saving = 0;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (bi != bin.begin()) {
				addr_t new_pc = (bi-1)->pc + inst_length((bi-1)->inst);
				elf.update_sym_addr(dec.pc, new_pc);
				relocations[dec.pc] = new_pc;
				auto ci = continuations.find(dec.pc);
				dec.pc = new_pc;
				if (ci != continuations.end()) {
					continuations.erase(ci);
					ci = continuations.insert(std::pair<addr_t,label_t>(dec.pc, ci->second)).first;
				}
			}
			if (inst_length(dec.inst) == 4 && compress_inst_rv64(dec)) {
				dec.inst = encode_inst(dec);
				bytes += 2;
				saving += 2;
			} else {
				bytes += 4;
			}
		}
		return std::pair<size_t,size_t>(bytes, saving);
	}

	void relocate(std::deque<spasm> &bin, addr_t start, addr_t end)
	{
		std::map<label_t,addr_t> label_addr;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (dec.label_target == 0) continue;
			label_addr[dec.label_target] = dec.pc;
		}
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (dec.label_pair > 0) {
				auto rbi = bi;
				if (dec.label_branch) dec.addr = label_addr[dec.label_branch];
				while (rbi->label_target != dec.label_pair) rbi--;
				int64_t addr = dec.addr - rbi->pc;
				int64_t upper = ((addr + 0x800) >> 12) << 12;
				dec.imm = addr - upper;
				rbi->imm = upper;
				if (dec.imm + rbi->imm + rbi->pc != dec.addr) {
					panic("unable to relocate instruction pair: %d", dec.label_pair);
					print_continuation_disassembly(dec);
				} else {
					dec.inst = encode_inst(dec);
					rbi->inst = encode_inst(*rbi);
				}
			} else if (dec.label_branch) {
				dec.imm = label_addr[dec.label_branch] - addr_t(dec.pc);
				dec.addr = dec.pc + dec.imm;
				dec.inst = encode_inst(dec);
			}
		}
	}

	void pad_with_nops(std::deque<spasm> &bin, size_t size)
	{
		while (size > 0) {
			spasm dec;
			dec.inst = emit_addi(rv_ireg_x0, rv_ireg_x0, 0);
			decode_inst_rv64(dec, dec.inst);
			dec.pc = bin.back().pc + inst_length(bin.back().inst);
			if (size == 2) {
				compress_inst_rv64(dec);
				dec.inst = encode_inst(dec);
			}
			size -= inst_length(dec.inst);
			bin.push_back(dec);
		}
	}

	void reassemble(std::deque<spasm> &bin, addr_t start, addr_t end, addr_t pc_bias)
	{
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			addr_t pc = dec.pc + pc_bias;
			if (pc < start || pc > end) {
				panic("pc outside of section range");
			}
			size_t inst_len = inst_length(dec.inst);
			switch (inst_len) {
				case 2: *((u16*)pc) = htole16(dec.inst); break;
				case 4: *((u32*)pc) = htole32(dec.inst); break;
				default: panic("can only handle 2 or 4 byte insts");
			}
			pc += inst_len;
		}
	}

	void print_external(std::deque<spasm> &bin, addr_t start, addr_t end, addr_t pc_bias)
	{
		std::map<label_t,addr_t> label_addr;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if ((dec.is_pcrel || dec.is_abs || dec.is_gprel) &&
				(dec.addr < (start - pc_bias) || dec.addr >= (end - pc_bias))) {
				print_continuation_disassembly(dec);
			}
		}
	}

	void print_continuations(std::deque<spasm> &bin, addr_t gp)
	{
		size_t line = 0;
		std::deque<disasm> dec_hist;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			if (line % 20 == 0) print_continuation_disassembly_header();
			print_continuation_disassembly(dec);
			line++;
		}
	}

	void print_disassembly(std::deque<spasm> &bin, addr_t gp)
	{
		std::deque<disasm> dec_hist;
		for (auto bi = bin.begin(); bi != bin.end(); bi++) {
			auto &dec = *bi;
			disasm_inst_print(dec, dec_hist, dec.pc, 0, gp,
				std::bind(&rv_compress_elf::symlookup, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&rv_compress_elf::colorize, this, std::placeholders::_1));
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
				debug("relocate section %s: 0x%016llx -> 0x%016llx", elf.shdr_name(shdr_idx), addr_t(*addr), addr_t(ri->second));
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
				printf("\nSection[%2lu] %s (0x%llx - 0x%llx)\n",
					i, elf.shdr_name(i), addr_t(shdr.sh_addr), addr_t(shdr.sh_addr + shdr.sh_size));

				std::deque<spasm> bin;
				addr_t offset = (addr_t)elf.sections[i].buf.data();
				disassemble(bin, offset, offset + shdr.sh_size, offset - shdr.sh_addr);
				scan_continuations(bin, shdr.sh_addr, shdr.sh_addr + shdr.sh_size, addr_t(gp_sym ? gp_sym->st_value : 0));
				label_contntinuations(bin);
				auto res = compress(bin);
				relocate(bin, offset, offset + shdr.sh_size);

				// TODO - initial prototype pads text section with nop (addi x0,x0,0)
				//      - relocate doesn't relocate absolute references.
				//      - relocate doesn't relocate references outside the text segment
				//      - relocate may need to scan data segment for code pointers.

				pad_with_nops(bin, res.second); /* res.second = saving */
				reassemble(bin, offset, offset + shdr.sh_size, offset - shdr.sh_addr);

				bytes += res.first;
				saving += res.second;

				if (do_print_external) {
					print_external(bin, offset, offset + shdr.sh_size, offset - shdr.sh_addr);
				}

				if (do_print_continuations) {
					print_continuations(bin, addr_t(gp_sym ? gp_sym->st_value : 0));
				}

				if (do_print_disassembly) {
					print_disassembly(bin, addr_t(gp_sym ? gp_sym->st_value : 0));
				}
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
				"Print Relocations (.init .fini)",
				[&](std::string s) { return (do_print_relocations = true); } },
			{ "-e", "-print-external", cmdline_arg_type_none,
				"Print Not Relocated (referenecs outside .text)",
				[&](std::string s) { return (do_print_external = true); } },
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

int rv_compress_main(int argc, const char *argv[])
{
	printf("\n");
	printf("rv-compress-0.0.0-alpha-0\n");
	printf("\n");
	rv_compress_elf elf_compress;
	elf_compress.parse_commandline(argc, argv);
	elf_compress.run();
	return 0;
}
