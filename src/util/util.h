//
//  util.h
//

#ifndef riscv_util_h
#define riscv_util_h

extern bool parse_integral(std::string valstr, long long &val);

extern std::string format_string(const char* fmt, ...);
extern void log_format(FILE *file, const char* fmt, va_list arg);
extern void panic(const char* fmt, ...) __attribute__ ((noreturn));
extern void debug(const char* fmt, ...);

extern const char* signal_name(int signum);

extern std::string ltrim(std::string s);
extern std::string rtrim(std::string s);
extern std::string ltrim(std::string s, int(*p)(int));
extern std::string rtrim(std::string s, int(*p)(int));
extern std::vector<std::string> split(std::string str, std::string sep,
	bool inc_empty = true, bool inc_sep = false);
extern std::string replace(std::string haystack, const std::string needle, const std::string noodle);

template <typename T>
struct bit_char_array_t : std::array<char, (sizeof(T)<<3)+1>
{
	enum size_t { msb = (sizeof(T)<<3)-1 };
	inline operator char*() { return std::array<char, (sizeof(T)<<3)+1>::data(); }
};

template<typename B, typename T, size_t N>
struct to_binary_loop
{
	static inline void apply(B &buf, T val)
	{
		buf[B::msb-N] = '0' + bool(val & T(1) << N);
		to_binary_loop<B,T,N-1>::apply(buf, val);
	}
};

template<typename B, typename T>
struct to_binary_loop<B,T,0>
{
	static inline void apply(B &buf, T val)
	{
		buf[B::msb] = '0' + bool(val & T(1));
	}
};

template<typename T, typename B = bit_char_array_t<T>>
inline B to_binary(T val)
{
	B buf;
	to_binary_loop<B,T,B::msb>::apply(buf, val);
	buf[B::msb+1] = '\0';
	return buf;
}

#endif
