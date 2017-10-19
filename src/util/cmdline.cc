//
//  cmdline.cc
//

#include <functional>
#include <string>
#include <vector>
#include <deque>
#include <map>

#include "cmdline.h"

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
	
	bool passthrough = false;
	arg_list remaining_args;
	while (args.size() > 0) {
		std::string arg = args.front();
		if (passthrough) {
			args.pop_front();
			remaining_args.push_back(arg);
			continue;
		}
		cmdline_option *o = options, *found_opt = nullptr;
		while (o->short_option) {
			if (arg == o->short_option || arg == o->long_option) {
				found_opt = o;
				break;
			}
			o++;
		}
		args.pop_front();
		if (arg == "--") {
			passthrough = true;
			continue;
		}
		if (!found_opt) {
			remaining_args.push_back(arg);
			continue;
		}
		std::string s;
		if (found_opt->arg_type != cmdline_arg_type_none) {
			if (args.size() == 0) {
				return std::pair<arg_list,bool>(arg_list(), false);
			}
			s = args.front();
			args.pop_front();
		}
		if (!found_opt->fn(s)) {
			return std::pair<arg_list,bool>(arg_list(), false);
		}
	}
	
	return std::pair<arg_list,bool>(remaining_args, true);
}

void cmdline_option::print_options(cmdline_option *options)
{
	cmdline_option *o = options;
	while (o->short_option) {
		fprintf(stderr, "%30s, %-3s%-10s %s\n",
				o->long_option, o->short_option, arg_types[o->arg_type], o->help_text);
		o++;
	}
}
