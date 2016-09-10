//
//  riscv-gen-constraints.cc
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

std::vector<cmdline_option> riscv_gen_constraints::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-XC", "--print-constraints-h", cmdline_arg_type_none,
			"Print constraints header",
			[&](std::string s) { return gen->set_option("print_constraints_h"); } },
	};
}

static void print_constraints_h(riscv_gen *gen)
{
	static const char* kConstraintsHeader =

R"C(#ifndef riscv_constraints_h
#define riscv_constraints_h

template <typename T>
inline bool constraint_check(T &dec, const rvc_constraint *c)
{
	auto imm = dec.imm;
	auto rd = dec.rd, rs1 = dec.rs1, rs2 = dec.rs2;
	while (*c != rvc_end) {
		switch (*c) {
)C";

static const char* kConstraintsFooter =

R"C(			case rvc_end:           break;
		}
		c++;
	}
	return true;
}

#endif
)C";

	printf(kCHeader, "riscv-constraints.h");
	printf("%s", kConstraintsHeader);
	for (auto &constraint : gen->constraints) {
		printf("\t\t\tcase rvc_%-16sif (!(%s)) return false; break;\n",
			format_string("%s:", constraint->name.c_str()).c_str(),
			constraint->expression.c_str());
	}
	printf("%s", kConstraintsFooter);
}

void riscv_gen_constraints::generate()
{
	if (gen->has_option("print_constraints_h")) print_constraints_h(gen);
}
