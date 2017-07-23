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
	static const char* fmt = "%-25s|%-40s|%s\n";

	for (auto &ext : gen->extensions) {
		// check if this extension is in the selected subset
		if (ext->opcodes.size() == 0 || (gen->ext_subset.size() > 0 &&
			std::find(gen->ext_subset.begin(), gen->ext_subset.end(), ext) == gen->ext_subset.end())) {
			continue;
		}

		// print heading
		printf("_**%s**_\n\n", ext->description.c_str());
		printf(fmt, "Format", "Name", "Pseudocode");
		printf(fmt, ":--", ":--", ":--");

		// print opcodes
		for (auto &opcode : ext->opcodes) {
			// skip psuedo opcode
			if (opcode->is_pseudo()) continue;

			auto name = opcode->name;
			auto operand_comps = split(opcode->format->operands, ",");
			std::transform(name.begin(), name.end(), name.begin(), ::toupper);
			if (operand_comps.size() == 1 && operand_comps[0] == "none") operand_comps[0] = "";
			std::string opcode_format;
			opcode_format.append("`");
			opcode_format.append(name);
			if (operand_comps.size() > 0) {
				opcode_format.append(" ");
				opcode_format.append(join(operand_comps, ", "));
			}
			opcode_format.append("`");

			printf(fmt,
				opcode_format.c_str(),
				opcode->fullname.c_str(),
				opcode->pseudocode_alt.c_str());
		}
		printf("\n");
	}
}

void rv_gen_markdown::generate()
{
	if (gen->has_option("print_markdown")) print_markdown(gen);
}
