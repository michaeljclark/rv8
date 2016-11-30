//
//  riscv-test-bits.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <vector>
#include <string>
#include <map>

#include "riscv-endian.h"
#include "riscv-types.h"
#include "riscv-bits.h"
#include "riscv-host.h"
#include "riscv-util.h"

using namespace riscv;

const int TIME_ITERATIONS = 16;

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

template <typename T>
void test_limits(const char* min_chk, const char* max_chk)
{
	const char* print_fmt_signed = "%s %-24s %s=%-20lld (0x%016llx) chk=%-24s\n";
	const char* print_fmt_unsigned = "%s %-24s %s=%-20llu (0x%016llx) chk=%-24s\n";
	char min_str[32], max_str[32];
	bool is_signed = type_min<T>() < T(0);
	if (is_signed) {
		snprintf(min_str, sizeof(min_str), "%lld", (u64)type_min<T>());
		snprintf(max_str, sizeof(max_str), "%lld", (u64)type_max<T>());
	} else {
		snprintf(min_str, sizeof(min_str), "%llu", (u64)type_min<T>());
		snprintf(max_str, sizeof(max_str), "%llu", (u64)type_max<T>());
	}
	bool min_pass = strcmp(min_str, min_chk) == 0;
	bool max_pass = strcmp(max_str, max_chk) == 0;
	printf(is_signed ? print_fmt_signed : print_fmt_unsigned,
		   min_pass ? "PASS" : "FAIL", __func__, "min",
		   (s64)type_min<T>(), (u64)type_min<T>(), min_chk);
	printf(is_signed ? print_fmt_signed : print_fmt_unsigned,
		   max_pass ? "PASS" : "FAIL", __func__, "max",
		   (s64)type_max<T>(), (u64)type_max<T>(), max_chk);
}

template <typename T>
void test_binary_string(T val, const char* chk)
{
	bool pass = strcmp(to_binary_string<T>(val).c_str(), chk) == 0;
	printf("%s %-24s val=%-20llu (0x%016llx) chk=%s\n",
		   pass ? "PASS" : "FAIL", __func__,
		   (u64)val, (u64)val, chk);
}

template <typename T>
void test_add_overflow(T a, T b, bool expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		add_overflow<T>(a, b);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	bool overflow = add_overflow<T>(a, b);
	bool pass = overflow == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s a=%-20llu (0x%016llx) b=%-20llu (0x%016llx) overflow=%d expected=%d\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)a, (u64)a,
		   (u64)b, (u64)b,
		   overflow, expected);
}

template <typename T>
void test_sub_underflow(T a, T b, bool expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		sub_underflow<T>(a, b);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	bool underflow = sub_underflow<T>(a, b);
	bool pass = underflow == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s a=%-20llu (0x%016llx) b=%-20llu (0x%016llx) overflow=%d expected=%d\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)a, (u64)a,
		   (u64)b, (u64)b,
		   underflow, expected);
}

template <typename T>
void test_bitextend(T val, int bit, T expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		bitextend<T>(val, bit);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	T result = bitextend<T>(val, bit);
	bool pass = result == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s val=%-20llu (0b%s) result=%lld (0b%s) expected=%lld (0b%s)\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)val, to_binary_string<T>(val).c_str(),
		   (u64)result, to_binary_string<T>(result).c_str(),
		   (u64)expected, to_binary_string<T>(expected).c_str());
}

template <typename T>
void test_popcount(T val, int expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		popcount<T>(val);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	int count = popcount<T>(val);
	bool pass = count == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s val=%-20llu (0b%s) count=%d expected=%d\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)val, to_binary_string<T>(val).c_str(), count, expected);
}

template <typename T>
void test_ispow2(T val, bool expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		ispow2<T>(val);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	bool pow2 = ispow2<T>(val);
	bool pass = pow2 == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s val=%-20llu (0b%s) pow2=%d expected=%d\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)val, to_binary_string<T>(val).c_str(), pow2, expected);
}

template <typename T>
void test_roundpow2(T val, int expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		roundpow2<T>(val);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	int rounded = roundpow2<T>(val);
	bool pass = rounded == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s val=%-20llu (0b%s) rounded=%d expected=%d\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)val, to_binary_string<T>(val).c_str(), rounded, expected);
}

template <typename T>
void test_clz(T val, int expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		clz<T>(val);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	int count = clz<T>(val);
	bool pass = count == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s val=%-20llu (0b%s) count=%d expected=%d\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)val, to_binary_string<T>(val).c_str(), count, expected);
}

template <typename T>
void test_ctz(T val, int expected, int iters = TIME_ITERATIONS)
{
	uint64_t tstart = 0, trun = 0, tmin = 0, tmax = 0, ttotal = 0, tavg = 0;
	for (int i = 0; i < iters; i++) {
		tstart = cpu_cycle_clock();
		ctz<T>(val);
		ttotal += (trun = (cpu_cycle_clock() - tstart));
		if (tmin == 0 || trun < tmin) tmin = trun;
		if (tmax == 0 || trun > tmax) tmax = trun;
	}
	tavg = ttotal / iters;

	int count = ctz<T>(val);
	bool pass = count == expected;
	printf("%s [%4llu,%4llu,%4llu] %-24s val=%-20llu (0b%s) count=%d expected=%d\n",
		   pass ? "PASS" : "FAIL", (u64)tmin, (u64)tmax, (u64)tavg, __func__,
		   (u64)val, to_binary_string<T>(val).c_str(), count, expected);
}

void test_limits()
{
	test_limits<int8_t>("-128", "127");
	test_limits<uint8_t>("0", "255");
	test_limits<int16_t>("-32768", "32767");
	test_limits<uint16_t>("0", "65535");
	test_limits<int32_t>("-2147483648", "2147483647");
	test_limits<uint32_t>("0", "4294967295");
	test_limits<int64_t>("-9223372036854775808", "9223372036854775807");
	test_limits<uint64_t>("0", "18446744073709551615");
}

void test_binary_string()
{
	test_binary_string<uint16_t>(0x0000, "0000000000000000");
	test_binary_string<uint16_t>(0x0001, "0000000000000001");
	test_binary_string<uint16_t>(0x1000, "0001000000000000");
	test_binary_string<uint16_t>(0x0c0c, "0000110000001100");
	test_binary_string<uint16_t>(0xcccc, "1100110011001100");
	test_binary_string<uint16_t>(0xffff, "1111111111111111");
	test_binary_string<uint32_t>(0x00000000, "00000000000000000000000000000000");
	test_binary_string<uint32_t>(0x00000001, "00000000000000000000000000000001");
	test_binary_string<uint32_t>(0x10000000, "00010000000000000000000000000000");
	test_binary_string<uint32_t>(0x0c0c0c0c, "00001100000011000000110000001100");
	test_binary_string<uint32_t>(0xcccccccc, "11001100110011001100110011001100");
	test_binary_string<uint32_t>(0xffffffff, "11111111111111111111111111111111");
	test_binary_string<uint64_t>(0x0000000000000000ULL, "0000000000000000000000000000000000000000000000000000000000000000");
	test_binary_string<uint64_t>(0x0000000000000001ULL, "0000000000000000000000000000000000000000000000000000000000000001");
	test_binary_string<uint64_t>(0x1000000000000000ULL, "0001000000000000000000000000000000000000000000000000000000000000");
	test_binary_string<uint64_t>(0x0c0c0c0c0c0c0c0cULL, "0000110000001100000011000000110000001100000011000000110000001100");
	test_binary_string<uint64_t>(0xccccccccccccccccULL, "1100110011001100110011001100110011001100110011001100110011001100");
	test_binary_string<uint64_t>(0xffffffffffffffffULL, "1111111111111111111111111111111111111111111111111111111111111111");
}

void test_add_overflow()
{
	test_add_overflow<uint8_t>(0, 0, false);
	test_add_overflow<uint8_t>(127, 128, false);
	test_add_overflow<uint8_t>(127, 129, true);
	test_add_overflow<uint8_t>(128, 128, true);
	test_add_overflow<uint8_t>(128, 127, false);
	test_add_overflow<uint8_t>(129, 127, true);
}

void test_sub_underflow()
{
	test_sub_underflow<uint8_t>(0, 0, false);
	test_sub_underflow<uint8_t>(3, 2, false);
	test_sub_underflow<uint8_t>(2, 3, true);
	test_sub_underflow<uint8_t>(255, 255, false);
	test_sub_underflow<uint8_t>(255, 254, false);
	test_sub_underflow<uint8_t>(254, 255, true);
}

void test_ispow2()
{
	test_ispow2<uint32_t>(1, true);
	test_ispow2<uint32_t>(2, true);
	test_ispow2<uint32_t>(3, false);
	test_ispow2<uint32_t>(4, true);
	test_ispow2<uint32_t>(5, false);
	test_ispow2<uint32_t>(7, false);
	test_ispow2<uint32_t>(8, true);
	test_ispow2<uint32_t>(9, false);
	test_ispow2<uint32_t>(15, false);
	test_ispow2<uint32_t>(16, true);
	test_ispow2<uint32_t>(17, false);
	test_ispow2<uint32_t>(31, false);
	test_ispow2<uint32_t>(32, true);
	test_ispow2<uint32_t>(33, false);
	test_ispow2<uint32_t>(63, false);
	test_ispow2<uint32_t>(64, true);
	test_ispow2<uint32_t>(65, false);
	test_ispow2<uint32_t>(127, false);
	test_ispow2<uint32_t>(128, true);
	test_ispow2<uint32_t>(129, false);
	test_ispow2<uint32_t>(255, false);
	test_ispow2<uint32_t>(256, true);
	test_ispow2<uint32_t>(257, false);
	test_ispow2<uint32_t>(511, false);
	test_ispow2<uint32_t>(512, true);
	test_ispow2<uint32_t>(513, false);
	test_ispow2<uint32_t>(1023, false);
	test_ispow2<uint32_t>(1024, true);
	test_ispow2<uint32_t>(1025, false);
}

void test_roundpow2()
{
	test_roundpow2<uint32_t>(1, 1);
	test_roundpow2<uint32_t>(2, 2);
	test_roundpow2<uint32_t>(3, 4);
	test_roundpow2<uint32_t>(4, 4);
	test_roundpow2<uint32_t>(5, 8);
	test_roundpow2<uint32_t>(6, 8);
	test_roundpow2<uint32_t>(7, 8);
	test_roundpow2<uint32_t>(8, 8);
	test_roundpow2<uint32_t>(9, 16);
	test_roundpow2<uint32_t>(10, 16);
	test_roundpow2<uint32_t>(11, 16);
	test_roundpow2<uint32_t>(12, 16);
	test_roundpow2<uint32_t>(13, 16);
	test_roundpow2<uint32_t>(14, 16);
	test_roundpow2<uint32_t>(15, 16);
	test_roundpow2<uint32_t>(16, 16);
}

void test_bitextend()
{
#if ENABLE_BROKEN
	test_bitextend<uint16_t>(0b010, 3, 0b0010);
	test_bitextend<uint16_t>(0b011, 3, 0b0011);
	test_bitextend<uint16_t>(0b110, 3, 0xfffe);
	test_bitextend<uint16_t>(0b111, 3, 0xffff);
#endif
	test_bitextend<uint32_t>(0b010, 3, 0b010);
	test_bitextend<uint32_t>(0b011, 3, 0b011);
	test_bitextend<uint32_t>(0b110, 3, 0xfffffffe);
	test_bitextend<uint32_t>(0b111, 3, 0xffffffff);
#if ENABLE_BROKEN
	test_bitextend<uint64_t>(0b010, 3, 0b010ULL);
	test_bitextend<uint64_t>(0b011, 3, 0b011ULL);
	test_bitextend<uint64_t>(0b110, 3, 0xfffffffffffffffeULL);
	test_bitextend<uint64_t>(0b111, 3, 0xffffffffffffffffULL);
#endif
}

void test_popcount()
{
	test_popcount<uint16_t>(0x0000, 0);
	test_popcount<uint16_t>(0x0000, 0);
	test_popcount<uint16_t>(0x0001, 1);
	test_popcount<uint16_t>(0x1000, 1);
	test_popcount<uint16_t>(0x000c, 2);
	test_popcount<uint16_t>(0x0c0c, 4);
	test_popcount<uint16_t>(0xcccc, 8);
	test_popcount<uint16_t>(0xf000, 4);
	test_popcount<uint16_t>(0xf0f0, 8);
	test_popcount<uint16_t>(0xffff, 16);
	test_popcount<uint32_t>(0x00000000, 0);
	test_popcount<uint32_t>(0x00000000, 0);
	test_popcount<uint32_t>(0x00000001, 1);
	test_popcount<uint32_t>(0x10000000, 1);
	test_popcount<uint32_t>(0x0000000c, 2);
	test_popcount<uint32_t>(0x0c0c0c0c, 8);
	test_popcount<uint32_t>(0xcccccccc, 16);
	test_popcount<uint32_t>(0xf0000000, 4);
	test_popcount<uint32_t>(0xf0f0f0f0, 16);
	test_popcount<uint32_t>(0xffffffff, 32);
	test_popcount<uint64_t>(0x0000000000000000ULL, 0);
	test_popcount<uint64_t>(0x0000000000000000ULL, 0);
	test_popcount<uint64_t>(0x0000000000000001ULL, 1);
	test_popcount<uint64_t>(0x0000000000000002ULL, 1);
	test_popcount<uint64_t>(0x0000000000000003ULL, 2);
	test_popcount<uint64_t>(0x0000000000000004ULL, 1);
	test_popcount<uint64_t>(0x0000000000000005ULL, 2);
	test_popcount<uint64_t>(0x0000000000000006ULL, 2);
	test_popcount<uint64_t>(0x0000000000000007ULL, 3);
	test_popcount<uint64_t>(0x0000000000000008ULL, 1);
	test_popcount<uint64_t>(0x0000000000000009ULL, 2);
	test_popcount<uint64_t>(0x000000000000000aULL, 2);
	test_popcount<uint64_t>(0x000000000000000bULL, 3);
	test_popcount<uint64_t>(0x000000000000000cULL, 2);
	test_popcount<uint64_t>(0x000000000000000dULL, 3);
	test_popcount<uint64_t>(0x000000000000000eULL, 3);
	test_popcount<uint64_t>(0x000000000000000fULL, 4);
	test_popcount<uint64_t>(0x0000000000000000ULL, 0);
	test_popcount<uint64_t>(0x0000000000000000ULL, 0);
	test_popcount<uint64_t>(0x0000000000000011ULL, 2);
	test_popcount<uint64_t>(0x0000000000000022ULL, 2);
	test_popcount<uint64_t>(0x0000000000000033ULL, 4);
	test_popcount<uint64_t>(0x0000000000000044ULL, 2);
	test_popcount<uint64_t>(0x0000000000000055ULL, 4);
	test_popcount<uint64_t>(0x0000000000000066ULL, 4);
	test_popcount<uint64_t>(0x0000000000000077ULL, 6);
	test_popcount<uint64_t>(0x0000000000000088ULL, 2);
	test_popcount<uint64_t>(0x0000000000000099ULL, 4);
	test_popcount<uint64_t>(0x00000000000000aaULL, 4);
	test_popcount<uint64_t>(0x00000000000000bbULL, 6);
	test_popcount<uint64_t>(0x00000000000000ccULL, 4);
	test_popcount<uint64_t>(0x00000000000000ddULL, 6);
	test_popcount<uint64_t>(0x00000000000000eeULL, 6);
	test_popcount<uint64_t>(0x00000000000000ffULL, 8);
	test_popcount<uint64_t>(0x0000000000001111ULL, 4);
	test_popcount<uint64_t>(0x0000000000002222ULL, 4);
	test_popcount<uint64_t>(0x0000000000003333ULL, 8);
	test_popcount<uint64_t>(0x0000000000004444ULL, 4);
	test_popcount<uint64_t>(0x0000000000005555ULL, 8);
	test_popcount<uint64_t>(0x0000000000006666ULL, 8);
	test_popcount<uint64_t>(0x0000000000007777ULL, 12);
	test_popcount<uint64_t>(0x0000000000008888ULL, 4);
	test_popcount<uint64_t>(0x0000000000009999ULL, 8);
	test_popcount<uint64_t>(0x000000000000aaaaULL, 8);
	test_popcount<uint64_t>(0x000000000000bbbbULL, 12);
	test_popcount<uint64_t>(0x000000000000ccccULL, 8);
	test_popcount<uint64_t>(0x000000000000ddddULL, 12);
	test_popcount<uint64_t>(0x000000000000eeeeULL, 12);
	test_popcount<uint64_t>(0x000000000000ffffULL, 16);
	test_popcount<uint64_t>(0x0000000011111111ULL, 8);
	test_popcount<uint64_t>(0x0000000022222222ULL, 8);
	test_popcount<uint64_t>(0x0000000033333333ULL, 16);
	test_popcount<uint64_t>(0x0000000044444444ULL, 8);
	test_popcount<uint64_t>(0x0000000055555555ULL, 16);
	test_popcount<uint64_t>(0x0000000066666666ULL, 16);
	test_popcount<uint64_t>(0x0000000077777777ULL, 24);
	test_popcount<uint64_t>(0x0000000088888888ULL, 8);
	test_popcount<uint64_t>(0x0000000099999999ULL, 16);
	test_popcount<uint64_t>(0x00000000aaaaaaaaULL, 16);
	test_popcount<uint64_t>(0x00000000bbbbbbbbULL, 24);
	test_popcount<uint64_t>(0x00000000ccccccccULL, 16);
	test_popcount<uint64_t>(0x00000000ddddddddULL, 24);
	test_popcount<uint64_t>(0x00000000eeeeeeeeULL, 24);
	test_popcount<uint64_t>(0x00000000ffffffffULL, 32);
	test_popcount<uint64_t>(0x1111111111111111ULL, 16);
	test_popcount<uint64_t>(0x2222222222222222ULL, 16);
	test_popcount<uint64_t>(0x3333333333333333ULL, 32);
	test_popcount<uint64_t>(0x4444444444444444ULL, 16);
	test_popcount<uint64_t>(0x5555555555555555ULL, 32);
	test_popcount<uint64_t>(0x6666666666666666ULL, 32);
	test_popcount<uint64_t>(0x7777777777777777ULL, 48);
	test_popcount<uint64_t>(0x8888888888888888ULL, 16);
	test_popcount<uint64_t>(0x9999999999999999ULL, 32);
	test_popcount<uint64_t>(0xaaaaaaaaaaaaaaaaULL, 32);
	test_popcount<uint64_t>(0xbbbbbbbbbbbbbbbbULL, 48);
	test_popcount<uint64_t>(0xccccccccccccccccULL, 32);
	test_popcount<uint64_t>(0xddddddddddddddddULL, 48);
	test_popcount<uint64_t>(0xeeeeeeeeeeeeeeeeULL, 48);
	test_popcount<uint64_t>(0xffffffffffffffffULL, 64);
	test_popcount<uint64_t>(0x1000000000000000ULL, 1);
	test_popcount<uint64_t>(0x0c0c0c0c0c0c0c0cULL, 16);
	test_popcount<uint64_t>(0xccccccccccccccccULL, 32);
	test_popcount<uint64_t>(0xf000000000000000ULL, 4);
	test_popcount<uint64_t>(0x0f0f0f0f0f0f0f0fULL, 32);
	test_popcount<uint64_t>(0xf0f0f0f0f0f0f0f0ULL, 32);
	test_popcount<uint64_t>(0xffffffffffffffffULL, 64);
}

void test_clz()
{
	test_clz<uint16_t>(0x0, 16);
	test_clz<uint16_t>(0x0, 16);
	test_clz<uint16_t>(0x1, 15);
	test_clz<uint16_t>(0x2, 14);
	test_clz<uint16_t>(0x4, 13);
	test_clz<uint16_t>(0x5, 13);
	test_clz<uint16_t>(0x6, 13);
	test_clz<uint16_t>(0x7, 13);
	test_clz<uint16_t>(0x8, 12);
	test_clz<uint16_t>(0x7fff, 1);
	test_clz<uint16_t>(0x4000, 1);
	test_clz<uint16_t>(0xffff, 0);
	test_clz<uint16_t>(0x8000, 0);
	test_clz<uint32_t>(0x0, 32);
	test_clz<uint32_t>(0x0, 32);
	test_clz<uint32_t>(0x1, 31);
	test_clz<uint32_t>(0x2, 30);
	test_clz<uint32_t>(0x4, 29);
	test_clz<uint32_t>(0x5, 29);
	test_clz<uint32_t>(0x6, 29);
	test_clz<uint32_t>(0x7, 29);
	test_clz<uint32_t>(0x8, 28);
	test_clz<uint32_t>(0x7fffffff, 1);
	test_clz<uint32_t>(0x40000000, 1);
	test_clz<uint32_t>(0xffffffff, 0);
	test_clz<uint32_t>(0x80000000, 0);
	test_clz<uint64_t>(0x0, 64);
	test_clz<uint64_t>(0x0, 64);
	test_clz<uint64_t>(0x1, 63);
	test_clz<uint64_t>(0x2, 62);
	test_clz<uint64_t>(0x4, 61);
	test_clz<uint64_t>(0x5, 61);
	test_clz<uint64_t>(0x6, 61);
	test_clz<uint64_t>(0x7, 61);
	test_clz<uint64_t>(0x8, 60);
	test_clz<uint64_t>(0x000000007fffffffULL, 33);
	test_clz<uint64_t>(0x00000000ffffffffULL, 32);
	test_clz<uint64_t>(0x00000001ffffffffULL, 31);
	test_clz<uint64_t>(0x7fffffffffffffffULL, 1);
	test_clz<uint64_t>(0x4000000000000000ULL, 1);
	test_clz<uint64_t>(0xffffffffffffffffULL, 0);
	test_clz<uint64_t>(0x8000000000000000ULL, 0);
}

void test_ctz()
{
	test_ctz<uint16_t>(0x0, 16);
	test_ctz<uint16_t>(0x0, 16);
	test_ctz<uint16_t>(0x1, 0);
	test_ctz<uint16_t>(0x2, 1);
	test_ctz<uint16_t>(0x4, 2);
	test_ctz<uint16_t>(0x5, 0);
	test_ctz<uint16_t>(0x6, 1);
	test_ctz<uint16_t>(0x7, 0);
	test_ctz<uint16_t>(0x8, 3);
	test_ctz<uint16_t>(0x7fff, 0);
	test_ctz<uint16_t>(0x4000, 14);
	test_ctz<uint16_t>(0xffff, 0);
	test_ctz<uint16_t>(0x8000, 15);
	test_ctz<uint32_t>(0x0, 32);
	test_ctz<uint32_t>(0x0, 32);
	test_ctz<uint32_t>(0x1, 0);
	test_ctz<uint32_t>(0x2, 1);
	test_ctz<uint32_t>(0x4, 2);
	test_ctz<uint32_t>(0x5, 0);
	test_ctz<uint32_t>(0x6, 1);
	test_ctz<uint32_t>(0x7, 0);
	test_ctz<uint32_t>(0x8, 3);
	test_ctz<uint32_t>(0x7fffffff, 0);
	test_ctz<uint32_t>(0x40000000, 30);
	test_ctz<uint32_t>(0xffffffff, 0);
	test_ctz<uint32_t>(0x80000000, 31);
	test_ctz<uint64_t>(0x0, 64);
	test_ctz<uint64_t>(0x0, 64);
	test_ctz<uint64_t>(0x1, 0);
	test_ctz<uint64_t>(0x2, 1);
	test_ctz<uint64_t>(0x4, 2);
	test_ctz<uint64_t>(0x5, 0);
	test_ctz<uint64_t>(0x6, 1);
	test_ctz<uint64_t>(0x7, 0);
	test_ctz<uint64_t>(0x8, 3);
	test_ctz<uint64_t>(0xffffffff80000000ULL, 31);
	test_ctz<uint64_t>(0xffffffff00000000ULL, 32);
	test_ctz<uint64_t>(0xfffffffe00000000ULL, 33);
	test_ctz<uint64_t>(0x7fffffffffffffffULL, 0);
	test_ctz<uint64_t>(0x4000000000000000ULL, 62);
	test_ctz<uint64_t>(0xffffffffffffffffULL, 0);
	test_ctz<uint64_t>(0x8000000000000000ULL, 63);
}

void bits_run_tests()
{
	test_limits();
	test_binary_string();
	test_add_overflow();
	test_sub_underflow();
	test_ispow2();
	test_roundpow2();
	test_bitextend();
	test_popcount();
	test_clz();
	test_ctz();
}

int main(int argc, const char * argv[])
{
	bits_run_tests();
}
