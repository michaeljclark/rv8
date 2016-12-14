//
//  test-config.cc
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

#include "types.h"
#include "util.h"
#include "config-parser.h"
#include "config-string.h"

using namespace riscv;

int main(int argc, const char * argv[])
{
	if (argc != 2) {
		fprintf(stderr, "usage: %s <configfile>\n", argv[0]);
		exit(1);
	}

	config_string cfg;
	cfg.read(argv[1]);

	printf("%s", cfg.to_string().c_str());

	return 0;
}
