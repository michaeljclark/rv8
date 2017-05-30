//
//  gen-cc.cc
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

std::vector<cmdline_option> rv_gen_cc::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-CC", "--print-inst-cc", cmdline_arg_type_none,
			"Print instruction source",
			[&](std::string s) { return gen->set_option("print_fpu_asm_cc"); } },
	};
}

typedef std::pair<const rv_primitive_type*,std::string> rv_operand_desc;

static rv_operand_desc rv_fpu_operand_type(rv_opcode_ptr &opcode, rv_extension_ptr &ext, rv_operand_ptr &operand, size_t i)
{
	// create operand name
	std::string operand_name = operand->name;
	operand_name = replace(operand_name, "frd", "d");
	operand_name = replace(operand_name, "rd", "d");
	operand_name = replace(operand_name, "frs", "s");
	operand_name = replace(operand_name, "rs", "s");

	return rv_operand_desc(rv_meta_model::infer_operand_primitive(opcode, ext, operand, i), operand_name);
}

template <typename T> void typed_value_set(std::set<std::string> &values, const rv_primitive_type *primitive)
{
	char fmt[16];
	char buf[256];
	snprintf(fmt, sizeof(fmt), "%s%s", primitive->c_fmt, primitive->c_suffix);
	snprintf(buf, sizeof(buf), fmt, T(2)); values.insert(buf);
	if (std::numeric_limits<T>::is_signed) {
		snprintf(buf, sizeof(buf), fmt, T(-2)); values.insert(buf);
	}
	if (std::numeric_limits<T>::is_iec559) {
		values.insert("NAN");
		values.insert("INFINITY");
		snprintf(buf, sizeof(buf), fmt, T(0.5)); values.insert(buf);
		snprintf(buf, sizeof(buf), fmt, T(0.25)); values.insert(buf);
	}
}

static std::set<std::string> test_values(const rv_primitive_type *primitive)
{
	std::set<std::string> values;
	switch (primitive->enum_type) {
		case rvt_sx:  typed_value_set<int64_t>(values, primitive); break;
		case rvt_ux:  typed_value_set<uint64_t>(values, primitive); break;
		case rvt_s32: typed_value_set<int32_t>(values, primitive); break;
		case rvt_u32: typed_value_set<uint32_t>(values, primitive); break;
		case rvt_s64: typed_value_set<int64_t>(values, primitive); break;
		case rvt_u64: typed_value_set<uint64_t>(values, primitive); break;
		case rvt_f32: typed_value_set<float>(values, primitive); break;
		case rvt_f64: typed_value_set<double>(values, primitive); break;
		default: break;
	};
	return values;
}

static std::string opcode_substitute(rv_opcode_ptr opcode)
{
	std::string inst = opcode->pseudocode_c;
	inst = replace(inst, "u32(f32(NAN))", "0x7fc00000");
	inst = replace(inst, "u64(f64(NAN))", "0x7ff8000000000000ULL");
	inst = replace(inst, "isnan", "std::isnan");
	inst = replace(inst, "f32(frd)", "d");
	inst = replace(inst, "f32(frs1)", "s1");
	inst = replace(inst, "f32(frs2)", "s2");
	inst = replace(inst, "f32(frs3)", "s3");
	inst = replace(inst, "f64(frd)", "d");
	inst = replace(inst, "f64(frs1)", "s1");
	inst = replace(inst, "f64(frs2)", "s2");
	inst = replace(inst, "f64(frs3)", "s3");
	inst = replace(inst, "u32(frd)", "d");
	inst = replace(inst, "u32(frs1)", "s1");
	inst = replace(inst, "u32(frs2)", "s2");
	inst = replace(inst, "u64(frd)", "d");
	inst = replace(inst, "u64(frs1)", "s1");
	inst = replace(inst, "u64(frs2)", "s2");
	inst = replace(inst, "s32(frd)", "d");
	inst = replace(inst, "s32(frs1)", "s1");
	inst = replace(inst, "s32(frs2)", "s2");
	inst = replace(inst, "s64(frd)", "d");
	inst = replace(inst, "s64(frs1)", "s1");
	inst = replace(inst, "s64(frs2)", "s2");
	inst = replace(inst, "rd", "d");
	inst = replace(inst, "rs1", "s1");
	inst = replace(inst, "rs2", "s2");
	inst = replace(inst, "rs3", "s3");
	return inst;
}

static void print_fpu_asm_cc(rv_gen *gen)
{
	static const char* kFpuSourceHeader =

R"C(#include <cstdio>
#include <cstdint>
#include <cfloat>
#include <cfenv>
#include <cmath>
#include <limits>

#include "host-endian.h"
#include "types.h"
#include "meta.h"
#include "fpu.h"

using namespace riscv;

typedef signed long        sx;
typedef unsigned long      ux;

static u32 fcsr = 0;
static s32 rm = 0;

)C";

	printf(kCHeader, "test-cc.cc");
	printf("%s", kFpuSourceHeader);

	std::vector<std::pair<rv_extension_ptr,std::string>> test_cases;

	// loop through FPU instructions
	for (auto &opcode : gen->opcodes) {

		// skip pseudo opcodes
		if (opcode->is_pseudo()) continue;

		// sign inject not supported
		if (opcode->name.find("fsgn") == 0) continue;

		// find extension with minimum isa width
		auto ext_min_width_i = std::min_element(opcode->extensions.begin(), opcode->extensions.end(),
			[](rv_extension_ptr &a, rv_extension_ptr &b){ return a->isa_width < b->isa_width; });
		if (ext_min_width_i == opcode->extensions.end()) continue;
		auto ext = *ext_min_width_i;

		// skip non floating point instructions
		if (ext->alpha_code != 'f' && ext->alpha_code != 'd') continue;

		// infer C argument types for test function
		bool skip_imm = false;
		std::vector<rv_operand_desc> operand_list;
		for (size_t i = 0; i < opcode->codec->operands.size(); i++) {
			auto operand = opcode->codec->operands[i];
			// round mode operand not supported
			if (operand->name == "rm") continue;
			// instructions with immediates not supported (fld, fsw)
			if (operand->name.find("imm") != std::string::npos) skip_imm = true;
			// infer primitive type based on the opcode components and operand types
			operand_list.push_back(rv_fpu_operand_type(opcode, ext, operand, i));
		}
		if (skip_imm) continue;

		std::vector<std::string> fn_arity;
		std::transform(operand_list.begin() + 1, operand_list.end(),
			std::back_inserter(fn_arity), [](const rv_operand_desc& operand) {
				return format_string("%s %s", operand.first->meta_type, operand.second.c_str());
		});
		std::vector<std::string> operand_fmt;
		std::transform(operand_list.begin(), operand_list.end(),
			std::back_inserter(operand_fmt), [](const rv_operand_desc& operand) {
				return format_string("%s", operand.first->c_fmt);
		});

		// generate test functions
		if (ext->isa_width == 64) printf("#if _RISCV_SZPTR != _RISCV_SZINT\n");
		std::string opcode_name = rv_meta_model::opcode_format("", opcode, "_");
		std::string inst = opcode_substitute(opcode);
		printf("extern \"C\" %s %s(%s)\n{\n", operand_list.front().first->meta_type, opcode_name.c_str(), join(fn_arity, ", ").c_str());
		printf("\t%s d = 0;\n", operand_list.front().first->meta_type);
		printf("\t%s;\n", inst.c_str());
		printf("\treturn d;\n");
		printf("}\n\n");
		if (ext->isa_width == 64) printf("#endif\n");
	}

	// loop through FPU instructions
	for (auto &opcode : gen->opcodes) {

		// skip pseudo opcodes
		if (opcode->is_pseudo()) continue;

		// sign inject not supported
		if (opcode->name.find("fsgn") == 0) continue;

		// find extension with minimum isa width
		auto ext_min_width_i = std::min_element(opcode->extensions.begin(), opcode->extensions.end(),
			[](rv_extension_ptr &a, rv_extension_ptr &b){ return a->isa_width < b->isa_width; });
		if (ext_min_width_i == opcode->extensions.end()) continue;
		auto ext = *ext_min_width_i;

		// skip non floating point instructions
		if (ext->alpha_code != 'f' && ext->alpha_code != 'd') continue;

		// infer C argument types for test function
		bool skip_imm = false;
		std::vector<rv_operand_desc> operand_list;
		for (size_t i = 0; i < opcode->codec->operands.size(); i++) {
			auto operand = opcode->codec->operands[i];
			// round mode operand not supported
			if (operand->name == "rm") continue;
			// instructions with immediates not supported (fld, fsw)
			if (operand->name.find("imm") != std::string::npos) skip_imm = true;
			// infer primitive type based on the opcode components and operand types
			operand_list.push_back(rv_fpu_operand_type(opcode, ext, operand, i));
		}
		if (skip_imm) continue;

		std::vector<std::string> fn_arity;
		std::transform(operand_list.begin(), operand_list.end(),
			std::back_inserter(fn_arity), [](const rv_operand_desc& operand) {
				return format_string("%s %s", operand.first->meta_type, operand.second.c_str());
		});
		std::vector<std::string> operand_fmt;
		std::transform(operand_list.begin(), operand_list.end(),
			std::back_inserter(operand_fmt), [](const rv_operand_desc& operand) {
				return format_string("%s", operand.first->c_fmt);
		});

		// generate test cases
		if (ext->isa_width == 64) printf("#if _RISCV_SZPTR != _RISCV_SZINT\n");
		switch (operand_list.size()) {
			case 2: {
				std::set<std::string> values1 = test_values(operand_list[1].first);
				int test_num = 0;
				for (auto &v1 : values1) {
					test_num++;
					std::string opcode_name = rv_meta_model::opcode_format("", opcode, "_");
					std::string test_name = format_string("test_%s_%d", opcode_name.c_str(), test_num);
					std::string inst = opcode_substitute(opcode);
					printf("void %s()\n{\n", test_name.c_str());
					printf("\t%s = %s;\n", fn_arity[1].c_str(), v1.c_str());
					printf("\t%s = %s(s1);\n", fn_arity[0].c_str(), opcode_name.c_str());
					printf("\tprintf(\"d=%s\\n\", d);\n", operand_fmt[0].c_str());
					printf("}\n\n");
					test_cases.push_back(std::pair<rv_extension_ptr,std::string>(ext, test_name));
				}
				break;
			}
			case 3: {
				std::set<std::string> values1 = test_values(operand_list[1].first);
				std::set<std::string> values2 = test_values(operand_list[2].first);
				int test_num = 0;
				for (auto &v1 : values1) {
					for (auto &v2 : values2) {
						test_num++;
						std::string opcode_name = rv_meta_model::opcode_format("", opcode, "_");
						std::string test_name = format_string("test_%s_%d", opcode_name.c_str(), test_num);
						std::string inst = opcode_substitute(opcode);
						printf("void %s()\n{\n", test_name.c_str());
						printf("\t%s = %s;\n", fn_arity[1].c_str(), v1.c_str());
						printf("\t%s = %s;\n", fn_arity[2].c_str(), v2.c_str());
						printf("\t%s = %s(s1, s2);\n", fn_arity[0].c_str(), opcode_name.c_str());
						printf("\tprintf(\"d=%s\\n\", d);\n", operand_fmt[0].c_str());
						printf("}\n\n");
						test_cases.push_back(std::pair<rv_extension_ptr,std::string>(ext, test_name));
					}
				}
				break;
			}
			case 4: {
				std::set<std::string> values1 = test_values(operand_list[1].first);
				std::set<std::string> values2 = test_values(operand_list[2].first);
				std::set<std::string> values3 = test_values(operand_list[3].first);
				int test_num = 0;
				for (auto &v1 : values1) {
					for (auto &v2 : values2) {
						for (auto &v3 : values3) {
							test_num++;
							std::string opcode_name = rv_meta_model::opcode_format("", opcode, "_");
							std::string test_name = format_string("test_%s_%d", opcode_name.c_str(), test_num);
							std::string inst = opcode_substitute(opcode);
							printf("void %s()\n{\n", test_name.c_str());
							printf("\t%s = %s;\n", fn_arity[1].c_str(), v1.c_str());
							printf("\t%s = %s;\n", fn_arity[2].c_str(), v2.c_str());
							printf("\t%s = %s;\n", fn_arity[3].c_str(), v3.c_str());
							printf("\t%s = %s(s1, s2, s3);\n", fn_arity[0].c_str(), opcode_name.c_str());
							printf("\tprintf(\"d=%s\\n\", d);\n", operand_fmt[0].c_str());
							printf("}\n\n");
							test_cases.push_back(std::pair<rv_extension_ptr,std::string>(ext, test_name));
						}
					}
				}
				break;
			}
		}
		if (ext->isa_width == 64) printf("#endif\n");
	}

	printf("int main()\n{\n");
	for (auto test : test_cases) {
		if (test.first->isa_width == 64) printf("#if _RISCV_SZPTR != _RISCV_SZINT\n");
		printf("\t%s();\n", test.second.c_str());
		if (test.first->isa_width == 64) printf("#endif\n");
	}
	printf("}\n");
}

void rv_gen_cc::generate()
{
	if (gen->has_option("print_fpu_asm_cc")) print_fpu_asm_cc(gen);
}
