#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <vector>
#include <string>

#include "riscv-util.h"

static const int INITIAL_BUFFER_SIZE = 256;

std::string format_string(const char* fmt, ...)
{
    std::vector<char> buf(INITIAL_BUFFER_SIZE);
    va_list ap;
    
    va_start(ap, fmt);
    int len = vsnprintf(buf.data(), buf.capacity(), fmt, ap);
    va_end(ap);
    
    std::string str;
    if (len >= (int)buf.capacity()) {
        buf.resize(len + 1);
        va_start(ap, fmt);
        vsnprintf(buf.data(), buf.capacity(), fmt, ap);
        va_end(ap);
    }
    str = buf.data();
    
    return str;
}

void log_format(const char* fmt, va_list arg)
{
	std::vector<char> buf(INITIAL_BUFFER_SIZE);

	int len = vsnprintf(buf.data(), buf.capacity(), fmt, arg);

	if (len >= (int)buf.capacity()) {
		buf.resize(len + 1);
		vsnprintf(buf.data(), buf.capacity(), fmt, arg);
	}

	printf("%s\n", buf.data());
}

void panic(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_format(fmt, ap);
	va_end(ap);
	exit(9);
}

void debug(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_format(fmt, ap);
	va_end(ap);
}
