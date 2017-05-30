//
//  gen-jit.cc
//

#include <cstdio>
#include <cstdlib>
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

std::vector<cmdline_option> rv_gen_jit::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-J", "--print-jit-h", cmdline_arg_type_none,
			"Print jit header",
			[&](std::string s) { return gen->set_option("print_jit_h"); } },
		{ "-K", "--print-jit-cc", cmdline_arg_type_none,
			"Print jit source",
			[&](std::string s) { return gen->set_option("print_jit_cc"); } },
	};
}

static void print_jit_h(rv_gen *gen)
{
	printf(kCHeader, "jit.h");
	printf("#ifndef rv_jit_h\n");
	printf("#define rv_jit_h\n");
	printf("\n");
	printf("namespace riscv\n{\n");
	for (auto &opcode : gen->opcodes) {
		// exclude compressed and psuedo instructions
		if (opcode->compressed || opcode->is_pseudo()) continue;

		// create emit interface
		std::string emit_name = rv_meta_model::opcode_format("emit_", opcode, "_");
		std::vector<std::string> operand_list;
		for (auto &operand : opcode->codec->operands) {
			auto type_name = rv_meta_model::format_type(operand) + " " + operand->name;
			operand_list.push_back(type_name);
		}

		// output emit interface
		printf("\tinst_t %s(%s);\n",
			emit_name.c_str(), join(operand_list, ", ").c_str());
	}
	printf("\n");

	for (auto &opcode : gen->opcodes) {
		// exclude compressed and psuedo instructions
		if (opcode->compressed || opcode->is_pseudo()) continue;

		// create emit interface
		std::string emit_name = rv_meta_model::opcode_format("asm_", opcode, "_");
		std::vector<std::string> operand_list;
		operand_list.push_back("assembler &as");
		for (auto &operand : opcode->codec->operands) {
			auto type_name = rv_meta_model::format_type(operand) + " " + operand->name;
			operand_list.push_back(type_name);
		}

		// output emit interface
		printf("\tbool %s(%s);\n",
			emit_name.c_str(), join(operand_list, ", ").c_str());
	}
	printf("}\n");
	printf("\n");

	printf("#endif\n");
}

static void print_jit_cc(rv_gen *gen)
{
	static const char* kJitSource =

R"C(#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <map>
#include <vector>
#include <memory>
#include <string>

#include "types.h"
#include "host-endian.h"
#include "bits.h"
#include "meta.h"
#include "codec.h"
#include "assembler.h"
#include "jit.h"

using namespace riscv;

)C";

	printf(kCHeader, "jit.cc");
	printf("%s", kJitSource);

	for (auto &opcode : gen->opcodes) {
		// exclude compressed and psuedo instructions
		if (opcode->compressed || opcode->is_pseudo()) continue;

		// create emit interface
		std::string emit_name = rv_meta_model::opcode_format("emit_", opcode, "_");
		std::vector<std::string> operand_list;
		for (auto &operand : opcode->codec->operands) {
			auto type_name = rv_meta_model::format_type(operand) + " " + operand->name;
			operand_list.push_back(type_name);
		}

		// output emit interface
		printf("inst_t riscv::%s(%s)\n{\n",
			emit_name.c_str(), join(operand_list, ", ").c_str());
		printf("\tdecode dec;\n");
		if (opcode->codec->operands.size() > 0) {
			std::vector<std::string> check_list;
			for (auto &operand : opcode->codec->operands) {
				auto check_name = operand->name + ".valid()";
				check_list.push_back(check_name);
			}
			printf("\tif (!(%s)) return 0; /* illegal instruction */\n",
				join(check_list, " && ").c_str());
		}
		printf("\tdec.op = %s;\n", rv_meta_model::opcode_format("rv_op_", opcode, "_").c_str());
		for (auto &operand : opcode->codec->operands) {
			if (operand->type == "offset" || operand->type == "simm" || operand->type == "uimm") {
				printf("\tdec.imm = %s;\n", operand->name.c_str());
			} else if (operand->type == "ireg") {
				printf("\tdec.%s = %s;\n", operand->name.c_str(), operand->name.c_str());
			} else if (operand->type == "freg") {
				printf("\tdec.%s = %s;\n", operand->name.substr(1).c_str(), operand->name.c_str());
			} else if (operand->type == "arg") {
				printf("\tdec.%s = %s;\n", operand->name.c_str(), operand->name.c_str());
			} else {
				printf("/* dec.? = %s unhandled */\n", operand->name.c_str());
			}
		}
		printf("\treturn encode_inst(dec);\n");
		printf("}\n\n");
	}
	printf("\n");

	for (auto &opcode : gen->opcodes) {
		// exclude compressed and psuedo instructions
		if (opcode->compressed || opcode->is_pseudo()) continue;

		// create emit interface
		std::string emit_name = rv_meta_model::opcode_format("asm_", opcode, "_");
		std::vector<std::string> operand_list;
		operand_list.push_back("assembler &as");
		for (auto &operand : opcode->codec->operands) {
			auto type_name = rv_meta_model::format_type(operand) + " " + operand->name;
			operand_list.push_back(type_name);
		}

		// output emit interface
		printf("bool riscv::%s(%s)\n{\n",
			emit_name.c_str(), join(operand_list, ", ").c_str());
		printf("\tdecode dec;\n");
		if (opcode->codec->operands.size() > 0) {
			std::vector<std::string> check_list;
			for (auto &operand : opcode->codec->operands) {
				auto check_name = operand->name + ".valid()";
				check_list.push_back(check_name);
			}
			printf("\tif (!(%s)) return false; /* illegal instruction */\n",
				join(check_list, " && ").c_str());
		}
		printf("\tdec.op = %s;\n", rv_meta_model::opcode_format("rv_op_", opcode, "_").c_str());
		for (auto &operand : opcode->codec->operands) {
			if (operand->type == "offset" || operand->type == "simm" || operand->type == "uimm") {
				printf("\tdec.imm = %s;\n", operand->name.c_str());
			} else if (operand->type == "ireg") {
				printf("\tdec.%s = %s;\n", operand->name.c_str(), operand->name.c_str());
			} else if (operand->type == "freg") {
				printf("\tdec.%s = %s;\n", operand->name.substr(1).c_str(), operand->name.c_str());
			} else if (operand->type == "arg") {
				printf("\tdec.%s = %s;\n", operand->name.c_str(), operand->name.c_str());
			} else {
				printf("/* dec.? = %s unhandled */\n", operand->name.c_str());
			}
		}
		printf("\tas.add_inst(encode_inst(dec));\n");
		printf("\treturn true;\n");
		printf("}\n\n");
	}
	printf("\n");
}

void rv_gen_jit::generate()
{
	if (gen->has_option("print_jit_h")) print_jit_h(gen);
	if (gen->has_option("print_jit_cc")) print_jit_cc(gen);
}
