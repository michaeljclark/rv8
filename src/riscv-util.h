//
//  riscv-util.h
//

#ifndef riscv_util_h
#define riscv_util_h

extern std::vector<uint8_t> read_file(std::string filename);
extern std::string format_string(const char* fmt, ...);
extern void log_format(const char* fmt, va_list arg);
extern void panic(const char* fmt, ...);
extern void debug(const char* fmt, ...);

#endif