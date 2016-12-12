//
//  cmdline.h
//

#ifndef riscv_cmdline_h
#define riscv_cmdline_h

enum cmdline_arg_type
{
	cmdline_arg_type_none,
	cmdline_arg_type_int,
	cmdline_arg_type_string
};

typedef std::deque<std::string> arg_list;

struct cmdline_option
{
	static const char* arg_types[];
	
	const char* short_option;
	const char* long_option;
	cmdline_arg_type arg_type;
	const char* help_text;
	std::function<bool(std::string)> fn;
	
	static std::pair<arg_list,bool> process_options(cmdline_option *options, int argc, const char *argv[]);
	static void print_options(cmdline_option *options);
};

#endif
