//
//  gen-meta.cc
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

std::vector<cmdline_option> rv_gen_meta::get_cmdline_options()
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

static void print_meta_h(rv_gen *gen)
{
	static const char* kMetaHeader =

R"C(#ifndef rv_meta_h
#define rv_meta_h

#ifdef __cplusplus
extern "C" {
#endif

)C";

	static const char* kMetaDeclarations =

R"C(/* Primitive data structure */

struct rv_primitive_data
{
	const char* name;
	const char* format;
	const char* hex_format;
};

/* Instruction compression data structure */
struct rv_comp_data
{
	const int op;
	const rvc_constraint* constraints;
};

/* Instruction operand structure */
struct rv_operand_data
{
	rv_operand_name operand_name;
	rv_operand_type operand_type;
	rv_primitive primitive;
	rv_type type;
	unsigned int width;
};

/* Opcode metadata tables */
extern const rv_primitive_data rv_type_primitives[];
extern const rv_codec rv_inst_codec[];
extern const char* rv_inst_format[];
extern const rv_operand_data* rv_inst_operand_data[];
extern const riscv::inst_t rv_inst_match[];
extern const riscv::inst_t rv_inst_mask[];
extern const rv_comp_data* rv_inst_pseudo[];
extern const rv_comp_data rv_inst_depseudo[];
)C";

	static const char* kMetaFooter =

R"C(
#ifdef __cplusplus
}
#endif

#endif
)C";

	bool no_comment = gen->has_option("no_comment");

	printf(kCHeader, "meta.h");
	printf("%s", kMetaHeader);

	// Primitives
	printf("enum rv_primitive\n{\n");
	printf("\trv_primitive_none,\n");
	for (const auto *ent = rv_primitive_type_table; ent->enum_type != rvt_none; ent++) {
		printf("\t%-35s%s\n",
			format_string("rv_primitive_%s,", ent->meta_type).c_str(),
			no_comment ? "" :
			format_string(" /* %s */", ent->c_type).c_str());
	}
	printf("};\n\n");

	// Enums
	std::string last_group;
	for (auto &enumv : gen->enums) {
		if (last_group != enumv->group) {
			if (last_group.size() != 0) printf("};\n\n");
			printf("enum rv_%s\n{\n", enumv->group.c_str());
		}
		printf("\t%-35s%s\n",
			format_string("rv_%s_%s = %" PRId64 ",",
			enumv->group.c_str(), enumv->name.c_str(), enumv->value).c_str(),
			no_comment || enumv->description.size() == 0 ? "" :
			format_string(" /* %s */", enumv->description.c_str()).c_str());
		last_group = enumv->group;
	}
	if (last_group.size() != 0) printf("};\n\n");

	// Constraint enum
	printf("enum rvc_constraint\n{\n");
	printf("\trvc_end,\n");
	for (auto &constraint : gen->constraints) {
		printf("\t%-35s%s\n",
			format_string("rvc_%s,", constraint->name.c_str()).c_str(),
			no_comment ? "" :
			format_string(" /* %s */", constraint->expression.c_str()).c_str());
	}
	printf("};\n\n");

	// CSR enum
	printf("enum rv_csr\n{\n");
	for (auto &csr : gen->csrs) {
		printf("\t%-35s%s\n",
			format_string("rv_csr_%s = %s,", csr->name.c_str(),
			csr->number.c_str()).c_str(),
			no_comment || csr->description.size() == 0 ? "" :
			format_string(" /* %s */", csr->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Integer register number enum
	printf("enum rv_ireg_num\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "ireg") continue;
		printf("\t%-35s%s\n",
			format_string("rv_ireg_%s,", reg->name.c_str()).c_str(),
			no_comment || reg->description.size() == 0 ? "" :
			format_string(" /* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Integer register ABI name enum
	printf("enum rv_ireg_name\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "ireg") continue;
		printf("\t%-35s%s\n",
			format_string("rv_ireg_%s,", reg->alias.c_str()).c_str(),
			no_comment || reg->description.size() == 0 ? "" :
			format_string(" /* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Floating Point register number enum
	printf("enum rv_freg_num\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "freg") continue;
		printf("\t%-35s%s\n",
			format_string("rv_freg_%s,", reg->name.c_str()).c_str(),
			no_comment || reg->description.size() == 0 ? "" :
			format_string(" /* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Floating Point register ABI name enum
	printf("enum rv_freg_name\n{\n");
	for (auto &reg : gen->registers) {
		if (reg->type != "freg") continue;
		printf("\t%-35s%s\n",
			format_string("rv_freg_%s,", reg->alias.c_str()).c_str(),
			no_comment || reg->description.size() == 0 ? "" :
			format_string(" /* %s */", reg->description.c_str()).c_str());
	}
	printf("};\n\n");

	// Instruction codec enum
	printf("enum rv_codec\n{\n");
	printf("\trv_codec_illegal,\n");
	for (auto &codec : gen->get_unique_codecs()) {
		printf("\trv_codec_%s,\n", codec.c_str());
	}
	printf("};\n\n");

	// Instruction operand name enum
	printf("enum rv_operand_name\n{\n");
	printf("\trv_operand_name_none,\n");
	for (auto &operand : gen->operands) {
		printf("\trv_operand_name_%s,\n", operand->name.c_str());
	}
	printf("};\n\n");

	// instruction operand type enum
	std::vector<std::string> operand_types;
	for (auto &operand : gen->operands) {
		auto type_name = rv_meta_model::format_type(operand);
		if (std::find(operand_types.begin(), operand_types.end(), type_name) == operand_types.end()) {
			operand_types.push_back(type_name);
		}
	}
	printf("enum rv_operand_type\n{\n");
	printf("\trv_operand_type_none,\n");
	for (auto &operand_type : operand_types) {
		printf("\trv_operand_type_%s,\n", operand_type.c_str());
	}
	printf("};\n\n");

	// Instruction opcode enum
	printf("enum rv_op\n{\n");
	printf("\trv_op_illegal = 0,\n");
	for (auto &opcode : gen->opcodes) {
		printf("\t%-35s%s\n",
			format_string("%s = %lu,",
			rv_meta_model::opcode_format("rv_op_", opcode, "_").c_str(),
			opcode->num).c_str(),
			no_comment || opcode->fullname.size() == 0 ? "" :
			format_string("\t/* %s */", opcode->fullname.c_str()).c_str());
	}
	printf("};\n\n");

	// Array declarations
	printf("%s", kMetaDeclarations);
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("extern const rv_comp_data* rv_inst_comp_%s[];\n",
			isa_width.second.c_str());
	}
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("extern const int rv_inst_decomp_%s[];\n",
			isa_width.second.c_str());
	}

	printf("%s", kMetaFooter);
}

static void print_meta_cc(rv_gen *gen)
{
	static const char* kMetaSource =

R"C(#include "types.h"
#include "format.h"
#include "meta.h"

)C";

	bool no_comment = gen->has_option("no_comment");
	bool zero_not_oh = gen->has_option("zero_not_oh");

	printf(kCHeader, "meta.cc");
	printf("%s", kMetaSource);

	// Primitives
	printf("const rv_primitive_data rv_type_primitives[] = {\n");
	printf("\t{ \"none\", \"none\" },\n");
	for (const auto *ent = rv_primitive_type_table; ent->enum_type != rvt_none; ent++) {
		printf("\t{ \"%s\", \"%s\" , \"%s\" },\n", ent->meta_type,
			ent->c_fmt ? ent->c_fmt : "none",
			ent->c_hex_fmt ? ent->c_hex_fmt : "none");
	}
	printf("};\n\n");

	// RVC Compression constraints
	for (auto &opcode : gen->all_opcodes) {
		if (!opcode->compressed) continue;
		printf("const rvc_constraint %s[] = {\n",
			rv_meta_model::opcode_format("rvcc_", opcode, "_", true).c_str());
		for (auto &constraint : opcode->compressed->constraint_list) {
			printf("\trvc_%s,\n", constraint->name.c_str());
		}
		printf("\trvc_end\n};\n\n");
	}
	printf("\n");

	// RVC Compression opcode constraint data (per isa width)
	std::set<std::string> rvcd_set;
	for (auto isa_width : gen->isa_width_prefixes()) {
		std::string isa_prefix = "rvcd_" + isa_width.second + "_";
		for (auto &opcode : gen->all_opcodes) {
			rv_compressed_list include_compressions;
			for (auto comp : opcode->compressions) {
				bool include_isa = opcode->include_isa(isa_width.first) &&
					comp->comp_opcode->include_isa(isa_width.first);
				if (include_isa) include_compressions.push_back(comp);
			}
			if (include_compressions.size() == 0) continue;
			std::string rvcd_name = rv_meta_model::opcode_format(isa_prefix, opcode, "_");
			rvcd_set.insert(rvcd_name);
			printf("const rv_comp_data %s[] = {\n", rvcd_name.c_str());
			for (auto &comp : include_compressions) {
				printf("\t{ %s, %s },\n",
					(zero_not_oh ?
						format_string("%lu", comp->comp_opcode->num).c_str() :
						rv_meta_model::opcode_format("rv_op_", comp->comp_opcode, "_").c_str()),
						rv_meta_model::opcode_format("rvcc_", comp->comp_opcode, "_", true).c_str());
			}
			printf("\t{ %s, nullptr }\n};\n\n",
				(zero_not_oh ?
						"0" :
						"rv_op_illegal"));
		}
		printf("\n");
	}

	/*
		Find distinct set of operand type combinations

		Note: Typed operands is broader than the set of codecs as codecs do not
		contain operand type information. For example the 3 fcvt codecs:

			r·m+rf, r·m+fr, r·m+ff

		Have 18 distinct operand type combinations:

			fcvt.w.s   rd:s32  frs1:f32  r·m+rf
			fcvt.wu.s  rd:u32  frs1:f32  r·m+rf
			fcvt.s.w   frd:f32 rs1:s32   r·m+fr
			fcvt.s.wu  frd:f32 rs1:u32   r·m+fr
			fcvt.l.s   rd:s64  frs1:f32  r·m+rf
			fcvt.lu.s  rd:u64  frs1:f32  r·m+rf
			fcvt.s.l   frd:f32 rs1:s36   r·m+fr
			fcvt.s.lu  frd:f32 rs1:u64   r·m+fr
			fcvt.s.d   frd:f32 frs1:f32  r·m+ff
			fcvt.d.s   frd:f64 frs1:f32  r·m+ff
			fcvt.w.d   rd:s32  frs1:f64  r·m+rf
			fcvt.wu.d  rd:u32  frs1:f64  r·m+rf
			fcvt.d.w   frd:f64 rs1:s32   r·m+fr
			fcvt.d.wu  frd:f64 rs1:u32   r·m+fr
			fcvt.l.d   rd:s64  frs1:f64  r·m+rf
			fcvt.lu.d  rd:u64  frs1:f64  r·m+rf
			fcvt.d.l   frd:f64 rs1:s64   r·m+fr
			fcvt.d.lu  frd:f64 rs1:u64   r·m+fr
	*/

	std::map<rv_opcode_ptr,std::string> opcode_operand_map;
	std::map<std::string,std::vector<std::pair<const rv_primitive_type*,rv_operand_ptr>>> operand_data_map;
	for (auto &opcode : gen->opcodes) {

		// find extension with minimum isa width
		auto ext_min_width_i = std::min_element(opcode->extensions.begin(), opcode->extensions.end(),
			[](rv_extension_ptr &a, rv_extension_ptr &b){ return a->isa_width < b->isa_width; });
		if (ext_min_width_i == opcode->extensions.end()) ext_min_width_i = gen->extensions.begin();
		auto ext = *ext_min_width_i;

		// create set of primitives for each operand of the opcode
		std::vector<std::pair<const rv_primitive_type*,rv_operand_ptr>> operand_list;
		std::vector<std::string> operand_key_list;
		// NOTE: we use the opcode operands, not the codec operands
		for (size_t i = 0; i < opcode->operands.size(); i++) {
			auto operand = opcode->operands[i];
			// we infer primitive type for register operands
			const rv_primitive_type *primitive = nullptr;
			std::string operand_key;
			if (operand->type == "ireg" || operand->type == "freg") {
				primitive = rv_meta_model::infer_operand_primitive(opcode, ext, operand, i);
				operand_key = std::string(primitive->meta_type) + "_" + operand->name;
			} else {
				operand_key = "T_" + operand->name;
			}
			operand_list.push_back(std::pair<const rv_primitive_type*,rv_operand_ptr>(primitive, operand));
			operand_key_list.push_back(operand_key);
		}
		std::string operand_list_key = join(operand_key_list, "_");
		if (operand_list_key.size() == 0) operand_list_key = "none";
		operand_data_map[operand_list_key] = operand_list;
		opcode_operand_map[opcode] = operand_list_key;
	}

	// Output operand data
	for (auto &ent : operand_data_map) {
		auto &operand_list_key = ent.first;
		auto &operand_list = ent.second;
		printf("const rv_operand_data rv_operands_%s[] = {\n",
			operand_list_key.c_str());
		for (auto &operand_list_ent : operand_list) {
			auto &primitive = operand_list_ent.first;
			auto &operand = operand_list_ent.second;
			size_t width = operand->bitspec.decoded_msb() > 0 ?
				operand->bitspec.decoded_msb() + 1 :
				operand->bitspec.segments.front().first.msb - operand->bitspec.segments.back().first.lsb + 1;
			printf("\t{ rv_operand_name_%s, rv_operand_type_%s, rv_primitive_%s, rv_type_%s, %lu },\n",
				operand->name.c_str(),
				rv_meta_model::format_type(operand).c_str(),
				primitive ? primitive->meta_type : "none",
				operand->type.c_str(), width);
		}
		printf("\t{ rv_operand_name_none, rv_operand_type_none, rv_primitive_none, rv_type_none, 0 }\n};\n\n");
	}

	// Instruction codecs
	printf("const rv_codec rv_inst_codec[] = {\n");
	print_array_illegal_enum("rv_codec_illegal", no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s%s,\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			rv_meta_model::format_codec("rv_codec_", opcode->codec, "_").c_str());
	}
	printf("};\n\n");

	// Instruction formats
	printf("const char* rv_inst_format[] = {\n");
	print_array_illegal_enum("rv_fmt_none", no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s%s,\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			rv_meta_model::format_format("rv_fmt_", opcode->format, '_').c_str());
	}
	printf("};\n\n");

	// Instruction codecs operand data table
	printf("const rv_operand_data* rv_inst_operand_data[] = {\n");
	print_array_illegal_enum("rv_operands_none", no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%srv_operands_%s,\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			opcode_operand_map[opcode].c_str());
	}
	printf("};\n\n");

	// Instruction match bits
	printf("const riscv::inst_t rv_inst_match[] = {\n");
	print_array_illegal_uint64(0ULL, no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s0x%016llx,\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			opcode->match);
	}
	printf("};\n\n");

	// Instruction mask bits
	printf("const riscv::inst_t rv_inst_mask[] = {\n");
	print_array_illegal_uint64(0ULL, no_comment);
	for (auto &opcode : gen->opcodes) {
		printf("\t%s0x%016llx,\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			opcode->mask);
	}
	printf("};\n\n");

	// Pseudoinstruction constraints
	for (auto &opcode : gen->opcodes) {
		if (!opcode->pseudo) continue;
		printf("const rvc_constraint %s[] = {\n",
			rv_meta_model::opcode_format("rvcc_", opcode, "_", true).c_str());
		for (auto &constraint : opcode->pseudo->constraint_list) {
			printf("\trvc_%s,\n", constraint->name.c_str());
		}
		printf("\trvc_end\n};\n\n");
	}
	printf("\n");

	// Pseudoinstruction opcode constraint data
	std::set<std::string> rvcp_set;
	for (auto &opcode : gen->opcodes) {
		if (opcode->pseudos.size() == 0) continue;
		std::string rvcp_name = rv_meta_model::opcode_format("rvcp_", opcode, "_");
		rvcp_set.insert(rvcp_name);
		printf("const rv_comp_data %s[] = {\n", rvcp_name.c_str());
		for (auto &pseudo : opcode->pseudos) {
			printf("\t{ %s, %s },\n",
				(zero_not_oh ?
					format_string("%lu", pseudo->pseudo_opcode->num).c_str() :
					rv_meta_model::opcode_format("rv_op_", pseudo->pseudo_opcode, "_").c_str()),
					rv_meta_model::opcode_format("rvcc_", pseudo->pseudo_opcode, "_", true).c_str());
		}
		printf("\t{ %s, nullptr }\n};\n\n",
			(zero_not_oh ?
					"0" :
					"rv_op_illegal"));
	}
	printf("\n");

	// Pseudoinstruction table
	printf("const rv_comp_data* rv_inst_pseudo[] = {\n");
	print_array_illegal_enum("nullptr", no_comment);
	for (auto &opcode : gen->opcodes) {
		std::string opcode_key = rv_meta_model::opcode_format("", opcode, ".");
		std::string rvcp_name = rv_meta_model::opcode_format("rvcp_", opcode, "_");
		bool include_isa = rvcp_set.find(rvcp_name) != rvcp_set.end();
		printf("\t%s%s,\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			include_isa && opcode->pseudos.size() > 0 ? rvcp_name.c_str() : "nullptr");
	}
	printf("};\n\n");

	// Depseudoinstruction table (per isa width)
	printf("const rv_comp_data rv_inst_depseudo[] = {\n");
	printf("\t%s{ %s, nullptr },\n",
		no_comment ? "" : unknown_op_comment,
		zero_not_oh ? "0" : "rv_op_illegal");
	for (auto &opcode : gen->opcodes) {
		std::string opcode_key = rv_meta_model::opcode_format("", opcode, ".");
		printf("\t%s{ %s, %s },\n",
			rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
			opcode->pseudo ?
				(zero_not_oh ?
					format_string("%lu", opcode->pseudo->real_opcode->num).c_str() :
					rv_meta_model::opcode_format("rv_op_", opcode->pseudo->real_opcode, "_").c_str()) :
				(zero_not_oh ?
					"0" :
					"rv_op_illegal"),
			opcode->pseudo ?
				rv_meta_model::opcode_format("rvcc_", opcode, "_", true).c_str() :
				"nullptr");
	}
	printf("};\n\n");

	// RVC compression table (per isa width)
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("const rv_comp_data* rv_inst_comp_%s[] = {\n", isa_width.second.c_str());
		print_array_illegal_enum("nullptr", no_comment);
		std::string isa_prefix = "rvcd_" + isa_width.second + "_";
		for (auto opcode : gen->opcode_list_by_width(isa_width.first)) {
			std::string opcode_key = rv_meta_model::opcode_format("", opcode, ".");
			std::string rvcd_name = rv_meta_model::opcode_format(isa_prefix, opcode, "_");
			bool include_isa = rvcd_set.find(rvcd_name) != rvcd_set.end();
			printf("\t%s%s,\n",
				rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
				include_isa && opcode->compressions.size() > 0 ? rvcd_name.c_str() : "nullptr");
		}
		printf("};\n\n");
	}

	// RVC decompression table (per isa width)
	for (auto isa_width : gen->isa_width_prefixes()) {
		printf("const int rv_inst_decomp_%s[] = {\n", isa_width.second.c_str());
		if (zero_not_oh) print_array_illegal_enum("0", no_comment);
		else print_array_illegal_enum("rv_op_illegal", no_comment);
		for (auto opcode : gen->opcode_list_by_width(isa_width.first)) {
			bool include_isa = opcode->include_isa(isa_width.first);
			std::string opcode_key = rv_meta_model::opcode_format("", opcode, ".");
			printf("\t%s%s,\n",
				rv_meta_model::opcode_comment(opcode, no_comment).c_str(),
				include_isa && opcode->compressed ?
					(zero_not_oh ?
						format_string("%lu", opcode->compressed->decomp_opcode->num).c_str() :
						rv_meta_model::opcode_format("rv_op_", opcode->compressed->decomp_opcode, "_").c_str()) :
					(zero_not_oh ?
						"0" :
						"rv_op_illegal"));
		}
		printf("};\n\n");
	}
}

void rv_gen_meta::generate()
{
	if (gen->has_option("print_meta_h")) print_meta_h(gen);
	if (gen->has_option("print_meta_cc")) print_meta_cc(gen);
}
