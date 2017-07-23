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

std::vector<cmdline_option> rv_gen_markdown::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-md", "--print-markdown", cmdline_arg_type_none,
			"Print instruction reference in markdown",
			[&](std::string s) { return gen->set_option("print_markdown"); } },
	};
}

static void print_markdown(rv_gen *gen)
{
	static const char* fmt = "%s | %s | %s\n";

	for (auto &ext : gen->extensions) {
		if (ext->opcodes.size() == 0 || (gen->ext_subset.size() > 0 &&
			std::find(gen->ext_subset.begin(), gen->ext_subset.end(), ext) == gen->ext_subset.end())) {
			continue;
		}

		printf("_**%s**_\n\n", ext->description.c_str());
		printf(fmt, "Format", "Name", "Pseudocode");
		printf(fmt, ":--", ":--", ":--");

		for (auto &opcode : ext->opcodes) {
			if (opcode->is_pseudo()) continue;

			auto name = opcode->name;
			auto operand_comps = split(opcode->format->operands, ",");
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);
			if (operand_comps.size() == 1 && operand_comps[0] == "none") operand_comps[0] = "";
			std::string format;
			format.append("<code><sub>");
			format.append(name);
			if (operand_comps.size() > 0) {
				format.append(" ");
				format.append(join(operand_comps, ","));
			}
			format.append("</sub></code>");

			std::string fullname;
			fullname.append("<sub>");
			fullname.append(opcode->fullname);
			fullname.append("</sub>");

			std::string pseudocode;
			pseudocode.append("<sub>");
			pseudocode.append(opcode->pseudocode_alt);
			pseudocode.append("</sub>");

			printf(fmt, format.c_str(), fullname.c_str(), pseudocode.c_str());
		}
		printf("\n");
	}
}

void rv_gen_markdown::generate()
{
	if (gen->has_option("print_markdown")) print_markdown(gen);
}
