//
//  riscv-parse-meta.cc
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

/* main */

int main(int argc, const char *argv[])
{
	riscv_gen gen;
	gen.generate(argc, argv);
	exit(0);
}
