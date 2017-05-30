//
//  gen-operands.cc
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

std::vector<cmdline_option> rv_gen_operands::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-A", "--print-operands-h", cmdline_arg_type_none,
			"Print operands header",
			[&](std::string s) { return gen->set_option("print_operands_h"); } },
	};
}

static void print_operands_h(rv_gen *gen)
{
	printf(kCHeader, "operands.h");
	printf("#ifndef rv_operands_h\n");
	printf("#define rv_operands_h\n");
	printf("\n");

	// print immediate decoders
	for (auto operand: gen->operands) {
		printf("typedef %s%-60s operand_%s;\n",
			(operand->type == "simm" || operand->type == "offset") ? "simm_operand_t" : "uimm_operand_t",
			operand->bitspec.to_template().c_str(),
			operand->name.c_str());
	}
	printf("\n");
	printf("#endif\n");
}

void rv_gen_operands::generate()
{
	if (gen->has_option("print_operands_h")) print_operands_h(gen);
}
