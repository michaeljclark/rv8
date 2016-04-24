#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
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
#include "riscv-regs.h"
#include "riscv-processor.h"
#include "riscv-format.h"
#include "riscv-opcodes.h"
#include "riscv-util.h"
#include "riscv-cmdline.h"
#include "riscv-color.h"
#include "riscv-imm.h"
#include "riscv-decode.h"
#include "riscv-csr.h"
#include "riscv-compression.h"
#include "riscv-disasm.h"
#include "riscv-elf.h"
#include "riscv-elf-file.h"
#include "riscv-elf-format.h"

struct riscv_parse_elf
{
	elf_file elf;
	std::string filename;
	std::map<riscv_ptr,std::string> branch_labels;

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
			return S_COLOR S_BOLD F_WHITE B_BLACK;
		} else if (strcmp(type, "legend") == 0) {
			return S_COLOR S_BOLD F_MAGENTA B_BLACK;
		} else if (strcmp(type, "title") == 0) {
			return S_COLOR S_BOLD F_WHITE B_BLACK;
		} else if (strcmp(type, "opcode") == 0) {
			return S_COLOR S_BOLD F_CYAN B_BLACK;
		} else if (strcmp(type, "location") == 0) {
			return S_COLOR F_GREEN B_BLACK;
		} else if (strcmp(type, "address") == 0) {
			return S_COLOR F_YELLOW B_BLACK;
		} else if (strcmp(type, "symbol") == 0) {
			return S_COLOR F_WHITE B_BLACK;
		} else if (strcmp(type, "reset") == 0) {
			return T_RESET;
		}
		return "";
	}

	const char* symloopup(riscv_ptr addr)
	{
		auto sym = elf_sym_by_addr(elf, (Elf64_Addr)addr);
		if (sym) return elf_sym_name(elf, sym);
		auto branch_label_i = branch_labels.find(addr);
		if (branch_label_i != branch_labels.end()) return branch_label_i->second.c_str();
		return nullptr;
	}

	void scan_branch_labels(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset)
	{
		ssize_t branch_label_num = 1;
		char branch_label_buf[32];

		riscv_decode dec;
		riscv_proc_state proc = { 0 };
		proc.p_type = riscv_proc_type_rv64i;
		proc.pc = start;
		uint64_t addr = 0;
		while (proc.pc < end) {
			riscv_ptr next_pc = riscv_decode_instruction(dec, proc.pc);
			const riscv_inst_comp_metadata *comp = riscv_lookup_comp_metadata((riscv_op)dec.op);
			if (comp) {
				dec.op = comp->op;
				dec.type = comp->type;
			}
			switch (dec.type) {
				case riscv_inst_type_sb:
				case riscv_inst_type_uj:
				{
					addr = proc.pc - pc_offset + dec.imm;
					snprintf(branch_label_buf, sizeof(branch_label_buf), "LOC_%06lu", branch_label_num++);
					branch_labels[(riscv_ptr)addr] = branch_label_buf;
					break;
				}
				default:
					break;
			}
			proc.pc = next_pc;
		}
	}

	void scan_branch_labels()
	{
		branch_labels.clear();
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				scan_branch_labels(elf.buf.data() + shdr.sh_offset,
					elf.buf.data() + shdr.sh_offset + shdr.sh_size,
					elf.buf.data() + shdr.sh_offset - shdr.sh_addr);
			}
		}
	}

	void print_disassembly(riscv_ptr start, riscv_ptr end, riscv_ptr pc_offset, riscv_ptr gp)
	{
		riscv_decode dec, last_dec;
		riscv_ptr pc = start;
		while (pc < end) {
			riscv_ptr next_pc = riscv_decode_instruction(dec, pc);
			riscv_disasm_instruction(dec, last_dec, pc, next_pc, pc_offset, gp,
				std::bind(&riscv_parse_elf::symloopup, this, std::placeholders::_1),
				std::bind(&riscv_parse_elf::colorize, this, std::placeholders::_1));
			pc = next_pc;
			last_dec = dec;
		}
	}

	void print_disassembly()
	{
		const Elf64_Sym *gp_sym = elf_sym_by_name(elf, "_gp");
		for (size_t i = 0; i < elf.shdrs.size(); i++) {
			Elf64_Shdr &shdr = elf.shdrs[i];
			if (shdr.sh_flags & SHF_EXECINSTR) {
				printf("%sSection[%2lu] %-111s%s\n", colorize("title"), i, elf_shdr_name(elf, i), colorize("reset"));
				print_disassembly(elf.buf.data() + shdr.sh_offset,
					elf.buf.data() + shdr.sh_offset + shdr.sh_size,
					elf.buf.data() + shdr.sh_offset - shdr.sh_addr,
					riscv_ptr(gp_sym ? gp_sym->st_value : 0));
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
		printf("\n%s---[ %s ]", colorize("header"), heading.c_str());
		for (size_t i = 0; i < 116 - heading.length(); i++) printf("-");
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
		printf("\n");
	}
};

int main(int argc, const char *argv[])
{
	riscv_parse_elf elf_parser;
	elf_parser.parse_commandline(argc, argv);
	elf_parser.run();
	return 0;
}
