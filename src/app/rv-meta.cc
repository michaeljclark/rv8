//
//  rv-meta.cc
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

const ssize_t kMaxInstructionWidth = 32;

const char* kCHeader =
R"C(//
//  %s
//
//  DANGER - This is machine generated code
//

)C";

void rv_codec_node::clear()
{
	bits.clear();
	vals.clear();
	val_opcodes.clear();
	val_decodes.clear();
}

rv_gen::rv_gen()
{
	generators.push_back(std::make_shared<rv_gen_cc>(this));
	generators.push_back(std::make_shared<rv_gen_constraints>(this));
	generators.push_back(std::make_shared<rv_gen_fpu_test>(this));
	generators.push_back(std::make_shared<rv_gen_interp>(this));
	generators.push_back(std::make_shared<rv_gen_jit>(this));
	generators.push_back(std::make_shared<rv_gen_latex>(this));
	generators.push_back(std::make_shared<rv_gen_latex_alt>(this));
	generators.push_back(std::make_shared<rv_gen_map>(this));
	generators.push_back(std::make_shared<rv_gen_markdown>(this));
	generators.push_back(std::make_shared<rv_gen_meta>(this));
	generators.push_back(std::make_shared<rv_gen_operands>(this));
	generators.push_back(std::make_shared<rv_gen_strings>(this));
	generators.push_back(std::make_shared<rv_gen_switch>(this));
	generators.push_back(std::make_shared<rv_gen_tablegen>(this));
}

void rv_gen::generate(int argc, const char *argv[])
{
	std::string isa_spec = "";
	bool help_or_error = false;

	// get generator command line options
	std::vector<cmdline_option> options = {
		{ "-h", "--help", cmdline_arg_type_none,
			"Show help",
			[&](std::string s) { return (help_or_error = true); } },
		{ "-I", "--isa-subset", cmdline_arg_type_string,
			"ISA subset (e.g. RV32IMA, RV32G, RV32GSC, RV64IMA, RV64G, RV64GSC)",
			[&](std::string s) { isa_spec = s; return true; } },
		{ "-r", "--read-isa", cmdline_arg_type_string,
			"Read instruction set metadata from directory",
			[&](std::string s) { return read_metadata(s); } },
		{ "-N", "--no-comment", cmdline_arg_type_none,
			"Don't emit comments in generated source",
			[&](std::string s) { return set_option("no_comment"); } },
		{ "-0", "--numeric-constants", cmdline_arg_type_none,
			"Use numeric constants in generated source",
			[&](std::string s) { return set_option("zero_not_oh"); } },
	};
	for (auto &gen : generators) {
		auto gen_opts = gen->get_cmdline_options();
		std::copy(gen_opts.begin(), gen_opts.end(), std::back_inserter(options));
	}
	options.push_back(
		{ nullptr, nullptr, cmdline_arg_type_none,   nullptr, nullptr }
	);

	auto result = cmdline_option::process_options(options.data(), argc, argv);
	if (!result.second) {
		help_or_error = true;
	} else if (result.first.size() != 0) {
		printf("%s: wrong number of arguments\n", argv[0]);
		help_or_error = true;
	}
	if (help_or_error) {
		printf("usage: %s [<options>]\n", argv[0]);
		cmdline_option::print_options(options.data());
		exit(9);
	}

	ext_subset = decode_isa_extensions(isa_spec);
	generate_map();

	for (auto &gen : generators) {
		gen->generate();
	}
}

void rv_gen::generate_map()
{
	for (auto &opcode : all_opcodes) {
		for (auto &mask : opcode->masks) {
			ssize_t msb = mask.first.msb;
			ssize_t lsb = mask.first.lsb;
			ssize_t val = mask.second;
			for (ssize_t bit = msb; bit >= lsb; bit--) {
				opcode->mask |= (1ULL << bit);
				opcode->match |= ((uint64_t(val) >> (bit - lsb)) & 1) << bit;
			}
		}
	}
}

void rv_gen::generate_codec()
{
	// make list of opcodes to include
	rv_opcode_list opcodes_copy;
	for (auto &opcode : all_opcodes) {
		if (opcode->is_pseudo()) continue;
		opcodes_copy.push_back(opcode);
	}

	// generate decode
	root_node.clear();
	generate_codec_node(root_node, opcodes_copy);
}

void rv_gen::generate_codec_node(rv_codec_node &node, rv_opcode_list &opcode_list)
{
	// calculate row coverage for each column
	std::vector<ssize_t> sum;
	sum.resize(32);
	for (auto &opcode : opcode_list) {
		for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
			if ((opcode->mask & (1 << bit)) && !(opcode->done & (1 << bit))) sum[bit]++;
		}
	}

	// find column with maximum row coverage
	ssize_t max_rows = 0;
	for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
		if (sum[bit] > max_rows) max_rows = sum[bit];
	}

	if (max_rows == 0) return; // no bits to match

	// select bits that cover maximum number of rows
	for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
		if (sum[bit] == max_rows) node.bits.push_back(bit);
	}

	// find distinct values for the chosen bits
	for (auto &opcode : opcode_list) {

		ssize_t val = 0;
		bool partial_match = false;
		for (auto &bit : node.bits) {
			if (!(opcode->mask & (1 << bit))) {
				partial_match = true;
			} else {
				val = (val << 1) | ((opcode->match & (1 << bit)) ? 1 : 0);
			}
		}

		// partial match is default in switch containing more specific masks
		if (partial_match) {
			val = DEFAULT;
		}

		// add value to list
		if (std::find(node.vals.begin(), node.vals.end(), val) == node.vals.end()) {
			node.vals.push_back(val);
		}

		// create opcode list for this value and add opcode to the list
		auto val_opcode_list_i = node.val_opcodes.find(val);
		if (val_opcode_list_i == node.val_opcodes.end()) {
			val_opcode_list_i = node.val_opcodes.insert(node.val_opcodes.begin(),
				std::pair<ssize_t,rv_opcode_list>(val, rv_opcode_list()));
		}
		val_opcode_list_i->second.push_back(opcode);
	}

	// sort values
	std::sort(node.vals.begin(), node.vals.end());

	// mark chosen bits as done
	for (auto &opcode : opcode_list) {
		for (auto &bit : node.bits) {
			opcode->done |= (1 << bit);
		}
	}

	// recurse
	for (auto &val : node.vals) {
		if (node.val_decodes.find(val) == node.val_decodes.end()) {
			node.val_decodes.insert(node.val_decodes.begin(),
				std::pair<ssize_t,rv_codec_node>(val, rv_codec_node()));
		}
		generate_codec_node(node.val_decodes[val], node.val_opcodes[val]);
	}
}

/* main */

int main(int argc, const char *argv[])
{
	rv_gen gen;
	gen.generate(argc, argv);
	exit(0);
}
