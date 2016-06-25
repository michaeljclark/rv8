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

struct riscv_parse_elf
{
	elf_file elf;
	std::string filename;
	std::map<uintptr_t,std::string> branch_labels;

	bool enable_color = false;
	bool elf_header = false;
	bool section_headers = false;
	bool program_headers = false;
	bool symbol_table = false;
	bool disassebly = false;
	bool help_or_error = false;

	const char* colorize(const char *type)
	{
		if (!enable_color || !isatty(fileno(stdout))) {
			return "";
		} else if (strcmp(type, "header") == 0) {
			return _COLOR_BEGIN _COLOR_BOLD _COLOR_END;
		} else if (strcmp(type, "title") == 0) {
			return _COLOR_BEGIN _COLOR_BOLD _COLOR_SEP _COLOR_FG_CYAN _COLOR_END;
		} else if (strcmp(type, "legend") == 0) {
			return _COLOR_BEGIN _COLOR_BOLD _COLOR_SEP _COLOR_FG_MAGENTA _COLOR_END;
		} else if (strcmp(type, "opcode") == 0) {
			return _COLOR_BEGIN _COLOR_BOLD _COLOR_SEP _COLOR_FG_CYAN _COLOR_END;
		} else if (strcmp(type, "address") == 0) {
			return _COLOR_BEGIN _COLOR_FG_YELLOW _COLOR_END;
		} else if (strcmp(type, "label") == 0) {
			return _COLOR_BEGIN _COLOR_FG_GREEN _COLOR_END;
		} else if (strcmp(type, "reset") == 0) {
			return _COLOR_RESET;
		}
		return "";
	}

	const char* symlookup(uintptr_t addr, bool nearest)
	{
		static char symbol_tmpname[256];
		auto sym = elf.sym_by_addr((Elf64_Addr)addr);
		auto bli = branch_labels.find(addr);
		if (sym && bli != branch_labels.end()) {
				snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"%s:<%s>", bli->second.c_str(), elf.sym_name(sym));
			return symbol_tmpname;
		}
		if (sym) {
			snprintf(symbol_tmpname, sizeof(symbol_tmpname),
					"<%s>", elf.sym_name(sym));
			return symbol_tmpname;
		}
		if (bli != branch_labels.end()) return bli->second.c_str();
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

	void scan_branch_labels(uintptr_t start, uintptr_t end, uintptr_t pc_offset)
	{
		ssize_t branch_num = 1;
		char branch_label[32];

		riscv_disasm dec;
		uintptr_t pc = start, next_pc;
		uint64_t addr = 0;
		while (pc < end) {
			dec.pc = pc;
			dec.insn = riscv_get_insn(pc, &next_pc);
			riscv_decode_rv64(dec, dec.insn);
			switch (dec.codec) {
				case riscv_codec_sb:
				case riscv_codec_uj:
					addr = pc - pc_offset + dec.imm;
					snprintf(branch_label, sizeof(branch_label), "LOC_%06lu", branch_num++);
					branch_labels[(uintptr_t)addr] = branch_label;
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
				uintptr_t offset = (uintptr_t)elf.offset(shdr.sh_offset);
				scan_branch_labels(offset, offset + shdr.sh_size, offset - shdr.sh_addr);
			}
		}
	}

	void print_disassembly(uintptr_t start, uintptr_t end, uintptr_t pc_offset, uintptr_t gp)
	{
		riscv_disasm dec;
		std::deque<riscv_disasm> dec_hist;
		uintptr_t pc = start, next_pc;
		while (pc < end) {
			dec.pc = pc;
			dec.insn = riscv_get_insn(pc, &next_pc);
			riscv_decode_rv64(dec, dec.insn);
			riscv_disasm_insn(dec, dec_hist, pc, next_pc, pc_offset, gp,
				std::bind(&riscv_parse_elf::symlookup, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&riscv_parse_elf::colorize, this, std::placeholders::_1));
			pc = next_pc;
		}
	}

	void print_disassembly()
	{
		const Elf64_Sym *gp_sym = elf.sym_by_name("_gp");
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				uintptr_t offset = (uintptr_t)elf.offset(shdr.sh_offset);
				printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf.shdr_name(i), colorize("reset"));
				print_disassembly(offset, offset + shdr.sh_size, offset- shdr.sh_addr,
					uintptr_t(gp_sym ? gp_sym->st_value : 0));
				printf("\n");
			}
		}
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-c", "--color", cmdline_arg_type_none,
				"Enable Color",
				[&](std::string s) { return (enable_color = true); } },
			{ "-e", "--print-elf-header", cmdline_arg_type_none,
				"Print ELF header",
				[&](std::string s) { return (elf_header = true); } },
			{ "-s", "--print-section-headers", cmdline_arg_type_none,
				"Print Section headers",
				[&](std::string s) { return (section_headers = true); } },
			{ "-p", "--print-program-headers", cmdline_arg_type_none,
				"Print Program headers",
				[&](std::string s) { return (program_headers = true); } },
			{ "-t", "--print-symbol-table", cmdline_arg_type_none,
				"Print Symbol Table",
				[&](std::string s) { return (symbol_table = true); } },
			{ "-d", "--print-disassembly", cmdline_arg_type_none,
				"Print Disassembly",
				[&](std::string s) { return (disassebly = true); } },
			{ "-a", "--print-all", cmdline_arg_type_none,
				"Print All",
				[&](std::string s) { return (elf_header = section_headers = program_headers = symbol_table = disassebly = true); } },
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

		if ((help_or_error |= !elf_header && !section_headers &&
			!program_headers && !symbol_table && !disassebly))
		{
			printf("usage: %s [<options>] <elf_file>\n", argv[0]);
			cmdline_option::print_options(options);
			exit(9);
		}

		filename = result.first[0];
	}

	void print_heading(std::string heading)
	{
		printf("\n%s███ %s ", colorize("header"), heading.c_str());
		for (size_t i = 0; i < 118 - heading.length(); i++) printf("█");
		printf("%s\n\n", colorize("reset"));
	}

	void run()
	{
		elf.load(filename);
		if (elf_header) {
			print_heading("ELF Header");
			elf_print_header_info(elf, std::bind(&riscv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (section_headers) {
			print_heading("Section Headers");
			elf_print_section_headers(elf, std::bind(&riscv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (program_headers) {
			print_heading("Program Headers");
			elf_print_program_headers(elf, std::bind(&riscv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (symbol_table) {
			print_heading("Symbol Table");
			elf_print_symbol_table(elf, std::bind(&riscv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (disassebly && elf.ehdr.e_machine == EM_RISCV) {
			print_heading("Disassembly");
			scan_branch_labels();
			print_disassembly();
		}
	}
};

int main(int argc, const char *argv[])
{
	riscv_parse_elf elf_parser;
	elf_parser.parse_commandline(argc, argv);
	elf_parser.run();
	return 0;
}
