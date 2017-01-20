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
#include "jit.h"
#include "elf.h"
#include "elf-file.h"
#include "elf-format.h"
#include "shunting-yard.h"
#include "fmt.h"

#define DEBUG 1

using namespace riscv;
using namespace std::placeholders;

struct asm_filename;
struct asm_line;
struct asm_macro;
struct asm_macro_expand;

typedef std::shared_ptr<asm_filename> asm_filename_ptr;
typedef std::shared_ptr<asm_line> asm_line_ptr;
typedef std::shared_ptr<asm_macro> asm_macro_ptr;
typedef std::shared_ptr<asm_macro_expand> asm_macro_expand_ptr;
typedef std::function<bool(asm_line_ptr&)>asm_directive;
typedef std::function<int(decode&)> reloc_directive;

const char* kInvalidOperands =             "%s *** invalid operands: %s\n";
const char* kMissingOperands =             "%s *** missing operand: %s\n";
const char* kInvalidNumber =               "%s *** invalid number: %s\n";
const char* kAlreadyDefiningMacro =        "%s *** already defining macro: %s\n";
const char* kSectionMustBeginWithDot =     "%s *** section must begin with '.': %s\n";
const char* kValueOutOfRange =             "%s *** value out of range: %s\n";
const char* kInvalidRegister =             "%s *** invalid register: %s\n";
const char* kMissingRegisterOperand =      "%s *** missing register operand: %s\n";
const char* kMissingImmediateOperand =     "%s *** missing immediate operand: %s\n";
const char* kInvalidImmediateOperand =     "%s *** invalid immediate operand: %s\n";
const char* kMissingCSROperand =           "%s *** missing csr operand: %s\n";
const char* kInvalidCSROperand =           "%s *** invalid csr operand: %s\n";
const char* kUnknownCSROperand =           "%s *** unknown csr operand: %s\n";
const char* kInvalidStatement =            "%s *** invalid statement: %s\n";
const char* kInvalidMacroOperands =        "%s *** invalid macro operands: %s\n";
const char* kUnknownRelocation =           "%s *** unknown relocation: %s\n";
const char* kDuplicateSymbol =             "%s *** duplicate symbol %s\n";
const char* kUnimplementedOperation =      "%s *** unimplemented operation %s\n";

template <typename T>
std::string join(std::vector<T> list, std::string sep)
{
	std::stringstream ss;
	for (auto i = list.begin(); i != list.end(); i++) {
		ss << (i != list.begin() ? sep : "") << *i;
	}
	return ss.str();
}

static std::vector<std::string> parse_line(std::string line);
static void read_source(std::vector<asm_line_ptr> &data, std::string filename);

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
	bool has_error;
	std::vector<std::string> args;

	asm_line(asm_filename_ptr file, int line_num, std::vector<std::string> args) :
		file(file), line_num(line_num), has_error(false), args(args) {}

	std::string ref()
	{
		return file->filename + ":" + std::to_string(line_num);
	}

	std::string str()
	{
		return join(args, " ");
	}

	bool error(const char *fmt)
	{
		has_error = true;
		printf(fmt, ref().c_str(), str().c_str());
		return false;
	}

	std::deque<std::vector<std::string>> split_args(std::string sep)
	{
		std::deque<std::vector<std::string>> vec;
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

struct asm_macro_expand
{
	std::map<std::string,std::string> map;

	asm_macro_expand(std::deque<std::vector<std::string>> macro_args,
		std::deque<std::vector<std::string>> param_args)
	{
		/* create substitution map */
		for (auto mi = macro_args.begin(), pi = param_args.begin();
			mi != macro_args.end() && pi != param_args.end(); mi++, pi++)
		{
			map[std::string("\\") + join(*mi, " ")] = join(*pi, " ");
		}
	}

	asm_line_ptr substitute(asm_line_ptr &line)
	{
		/* substitute macro parameters */
		std::vector<std::string> args;
		for (auto arg : line->args) {
			for (auto &ent : map) {
				arg = replace(arg, ent.first, ent.second);
			}
			args.push_back(arg);
		}
		return std::make_shared<asm_line>(line->file, line->line_num,
			parse_line(join(args, " ")));
	}
};

struct asm_macro
{
	std::deque<std::vector<std::string>> macro_args;
	std::vector<asm_line_ptr> macro_lines;

	asm_macro(asm_line_ptr macro_def)
	{
		/* save the macro args removing .macro prefix */
		macro_args = macro_def->split_args(",");
		macro_args[0].erase(macro_args[0].begin());
	}

	asm_macro_expand_ptr get_expander(asm_line_ptr &param_line)
	{
		/* return expander if macro has the same number of parameters */
		auto param_args = param_line->split_args(",");
		return macro_args.size() == param_args.size() ?
			std::make_shared<asm_macro_expand>(macro_args, param_args) :
			asm_macro_expand_ptr();
	}
};

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

struct rv_assembler
{
	std::string input_filename;
	std::string output_filename = "a.out";
	bool help_or_error = false;
	bool bail_on_errors = false;
	bool debug = false;

	int ext = rv_set_imafdc;
	int width = rv_isa_rv64;

	assembler as;
	TokenMap vars;
	asm_macro_ptr defining_macro;
	std::map<std::string,size_t> ireg_map;
	std::map<std::string,size_t> freg_map;
	std::map<std::string,size_t> csr_map;
	std::map<std::string,size_t> opcode_map;
	std::map<std::string,asm_macro_ptr> macro_map;
	std::map<std::string,asm_directive> directive_map;
	std::map<std::string,reloc_directive> reloc_map;

	rv_assembler()
	{
		configure_maps();
		configure_directives();
		configure_relocs();
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
		for (size_t i = 0; i < 32; i++) {
			std::string ireg = "x" + std::to_string(i);
			std::string freg = "f" + std::to_string(i);
			ireg_map[ireg] = i;
			freg_map[freg] = i;
		}
		populate_map(ireg_map, rv_ireg_name_sym);
		populate_map(freg_map, rv_freg_name_sym);
		populate_map(opcode_map, rv_inst_name_sym);
		populate_csr_map(csr_map, rv_csr_name_sym);
	}

	void configure_directives()
	{
		directive_map[".align"] = std::bind(&rv_assembler::handle_p2align, this, _1);
		directive_map[".p2align"] = std::bind(&rv_assembler::handle_p2align, this, _1);
		directive_map[".balign"] = std::bind(&rv_assembler::handle_balign, this, _1);
		directive_map[".equ"] = std::bind(&rv_assembler::handle_equ, this, _1);
		directive_map[".eqv"] = std::bind(&rv_assembler::handle_equ, this, _1);
		directive_map[".file"] = std::bind(&rv_assembler::handle_file, this, _1);
		directive_map[".globl"] = std::bind(&rv_assembler::handle_globl, this, _1);
		directive_map[".weak"] = std::bind(&rv_assembler::handle_weak, this, _1);
		directive_map[".ident"] = std::bind(&rv_assembler::handle_ident, this, _1);
		directive_map[".macro"] = std::bind(&rv_assembler::handle_macro, this, _1);
		directive_map[".endm"] = std::bind(&rv_assembler::handle_endm, this, _1);
		directive_map[".section"] = std::bind(&rv_assembler::handle_section, this, _1);
		directive_map[".text"] = std::bind(&rv_assembler::handle_text, this, _1);
		directive_map[".data"] = std::bind(&rv_assembler::handle_data, this, _1);
		directive_map[".rodata"] = std::bind(&rv_assembler::handle_rodata, this, _1);
		directive_map[".bss"] = std::bind(&rv_assembler::handle_bss, this, _1);
		directive_map[".size"] = std::bind(&rv_assembler::handle_size, this, _1);
		directive_map[".string"] = std::bind(&rv_assembler::handle_string, this, _1);
		directive_map[".asciz"] = std::bind(&rv_assembler::handle_string, this, _1);
		directive_map[".type"] = std::bind(&rv_assembler::handle_type, this, _1);
		directive_map[".byte"] = std::bind(&rv_assembler::handle_byte, this, _1);
		directive_map[".half"] = std::bind(&rv_assembler::handle_half, this, _1);
		directive_map[".word"] = std::bind(&rv_assembler::handle_word, this, _1);
		directive_map[".dword"] = std::bind(&rv_assembler::handle_dword, this, _1);
		directive_map[".dtprelword"] = std::bind(&rv_assembler::handle_dtprelword, this, _1);
		directive_map[".dtpreldword"] = std::bind(&rv_assembler::handle_dtpreldword, this, _1);
		directive_map[".option"] = std::bind(&rv_assembler::handle_option, this, _1);
		directive_map[".zero"] = std::bind(&rv_assembler::handle_zero, this, _1);
		directive_map[".cfi_startproc"] = std::bind(&rv_assembler::handle_none, this, _1);
		directive_map[".cfi_endproc"] = std::bind(&rv_assembler::handle_none, this, _1);
		directive_map[".cfi_offset"] = std::bind(&rv_assembler::handle_none, this, _1);
		directive_map[".cfi_def_cfa"] = std::bind(&rv_assembler::handle_none, this, _1);
		directive_map[".cfi_def_cfa_offset"] = std::bind(&rv_assembler::handle_none, this, _1);
		directive_map["la"] = std::bind(&rv_assembler::handle_la, this, _1);
		directive_map["lla"] = std::bind(&rv_assembler::handle_lla, this, _1);
		directive_map["li"] = std::bind(&rv_assembler::handle_li, this, _1);
		directive_map["call"] = std::bind(&rv_assembler::handle_call, this, _1);
		directive_map["tail"] = std::bind(&rv_assembler::handle_tail, this, _1);
	}

	void configure_relocs()
	{
		/*
		 * %hi(symbol)               Absolute imm20
		 * %lo(symbol)               Absolute imm12
		 * %pcrel_hi(symbol)         PC-relative imm20
		 * %pcrel_lo(label)          PC-relative imm12
		 * %tls_ie_pcrel_hi(symbol)  TLS IE GOT "Initial Exec"
		 * %tls_gd_pcrel_hi(symbol)  TLS GD GOT "Global Dynamic"
		 * %tprel_hi(symbol)         TLS LE "Local Exec"
		 * %tprel_lo(label)          TLS LE "Local Exec"
		 * %tprel_add(expr)          TLS LE "Local Exec"
		 * %gprel(symbol)            GP-relative
		 */
		reloc_map["hi"] = std::bind(&rv_assembler::handle_reloc_hi, this, _1);
		reloc_map["lo"] = std::bind(&rv_assembler::handle_reloc_lo, this, _1);
		reloc_map["pcrel_hi"] = std::bind(&rv_assembler::handle_reloc_pcrel_hi, this, _1);
		reloc_map["pcrel_lo"] = std::bind(&rv_assembler::handle_reloc_pcrel_lo, this, _1);
		reloc_map["tls_ie_pcrel_hi"] = std::bind(&rv_assembler::handle_reloc_tls_ie_pcrel_hi, this, _1);
		reloc_map["tls_gd_pcrel_hi"] = std::bind(&rv_assembler::handle_reloc_tls_gd_pcrel_hi, this, _1);
		reloc_map["tprel_hi"] = std::bind(&rv_assembler::handle_reloc_tprel_hi, this, _1);
		reloc_map["tprel_lo"] = std::bind(&rv_assembler::handle_reloc_tprel_lo, this, _1);
		reloc_map["tprel_add"] = std::bind(&rv_assembler::handle_reloc_tprel_add, this, _1);
		reloc_map["gprel"] = std::bind(&rv_assembler::handle_reloc_gprel, this, _1);
	}

	static rv_set decode_isa_ext(std::string ext)
	{
		if (ext == "i") return rv_set_i;
		else if (ext == "ima") return rv_set_ima;
		else if (ext == "imac") return rv_set_imac;
		else if (ext == "imafd") return rv_set_imafd;
		else if (ext == "imafdc") return rv_set_imafdc;
		else return rv_set_none;
	}

	void parse_commandline(int argc, const char *argv[])
	{
		cmdline_option options[] =
		{
			{ "-h", "--help", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (help_or_error = true); } },
			{ "-d", "--debug", cmdline_arg_type_none,
				"Show help",
				[&](std::string s) { return (debug = true); } },
			{ "-i", "--isa", cmdline_arg_type_string,
				"ISA Extensions (ima, imac, imafd, imafdc)",
				[&](std::string s) { return (ext = decode_isa_ext(s)); } },
			{ "-m32", "--riscv32", cmdline_arg_type_string,
				"Assembler for RISC-V 32",
				[&](std::string s) { return (width = rv_isa_rv32); } },
			{ "-m64", "--riscv64", cmdline_arg_type_string,
				"Assembler for RISC-V 64 (default)",
				[&](std::string s) { return (width = rv_isa_rv64); } },
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

	bool check_symbol(std::vector<std::string> &args)
	{
		return (args.size() == 1 && assembler::check_symbol(args[0]));
	}

	bool check_private(std::vector<std::string> &args)
	{
		return (args.size() == 1 && assembler::check_private(args[0]));
	}

	bool check_local(std::vector<std::string> &args)
	{
		return (args.size() == 1 && assembler::check_local(args[0]));
	}

	bool check_function(std::vector<std::string> &args)
	{
		return (args.size() == 5 && args[0] == "%" &&
			(reloc_map.find(args[1]) != reloc_map.end()) &&
			args[2] == "(" && args[4] == ")" &&
			(assembler::check_symbol(args[3]) ||
			assembler::check_local(args[3]) ||
			assembler::check_private(args[3])));
	}

	bool check_reloc(std::vector<std::string> &args)
	{
		if (check_symbol(args)) return true;
		if (check_private(args)) return true;
		if (check_local(args)) return true;
		if (check_function(args)) return true;
		return false;
	}

	bool eval(asm_line_ptr &line, std::vector<std::string> tokens, packToken &result)
	{
		/* TODO - handle functions on constant expressions */
		if (check_function(tokens) || check_private(tokens) || check_local(tokens)) return false;
		if (tokens.size() == 1) {
			s64 val;
			if (parse_integral(tokens[0], val)) {
				result = packToken(int64_t(val));
				return true;
			}
		}
		std::string expr = join(tokens, " ");
		calculator calc(expr.c_str());
		result = calc.eval(vars);
		return (result->type == NUM || result->type == INT || result->type == REAL);
	}

	/* handlers */

	bool handle_balign(asm_line_ptr &line)
	{
		auto argv = line->split_args(",");
		if (argv.size() < 1) {
			return line->error(kMissingOperands);
		}
		packToken result;
		if (!eval(line, argv[0], result)) {
			return line->error(kInvalidOperands);
		}
		s64 val = result.asInt();
		as.balign(val);
		return true;
	}

	bool handle_p2align(asm_line_ptr &line)
	{
		auto argv = line->split_args(",");
		if (argv.size() < 1) {
			return line->error(kMissingOperands);
		}
		packToken result;
		if (!eval(line, argv[0], result)) {
			return line->error(kInvalidOperands);
		}
		s64 val = result.asInt();
		as.p2align(val);
		return true;
	}

	bool handle_equ(asm_line_ptr &line)
	{
		auto argv = line->split_args(",");
		if (argv.size() != 2 || argv[0].size() != 1) {
			return line->error(kMissingOperands);
		}
		packToken result;
		if (!eval(line, argv[1], result)) {
			return line->error(kInvalidOperands);
		}
		vars[argv[0][0]] = result;
		return true;
	}

	bool handle_file(asm_line_ptr &line)
	{
		if (line->args.size() != 2) {
			return line->error(kInvalidOperands);
		}
		/* TODO */
		return true;
	}

	bool handle_globl(asm_line_ptr &line)
	{
		if (line->args.size() != 2) {
			return line->error(kInvalidOperands);
		}
		as.global(line->args[1]);
		return true;
	}

	bool handle_weak(asm_line_ptr &line)
	{
		if (line->args.size() != 2) {
			return line->error(kInvalidOperands);
		}
		as.weak(line->args[1]);
		return true;
	}

	bool handle_ident(asm_line_ptr &line)
	{
		if (line->args.size() != 2) {
			return line->error(kInvalidOperands);
		}
		/* ignore */
		return true;
	}

	bool handle_macro(asm_line_ptr &line)
	{
		if (line->args.size() < 2) {
			return line->error(kInvalidOperands);
		}
		if (defining_macro) {
			return line->error(kAlreadyDefiningMacro);
		}
		defining_macro = std::make_shared<asm_macro>(line);
		macro_map[line->args[1]] = defining_macro;
		return true;
	}

	bool handle_endm(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			return line->error(kInvalidOperands);
		}
		defining_macro = asm_macro_ptr();
		return true;
	}

	bool handle_section(asm_line_ptr &line)
	{
		auto argv = line->split_args(",");
		if (argv.size() < 1 || argv[0].size() < 1) {
			return line->error(kInvalidOperands);
		}
		if (argv[0][0][0] != '.') {
			return line->error(kSectionMustBeginWithDot);
		}
		as.get_section(argv[0][0]);
		return true;
	}

	bool handle_text(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			return line->error(kInvalidOperands);
		}
		as.get_section(".text");
		return true;
	}

	bool handle_data(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			return line->error(kInvalidOperands);
		}
		as.get_section(".data");
		return true;
	}

	bool handle_rodata(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			return line->error(kInvalidOperands);
		}
		as.get_section(".rodata");
		return true;
	}

	bool handle_bss(asm_line_ptr &line)
	{
		if (line->args.size() != 1) {
			return line->error(kInvalidOperands);
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
			return line->error(kInvalidOperands);
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

	template <typename T>
	bool handle_words(asm_line_ptr &line)
	{
		auto argv = line->split_args(",");
		if (argv.size() < 1 ) {
			return line->error(kMissingOperands);
		}
		for (size_t i = 0; i < argv.size(); i++) {
			packToken result;
			if (!eval(line, argv[i], result)) {
				if (!(check_symbol(argv[i]) || check_private(argv[i]))) {
					return line->error(kInvalidOperands);
				}
				if (sizeof(T) == 4) {
					as.add_reloc(argv[i][0], R_RISCV_32);
					as.append(T(0));
					return true;
				} else if (sizeof(T) == 8) {
					as.add_reloc(argv[i][0], R_RISCV_64);
					as.append(T(0));
					return true;
				} else {
					return line->error(kInvalidOperands);
				}
			}
			if (T(result.asInt()) > std::numeric_limits<T>::max() ||
				T(result.asInt()) < std::numeric_limits<T>::min()) {
				return line->error(kValueOutOfRange);
			}
			as.append(T(result.asInt()));
		}
		return true;
	}

	bool handle_byte(asm_line_ptr &line)
	{
		return handle_words<u8>(line);
	}

	bool handle_half(asm_line_ptr &line)
	{
		return handle_words<u16>(line);
	}

	bool handle_word(asm_line_ptr &line)
	{
		return handle_words<u32>(line);
	}

	bool handle_dword(asm_line_ptr &line)
	{
		return handle_words<u64>(line);
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

	bool handle_zero(asm_line_ptr &line)
	{
		auto argv = line->split_args(",");
		if (argv.size() != 1) {
			return line->error(kInvalidOperands);
		}
		packToken result;
		if (!eval(line, argv[0], result)) {
			return line->error(kInvalidOperands);
		}
		s64 val = result.asInt();
		for (s64 i = 0; i < val; i++) {
			as.append(u8(0));
		}
		return true;
	}

	bool handle_none(asm_line_ptr &line)
	{
		return true;
	}

	bool handle_la(asm_line_ptr &line)
	{
		/*
		 * TODO
		 *
		 * .1: auipc r,    %pcrel_hi(symbol)
		 *     addi  r, r, %pcrel_lo(1b)
		 *
		 * relocs: R_RISCV_PCREL_HI20, R_RISCV_PCREL_LO12_I
		 */
		auto argv = line->split_args(",");
		if (argv.size() != 2) {
			return line->error(kInvalidOperands);
		}

		/* register operand */
		auto ri = argv[0].size() == 1 ? ireg_map.find(argv[0][0]) : ireg_map.end();
		if (ri == ireg_map.end()) {
			return line->error(kInvalidRegister);
		}

		/* symbol operand */
		if (!check_symbol(argv[1]))
		{
			return line->error(kInvalidOperands);
		}

		/* emit auipc with R_RISCV_PCREL_HI20 */
		as.add_label(1);
		as.add_reloc(argv[1][0], R_RISCV_PCREL_HI20);
		asm_auipc(as, ri->second, 0);

		/* emit addi with R_RISCV_PCREL_LO12_I */
		as.add_reloc("1b", R_RISCV_PCREL_LO12_I);
		asm_addi(as, ri->second, ri->second, 0);

		return true;
	}

	bool handle_lla(asm_line_ptr &line)
	{
		/*
		 * TODO
		 *
		 * .1: auipc r,    %pcrel_hi(symbol)
		 *     addi  r, r, %pcrel_lo(1b)
		 *
		 * relocs: R_RISCV_PCREL_HI20, R_RISCV_PCREL_LO12_I
		 */
		return handle_la(line);
	}

	bool handle_li(asm_line_ptr &line)
	{
		/*
		 * TODO - implement large immediates
		 *
		 * { lui, addiw, slli, addi }
		 */
		auto argv = line->split_args(",");
		if (argv.size() != 2) {
			return line->error(kInvalidOperands);
		}

		/* register operand */
		auto ri = argv[0].size() == 1 ? ireg_map.find(argv[0][0]) : ireg_map.end();
		if (ri == ireg_map.end()) {
			return line->error(kInvalidRegister);
		}

		/* immediate operand */
		packToken result;
		if (!eval(line, argv[1], result)) {
			return line->error(kInvalidOperands);
		}
		s64 imm = result.asInt();
		as.load_imm(ri->second, imm);

		return true;
	}

	bool handle_call(asm_line_ptr &line)
	{
		/*
		 * TODO
		 *
		 * .1: auipc t1,     %pcrel_hi(symbol)
		 *     jalr  ra, t1, %pcrel_lo(1b)
		 *
		 * relocs: R_RISCV_CALL_PLT
		 */
		return line->error(kUnimplementedOperation);
	}

	bool handle_tail(asm_line_ptr &line)
	{
		/*
		 * TODO
		 *
		 * .1: auipc t1,       %pcrel_hi(symbol)
		 *     jalr  zero, t1, %pcrel_lo(1b)
		 */
		return line->error(kUnimplementedOperation);
	}

	std::vector<rv_operand_data> opcode_operand_data(size_t op)
	{
		std::vector<rv_operand_data> op_data;
		const rv_operand_data *data = rv_inst_operand_data[op];
		while(data->type != rv_type_none) {
			op_data.push_back(*data++);
		}
		return op_data;
	}

	bool remove_operand(std::vector<rv_operand_data> &op_data, rv_type type)
	{
		for (auto oi = op_data.begin(); oi != op_data.end(); ) {
			if (oi->type == type) {
				oi = op_data.erase(oi);
				return true;
			} else {
				oi++;
			}
		}
		return false;
	}

	bool handle_opcode(size_t op, asm_line_ptr &line)
	{
		auto argv = line->split_args(",");

		decode dec{};
		dec.op = op;
		auto op_data = opcode_operand_data(op);
		const char *fmt = rv_inst_format[op];

		while (*fmt) {
			switch (*fmt) {
				case '(': break;
				case ',': break;
				case ')': break;
				case '0':
				case '1':
				case '2':
				{
					if (argv.size() == 0) {
						if (op == rv_op_jalr) {
							dec.rs1 = dec.rd;
							dec.rd = rv_ireg_ra;
							break;
						} else {
							return line->error(kMissingRegisterOperand);
						}
					}
					auto arg = argv.front();
					auto ri = arg.size() == 1 ? ireg_map.find(arg[0]) : ireg_map.end();
					if (ri == ireg_map.end()) {
						bool has_imm = false;
						switch (dec.op) {
							case rv_op_sll: dec.op = rv_op_slli; has_imm = true; break;
							case rv_op_srl: dec.op = rv_op_srli; has_imm = true; break;
							case rv_op_sra: dec.op = rv_op_srai; has_imm = true; break;
							case rv_op_add: dec.op = rv_op_addi; has_imm = true; break;
							case rv_op_addw: dec.op = rv_op_addiw; has_imm = true; break;
							case rv_op_and: dec.op = rv_op_andi; has_imm = true; break;
							case rv_op_xor: dec.op = rv_op_xori; has_imm = true; break;
							case rv_op_or: dec.op = rv_op_ori; has_imm = true; break;
							case rv_op_slt: dec.op = rv_op_slti; has_imm = true; break;
							case rv_op_sltu: dec.op = rv_op_sltiu; has_imm = true; break;
						}
						if (has_imm) {
							packToken result;
							bool do_reloc = false;
							if (!eval(line, arg, result) && !(do_reloc = check_reloc(arg))) {
								return line->error(kInvalidImmediateOperand);
							}
							if (do_reloc) {
								if (!handle_reloc(line, dec, arg)) {
									return line->error(kInvalidOperands);
								}
								dec.imm = 0;
							} else {
								dec.imm = result.asInt();
							}
							goto out;
						} else {
							return line->error(kInvalidRegister);
						}
					}
					switch (*fmt) {
						case '0': dec.rd = ri->second; break;
						case '1': dec.rs1 = ri->second; break;
						case '2': dec.rs2 = ri->second; break;
					}
					remove_operand(op_data, rv_type_ireg);
					argv.pop_front();
					break;
				}
				case '3':
				case '4':
				case '5':
				case '6':
				{
					if (argv.size() == 0) {
						return line->error(kMissingRegisterOperand);
					}
					auto arg = argv.front();
					auto ri = arg.size() == 1 ? ireg_map.find(arg[0]) : ireg_map.end();
					if (ri == freg_map.end()) {
						return line->error(kInvalidRegister);
					}
					switch (*fmt) {
						case '3': dec.rd = ri->second; break;
						case '4': dec.rs1 = ri->second; break;
						case '5': dec.rs2 = ri->second; break;
						case '6': dec.rs2 = ri->second; break;
					}
					remove_operand(op_data, rv_type_freg);
					argv.pop_front();
					break;
				}
				case '7':
				{
					if (argv.size() == 0) {
						return line->error(kMissingImmediateOperand);
					}
					auto arg = argv.front();
					packToken result;
					if (!eval(line, arg, result)) {
						return line->error(kInvalidImmediateOperand);
					}
					dec.rs1 = result.asInt();
					remove_operand(op_data, rv_type_simm);
					remove_operand(op_data, rv_type_uimm);
					argv.pop_front();
					break;
				}
				case 'i':
				case 'o':
				{
					/* check for load store address format imm(rs1) */
					if (*(fmt + 1) && *(fmt + 1) == '(' && *(fmt + 2) == '1' && *(fmt + 3) == ')') {
						goto load_store;
					}
					if (argv.size() == 0) {
						if (op == rv_op_jalr) {
							dec.imm = 0;
							break;
						} else {
							return line->error(kMissingImmediateOperand);
						}
					}
					auto arg = argv.front();
					packToken result;
					bool do_reloc = false;
					if (!eval(line, arg, result) && !(do_reloc = check_reloc(arg))) {
						return line->error(kInvalidOperands);
					}
					if (do_reloc) {
						/*
						 * emit relocation
						 *
						 * I-Type  (addi)
						 * UJ-Type (jal) R_RISCV_JAL
						 * SB-Type (beq,bne,nlt,bge,bltu,bgeu) R_RISCV_BRANCH
						 * U-Type  (auipc,lui) R_RISCV_HI20, R_RISCV_PCREL_HI20
						 *
						 * 1f
						 * .L11
						 * label
						 * %lo(symbol)
						 * %pcrel_lo(symbol)
						 */
						if (!handle_reloc(line, dec, arg)) {
							return line->error(kInvalidOperands);
						}
						dec.imm = 0;
					} else {
						dec.imm = result.asInt();
					}
					remove_operand(op_data, rv_type_simm);
					remove_operand(op_data, rv_type_uimm);
					argv.pop_front();
					break;
				}
				case 'c':
				{
					if (argv.size() == 0) {
						return line->error(kMissingCSROperand);
					}
					auto arg = argv.front();
					if (arg.size() != 1) {
						return line->error(kInvalidCSROperand);
					}
					s64 val;
					if (parse_integral(arg[0], val)) {
						dec.imm = val;
					} else {
						auto ci = csr_map.find(arg[0]);
						if (ci == csr_map.end()) {
							return line->error(kUnknownCSROperand);
						}
						dec.imm = ci->second;
					}
					remove_operand(op_data, rv_type_uimm);
					argv.pop_front();
					break;
				}
				case 'r':
					switch(dec.rm) {
						case rv_rm_rne: /* */; break;
						case rv_rm_rtz: /* */; break;
						case rv_rm_rdn: /* */; break;
						case rv_rm_rup: /* */; break;
						case rv_rm_rmm: /* */; break;
						case rv_rm_dyn: /* */; break;
					}
					break;
				case 'p':
					/* (dec.pred & rv_fence_i); */
					/* (dec.pred & rv_fence_o); */
					/* (dec.pred & rv_fence_r); */
					/* (dec.pred & rv_fence_w); */
					break;
				case 's':
					/* (dec.succ & rv_fence_i); */
					/* (dec.succ & rv_fence_o); */
					/* (dec.succ & rv_fence_r); */
					/* (dec.succ & rv_fence_w); */
					break;
				case 'O': break;
				case '\t': break;
				case 'A': /* if (dec.aq) */ break;
				case 'R': /* if (dec.rl) */ break;
				default:
					break;
			}
			fmt++;
		}
out:
		/* translate pseudo instructions to regular instruction */
		encode_pseudo(dec);

		/* encode instruction */
		as.append(u32(encode_inst(dec)));

		return true;

load_store:
		if (argv.size() != 1) {
			return line->error(kInvalidOperands);
		}
		std::vector<std::string> arg = argv[0];

		if (arg.size() < 3 || (*(arg.end() - 3) != "(") || (*(arg.end() - 1) != ")")) {
			return line->error(kInvalidOperands);
		}

		auto ri = ireg_map.find(arg[arg.size() - 2]);
		if (ri == ireg_map.end()) {
			return line->error(kInvalidRegister);
		}
		dec.rs2 = ri->second;

		if (arg.size() > 3) {
			arg.erase(arg.begin() + arg.size() - 3, arg.end());
			packToken result;

			bool do_reloc = false;
			if (!eval(line, arg, result) && !(do_reloc = check_reloc(arg))) {
				return line->error(kInvalidOperands);
			}
			if (do_reloc) {
				/*
				 * emit relocation
				 *
				 * I-Type (lb,lbu,lh,lhu,lw,lwu,flw,fld) R_RISCV_LO12_I, R_RISCV_PCREL_LO12_I
				 * S-Type (sb,sh,sw,fsw,fsd) R_RISCV_PCREL_LO12_S, R_RISCV_LO12_S
				 *
				 * %lo(symbol)
				 * %pcrel_lo(symbol)
				 */
				if (!handle_reloc(line, dec, arg)) {
					return line->error(kInvalidOperands);
				}
				dec.imm = 0;
			} else {
				dec.imm = result.asInt();
			}
		} else {
			dec.imm = 0;
		}

		goto out;
	}

	bool handle_label(asm_line_ptr line)
	{
		s64 num;
		std::string label = line->args[0];
		if (parse_integral(label, num)) {
			as.add_label(num);
		} else {
			auto l = as.lookup_label(label);
			if (l) return line->error(kDuplicateSymbol);
			as.add_label(label);
		}
		line->args.erase(line->args.begin(), line->args.begin() + 2);
		return true;
	}

	void write_elf(std::string filename)
	{
		/* TODO */
	}

	bool process_line(asm_line_ptr line)
	{
		if (debug) {
			printf("%-30s %s\n",
				format_string("%s:%d", line->file->filename.c_str(), line->line_num).c_str(),
				line->str().c_str());
		}

		/* check if we are defining a macro */
		if (defining_macro && !(line->args.size() > 0 && line->args[0] == ".endm")) {
			defining_macro->macro_lines.push_back(line);
			return true;
		}

		/* check for label */
		if (line->args.size() >= 2 && line->args[1] == ":") {
			handle_label(line);
		}
		if (line->args.size() == 0) {
			return true;
		}

		/* check for internal directives */
		auto di = directive_map.find(line->args[0]);
		if (di != directive_map.end()) {
			return di->second(line);
		}

		/* check for opcode */
		auto oi = opcode_map.find(line->args[0]);
		if (oi != opcode_map.end()) {
			return handle_opcode(oi->second, line);
		}

		/* check for macro */
		auto mi = macro_map.find(line->args[0]);
		if (mi != macro_map.end()) {
			auto expander = mi->second->get_expander(line);
			if (!expander) {
				return line->error(kInvalidMacroOperands);
			}
			for (auto macro_line : mi->second->macro_lines) {
				process_line(expander->substitute(macro_line));
			}
			return true;
		}

		return line->error(kInvalidStatement);
	}

	int handle_reloc_hi(decode &dec)
	{
		switch (dec.op) {
			case rv_op_lui:
			case rv_op_auipc:
				return R_RISCV_HI20;
		}
		return R_RISCV_NONE;
	}

	int handle_reloc_lo(decode &dec)
	{
		switch (dec.op) {
			case rv_op_addi:
			case rv_op_addiw:
			case rv_op_lb:
			case rv_op_lbu:
			case rv_op_lh:
			case rv_op_lhu:
			case rv_op_lw:
			case rv_op_lwu:
			case rv_op_ld:
			case rv_op_fld:
			case rv_op_flw:
				return R_RISCV_LO12_I;
			case rv_op_sb:
			case rv_op_sh:
			case rv_op_sw:
			case rv_op_sd:
			case rv_op_fsw:
			case rv_op_fsd:
				return R_RISCV_LO12_S;
		}
		return R_RISCV_NONE;
	}

	int handle_reloc_pcrel_hi(decode &dec)
	{
		switch (dec.op) {
			case rv_op_lui:
			case rv_op_auipc:
				return R_RISCV_PCREL_HI20;
		}
		return R_RISCV_NONE;
	}

	int handle_reloc_pcrel_lo(decode &dec)
	{
		switch (dec.op) {
			case rv_op_addi:
			case rv_op_addiw:
			case rv_op_lb:
			case rv_op_lbu:
			case rv_op_lh:
			case rv_op_lhu:
			case rv_op_lw:
			case rv_op_lwu:
			case rv_op_ld:
			case rv_op_fld:
			case rv_op_flw:
				return R_RISCV_PCREL_LO12_I;
			case rv_op_sb:
			case rv_op_sh:
			case rv_op_sw:
			case rv_op_sd:
			case rv_op_fsw:
			case rv_op_fsd:
				return R_RISCV_PCREL_LO12_S;
		}
		return R_RISCV_NONE;
	}

	int handle_reloc_tls_ie_pcrel_hi(decode &dec)
	{
		return R_RISCV_NONE; /* TODO */
	}

	int handle_reloc_tls_gd_pcrel_hi(decode &dec)
	{
		return R_RISCV_NONE; /* TODO */
	}

	int handle_reloc_tprel_hi(decode &dec)
	{
		return R_RISCV_NONE; /* TODO */
	}

	int handle_reloc_tprel_lo(decode &dec)
	{
		return R_RISCV_NONE; /* TODO */
	}

	int handle_reloc_tprel_add(decode &dec)
	{
		return R_RISCV_NONE; /* TODO */
	}

	int handle_reloc_gprel(decode &dec)
	{
		switch (dec.op) {
			case rv_op_addi:
			case rv_op_addiw:
			case rv_op_lb:
			case rv_op_lbu:
			case rv_op_lh:
			case rv_op_lhu:
			case rv_op_lw:
			case rv_op_lwu:
			case rv_op_ld:
			case rv_op_fld:
			case rv_op_flw:
				return R_RISCV_GPREL_I;
			case rv_op_sb:
			case rv_op_sh:
			case rv_op_sw:
			case rv_op_sd:
			case rv_op_fsw:
			case rv_op_fsd:
				return R_RISCV_GPREL_S;
		}
		return R_RISCV_NONE;
	}

	int handle_reloc_other(decode &dec)
	{
		switch (dec.op) {
			case rv_op_beq:
			case rv_op_bne:
			case rv_op_blt:
			case rv_op_bge:
			case rv_op_bltu:
			case rv_op_bgeu:
			case rv_op_beqz:
			case rv_op_bnez:
			case rv_op_blez:
			case rv_op_bgez:
			case rv_op_bltz:
			case rv_op_bgtz:
				return R_RISCV_BRANCH;
			case rv_op_j:
			case rv_op_jal:
				return R_RISCV_JAL;
		}
		return R_RISCV_NONE;
	}

	bool handle_reloc(asm_line_ptr &line, decode &dec, std::vector<std::string> args)
	{
		/*
		 * handle % function relocations
		 *
		 *
		 * %hi(symbol)               Absolute imm20
		 * %lo(symbol)               Absolute imm12
		 * %pcrel_hi(symbol)         PC-relative imm20
		 * %pcrel_lo(label)          PC-relative imm12
		 * %tls_ie_pcrel_hi(symbol)  TLS IE GOT "Initial Exec"
		 * %tls_gd_pcrel_hi(symbol)  TLS GD GOT "Global Dynamic"
		 * %tprel_hi(symbol)         TLS LE "Local Exec"
		 * %tprel_lo(label)          TLS LE "Local Exec"
		 * %tprel_add(symbol)        TLS LE "Local Exec"
		 * %gprel(symbol)            GP-relative
		 */
		if (check_function(args)) {
			auto ri = reloc_map.find(args[1]);
			if (ri != reloc_map.end()) {
				int reloc_type = ri->second(dec);
				if (reloc_type != R_RISCV_NONE) {
					as.add_reloc(args[3], reloc_type);
					return true;
				} else {
					return false;
				}
			}
		} else if (check_symbol(args) || check_private(args) || check_local(args)) {
			int reloc_type = handle_reloc_other(dec);
			if (reloc_type != R_RISCV_NONE) {
				as.add_reloc(args[0], reloc_type);
				return true;
			} else {
				return false;
			}
		}
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
		as.link();
		if (debug) {
			dump();
		}
		write_elf(output_filename);
	}

	std::string format_inst(inst_t inst)
	{
		std::string buf;
		switch (inst_length(inst)) {
			case 2:  sprintf(buf, "%04llx    ", inst); break;
			case 4:  sprintf(buf, "%08llx", inst); break;
			case 6:  sprintf(buf, "%012llx", inst); break;
			case 8:  sprintf(buf, "%016llx", inst); break;
			default: sprintf(buf, "(invalid)"); break;
		}
		return buf;
	}

	void dump()
	{
		std::vector<u8> &buf = as.get_section(".text")->buf;
		addr_t pc = 0, end = buf.size();
		addr_t pc_offset;
		decode dec;
		printf("\nDissasembly\n\n");
		while (pc < end) {
			inst_t inst = inst_fetch(addr_t(buf.data() + pc), pc_offset);
			decode_inst_rv64(dec, inst);
			decode_pseudo_inst(dec);
			std::string args = disasm_inst_simple(dec);
			printf("%8llx\t(%8s)\t%s\n",
				pc, format_inst(inst).c_str(), args.c_str());
			pc += pc_offset;
		}
		printf("\nRelocations\n\n");
		for (auto ent : as.relocs_byoffset) {
			auto &reloc = ent.second;
			printf("%8zx\t%-20s\t%s\n",
				reloc->offset.second,
				elf_rela_type_name(reloc->rela_type),
				reloc->name.c_str());
		}
	}
};

/* program main */

int main(int argc, const char* argv[])
{
	printf("\n");
	printf("rv-asm-0.0.0-prealpha-0\n");
	printf("\n");
	rv_assembler as;
	as.parse_commandline(argc, argv);
	as.assemble();
	return 0;
}
