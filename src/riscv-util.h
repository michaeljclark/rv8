//
//  riscv-util.h
//

#ifndef riscv_util_h
#define riscv_util_h

extern std::vector<uint8_t> read_file(std::string filename);
extern std::string format_string(const char* fmt, ...);
extern void log_format(FILE *file, const char* fmt, va_list arg);
extern void panic(const char* fmt, ...) __attribute__ ((noreturn));
extern void debug(const char* fmt, ...);

extern std::string ltrim(std::string s);
extern std::string rtrim(std::string s);
extern std::vector<std::string> split(std::string str, std::string sep,
	bool inc_empty, bool inc_sep);

#endif