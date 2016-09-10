//
//  riscv-gen-operands.cc
//

#include <cstdio>
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

std::vector<cmdline_option> riscv_gen_operands::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-A", "--print-operands-h", cmdline_arg_type_none,
			"Print operands header",
			[&](std::string s) { return gen->set_option("print_operands_h"); } },
	};
}

static void print_operands_h(riscv_gen *gen)
{
	printf(kCHeader, "riscv-operands.h");
	printf("#ifndef riscv_operands_h\n");
	printf("#define riscv_operands_h\n");
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

void riscv_gen_operands::generate()
{
	if (gen->has_option("print_operands_h")) print_operands_h(gen);
}
