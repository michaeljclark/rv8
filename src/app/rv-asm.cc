//
//  rv-asm.cc
//

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cinttypes>
#include <cstdarg>
#include <cerrno>
#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
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
#include "assembler.h"
#include "elf.h"
#include "elf-file.h"
#include "elf-format.h"
#include "shunting-yard.h"

#define DEBUG 1

using namespace riscv;

struct asm_filename;
struct asm_line;
struct asm_macro;

typedef std::shared_ptr<asm_filename> asm_filename_ptr;
typedef std::shared_ptr<asm_line> asm_line_ptr;
typedef std::shared_ptr<asm_macro> asm_macro_ptr;
typedef std::function<bool(asm_line_ptr&)>asm_directive;

struct asm_filename
{
	std::string filename;

	asm_filename(std::string filename) :
		filename(filename) {}
};

struct asm_line
{
	asm_filename_ptr file;
	int line_num;
	std::vector<std::string> args;

	asm_line(asm_filename_ptr file, int line_num, std::vector<std::string> args) :
		file(file), line_num(line_num), args(args) {}

	std::string ref()
	{
		return file->filename + ":" + std::to_string(line_num);
	}

	std::vector<std::vector<std::string>> split_args(std::string sep)
	{
		std::vector<std::vector<std::string>> vec;
		vec.push_back(std::vector<std::string>());
		for (auto i = args.begin() + 1; i != args.end(); i++) {
			if (*i == sep) {
				vec.push_back(std::vector<std::string>());
			} else {
				vec.back().push_back(*i);
			}
		}
		return vec;
	}
};

struct asm_macro
{
	asm_line_ptr macro_def;
	std::vector<asm_line_ptr> macro_lines;

	asm_macro(asm_line_ptr macro_def) :
		macro_def(macro_def) {}
};

struct riscv_assembler
{
	std::string input_filename;
	std::string output_filename = "a.out";
	bool help_or_error = false;
	bool bail_on_errors = true;

	int ext = rv_isa_imafdc;
	int width = riscv_isa_rv64;

	TokenMap vars;
	assembler as;
	asm_macro_ptr defining_macro;
	std::vector<asm_macro_ptr> macro_stack;

	std::map<std::string,size_t> reg_map;
	std::map<std::string,size_t> csr_map;
	std::map<std::string,size_t> opcode_map;
	std::map<std::string,asm_macro_ptr> macro_map;
	std::map<std::string,asm_directive> map;

	riscv_assembler()
	{
		configure_maps();
		configure_directives();
	}

	void populate_map(std::map<std::string,size_t> &map, const char** arr)
	{
		size_t i = 0;
		while(arr[i] != nullptr) {
			map[arr[i]] = i;
			i++;
		}
	}

	void populate_csr_map(std::map<std::string,size_t> &map, const char** arr)
	{
		for (int i = 0; i < 4096; i++) {
			if (arr[i] == nullptr) continue;
			map[arr[i]] = i;
		}
	}

	void configure_maps()
	{
		populate_map(reg_map, riscv_ireg_name_sym);
		populate_map(reg_map, riscv_freg_name_sym);
		populate_map(opcode_map, riscv_inst_name_sym);
		populate_csr_map(csr_map, riscv_csr_name_sym);
	}

	void configure_directives()
	{
		map[".align"] = std::bind(&riscv_assembler::handle_align, this, std::placeholders::_1);
		map[".p2align"] = std::bind(&riscv_assembler::handle_p2align, this, std::placeholders::_1);
		map[".equ"] = std::bind(&riscv_assembler::handle_equ, this, std::placeholders::_1);
		map[".eqv"] = std::bind(&riscv_assembler::handle_equ, this, std::placeholders::_1);
		map[".file"] = std::bind(&riscv_assembler::handle_file, this, std::placeholders::_1);
		map[".globl"] = std::bind(&riscv_assembler::handle_globl, this, std::placeholders::_1);
		map[".ident"] = std::bind(&riscv_assembler::handle_ident, this, std::placeholders::_1);
		map[".macro"] = std::bind(&riscv_assembler::handle_macro, this, std::placeholders::_1);
		map[".endm"] = std::bind(&riscv_assembler::handle_endm, this, std::placeholders::_1);
		map[".section"] = std::bind(&riscv_assembler::handle_section, this, std::placeholders::_1);
		map[".text"] = std::bind(&riscv_assembler::handle_text, this, std::placeholders::_1);
		map[".data"] = std::bind(&riscv_assembler::handle_data, this, std::placeholders::_1);
		map[".rodata"] = std::bind(&riscv_assembler::handle_rodata, this, std::placeholders::_1);
		map[".bss"] = std::bind(&riscv_assembler::handle_bss, this, std::placeholders::_1);
		map[".size"] = std::bind(&riscv_assembler::handle_size, this, std::placeholders::_1);
		map[".string"] = std::bind(&riscv_assembler::handle_string, this, std::placeholders::_1);
		map[".type"] = std::bind(&riscv_assembler::handle_type, this, std::placeholders::_1);
		map[".half"] = std::bind(&riscv_assembler::handle_half, this, std::placeholders::_1);
		map[".word"] = std::bind(&riscv_assembler::handle_word, this, std::placeholders::_1);
		map[".dword"] = std::bind(&riscv_assembler::handle_dword, this, std::placeholders::_1);
		map[".dtprelword"] = std::bind(&riscv_assembler::handle_dtprelword, this, std::placeholders::_1);
		map[".dtpreldword"] = std::bind(&riscv_assembler::handle_dtpreldword, this, std::placeholders::_1);
		map[".option"] = std::bind(&riscv_assembler::handle_option, this, std::placeholders::_1);
		map["la"] = std::bind(&riscv_assembler::handle_la, this, std::placeholders::_1);
		map["lla"] = std::bind(&riscv_assembler::handle_lla, this, std::placeholders::_1);
		map["li"] = std::bind(&riscv_assembler::handle_li, this, std::placeholders::_1);
		map["call"] = std::bind(&riscv_assembler::handle_call, this, std::placeholders::_1);
		map["tail"] = std::bind(&riscv_assembler::handle_tail, this, std::placeholders::_1);
	}

	static rv_isa decode_isa_ext(std::string isa_ext)
	{
		if (strncasecmp(isa_ext.c_str(), "IMA", isa_ext.size()) == 0) return rv_isa_ima;
		else if (strncasecmp(isa_ext.c_str(), "IMAC", isa_ext.size()) == 0) return rv_isa_imac;
		else if (strncasecmp(isa_ext.c_str(), "IMAFD", isa_ext.size()) == 0) return rv_isa_imafd;
		else if (strncasecmp(isa_ext.c_str(), "IMAFDC", isa_ext.size()) == 0) return rv_isa_imafdc;
		else return rv_isa_none;
	}

	template <typename T>
	std::string join(std::vector<T> list, std::string sep)
	{
		std::stringstream ss;
		for (auto i = list.begin(); i != list.end(); i++) {
			ss << (i != list.begin() ? sep : "") << *i;
		}
		return ss.str();
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ "-i", "--isa", cmdline_arg_type_string,
				"ISA Extensions (IMA, IMAC, IMAFD, IMAFDC)",
				[&](std::string s) { return (ext = decode_isa_ext(s)); } },
			{ "-m32", "--riscv32", cmdline_arg_type_string,
				"Assembler for RISC-V 32",
				[&](std::string s) { return (width = riscv_isa_rv32); } },
			{ "-m64", "--riscv64", cmdline_arg_type_string,
				"Assembler for RISC-V 64 (default)",
				[&](std::string s) { return (width = riscv_isa_rv32); } },
			{ "-o", "--output", cmdline_arg_type_string,
				"Output ELF file (default a.out)",
				[&](std::string s) { output_filename = s; return true; } },
			{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
		};

		auto result = cmdline_option::process_options(options, argc, argv);
		if (!result.second) {
			help_or_error = true;
		} else if (result.first.size() != 1 && !help_or_error) {
			printf("%s: wrong number of arguments\n", argv[0]);
			help_or_error = true;
		}

		if (help_or_error)
		{
			printf("usage: %s [<options>] <asm_file>\n", argv[0]);
			cmdline_option::print_options(options);
			exit(9);
		}

		input_filename = result.first[0];
	}

	std::vector<std::string> parse_line(std::string line)
	{
		// simple parsing routine that handles tokens separated by whitespace
		// separator characters, double quoted tokens containing and # comments

		std::string specials = "%:,+-*/()";
		std::vector<char> token;
		std::vector<std::string> args;
		enum {
			whitespace,
			quoted_token,
			unquoted_token,
			comment
		} state = whitespace;

		size_t i = 0;
		while (i < line.size()) {
			char c = line[i];
			switch (state) {
				case whitespace: {
					if (::isspace(c)) {
						i++;
					} else if (c == '#') {
						state = comment;
					} else if (c == '"') {
						state = quoted_token;
						i++;
					} else {
						state = unquoted_token;
					}
					break;
				}
				case quoted_token: {
					if (c == '"') {
						args.push_back(std::string(token.begin(), token.end()));
						token.resize(0);
						state = whitespace;
					} else {
						token.push_back(c);
					}
					i++;
					break;
				}
				case unquoted_token: {
					auto s = specials.find(c);
					if (s != std::string::npos) {
						if (token.size() > 0) {
							args.push_back(std::string(token.begin(), token.end()));
						}
						args.push_back(specials.substr(s, 1));
						token.resize(0);
					} else if (::isspace(c)) {
						if (token.size() > 0) {
							args.push_back(std::string(token.begin(), token.end()));
						}
						token.resize(0);
						state = whitespace;
					} else {
						token.push_back(c);
					}
					i++;
					break;
				}
				case comment: {
					i++;
					break;
				}
			}
		}
		if (token.size() > 0) {
			args.push_back(std::string(token.begin(), token.end()));
		}
		return args;
	}

	void read_source(std::vector<asm_line_ptr> &data, std::string filename)
	{
		asm_filename_ptr file = std::make_shared<asm_filename>(filename);
		std::ifstream in(filename.c_str());
		std::string line;
		if (!in.is_open()) {
			panic("error opening %s\n", filename.c_str());
		}
		int line_num = 0;
		while (in.good())
		{
			line_num++;
			std::getline(in, line);
			size_t hoffset = line.find("#");
			if (hoffset != std::string::npos) {
				line = ltrim(rtrim(line.substr(0, hoffset)));
			}
			std::vector<std::string> args = parse_line(line);
			if (args.size() == 0) continue;
			if (args.size() == 2 && args[0] == ".include") {
				/* NOTE: we don't do any loop detection */
				read_source(data, args[1]);
			} else {
				data.push_back(std::make_shared<asm_line>(file, line_num, args));
			}
		}
		in.close();
	}

	/* handlers */

	bool handle_align(asm_line_ptr &line)
	{
		if (line->args.size() < 2) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		/* TODO - handle expression */
		s64 i;
		if (!parse_integral(line->args[1], i)) {
			printf("%s invalid integral\n", line->ref().c_str());
			return false;
		}
		as.align(i);
		return true;
	}

	bool handle_p2align(asm_line_ptr &line)
	{
		if (line->args.size() < 2) {
			printf("%s missing parameter\n", line->ref().c_str());
			return false;
		}
		/* TODO - handle expression */
		s64 i;
		if (!parse_integral(line->args[1], i)) {
			printf("%s invalid number\n", line->ref().c_str());
			return false;
		}
		as.p2align(i);
		return true;
	}

	packToken eval(std::vector<std::string> tokens)
	{
		if (tokens.size() == 1) {
			s64 val;
			if (parse_integral(tokens[0], val)) {
				return packToken(int64_t(val));
			}
		}
		std::string expr = join(tokens, " ");
		calculator calc(expr.c_str());
		auto result = calc.eval(vars);
		return result;
	}

	bool handle_equ(asm_line_ptr &line)
	{
		auto argv = line->split_args(",");
		if (argv.size() != 2 || argv[0].size() != 1 || argv[1].size() < 1) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		std::string var_name = argv[0][0];
		auto result = eval(argv[1]);
		vars[var_name] = result;
		return true;
	}

	bool handle_file(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_globl(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_ident(asm_line_ptr &line)
	{
		/* ignore */
		return true;
	}

	bool handle_macro(asm_line_ptr &line)
	{
		if (line->args.size() < 2) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		if (defining_macro) {
			printf("%s already defining macro\n", line->ref().c_str());
			return false;
		}
		defining_macro = std::make_shared<asm_macro>(line);
		macro_map[line->args[1]] = defining_macro;
		return true;
	}

	bool handle_endm(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		defining_macro = asm_macro_ptr();
		return true;
	}

	bool handle_section(asm_line_ptr &line)
	{
		if (line->args.size() != 2) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		if (line->args[1].size() ==0 || line->args[1][0] != '.') {
			printf("%s section must begin with '.'\n", line->ref().c_str());
			return false;
		}
		as.get_section(line->args[1]);
		return true;
	}

	bool handle_text(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		as.get_section(".text");
		return true;
	}

	bool handle_data(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		as.get_section(".data");
		return true;
	}

	bool handle_rodata(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		as.get_section(".rodata");
		return true;
	}

	bool handle_bss(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		as.get_section(".bss");
		return true;
	}

	bool handle_size(asm_line_ptr &line)
	{
		/* ignore */
		return true;
	}

	bool handle_string(asm_line_ptr &line)
	{
		if (line->args.size() != 2) {
			printf("%s invalid parameters\n", line->ref().c_str());
			return false;
		}
		std::string str =  line->args[1];
		for (size_t i = 0; i < str.size(); i++) {
			as.append(u8(str[i]));
		}
		as.append(u8(0));
		return true;
	}

	bool handle_type(asm_line_ptr &line)
	{
		/* ignore */
		return true;
	}

	bool handle_half(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_word(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_dword(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_dtprelword(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_dtpreldword(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_option(asm_line_ptr &line)
	{
		/* TODO - rvc,norvc,push,pop */
		return true;
	}

	bool handle_la(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_lla(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_li(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_call(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_tail(asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	asm_line_ptr macro_substitute(asm_line_ptr &line)
	{
		/* TODO - use macro_stack.back()->macro_def to substitute args */
		return line;
	}

	bool handle_opcode(size_t opcode, asm_line_ptr &line)
	{
		/* TODO */
		return true;
	}

	bool handle_label(std::string label)
	{
		as.add_label(label);
		return true;
	}

	void write_elf(std::string filename)
	{
		/* TODO */
	}

	bool process_line(asm_line_ptr line)
	{
		#if DEBUG
		printf("%-30s %s\n",
			format_string("%s:%05d", line->file->filename.c_str(), line->line_num).c_str(),
			join(line->args, " ").c_str());
		#endif

		/* check if we are defining a macro */
		if (defining_macro && !(line->args.size() > 0 && line->args[0] == ".endm")) {
			defining_macro->macro_lines.push_back(line);
			return true;
		}

		/* check for label */
		if (line->args.size() >= 2 && line->args[1] == ":") {
			handle_label(line->args[0]);
			line->args.erase(line->args.begin(), line->args.begin() + 2);
		}
		if (line->args.size() == 0) {
			return true;
		}

		/* check for internal directives */
		auto di = map.find(line->args[0]);
		if (di != map.end()) {
			if (!di->second(line)) {
				printf("%s invalid directive: %s\n",
					line->ref().c_str(), join(line->args, " ").c_str());
			}
			return true;
		}

		/* check for opcode */
		auto oi = opcode_map.find(line->args[0]);
		if (oi != opcode_map.end()) {
			if (!handle_opcode(oi->second, line)) {
				printf("%s invalid statement: %s\n",
					line->ref().c_str(), join(line->args, " ").c_str());
			}
			return true;
		}

		/* check for macro */
		auto mi = macro_map.find(line->args[0]);
		if (mi != macro_map.end()) {
			macro_stack.push_back(mi->second);
			for (auto macro_line : mi->second->macro_lines) {
				process_line(macro_substitute(macro_line));
			}
			macro_stack.pop_back();
			return true;
		}

		printf("%s unknown statement: %s\n",
			line->ref().c_str(), join(line->args, " ").c_str());

		return false;
	}

	void assemble()
	{
		/*
		 * TODO
		 *
		 * as allows mixed case
		 * as allows trailing semicolons
		 */

		std::vector<asm_line_ptr> data;
		read_source(data, input_filename);
		for (auto &line : data)
		{
			if (!process_line(line)) {
				if (bail_on_errors) {
					exit(9);
				}
			}
		}
		write_elf(output_filename);
	}
};

/* program main */

int main(int argc, const char* argv[])
{
	printf("\n");
	printf("rv-asm-0.0.0-prealpha-0\n");
	printf("\n");
	riscv_assembler as;
	as.parse_commandline(argc, argv);
	as.assemble();
	return 0;
}
