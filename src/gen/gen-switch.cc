//
//  gen-switch.cc
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

std::vector<cmdline_option> rv_gen_switch::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-S", "--print-switch-h", cmdline_arg_type_none,
			"Print switch header",
			[&](std::string s) { return gen->set_option("print_switch_h"); } },
	};
}

static void print_switch_decoder_node(rv_gen *gen, rv_codec_node &node, size_t indent)
{
	for (size_t i = 0; i < indent; i++) printf("\t");
	printf("switch (%s) {\n", rv_meta_model::format_bitmask(node.bits, "inst", true).c_str());
	for (auto &val : node.vals) {
		auto opcode_list = node.val_opcodes[val];
		if (node.val_decodes[val].bits.size() == 0 && opcode_list.size() >= 1) {
			for (size_t i = 0; i < indent; i++) printf("\t");
			if (val == rv_meta_model::DEFAULT) {
				printf("\tdefault: ");
			} else {
				printf("\tcase %lu: ", val);
			}

			// resolve distinct number of isa widths for this opcode
			std::vector<size_t> opcode_widths;
			for (auto opcode : opcode_list) {
				for (auto &ext : opcode->extensions) {
					if (std::find(opcode_widths.begin(), opcode_widths.end(),
							ext->isa_width) == opcode_widths.end()) {
						opcode_widths.push_back(ext->isa_width);
					}
				}
			}
			auto opcode = opcode_list.front();

			// different opcodes that share encoding on different isa widths
			if (opcode_list.size() > 1 && opcode_list.size() == opcode_widths.size())
			{
				// conditionals for different opcodes sharing encoding on different isa widths
				printf("\n");
				for (auto oi = opcode_list.begin(); oi != opcode_list.end(); oi++) {
					auto opcode = *oi;
					for (size_t i = 0; i < indent; i++) printf("\t");
					printf("\t\t%sif (%s && rv%lu) op = %s;\n",
						oi != opcode_list.begin() ? "else " : "",
						rv_meta_model::opcode_isa_shortname(opcode).c_str(),
						opcode->extensions.front()->isa_width,
						rv_meta_model::opcode_format("rv_op_", opcode, "_").c_str());
				}
				for (size_t i = 0; i < indent; i++) printf("\t");
				printf("\t\tbreak;\n");
			}
			else
			{
				// if ambiguous, chooses first opcode
				if (opcode_widths.size() == 1) {
					printf("if (%s && rv%lu) op = %s; break;",
						rv_meta_model::opcode_isa_shortname(opcode).c_str(),
						opcode->extensions.front()->isa_width,
						rv_meta_model::opcode_format("rv_op_", opcode, "_").c_str());
				} else {
					printf("if (%s) op = %s; break;",
						rv_meta_model::opcode_isa_shortname(opcode).c_str(),
						rv_meta_model::opcode_format("rv_op_", opcode, "_").c_str());
				}

				// if ambiguous, add comment
				if (opcode_list.size() > 1) {
					printf(" //");
					for (auto &opcode : opcode_list) {
						printf(" %s", opcode->name.c_str());
					}
				}
				printf("\n");
			}

		} else {
			for (size_t i = 0; i < indent; i++) printf("\t");
			if (val == rv_meta_model::DEFAULT) {
				printf("\tdefault: ");
			} else {
				printf("\tcase %lu:\n", val);
			}
			for (size_t i = 0; i < indent; i++) printf("\t");
			printf("\t\t//");
			int count = 0;
			for (auto &opcode : opcode_list) {
				if (count++ == 12) {
					printf(" ..."); break;
				}
					printf(" %s", opcode->name.c_str());
			}
			printf("\n");
			if (node.val_decodes[val].bits.size() > 0) {
				print_switch_decoder_node(gen, node.val_decodes[val], indent + 2);
			}
			for (size_t i = 0; i < indent; i++) printf("\t");
			printf("\t\tbreak;\n");
		}
	}
	for (size_t i = 0; i < indent; i++) printf("\t");
	printf("}\n");
}

static void print_switch_h(rv_gen *gen)
{
	printf(kCHeader, "switch.h");
	printf("#ifndef rv_switch_h\n");
	printf("#define rv_switch_h\n");
	printf("\n");

	// print opcode decoder
	std::vector<std::string> mnems = gen->get_inst_mnemonics(true, true);
	printf("/* Decode Instruction Opcode */\n\n");
	printf("template <");
	for (auto mi = mnems.begin(); mi != mnems.end(); mi++) {
		if (mi != mnems.begin()) printf(", ");
		printf("bool %s", mi->c_str());
	}
	printf(">\n");
	printf("inline opcode_t decode_inst_op(riscv::inst_t inst)\n");
	printf("{\n");
	printf("\topcode_t op = rv_op_illegal;\n");
	print_switch_decoder_node(gen, gen->root_node, 1);
	printf("\treturn op;\n");
	printf("}\n\n");

	// print type decoder
	printf("/* Decode Instruction Type */\n\n");
	printf("template <typename T>\n");
	printf("inline void decode_inst_type(T &dec, riscv::inst_t inst)\n");
	printf("{\n");
	printf("\tdec.codec = rv_inst_codec[dec.op];\n");
	printf("\tswitch (dec.codec) {\n");
	for (auto &codec : gen->get_unique_codecs()) {
		printf("\t\tcase %-26s %-50s break;\n",
			format_string("rv_codec_%s:", codec.c_str()).c_str(),
			format_string("riscv::decode_%s(dec, inst);", codec.c_str()).c_str());
	}
	printf("\t};\n");
	printf("}\n\n");

	// print encoder
	printf("/* Encode Instruction */\n\n");
	printf("template <typename T>\n");
	printf("inline riscv::inst_t encode_inst(T &dec)\n");
	printf("{\n");
	printf("\tdec.codec = rv_inst_codec[dec.op];\n");
	printf("\triscv::inst_t inst = rv_inst_match[dec.op];\n");
	printf("\tswitch (dec.codec) {\n");
	for (auto &codec : gen->get_unique_codecs()) {
		printf("\t\tcase %-26s %-50s break;\n",
			format_string("rv_codec_%s:", codec.c_str()).c_str(),
			format_string("return inst |= riscv::encode_%s(dec);", codec.c_str()).c_str());
	}
	printf("\t};\n");
	printf("\treturn inst;\n");
	printf("}\n");
	printf("\n");
	printf("#endif\n");
}

void rv_gen_switch::generate()
{
	if (gen->has_option("print_switch_h")) {
		gen->generate_codec();
		print_switch_h(gen);
	}
}
