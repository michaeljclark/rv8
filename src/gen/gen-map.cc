//
//  gen-map.cc
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

#include <unistd.h>

#include "util.h"
#include "cmdline.h"
#include "model.h"
#include "gen.h"
#include "color.h"

#define _COLOR_OPCODE _COLOR_BEGIN _COLOR_UNDERSCORE _COLOR_SEP _COLOR_FG_YELLOW _COLOR_END
#define _COLOR_BITS   _COLOR_BEGIN _COLOR_REVERSE    _COLOR_SEP _COLOR_FG_GREEN  _COLOR_END
#define _COLOR_FORMAT _COLOR_BEGIN _COLOR_BOLD       _COLOR_SEP _COLOR_FG_RED    _COLOR_END
#define _COLOR_LEGEND _COLOR_BEGIN _COLOR_BOLD       _COLOR_END
#define _COLOR_EXT    _COLOR_BEGIN _COLOR_FG_RED     _COLOR_END

std::vector<cmdline_option> rv_gen_map::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-c", "--color", cmdline_arg_type_none,
			"Enable ANSI color map",
			[&](std::string s) { return gen->set_option("enable_color"); } },
		{ "-m", "--print-map", cmdline_arg_type_none,
			"Print instruction map",
			[&](std::string s) { return gen->set_option("print_map"); } },
		{ "-mc", "--print-map-pseudocode-c", cmdline_arg_type_none,
			"Print instruction map with C pseudocode",
			[&](std::string s) { return gen->set_option("print_map") && gen->set_option("map_pseudo_code_c"); } },
		{ "-ma", "--print-map-pseudocode-alt", cmdline_arg_type_none,
			"Print instruction map with alternate pseudocode",
			[&](std::string s) { return gen->set_option("print_map") && gen->set_option("map_pseudo_code_alt"); } },
	};
}

static std::string colorize_operands(rv_gen *gen, rv_opcode_ptr opcode)
{
	std::vector<char> token;
	std::vector<std::string> comps;
	auto operands = opcode->format->operands;

	for (size_t i = 0; i < operands.length(); i++) {
		char c = operands[i];
		if (::isalnum(c)) {
			token.push_back(c);
		} else {
			comps.push_back(std::string(token.begin(), token.end()));
			token.resize(0);
			token.push_back(c);
			comps.push_back(std::string(token.begin(), token.end()));
			token.resize(0);
		}
	}
	if (token.size() > 0) {
		comps.push_back(std::string(token.begin(), token.end()));
	}

	for (size_t i = 0; i < comps.size(); i++) {
		auto comp = comps[i];
		auto operand = gen->operands_by_name[comp];
		if (operand) {
			auto new_comp = rv_colors_to_ansi_escape_sequence(
				operand->fg_color(), operand->bg_color(), ansi_color_normal
			);
			new_comp.append(comp);
			new_comp.append(_COLOR_RESET);
			comps[i] = new_comp;
		}
	}

	return join(comps, "");
}

static void print_map(rv_gen *gen)
{
	bool enable_colorize = gen->has_option("enable_color") && isatty(fileno(stdout));
	bool map_pseudo_code_c = gen->has_option("map_pseudo_code_c");
	bool map_pseudo_code_alt = gen->has_option("map_pseudo_code_alt");

	int i = 0;
	for (auto &opcode : gen->opcodes) {
		if (!opcode->match_extension(gen->ext_subset)) continue;
		if (opcode->extensions.size() == 0) continue;
		if (i % 22 == 0) {
			printf("// %s", enable_colorize ? _COLOR_LEGEND : "");
			for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
				char c = (bit % 10) == 0 ? (bit / 10) + '0' : ' ';
				printf("%c", c);
			}
			printf("%s\n", enable_colorize ? _COLOR_RESET : "");
			printf("// %s", enable_colorize ? _COLOR_LEGEND : "");
			for (ssize_t bit = kMaxInstructionWidth-1; bit >= 0; bit--) {
				char c = (bit % 10) + '0';
				printf("%c", c);
			}
			printf("%s\n", enable_colorize ? _COLOR_RESET : "");
		}
		i++;
		printf("// ");
		ssize_t bit_width = opcode->extensions[0]->inst_width;
		for (ssize_t i = 0; i < kMaxInstructionWidth-bit_width; i++) printf(" ");
		for (ssize_t bit = bit_width-1; bit >= 0; bit--) {
			char c = ((opcode->mask & (1 << bit)) ? ((opcode->match & (1 << bit)) ? '1' : '0') : '.');
			switch (c) {
				case '0':
				case '1':
				{
					printf("%s%c%s",
						enable_colorize ? _COLOR_BITS : "",
						c,
						enable_colorize ? _COLOR_RESET : "");
					break;
				}
				default:
				{
					rv_operand_ptr operand = opcode->find_operand(bit);
					if (operand && opcode->name[0] != '@') {
						printf("%s%s%s",
							enable_colorize ? rv_colors_to_ansi_escape_sequence(
								operand->fg_color(), operand->bg_color()
							).c_str() : "",
							operand->char_code().c_str(),
							enable_colorize ? _COLOR_RESET : "");
					} else {
						printf("%c", c);
					}
					break;
				}
			}
		}
		if (enable_colorize) {
			printf(" %s%s%s %s",
				_COLOR_OPCODE, opcode->name.c_str(), _COLOR_RESET, colorize_operands(gen, opcode).c_str());
		} else {
			printf(" %s %s",
				opcode->name.c_str(), opcode->format->operands.c_str());
		}
		ssize_t len = 30 - (opcode->name.length() + opcode->format->operands.length());
		std::string ws;
		for (ssize_t i = 0; i < len; i++) ws += ' ';
		printf("%s%s# %s%s%s\n",
			ws.c_str(),
			enable_colorize ? _COLOR_EXT : "",
			opcode->extensions.front()->name.c_str(),
			enable_colorize ? _COLOR_RESET : "",
			map_pseudo_code_c ? format_string("   %s", opcode->pseudocode_c.c_str()).c_str() :
			map_pseudo_code_alt ? format_string("   %s", opcode->pseudocode_alt.c_str()).c_str() : "");
	}
}

void rv_gen_map::generate()
{
	if (gen->has_option("print_map")) print_map(gen);
}
