//
//  rv.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <map>
#include <functional>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-bits.h"
#include "riscv-host.h"
#include "riscv-util.h"

using namespace riscv;

typedef int(*rv_cmd_fn)(int argc, const char **argv);

int rv_compress_main(int argc, const char **argv);
int rv_dump_main(int argc, const char **argv);
int rv_histogram_main(int argc, const char **argv);
int rv_pte_main(int argc, const char **argv);

struct rv_cmd {
	const char* name;
	rv_cmd_fn main_fn;
};

static rv_cmd cmds[] = {
	{ "compress",  rv_compress_main },
	{ "dump",      rv_dump_main },
	{ "histogram", rv_histogram_main },
	{ "pte",       rv_pte_main },
	{ nullptr,     nullptr },
};

int main(int argc, const char **argv)
{
	/* find subcommand and execute it */
	if (argc >= 2) {
		rv_cmd *cmd = cmds;
		while (cmd->name) {
			if (strcmp(cmd->name, argv[1]) == 0) {
				return cmd->main_fn(argc - 1, argv + 1);
			}
			cmd++;
		}
	}

	/* print usage and exit */
	fprintf(stderr, "usage: %s <command> [options]\n", argv[0]);
	fprintf(stderr, "usage: %s <command> -h\n", argv[0]);
	fprintf(stderr, "       commands = { ");
	rv_cmd *cmd = cmds;
	while (cmd->name) {
		fprintf(stderr, "%s%s", cmd != cmds ? ", " : "", cmd->name);
		cmd++;
	}
	fprintf(stderr, " }\n");
	exit(1);
}
