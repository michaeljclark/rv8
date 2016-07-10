//
//  riscv-test-config.cc
//

#include <cassert>
#include <cstring>
#include <functional>
#include <algorithm>
#include <string>
#include <memory>
#include <vector>
#include <deque>
#include <map>

#include "riscv-config-parser.h"
#include "riscv-config.h"

int main(int argc, const char * argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <configfile>\n", argv[0]);
		exit(1);
	}

	riscv_config cfg;
	cfg.read(argv[1]);

	printf("%s", cfg.to_string().c_str());

	return 0;
}
