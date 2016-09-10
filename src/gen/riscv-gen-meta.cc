//
//  riscv-gen-meta.cc
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

std::vector<cmdline_option> riscv_gen_meta::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-H", "--print-meta-h", cmdline_arg_type_none,
			"Print metadata header",
			[&](std::string s) { return gen->set_option("print_meta_h"); } },
		{ "-C", "--print-meta-cc", cmdline_arg_type_none,
			"Print metadata source",
			[&](std::string s) { return gen->set_option("print_meta_cc"); } },
	};
}

static const char* unknown_op_comment = "/*              unknown */ ";

static void print_array_illegal_enum(const char *str, bool no_comment)
{
	printf("\t%s%s,\n", no_comment ? "" : unknown_op_comment, str);
}

static void print_array_illegal_uint64(uint64_t num, bool no_comment)
{
	printf("\t%s0x%016" PRIx64 ",\n", no_comment ? "" : unknown_op_comment, num);
}

static void print_meta_h(riscv_gen *gen)
{
	static const char* kMetaHeader =

R"C(#ifndef riscv_meta_h
#define riscv_meta_h

#ifdef __cplusplus
extern "C" {
#endif

)C";

	static const char* kMetaDeclarations =

R"C(/* Instruction compression data structure */
struct riscv_comp_data
{
	const int op;
	const rvc_constraint* constraints;
};

/* Instruction operand structure */
struct riscv_operand_data
{
	const riscv_operand_name operand_name;
	const riscv_operand_type operand_type;
	const riscv_type type;
	const unsigned int width;
};

/* Opcode metadata tables */
extern const riscv_codec riscv_inst_codec[];
extern const char* riscv_inst_format[];
extern const riscv_operand_data* riscv_inst_operand_data[];
extern const uint64_t riscv_inst_match[];
extern const uint64_t riscv_inst_mask[];
)C";

	static const char* kMetaFooter =

R"C(
#ifdef __cplusplus
}
#endif

#endif
)C";

	bool no_comment = gen->has_option("no_comment");

	printf(kCHeader, "riscv-meta.h");
	printf("%s", kMetaHeader);

	// Enums
	std::string last_group;
	for (auto &enumv : gen->enums) {
		if (last_group != enumv->group) {
			if (last_group.size() != 0) printf("};\n\n");
			printf("enum riscv_%s\n{\n", enumv->group.c_str());
		}
		printf("\triscv_%s_%s = %" PRId64 ",%s\n",
			enumv->group.c_str(), enumv->name.c_str(), enumv->value,
			no_comment || enumv->description.size() == 0 ? "" :
				format_string("\t/* %s */", enumv->description.c_str()).c_str());
		last_group = enumv->group;
	}
	if (last_group.size() != 0) printf("};\n\n");

	// Constraint enum
	printf("enum rvc_constraint\n{\n");
	printf("\trvc_end,\n");
	for (auto &constraint : gen->constraints) {
		printf("\trvc_%s,%s\n", constraint->name.c_str(),
			no_comment ? "" : format_string("\t/* %s */", constraint->expression.c_str()).c_str());
	}
	printf("};\n\n");

	// CSR enum
	printf("enum riscv_csr\n{\n");
	for (auto &csr : gen->csrs) {
		printf("\triscv_csr_%s = %s,%s\n", csr->name.c_str(), csr->number.c_str(),
			no_comment || csr->description.size() == 0 ? "" :
				format_string("\t/* %s */", csr->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Integer register number enum
	printf("enum riscv_ireg_num\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,%s\n", reg->name.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Integer register ABI name enum
	printf("enum riscv_ireg_name\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "ireg") continue;
		printf("\triscv_ireg_%s,%s\n", reg->alias.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Floating Point register number enum
	printf("enum riscv_freg_num\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,%s\n", reg->name.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Floating Point register ABI name enum
	printf("enum riscv_freg_name\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "freg") continue;
		printf("\triscv_freg_%s,%s\n", reg->alias.c_str(),
			no_comment || reg->description.size() == 0 ? "" :
				format_string("\t/* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Instruction codec enum
	printf("enum riscv_codec\n{\n");
	printf("\triscv_codec_illegal,\n");
	for (auto &codec : gen->get_unique_codecs()) {
		printf("\triscv_codec_%s,\n", codec.c_str());
	}
	printf("};\n\n");

	// Instruction operand name enum
	printf("enum riscv_operand_name\n{\n");
	printf("\triscv_operand_name_none,\n");
	for (auto &operand : gen->operands) {
		printf("\triscv_operand_name_%s,\n", operand->name.c_str());
	}
	printf("};\n\n");

	// instruction operand type enum
	std::vector<std::string> operand_types;
	for (auto &operand : gen->operands) {
		auto type_name = riscv_meta_model::format_type(operand);
		if (std::find(operand_types.begin(), operand_types.end(), type_name) == operand_types.end()) {
			operand_types.push_back(type_name);
		}
	}
	printf("enum riscv_operand_type\n{\n");
	printf("\triscv_operand_type_none,\n");
	for (auto &operand_type : operand_types) {
		printf("\triscv_operand_type_%s,\n", operand_type.c_str());
	}
	printf("};\n\n");

	// Instruction opcode enum
	printf("enum riscv_op\n{\n");
	printf("\triscv_op_illegal = 0,\n");
	for (auto &opcode : gen->opcodes) {
		printf("\t%s = %lu,%s\n",
			riscv_meta_model::opcode_format("riscv_op_", opcode, "_").c_str(),
			opcode->num, no_comment || opcode->long_name.size() == 0 ? "" :
				format_string("\t/* %s */", opcode->long_name.c_str()).c_str());
	}
	printf("};\n\n");

	// Array declarations
	printf("%s", kMetaDeclarations);
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("extern const riscv_comp_data* riscv_inst_comp_%s[];\n",
			isa_width.second.c_str());
	}
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("extern const int riscv_inst_decomp_%s[];\n",
			isa_width.second.c_str());
	}

	printf("%s", kMetaFooter);
}

static void print_meta_cc(riscv_gen *gen)
{
	static const char* kMetaSource =

R"C(#include "riscv-types.h"
#include "riscv-format.h"
#include "riscv-meta.h"

)C";

	bool no_comment = gen->has_option("no_comment");
	bool zero_not_oh = gen->has_option("zero_not_oh");

	printf(kCHeader, "riscv-meta.cc");
	printf("%s", kMetaSource);

	// Compression constraints
	for (auto &opcode : gen->opcodes) {
		if (!opcode->compressed) continue;
		printf("const rvc_constraint %s[] = {\n",
			riscv_meta_model::opcode_format("rvcc_", opcode, "_").c_str());
		for (auto &constraint : opcode->compressed->constraint_list) {
			printf("\trvc_%s,\n", constraint->name.c_str());
		}
		printf("\trvc_end\n};\n\n");
	}
	printf("\n");

	// Compression data
	std::set<std::string> rvcd_set;
	for (auto isa_width : gen->isa_width_prefixes()) {
		std::string isa_prefix = "rvcd_" + isa_width.second + "_";
		for (auto &opcode : gen->opcodes) {
			riscv_compressed_list include_compressions;
			for (auto comp : opcode->compressions) {
				bool include_isa = opcode->include_isa(isa_width.first) &&
					comp->comp_opcode->include_isa(isa_width.first);
				if (include_isa) include_compressions.push_back(comp);
			}
			if (include_compressions.size() == 0) continue;
			std::string rvcd_name = riscv_meta_model::opcode_format(isa_prefix, opcode, "_");
			rvcd_set.insert(rvcd_name);
			printf("const riscv_comp_data %s[] = {\n", rvcd_name.c_str());
			for (auto &comp : include_compressions) {
				printf("\t{ %s, %s },\n",
					(zero_not_oh ?
						format_string("%lu", comp->comp_opcode->num).c_str() :
						riscv_meta_model::opcode_format("riscv_op_", comp->comp_opcode, "_").c_str()),
						riscv_meta_model::opcode_format("rvcc_", comp->comp_opcode, "_").c_str());
			}
			printf("\t{ %s, nullptr }\n};\n\n",
				(zero_not_oh ?
						"0" :
						"riscv_op_illegal"));
		}
		printf("\n");
	}

	// Codec operand data
	for (auto &codec : gen->codecs) {
		printf("const riscv_operand_data riscv_codec_%s_operands[] = {\n",
			riscv_meta_model::format_codec("", codec, "_", false).c_str());
		for (auto &operand : codec->operands) {
			size_t width = operand->bitspec.decoded_msb() > 0 ?
				operand->bitspec.decoded_msb() + 1 :
				operand->bitspec.segments.front().first.msb - operand->bitspec.segments.back().first.lsb + 1;
			printf("\t{ riscv_operand_name_%s, riscv_operand_type_%s, riscv_type_%s, %lu },\n",
				operand->name.c_str(),
				riscv_meta_model::format_type(operand).c_str(),
				operand->type.c_str(), width);
		}
		printf("\t{ riscv_operand_name_none, riscv_operand_type_none, riscv_type_none, 0 }\n};\n\n");
	}

	// Instruction codecs
	printf("const riscv_codec riscv_inst_codec[] = {\n");
	print_array_illegal_enum("riscv_codec_illegal", no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s%s,\n",
			riscv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			riscv_meta_model::format_codec("riscv_codec_", opcode->codec, "_").c_str());
	}
	printf("};\n\n");

	// Instruction formats
	printf("const char* riscv_inst_format[] = {\n");
	print_array_illegal_enum("riscv_fmt_none", no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s%s,\n",
			riscv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			riscv_meta_model::format_format("riscv_fmt_", opcode->format, '_').c_str());
	}
	printf("};\n\n");

	// Instruction codecs operand data table
	printf("const riscv_operand_data* riscv_inst_operand_data[] = {\n");
	printf("\triscv_codec_none_operands,\n");
	for (auto &opcode : gen->opcodes) {
		printf("\t%sriscv_codec_%s_operands,\n",
			riscv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			riscv_meta_model::format_codec("", opcode->codec, "_", false).c_str());
	}
	printf("};\n\n");

	// Instruction match bits
	printf("const uint64_t riscv_inst_match[] = {\n");
	print_array_illegal_uint64(0ULL, no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s0x%016" PRIx64 ",\n",
			riscv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			opcode->match);
	}
	printf("};\n\n");

	// Instruction mask bits
	printf("const uint64_t riscv_inst_mask[] = {\n");
	print_array_illegal_uint64(0ULL, no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s0x%016" PRIx64 ",\n",
			riscv_meta_model::opcode_comment(opcode, no_comment).c_str(), opcode->mask);
	}
	printf("};\n\n");

	// RVC compression table (per isa width)
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("const riscv_comp_data* riscv_inst_comp_%s[] = {\n", isa_width.second.c_str());
		print_array_illegal_enum("nullptr", no_comment);
		std::string isa_prefix = "rvcd_" + isa_width.second + "_";
		for (auto &opcode : gen->opcodes) {
			std::string opcode_key = riscv_meta_model::opcode_format("", opcode, ".");
			std::string rvcd_name = riscv_meta_model::opcode_format(isa_prefix, opcode, "_");
			bool include_isa = rvcd_set.find(rvcd_name) != rvcd_set.end();
			printf("\t%s%s,\n",
				riscv_meta_model::opcode_comment(opcode, no_comment).c_str(),
				include_isa && opcode->compressions.size() > 0 ? rvcd_name.c_str() : "nullptr");
		}
		printf("};\n\n");
	}

	// RVC decompression table
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("const int riscv_inst_decomp_%s[] = {\n", isa_width.second.c_str());
		if (zero_not_oh) print_array_illegal_enum("0", no_comment);
		else print_array_illegal_enum("riscv_op_illegal", no_comment);
		for (auto &opcode : gen->opcodes) {
			bool include_isa = opcode->include_isa(isa_width.first);
			std::string opcode_key = riscv_meta_model::opcode_format("", opcode, ".");
			printf("\t%s%s,\n",
				riscv_meta_model::opcode_comment(opcode, no_comment).c_str(),
				include_isa && opcode->compressed ?
					(zero_not_oh ?
						format_string("%lu", opcode->compressed->decomp_opcode->num).c_str() :
						riscv_meta_model::opcode_format("riscv_op_", opcode->compressed->decomp_opcode, "_").c_str()) :
					(zero_not_oh ?
						"0" :
						"riscv_op_illegal"));
		}
		printf("};\n\n");
	}
}

void riscv_gen_meta::generate()
{
	if (gen->has_option("print_meta_h")) print_meta_h(gen);
	if (gen->has_option("print_meta_cc")) print_meta_cc(gen);
}
