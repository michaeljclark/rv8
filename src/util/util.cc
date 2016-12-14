#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <csignal>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

#include "util.h"

static const int INITIAL_BUFFER_SIZE = 256;

bool parse_integral(std::string valstr, long long &val)
{
	char *endptr = nullptr;
	valstr = replace(valstr, "_", "");
	if (valstr.find("0x") == 0) {
		val = strtoull(valstr.c_str() + 2, &endptr, 16);
	} else if (valstr.find("0b") == 0) {
		val = strtoull(valstr.c_str() + 2, &endptr, 2);
	} else {
		val = strtoull(valstr.c_str(), &endptr, 10);
	}
	return (*endptr == '\0');
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

void log_format(FILE *file, const char* fmt, va_list arg)
{
	std::vector<char> buf(INITIAL_BUFFER_SIZE);

	int len = vsnprintf(buf.data(), buf.capacity(), fmt, arg);

	if (len >= (int)buf.capacity()) {
		buf.resize(len + 1);
		vsnprintf(buf.data(), buf.capacity(), fmt, arg);
	}

	fprintf(file, "%s\n", buf.data());
}

void panic(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_format(stderr, fmt, ap);
	va_end(ap);
	exit(9);
}

void debug(const char* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_format(stderr, fmt, ap);
	va_end(ap);
}

const char* signal_name(int signum)
{
	switch (signum) {
		case SIGHUP: return "SIGHUP";
		case SIGINT: return "SIGINT";
		case SIGQUIT: return "SIGQUIT";
		case SIGILL: return "SIGILL";
		case SIGABRT: return "SIGABRT";
		case SIGFPE: return "SIGFPE";
		case SIGSEGV: return "SIGSEGV";
		case SIGPIPE: return "SIGPIPE";
		case SIGTERM: return "SIGTERM";
		case SIGUSR1: return "SIGUSR1";
		case SIGUSR2: return "SIGUSR2";
		default: break;
	}
	return "SIG?";
}

std::string ltrim(std::string s)
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

std::string rtrim(std::string s)
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

std::string ltrim(std::string s, int(*p)(int))
{
	s.erase(s.begin(), std::find_if(s.begin(), s.end(),
			std::not1(std::ptr_fun<int, int>(p))));
	return s;
}

std::string rtrim(std::string s, int(*p)(int))
{
	s.erase(std::find_if(s.rbegin(), s.rend(),
			std::not1(std::ptr_fun<int, int>(p))).base(), s.end());
	return s;
}

std::vector<std::string> split(std::string str, std::string sep,
	bool inc_empty, bool inc_sep)
{
	size_t i, j = 0;
	std::vector<std::string> comps;
	while ((i = str.find_first_of(sep, j)) != std::string::npos) {
		if (inc_empty || i - j > 0) comps.push_back(str.substr(j, i - j));
		if (inc_sep) comps.push_back(sep);
		j = i + sep.length();
	}
	if (inc_empty || str.size() - j > 0) {
		comps.push_back(str.substr(j, str.size() - j));
	}
	return comps;
}

std::string replace(std::string haystack, const std::string needle, const std::string noodle)
{
    size_t i = 0;
    while((i = haystack.find(needle, i)) != std::string::npos) {
         haystack.replace(i, needle.length(), noodle);
         i += noodle.length();
    }
    return haystack;
}
