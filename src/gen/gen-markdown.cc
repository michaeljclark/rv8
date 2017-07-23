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
	for (auto &opcode : gen->opcodes) {
		if (!opcode->match_extension(gen->ext_subset)) continue;
		if (opcode->extensions.size() == 0) continue;

		printf("%-10s | %-40s | %s\n",
			opcode->name.c_str(),
			opcode->fullname.c_str(),
			opcode->pseudocode_alt.c_str());
	}
}

void rv_gen_markdown::generate()
{
	if (gen->has_option("print_markdown")) print_markdown(gen);
}
