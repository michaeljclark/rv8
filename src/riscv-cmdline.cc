//
//  riscv-cmdline.cc
//

#include <string>
#include <vector>
#include <deque>
#include <map>

#include "riscv-cmdline.h"

const char* cmdline_option::arg_types[] =
{
	"",
	"<int>",
	"<string>"
};

std::pair<arg_list,bool> cmdline_option::process_options(cmdline_option *options, int argc, const char *argv[])
{
	arg_list args;
	for (int i = 1; i < argc; i++) {
		args.push_back(argv[i]);
	}
	
	while (args.size() > 0) {
		std::string arg = args.front();
		cmdline_option *o = options, *found_opt = nullptr;
		while (o->short_option) {
			if (arg == o->short_option || arg == o->long_option) {
				found_opt = o;
				break;
			}
			o++;
		}
		if (!found_opt) {
			if (arg.size() > 0 && arg[0] == '-') {
				fprintf(stderr, "%s: invalid option: %s\n", argv[0], arg.c_str());
				return std::pair<arg_list,bool>(args, false);
			}
			break;
		}
		args.pop_front();
		std::string s;
		if (found_opt->arg_type != cmdline_arg_type_none) {
			if (args.size() == 0) {
				return std::pair<arg_list,bool>(args, false);
			}
			s = args.front();
			args.pop_front();
		}
		if (!found_opt->fn(s)) {
			return std::pair<arg_list,bool>(args, false);
		}
	}
	
	return std::pair<arg_list,bool>(args, true);
}

void cmdline_option::print_options(cmdline_option *options)
{
	cmdline_option *o = options;
	while (o->short_option) {
		fprintf(stderr, "%30s, %2s %-10s %s\n",
				o->long_option, o->short_option, arg_types[o->arg_type], o->help_text);
		o++;
	}
}
