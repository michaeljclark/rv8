//
//  rv-dump.cc
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
#include "util.h"
#include "cmdline.h"
#include "color.h"
#include "codec.h"
#include "strings.h"
#include "disasm.h"
#include "elf.h"
#include "elf-file.h"
#include "elf-format.h"

using namespace riscv;

struct rv_parse_elf
{
	elf_file elf;
	std::string filename;
	std::map<addr_t,uint32_t> continuations;
	ssize_t continuation_num = 1;

	bool enable_color = false;
	bool elf_header = false;
	bool elf_header_ext = false;
	bool section_headers = false;
	bool program_headers = false;
	bool symbol_table = false;
	bool disassebly = false;
	bool relocations = false;
	bool decode_pseudo = false;
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

	void scan_continuations(addr_t start, addr_t end, addr_t pc_bias)
	{
		disasm dec;
		addr_t pc = start;
		addr_t pc_offset;
		addr_t addr = 0;
		while (pc < end) {
			dec.pc = pc;
			dec.inst = inst_fetch(pc, pc_offset);
			decode_inst_rv64(dec, dec.inst);
			switch (dec.op) {
				case rv_op_jal:
				case rv_op_jalr:
					if (pc + pc_offset < end) {
						addr = pc - pc_bias + pc_offset;
						if (continuations.find(addr) == continuations.end()) {
							continuations.insert(std::pair<addr_t,uint32_t>(addr, continuation_num++));
						}
					}
					break;
				default:
					break;
			}
			switch (dec.codec) {
				case rv_codec_sb:
					addr = pc - pc_bias + dec.imm;
					if (continuations.find(addr) == continuations.end()) {
						continuations.insert(std::pair<addr_t,uint32_t>(addr, continuation_num++));
					}
					break;
				default:
					break;
			}
			pc += pc_offset;
		}
	}

	void print_disassembly(addr_t start, addr_t end, addr_t pc_bias, addr_t gp)
	{
		disasm dec;
		std::deque<disasm> dec_hist;
		addr_t pc = start;
		addr_t pc_offset;
		while (pc < end) {
			dec.pc = pc;
			dec.inst = inst_fetch(pc, pc_offset);
			decode_inst_rv64(dec, dec.inst);
			if (decode_pseudo) decode_pseudo_inst(dec);
			disasm_inst_print(dec, dec_hist, pc, pc_bias, gp,
				std::bind(&rv_parse_elf::symlookup, this, std::placeholders::_1, std::placeholders::_2),
				std::bind(&rv_parse_elf::colorize, this, std::placeholders::_1));
			pc += pc_offset;
		}
	}

	void print_disassembly()
	{
		const Elf64_Sym *gp_sym = elf.sym_by_name("_gp");
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				addr_t offset = (addr_t)elf.sections[i].buf.data();
				printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf.shdr_name(i), colorize("reset"));
				scan_continuations(offset, offset + shdr.sh_size, offset - shdr.sh_addr);
				print_disassembly(offset, offset + shdr.sh_size, offset- shdr.sh_addr,
					addr_t(gp_sym ? gp_sym->st_value : 0));
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
			{ "-x", "--print-elf-header-ext", cmdline_arg_type_none,
				"Print ELF header (extended)",
				[&](std::string s) { return (elf_header = elf_header_ext = true); } },
			{ "-s", "--print-section-headers", cmdline_arg_type_none,
				"Print Section headers",
				[&](std::string s) { return (section_headers = true); } },
			{ "-p", "--print-program-headers", cmdline_arg_type_none,
				"Print Program headers",
				[&](std::string s) { return (program_headers = true); } },
			{ "-t", "--print-symbol-table", cmdline_arg_type_none,
				"Print Symbol Table",
				[&](std::string s) { return (symbol_table = true); } },
			{ "-r", "--print-relocations", cmdline_arg_type_none,
				"Print Relocations",
				[&](std::string s) { return (relocations = true); } },
			{ "-d", "--print-disassembly", cmdline_arg_type_none,
				"Print Disassembly",
				[&](std::string s) { return (disassebly = true); } },
			{ "-P", "--pseudo", cmdline_arg_type_none,
				"Decode Pseudoinstructions",
				[&](std::string s) { return (decode_pseudo = true); } },
			{ "-h", "--print-headers", cmdline_arg_type_none,
				"Print All Headers",
				[&](std::string s) { return (elf_header = section_headers = program_headers = true); } },
			{ "-a", "--print-all", cmdline_arg_type_none,
				"Print All Headers and Disassembly",
				[&](std::string s) { return (elf_header = section_headers = program_headers = symbol_table = disassebly = true); } },
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() != 1 && !help_or_error) {
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
		printf("\n%s---[ %s ]", colorize("header"), heading.c_str());
		for (size_t i = 0; i < 116 - heading.length(); i++) printf("-");
		printf("%s\n\n", colorize("reset"));
	}

	void run()
	{
		elf.load(filename);
		if (elf_header) {
			print_heading("ELF Header");
			elf_print_header_info(elf, std::bind(&rv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (elf_header_ext) {
			elf_print_header_ext_info(elf, std::bind(&rv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (section_headers) {
			print_heading("Section Headers");
			elf_print_section_headers(elf, std::bind(&rv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (program_headers) {
			print_heading("Program Headers");
			elf_print_program_headers(elf, std::bind(&rv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (symbol_table) {
			print_heading("Symbol Table");
			elf_print_symbol_table(elf, std::bind(&rv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (relocations && elf.ehdr.e_machine == EM_RISCV) {
			print_heading("Relocations");
			elf_print_relocations(elf, std::bind(&rv_parse_elf::colorize, this, std::placeholders::_1));
		}
		if (disassebly && elf.ehdr.e_machine == EM_RISCV) {
			print_heading("Disassembly");
			print_disassembly();
		}
		printf("\n");
	}
};

int rv_dump_main(int argc, const char *argv[])
{
	rv_parse_elf elf_parser;
	elf_parser.parse_commandline(argc, argv);
	elf_parser.run();
	return 0;
}
