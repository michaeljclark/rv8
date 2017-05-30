//
//  gen-strings.cc
//

#include <cstdio>
#include <cstdlib>
#include <cinttypes>
#include <functional>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include "util.h"
#include "cmdline.h"
#include "model.h"
#include "gen.h"

std::vector<cmdline_option> rv_gen_strings::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-SH", "--print-strings-h", cmdline_arg_type_none,
			"Print strings header",
			[&](std::string s) { return gen->set_option("print_strings_h"); } },
		{ "-SC", "--print-strings-cc", cmdline_arg_type_none,
			"Print strings source",
			[&](std::string s) { return gen->set_option("print_strings_cc"); } },
	};
}

static const char* unknown_op_comment = "/*              unknown */ ";

static void print_array_illegal_str(const char *str, bool no_comment)
{
	printf("\t%s\"%s\",\n", no_comment ? "" : unknown_op_comment, str);
}

static void print_strings_h(rv_gen *gen)
{
	static const char* kStringsHeader =

R"C(#ifndef rv_strings_h
#define rv_strings_h

#ifdef __cplusplus
extern "C" {
#endif

extern const char* rv_ireg_name_sym[];
extern const char* rv_freg_name_sym[];
extern const char* rv_inst_name_sym[];
extern const char* rv_operand_name_sym[];
extern const char* rv_operand_type_sym[];
extern const char* rv_csr_name_sym[];
extern const char* rv_cause_name_sym[];
extern const char* rv_intr_name_sym[];

#ifdef __cplusplus
}
#endif

#endif
)C";

	printf(kCHeader, "strings.h");
	printf("%s", kStringsHeader);
}

static void print_strings_cc(rv_gen *gen)
{
	printf(kCHeader, "strings.cc");

	bool no_comment = gen->has_option("no_comment");

	printf("#include \"strings.h\"\n");
	printf("\n");

	// Integer register names
	printf("const char* rv_ireg_name_sym[] = {\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "ireg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("\tnullptr\n");
	printf("};\n\n");

	// Floating Point register names
	printf("const char* rv_freg_name_sym[] = {\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "freg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("\tnullptr\n");
	printf("};\n\n");

	// Instruction names
	printf("const char* rv_inst_name_sym[] = {\n");
	print_array_illegal_str("illegal", no_comment);
	for (auto &opcode : gen->opcodes) {
		std::string opcode_name = rv_meta_model::opcode_format("", opcode, ".");
		printf("\t%s\"%s\",\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			opcode_name.c_str());
	}
	printf("\tnullptr\n");
	printf("};\n\n");

	// Instruction operand names
	printf("const char* rv_operand_name_sym[] = {\n");
	printf("\t\"none\",\n");
	for (auto &operand : gen->operands) {
		printf("\t\"%s\",\n", operand->name.c_str());
	}
	printf("\tnullptr\n");
	printf("};\n\n");

	// Instruction operand type names
	std::vector<std::string> operand_types;
	for (auto &operand : gen->operands) {
		auto type_name = rv_meta_model::format_type(operand);
		if (std::find(operand_types.begin(), operand_types.end(), type_name) == operand_types.end()) {
			operand_types.push_back(type_name);
		}
	}
	printf("const char* rv_operand_type_sym[] = {\n");
	printf("\t\"none\",\n");
	for (auto &operand_type : operand_types) {
		printf("\t\"%s\",\n", operand_type.c_str());
	}
	printf("\tnullptr\n");
	printf("};\n\n");

	// CSR names
	std::map<int,rv_csr_ptr> csr_map;
	for (auto &csr : gen->csrs) {
		int csr_num = rv_parse_value(csr->number.c_str());
		csr_map[csr_num] = csr;
	}
	printf("const char* rv_csr_name_sym[] = {\n");
	for (int i = 0; i < 4096; i++) {
		auto ci = csr_map.find(i);
		printf("\t%s%s%s,\n",
			(ci != csr_map.end()) ? "\"" : "",
			(ci != csr_map.end()) ? ci->second->name.c_str() : "nullptr",
			(ci != csr_map.end()) ? "\"" : ""
		);
	}
	printf("};\n\n");

	// fault names
	printf("const char* rv_cause_name_sym[] = {\n");
	for (auto &en : gen->enums) {
		if (en->group != "cause") continue;
		printf("\t\"%s\",\n", en->name.c_str());
	}
	printf("\tnullptr\n");
	printf("};\n\n");

	// interrupt names
	printf("const char* rv_intr_name_sym[] = {\n");
	for (auto &en : gen->enums) {
		if (en->group != "intr") continue;
		printf("\t\"%s\",\n", en->name.c_str());
	}
	printf("\tnullptr\n");
	printf("};\n\n");
}

void rv_gen_strings::generate()
{
	if (gen->has_option("print_strings_h")) print_strings_h(gen);
	if (gen->has_option("print_strings_cc")) print_strings_cc(gen);
}
