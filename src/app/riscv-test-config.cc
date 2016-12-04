//
//  riscv-test-config.cc
//

#include <cassert>
#include <cstring>
#include <cstdlib>
#include <functional>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <map>

#include "riscv-types.h"
#include "riscv-util.h"
#include "riscv-config-parser.h"
#include "riscv-config.h"

using namespace riscv;

int main(int argc, const char * argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <configfile>\n", argv[0]);
		exit(1);
	}

	config cfg;
	cfg.read(argv[1]);

	printf("%s", cfg.to_string().c_str());

	return 0;
}
