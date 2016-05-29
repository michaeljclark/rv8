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
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-disasm.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

struct riscv_compress_elf
{
	elf_file elf;
	std::string filename;
	std::map<riscv_ptr,std::string> branch_labels;

	bool do_compress = false;
	bool do_decompress = false;
	bool help_or_error = false;

	const char* colorize(const char *type)
	{
		return "";
	}

	const char* symlookup(riscv_ptr addr, bool nearest)
	{
		auto sym = elf.sym_by_addr((Elf64_Addr)addr);
		if (sym) return elf.sym_name(sym);
		auto bli = branch_labels.find(addr);
		if (bli != branch_labels.end()) return bli->second.c_str();
		if (nearest) {
			sym = elf.sym_by_nearest_addr((Elf64_Addr)addr);
			if (sym) {
				static char symbol_tmpname[256];
				int64_t offset = int64_t(addr) - sym->st_value;
				snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"%s%s0x%" PRIx64, elf.sym_name(sym),
					offset < 0 ? "-" : "+", offset < 0 ? -offset : offset);
				return symbol_tmpname;
			}
		}
		return nullptr;
	}

	void scan_branch_labels(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset)
	{
		ssize_t branch_num = 1;
		char branch_label[32];

		riscv_disasm dec;
		riscv_ptr pc = start, next_pc;
		uint64_t addr = 0;
		while (pc < end) {
			dec.pc = pc;
			dec.inst = riscv_get_instruction(pc, &next_pc);
			riscv_decode_instruction(dec, dec.inst);
			riscv_decode_decompress(dec);
			switch (dec.codec) {
				case riscv_codec_sb:
				case riscv_codec_uj:
					addr = pc - pc_offset + dec.imm;
					snprintf(branch_label, sizeof(branch_label), "LOC_%06lu", branch_num++);
					branch_labels[(riscv_ptr)addr] = branch_label;
					break;
				default:
					break;
			}
			pc = next_pc;
		}
	}

	void scan_branch_labels()
	{
		branch_labels.clear();
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				riscv_ptr offset = (riscv_ptr)elf.offset(shdr.sh_offset);
				scan_branch_labels(offset, offset + shdr.sh_size, offset - shdr.sh_addr);
			}
		}
	}

	void compress(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset, riscv_ptr gp)
	{
		riscv_disasm dec;
		std::deque<riscv_disasm> dec_hist;
		riscv_ptr pc = start, next_pc;
		size_t bytes = 0, saving = 0;
		while (pc < end) {
			dec.pc = pc;
			dec.inst = riscv_get_instruction(pc, &next_pc);
			riscv_decode_instruction(dec, dec.inst);
			if (riscv_get_instruction_length(dec.inst) == 4 && riscv_encode_compress(dec)) {
				dec.inst = riscv_encode(dec);
				riscv_disasm_instruction(dec, dec_hist, pc, next_pc-2, pc_offset, gp,
					std::bind(&riscv_compress_elf::symlookup, this, std::placeholders::_1, std::placeholders::_2),
					std::bind(&riscv_compress_elf::colorize, this, std::placeholders::_1));
				bytes += 2;
				saving += 2;
			} else {
				riscv_disasm_instruction(dec, dec_hist, pc, next_pc, pc_offset, gp,
					std::bind(&riscv_compress_elf::symlookup, this, std::placeholders::_1, std::placeholders::_2),
					std::bind(&riscv_compress_elf::colorize, this, std::placeholders::_1));
				bytes += 4;
			}
			pc = next_pc;
		}
		printf("\nStats: before: %lu after: %lu saving: %lu (%5.2f %%)   // TODO - Relocate and save\n",
			bytes + saving, bytes, saving, (1.0f - (float)(bytes) / (float)(bytes + saving)) * 100.0f);
	}

	void compress()
	{
		const Elf64_Sym *gp_sym = elf.sym_by_name("_gp");
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				riscv_ptr offset = (riscv_ptr)elf.offset(shdr.sh_offset);
				printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf.shdr_name(i), colorize("reset"));
				compress(offset, offset + shdr.sh_size, offset- shdr.sh_addr,
					riscv_ptr(gp_sym ? gp_sym->st_value : 0));
			}
		}
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
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
			scan_branch_labels();
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
