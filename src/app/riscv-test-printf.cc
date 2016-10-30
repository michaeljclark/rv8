//
//  riscv-test-rand.cc
//

#include <cstdio>
#include <cstring>
#include <cassert>
#include <cmath>
#include <climits>
#include <cfloat>
#include <cfenv>
#include <limits>
#include <array>
#include <string>
#include <vector>
#include <type_traits>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-meta.h"
#include "riscv-fpu.h"
#include "riscv-printf-bigint.h"
#include "riscv-printf-itoa.h"
#include "riscv-printf-dtoa.h"
#include "riscv-printf-hdtoa.h"
#include "riscv-printf-format.h"

using namespace riscv;

static const char *hex_digits = "0123456789abcdef";

void test_hdtoa(double val)
{
	int sign, exp;
	std::string res = hdtoa(val, hex_digits, -1, &exp, &sign);
	printf("%s0x1%s%sp%s%d\n", sign ? "-" : "", res.size() > 1 ? "." : "", res.c_str() + 1, exp-1 > 0 ? "+" : "", exp-1);
	printf("%a\n", val);
}

void test_dtoa(double val)
{
	int sign, exp;
	std::string res = dtoa(val, 3, 12, &exp, &sign);
	printf("%s\n", res.c_str());
	printf("%17f\n", val);
}

void test_itoa(intmax_t val)
{
	std::string res = itoa(val, 10, hex_digits, 0, 0, 0);
	printf("%s\n", res.c_str());
	printf("%llu\n", (u64)val);
}

int main()
{
	printf("%2$s world %1$p\n", (void*)-1, "Hello");
	printf("%c %d %u %f\n", '!', 123, 0U, 1.1f);

	std::string buf;
	sprintf(buf, "%0$2s world %0$1p\n", (void*)-1, "Hello");
	sprintf(buf, "%c %d %u %f\n", '!', 123, 0U, 1.1f);

	std::string b("B");
	sprintf(buf, "%s\n", std::string("A"));
	sprintf(buf, "%s\n", b);
	sprintf(buf, "%s\n", std::to_string(123));

	test_hdtoa(1024);
	test_hdtoa(1024.6725);
	test_hdtoa(-1.0 / 1024);
	test_hdtoa(3.14159265358979323846);

	test_dtoa(1024);
	test_dtoa(1024.6725);
	test_dtoa(-1.0 / 1024);
	test_dtoa(3.14159265358979323846);
	test_dtoa(3.14159265358979323846e100);

	test_itoa(768);
	test_itoa(4294967296ULL);
	test_itoa(18446744073709551615ULL);

	return 0;
}
