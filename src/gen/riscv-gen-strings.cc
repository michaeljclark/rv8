//
//  riscv-gen-strings.cc
//

#include <cstdio>
#include <cinttypes>
#include <sstream>
#include <functional>
#include <algorithm>
#include <memory>
#include <string>
#include <vector>
#include <deque>
#include <map>
#include <set>

#include "riscv-util.h"
#include "riscv-cmdline.h"
#include "riscv-model.h"
#include "riscv-gen.h"

std::vector<cmdline_option> riscv_gen_strings::get_cmdline_options()
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

static void print_strings_h(riscv_gen *gen)
{
	static const char* kStringsHeader =

R"C(#ifndef riscv_strings_h
#define riscv_strings_h

#ifdef __cplusplus
extern "C" {
#endif

extern const char* riscv_ireg_name_sym[];
extern const char* riscv_freg_name_sym[];
extern const char* riscv_inst_name_sym[];
extern const char* riscv_operand_name_sym[];
extern const char* riscv_operand_type_sym[];
extern const char* riscv_csr_name_sym[];
extern const char* riscv_fault_name_sym[];
extern const char* riscv_intr_name_sym[];

#ifdef __cplusplus
}
#endif

#endif
)C";

	printf(kCHeader, "riscv-strings.h");
	printf("%s", kStringsHeader);
}

static void print_strings_cc(riscv_gen *gen)
{
	printf(kCHeader, "riscv-strings.cc");

	bool no_comment = gen->has_option("no_comment");

	printf("#include \"riscv-strings.h\"\n");
	printf("\n");

	// Integer register names
	printf("const char* riscv_ireg_name_sym[] = {\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "ireg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Floating Point register names
	printf("const char* riscv_freg_name_sym[] = {\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "freg") continue;
		printf("\t\"%s\",\n", reg->alias.c_str());
	}
	printf("};\n\n");

	// Instruction names
	printf("const char* riscv_inst_name_sym[] = {\n");
	print_array_illegal_str("illegal", no_comment);
	for (auto &opcode : gen->opcodes) {
		std::string opcode_name = riscv_meta_model::opcode_format("", opcode, ".", false);
		printf("\t%s\"%s\",\n",
			riscv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			opcode_name.c_str());
	}
	printf("};\n\n");

	// Instruction operand names
	printf("const char* riscv_operand_name_sym[] = {\n");
	printf("\t\"none\",\n");
	for (auto &operand : gen->operands) {
		printf("\t\"%s\",\n", operand->name.c_str());
	}
	printf("};\n\n");

	// Instruction operand type names
	std::vector<std::string> operand_types;
	for (auto &operand : gen->operands) {
		auto type_name = riscv_meta_model::format_type(operand);
		if (std::find(operand_types.begin(), operand_types.end(), type_name) == operand_types.end()) {
			operand_types.push_back(type_name);
		}
	}
	printf("const char* riscv_operand_type_sym[] = {\n");
	printf("\t\"none\",\n");
	for (auto &operand_type : operand_types) {
		printf("\t\"%s\",\n", operand_type.c_str());
	}
	printf("};\n\n");

	// CSR names
	std::map<int,riscv_csr_ptr> csr_map;
	for (auto &csr : gen->csrs) {
		int csr_num = riscv_parse_value(csr->number.c_str());
		csr_map[csr_num] = csr;
	}
	printf("const char* riscv_csr_name_sym[] = {\n");
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
	printf("const char* riscv_fault_name_sym[] = {\n");
	for (auto &en : gen->enums) {
		if (en->group != "intr") continue;
		printf("\t\"%s\"\n", en->name.c_str());
	}
	printf("};\n\n");

	// interrupt names
	printf("const char* riscv_intr_name_sym[] = {\n");
	for (auto &en : gen->enums) {
		if (en->group != "intr") continue;
		printf("\t\"%s\"\n", en->name.c_str());
	}
	printf("};\n\n");
}

void riscv_gen_strings::generate()
{
	if (gen->has_option("print_strings_h")) print_strings_h(gen);
	if (gen->has_option("print_strings_cc")) print_strings_cc(gen);
}
