//
//  riscv-test-rand.cc
//

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cassert>
#include <cmath>
#include <cctype>
#include <cwchar>
#include <climits>
#include <cfloat>
#include <cfenv>
#include <limits>
#include <array>
#include <string>
#include <vector>
#include <type_traits>

#include "riscv-types.h"
#include "riscv-args.h"
#include "riscv-bigint.h"
#include "riscv-itoa.h"
#include "riscv-dtoa.h"
#include "riscv-hdtoa.h"
#include "riscv-fmt.h"

using namespace riscv;

static const char *hex_digits = "0123456789abcdef";

void test_hdtoa(double val)
{
	int sign, exp;
	std::string res = hdtoa(val, hex_digits, -1, &exp, &sign);
	printf("%s0x1%s%sp%s%d\n", sign ? "-" : "", res.size() > 1 ? "." : "", res.c_str() + 1, exp-1 > 0 ? "+" : "", exp-1);
	printf("%a\n", val);
}

void test_dtoa(double val, int ndigits)
{
	char buf[64] = {0};
	int sign, exp;
	std::string res = dtoa(val, 3, ndigits, &exp, &sign);
	sprintf(buf, "%%-%d.%df%%%ds%%s\n", ndigits+10, ndigits, 17-ndigits);
	printf(buf, val, "", res.c_str());
}

void test_itoa(intmax_t val)
{
	std::string res = itoa(val, 10, hex_digits);
	printf("%s\n", res.c_str());
	printf("%llu\n", (u64)val);
}

void test_itoa()
{
	printf("// every pair of lines should match\n");
	test_itoa(768);
	test_itoa(4294967296ULL);
	test_itoa(18446744073709551615ULL);
}

void test_dtoa()
{
	printf("// generated digits of precision should match\n");
	printf("// sign, leading and trailing zeros do not match\n");
	test_dtoa(0, 6);
	test_dtoa(NAN, 6);
	test_dtoa(INFINITY, 6);
	test_dtoa(1024, 6);
	test_dtoa(1024, 12);
	test_dtoa(1024, 17);
	test_dtoa(1024.6725, 6);
	test_dtoa(1024.6725, 12);
	test_dtoa(1024.6725, 17);
	test_dtoa(-1.0 / 1024, 6);
	test_dtoa(-1.0 / 1024, 12);
	test_dtoa(-1.0 / 1024, 17);
	test_dtoa(3.141592653589793, 6);
	test_dtoa(3.141592653589793, 12);
	test_dtoa(3.141592653589793, 17);

	test_dtoa(3.141592653589793e300, 1);
}

void test_hdtoa()
{
	printf("// every pair of lines should match\n");
	test_hdtoa(1024);
	test_hdtoa(1024.6725);
	test_hdtoa(-1.0 / 1024);
	test_hdtoa(3.141592653589793);
	test_hdtoa(3.141592653589793e100);
}

#define test_printf(fmt, x...) \
	buf.clear(); \
	printf(fmt, x); \
	sprintf(buf, fmt, x); \
	printf("%s", buf.c_str());

void test_sprintf()
{
	std::string buf;

	printf("// every pair of lines should match\n");

	// sprintf test
	test_printf("%s world %p\n", "Hello", (void*)-1);
	test_printf("%-20s %d %u %f\n", "No pad", 123, 0U, 1.1f);
	test_printf("%-20s %8d %u %9f\n", "Right pad", 123, 0U, 1.1f);
	test_printf("%-20s %-8d %u %9f\n", "Left pad", 123, 0U, 1.1f);
	test_printf("%-20s %08d %u %17f\n", "Zero pad", 123, 0U, 1.1f);
	test_printf("%-20s %a\n", "Hex Float", 3.141592653589793);
	test_printf("%-20s %17f\n", "Double 17f", 3.141592653589793);
	test_printf("%-20s %17.15f\n", "Double 17.15f", 3.141592653589793);

	// sprintf append test
	buf.clear();
	std::string b("B");
	sprintf(buf, "%s", std::string("a"));
	sprintf(buf, "%s", b);
	sprintf(buf, "%c", 'c');
	sprintf(buf, "%s", std::to_string(123));
	printf("%s\n", buf.c_str());
	printf("aBc123\n");
}

int main()
{
	test_itoa();
	test_dtoa();
	test_hdtoa();
	test_sprintf();

	return 0;
}
