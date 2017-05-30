//
//  gen-fpu-test.cc
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

std::vector<cmdline_option> rv_gen_fpu_test::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-FH", "--print-fpu-test-h", cmdline_arg_type_none,
			"Print FPU test header",
			[&](std::string s) { return gen->set_option("print_fpu_test_h"); } },
		{ "-FC", "--print-fpu-test-c", cmdline_arg_type_none,
			"Print FPU test source",
			[&](std::string s) { return gen->set_option("print_fpu_test_c"); } },
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

static void print_fpu_test_h(rv_gen *gen)
{
	static const char* kFpuHeader =

R"C(
typedef signed int         s32;
typedef unsigned int       u32;
typedef signed long long   s64;
typedef unsigned long long u64;
typedef float              f32;
typedef double             f64;
typedef signed long        sx;
typedef unsigned long      ux;

#define nan NAN
#define nanf NAN
#define inf INFINITY
#define inff INFINITY

#define FPU_IDENTITY(fn, args...) printf("\tFPU_ASSERT(%s, " fmt_res_##fn ", " fmt_arg_##fn ");\n", #fn, test_##fn(args), args)
#define FPU_ASSERT(fn, result, args...) assert(isinf(result) ? isinf(test_##fn(args)) : isnan(result) ? isnan(test_##fn(args)) : test_##fn(args) == result)
)C";

	printf(kCHeader, "test-fpu-gen.h");
	printf("#ifndef test_fpu_gen_h\n");
	printf("#define test_fpu_gen_h\n");
	printf("%s\n", kFpuHeader);

	// loop through FPU instructions
	for (auto &opcode : gen->opcodes) {

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
		std::transform(operand_list.begin() + 1, operand_list.end(),
			std::back_inserter(operand_fmt), [](const rv_operand_desc& operand) {
				return format_string("%s%s", operand.first->c_fmt, operand.first->c_suffix);
		});

		std::vector<std::string> asm_arity;
		std::transform(operand_list.begin(), operand_list.end(),
			std::back_inserter(asm_arity), [](const rv_operand_desc& operand) {
				return format_string("%%[%s]", operand.second.c_str());
		});

		std::vector<std::string> asm_results;
		std::transform(operand_list.begin() + 1, operand_list.end(),
			std::back_inserter(asm_results), [](const rv_operand_desc& operand) {
				return format_string("[%s]\"%s\" (%s)",
					operand.second.c_str(), operand.first->asm_type, operand.second.c_str());
		});

		// output function
		if (ext->isa_width == 64) printf("#if _RISCV_SZPTR != _RISCV_SZINT\n");
		std::string opcode_dot = rv_meta_model::opcode_format("", opcode, ".");
		std::string opcode_name = rv_meta_model::opcode_format("", opcode, "_");
		printf(
			"#define fmt_res_%s \"%s%s\"\n"
			"#define fmt_arg_%s \"%s\"\n"
			"inline %s test_%s(%s)\n{\n\t%s %s;\n"
			"\tasm(\"%s %s\\n\"\n\t\t: [%s]\"=%s\" (%s)\n"
			"\t\t: %s\n\t\t:\n\t);\n\treturn %s;\n}\n",
			opcode_name.c_str(),
			operand_list[0].first->c_fmt,
			operand_list[0].first->c_suffix,
			opcode_name.c_str(),
			join(operand_fmt, ", ").c_str(),
			operand_list[0].first->meta_type,
			opcode_name.c_str(),
			join(fn_arity, ", ").c_str(),
			operand_list[0].first->meta_type,
			operand_list[0].second.c_str(),
			opcode_dot.c_str(),
			join(asm_arity, ", ").c_str(),
			operand_list[0].second.c_str(),
			operand_list[0].first->asm_type,
			operand_list[0].second.c_str(),
			join(asm_results, ", ").c_str(),
			operand_list[0].second.c_str()
		);
		if (ext->isa_width == 64) printf("#endif\n");
		printf("\n");
	}

	printf("#endif\n");
}

static void print_fpu_test_c(rv_gen *gen)
{
	static const char* kFpuSourceHeader =

R"C(#include <stdio.h>
#include <assert.h>
#include <math.h>

#include "test-fpu-gen.h"

int main()
{
	printf("#include <stdio.h>\n");
	printf("#include <assert.h>\n");
	printf("#include <math.h>\n");
	printf("\n");
	printf("#include \"test-fpu-gen.h\"\n");
	printf("\n");
	printf("int main()\n");
	printf("{\n");

)C";

	static const char* kFpuSourceFooter =

R"C(
	printf("\treturn 0;\n");
	printf("}\n");

	return 0;
}
)C";

	printf(kCHeader, "test-fpu-gen.c");
	printf("%s", kFpuSourceHeader);

	// loop through FPU instructions
	for (auto &opcode : gen->opcodes) {

		// skip pseudo opcodes
		if (opcode->is_pseudo()) continue;

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

		// generate test cases
		if (ext->isa_width == 64) printf("#if _RISCV_SZPTR != _RISCV_SZINT\n");
		switch (operand_list.size()) {
			case 2: {
				std::set<std::string> values1 = test_values(operand_list[1].first);
				for (auto &v1 : values1) {
					printf("\tFPU_IDENTITY(%s, %s);\n",
						rv_meta_model::opcode_format("", opcode, "_").c_str(),
						v1.c_str());
				}
				break;
			}
			case 3: {
				std::set<std::string> values1 = test_values(operand_list[1].first);
				std::set<std::string> values2 = test_values(operand_list[2].first);
				for (auto &v1 : values1) {
					for (auto &v2 : values2) {
						printf("\tFPU_IDENTITY(%s, %s, %s);\n",
							rv_meta_model::opcode_format("", opcode, "_").c_str(),
							v1.c_str(), v2.c_str());
					}
				}
				break;
			}
			case 4: {
				std::set<std::string> values1 = test_values(operand_list[1].first);
				std::set<std::string> values2 = test_values(operand_list[2].first);
				std::set<std::string> values3 = test_values(operand_list[3].first);
				for (auto &v1 : values1) {
					for (auto &v2 : values2) {
						for (auto &v3 : values3) {
							printf("\tFPU_IDENTITY(%s, %s, %s, %s);\n",
								rv_meta_model::opcode_format("", opcode, "_").c_str(),
								v1.c_str(), v2.c_str(), v3.c_str());
						}
					}
				}
				break;
			}
		}
		if (ext->isa_width == 64) printf("#endif\n");
	}

	printf("%s", kFpuSourceFooter);
}

void rv_gen_fpu_test::generate()
{
	if (gen->has_option("print_fpu_test_h")) print_fpu_test_h(gen);
	if (gen->has_option("print_fpu_test_c")) print_fpu_test_c(gen);
}
