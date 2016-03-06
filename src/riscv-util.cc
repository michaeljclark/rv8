#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cerrno>
#include <vector>
#include <string>

#include <sys/stat.h>

#include "riscv-util.h"

static const int INITIAL_BUFFER_SIZE = 256;

std::vector<char> read_file(std::string filename)
{
	std::vector<char> buf;
	struct stat stat_buf;

	FILE *file = fopen(filename.c_str(), "r");
	if (!file) {
		panic("error fopen: %s: %s", filename.c_str(), strerror(errno));
	}

	if (fstat(fileno(file), &stat_buf) < 0) {
		panic("error fstat: %s: %s", filename.c_str(), strerror(errno));
	}

	buf.resize(stat_buf.st_size);
	size_t bytes_read = fread(buf.data(), 1, stat_buf.st_size, file);
	if (bytes_read != (size_t)stat_buf.st_size) {
		panic("error fread: %s", filename.c_str());
	}
	fclose(file);

	return buf;
}

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
