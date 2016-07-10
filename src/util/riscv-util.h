//
//  riscv-util.h
//

#ifndef riscv_util_h
#define riscv_util_h

extern std::string format_string(const char* fmt, ...);
extern void log_format(FILE *file, const char* fmt, va_list arg);
extern void panic(const char* fmt, ...) __attribute__ ((noreturn));
extern void debug(const char* fmt, ...);

extern std::string ltrim(std::string s);
extern std::string rtrim(std::string s);
extern std::string ltrim(std::string s, int(*p)(int));
extern std::string rtrim(std::string s, int(*p)(int));
extern std::vector<std::string> split(std::string str, std::string sep,
	bool inc_empty = true, bool inc_sep = false);
extern std::string replace(std::string haystack, const std::string needle, const std::string noodle);

#endif
