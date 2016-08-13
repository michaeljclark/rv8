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

/* convert to binary string */
template <typename T>
static inline std::string to_binary_string(T val)
{
    const int bits = sizeof(T) << 3;
    char buf[bits+1];
    for (int i = 0; i < bits; ++i) {
        buf[bits-i-1] = val & T(1)<<i ? '1' : '0';
    }
    buf[bits] = '\0';
    return buf;
}

#endif
