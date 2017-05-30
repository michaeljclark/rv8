//
//  gen-constraints.cc
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

std::vector<cmdline_option> rv_gen_constraints::get_cmdline_options()
{
	return std::vector<cmdline_option>{
		{ "-XC", "--print-constraints-h", cmdline_arg_type_none,
			"Print constraints header",
			[&](std::string s) { return gen->set_option("print_constraints_h"); } },
	};
}

static void print_constraints_h(rv_gen *gen)
{
	static const char* kConstraintsHeader =

R"C(#ifndef rv_constraints_h
#define rv_constraints_h

)C";

	static const char* kConstraintsCheckHeader =

R"C(template <typename T>
inline bool constraint_check(T &dec, const rvc_constraint *c)
{
	auto imm = dec.imm;
	auto rd = dec.rd, rs1 = dec.rs1, rs2 = dec.rs2;
	while (*c != rvc_end) {
		switch (*c) {
)C";

static const char* kConstraintsCheckFooter =

R"C(			default:                 break;
		}
		c++;
	}
	return true;
}

)C";

	static const char* kConstraintsSetHeader =

R"C(template <typename T>
inline void constraint_set(T &dec, const rvc_constraint *c)
{
	auto &imm = dec.imm;
	auto &rd = dec.rd, &rs1 = dec.rs1, &rs2 = dec.rs2;
	while (*c != rvc_end) {
		switch (*c) {
)C";

static const char* kConstraintsSetFooter =

R"C(			default:                 break;
		}
		c++;
	}
}

)C";

static const char* kConstraintsFooter =

R"C(#endif
)C";

	printf(kCHeader, "constraints.h");
	printf("%s", kConstraintsHeader);

	printf("%s", kConstraintsCheckHeader);
	for (auto &constraint : gen->constraints) {
		printf("\t\t\tcase rvc_%-16sif (!(%s)) return false; break;\n",
			format_string("%s:", constraint->name.c_str()).c_str(),
			constraint->expression.c_str());
	}
	printf("%s", kConstraintsCheckFooter);

	printf("%s", kConstraintsSetHeader);
	for (auto &constraint : gen->constraints) {
		if (constraint->name.find("_eq_") == std::string::npos) continue;
		std::string constraint_set = replace(constraint->expression, "==", "=");
		printf("\t\t\tcase rvc_%-16s%s; break;\n",
			format_string("%s:", constraint->name.c_str()).c_str(),
			constraint_set.c_str());
	}
	printf("%s", kConstraintsSetFooter);

	printf("%s", kConstraintsFooter);
}

void rv_gen_constraints::generate()
{
	if (gen->has_option("print_constraints_h")) print_constraints_h(gen);
}
