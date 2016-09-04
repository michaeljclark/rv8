//
//  riscv-host.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>
#include <map>

#include "riscv-util.h"
#include "riscv-host.h"

using namespace riscv;

/* BSD/UNIX/Linux */

#if defined(__FreeBSD__) || defined(__NetBSD__) || \
	defined(__DragonFly__) || defined(__OpenBSD__) || \
	defined (__APPLE__) || defined(__unix__) || defined(__linux__)

#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/stat.h>

#define HAVE_DEV_URANDOM 1

#endif

/* GCC x86/x86-64 */

#if defined __GNUC__ && (defined __i386__ || defined __x86_64__)
#define HAS_X86_CPUID 1
#include <cpuid.h>

inline void x86_cpuid(int reg[], int level)
{
	__cpuid(level, reg[0], reg[1], reg[2], reg[3]);
}

inline void x86_cpuid_count(int reg[], int level, int count)
{
	__cpuid_count(level, count, reg[0], reg[1], reg[2], reg[3]);
}

#elif defined _MSC_VER && (defined _M_IX86 || defined _M_X64)

/* MSC x86/x86-64 */

#define HAS_X86_CPUID 1
#define x86_cpuid __cpuid
#define x86_cpuid_count __cpuidex

#endif

/* CPUID x86/x86-64 */

#ifdef HAS_X86_CPUID

#define X86_HAS_SSE         x86_cpuid_h1.r.edx & (1 << 25)
#define X86_HAS_SSE2        x86_cpuid_h1.r.edx & (1 << 26)
#define X86_HAS_SSE3        x86_cpuid_h1.r.ecx & (1 << 0)
#define X86_HAS_SSSE3       x86_cpuid_h1.r.ecx & (1 << 9)
#define X86_HAS_FMA         x86_cpuid_h1.r.ecx & (1 << 12)
#define X86_HAS_CX16        x86_cpuid_h1.r.ecx & (1 << 13)
#define X86_HAS_SSE4_1      x86_cpuid_h1.r.ecx & (1 << 19)
#define X86_HAS_SSE4_2      x86_cpuid_h1.r.ecx & (1 << 20)
#define X86_HAS_MOVBE       x86_cpuid_h1.r.ecx & (1 << 22)
#define X86_HAS_POPCNT      x86_cpuid_h1.r.ecx & (1 << 23)
#define X86_HAS_AES         x86_cpuid_h1.r.ecx & (1 << 25)
#define X86_HAS_AVX         x86_cpuid_h1.r.ecx & (1 << 28)
#define X86_HAS_RDRAND      x86_cpuid_h1.r.ecx & (1 << 30)
#define X86_HAS_LAHF_LM     x86_cpuid_h80000001.r.ecx & (1 << 0)
#define X86_HAS_ABM         x86_cpuid_h80000001.r.ecx & (1 << 5)
#define X86_HAS_PREFETCHW   x86_cpuid_h80000001.r.ecx & (1 << 8)
#define X86_HAS_BMI1        x86_cpuid_h7_c0.r.ebx & (1 << 3)
#define X86_HAS_AVX2        x86_cpuid_h7_c0.r.ebx & (1 << 5)
#define X86_HAS_BMI2        x86_cpuid_h7_c0.r.ebx & (1 << 8)
#define X86_HAS_MPX         x86_cpuid_h7_c0.r.ebx & (1 << 14)
#define X86_HAS_AVX512F     x86_cpuid_h7_c0.r.ebx & (1 << 16)
#define X86_HAS_AVX512DQ    x86_cpuid_h7_c0.r.ebx & (1 << 17)
#define X86_HAS_RDSEED      x86_cpuid_h7_c0.r.ebx & (1 << 18)
#define X86_HAS_ADX         x86_cpuid_h7_c0.r.ebx & (1 << 19)
#define X86_HAS_AVX512IFMA  x86_cpuid_h7_c0.r.ebx & (1 << 21)
#define X86_HAS_AVX512PF    x86_cpuid_h7_c0.r.ebx & (1 << 26)
#define X86_HAS_AVX512ER    x86_cpuid_h7_c0.r.ebx & (1 << 27)
#define X86_HAS_AVX512CD    x86_cpuid_h7_c0.r.ebx & (1 << 28)
#define X86_HAS_SHA         x86_cpuid_h7_c0.r.ebx & (1 << 29)
#define X86_HAS_AVX512BW    x86_cpuid_h7_c0.r.ebx & (1 << 30)
#define X86_HAS_AVX512VL    x86_cpuid_h7_c0.r.ebx & (1 << 31)
#define X86_HAS_PREFETCHWT2 x86_cpuid_h7_c0.r.ecx & (1 << 0)
#define X86_HAS_AVX512VBMI  x86_cpuid_h7_c0.r.ecx & (1 << 1)

union x86_cpuid_info
{
	struct
	{
		int eax;
		int ebx;
		int ecx;
		int edx;
	} r;
	int a[4];
	char s[16];
};

static char x86_cpu_vendor[13];
static x86_cpuid_info x86_cpuid_h0;
static x86_cpuid_info x86_cpuid_h1;
static x86_cpuid_info x86_cpuid_h7_c0;
static x86_cpuid_info x86_cpuid_h80000001;

#endif


/*
 * host_cpu
 */

bool host_cpu::enable_debug = false;

host_cpu& host_cpu::get_instance()
{
	/* thread safe static initializer pattern */
	static host_cpu singleton;
	return singleton;
}

host_cpu::host_cpu()
{
#if HAS_X86_CPUID
	/* zero x86 capabilities */
	memset(&x86_cpuid_h0, 0, sizeof(x86_cpuid_info));
	memset(&x86_cpuid_h1, 0, sizeof(x86_cpuid_info));
	memset(&x86_cpuid_h7_c0, 0, sizeof(x86_cpuid_info));
	memset(&x86_cpuid_h80000001, 0, sizeof(x86_cpuid_info));

	/* fetch x86 capabilities */
	x86_cpuid(x86_cpuid_h0.a, 0);
	x86_cpuid(x86_cpuid_h1.a, 1);
	x86_cpuid(x86_cpuid_h80000001.a, 0x80000001);
	if (x86_cpuid_h0.r.eax >= 7) {
		x86_cpuid_count(x86_cpuid_h7_c0.a, 7, 0);
	}

	/* fetch x86 cpu vendor */
	memcpy(x86_cpu_vendor, x86_cpuid_h0.s + 4, 4);
	memcpy(x86_cpu_vendor + 4, x86_cpuid_h0.s + 12, 4);
	memcpy(x86_cpu_vendor + 8, x86_cpuid_h0.s + 8, 4);
	x86_cpu_vendor[12] = '\0';

	/* < Haswell capabilities */
	caps["SSE"] = X86_HAS_SSE;
	caps["SSE2"] = X86_HAS_SSE2;
	caps["SSE3"] = X86_HAS_SSE3;
	caps["SSSE3"] = X86_HAS_SSSE3;
	caps["FMA"] = X86_HAS_FMA;
	caps["CX16"] = X86_HAS_CX16;
	caps["SSE4_1"] = X86_HAS_SSE4_1;
	caps["SSE4_2"] = X86_HAS_SSE4_2;
	caps["MOVBE"] = X86_HAS_MOVBE;
	caps["POPCNT"] = X86_HAS_POPCNT;
	caps["AES"] = X86_HAS_AES;
	caps["AVX"] = X86_HAS_AVX;
	caps["RDRAND"] = X86_HAS_RDRAND;
	caps["LAHF_LM"] = X86_HAS_LAHF_LM;
	caps["PREFETCHW"] = X86_HAS_PREFETCHW;

	/* >= Haswell capabilities */
	caps["ABM"] = X86_HAS_ABM;
	caps["BMI1"] = X86_HAS_BMI1;
	caps["AVX2"] = X86_HAS_AVX2;
	caps["BMI2"] = X86_HAS_BMI2;
	caps["MPX"] = X86_HAS_MPX;
	caps["AVX512F"] = X86_HAS_AVX512F;
	caps["AVX512DQ"] = X86_HAS_AVX512DQ;
	caps["RDSEED"] = X86_HAS_RDSEED;
	caps["ADX"] = X86_HAS_ADX;
	caps["AVX512IFMA"] = X86_HAS_AVX512IFMA;
	caps["AVX512PF"] = X86_HAS_AVX512PF;
	caps["AVX512ER"] = X86_HAS_AVX512ER;
	caps["AVX512CD"] = X86_HAS_AVX512CD;
	caps["SHA"] = X86_HAS_SHA;
	caps["AVX512BW"] = X86_HAS_AVX512BW;
	caps["AVX512VL"] = X86_HAS_AVX512VL;
	caps["PREFETCHWT2"] = X86_HAS_PREFETCHWT2;
	caps["AVX512VBMI"] = X86_HAS_AVX512VBMI;

	/* print x86 capabilities */
	if (host_cpu::enable_debug) {
		printf("%s ", x86_cpu_vendor);
		for (auto cap : caps) {
			if (cap.second) printf("%s ", cap.first.c_str());
		}
		printf("\n");
	}
#endif
}


/*
 * get_cryptgenrandom_seed
 *
 * Supports Win32
 */

#ifdef WIN32

#define HAVE_CRYPTGENRANDOM 1

#include <windows.h>
#pragma comment(lib, "advapi32.lib")

static int get_cryptgenrandom_seed()
{
	if (host_cpu::enable_debug) debug("get_cryptgenrandom_seed");

	HCRYPTPROV hProvider = 0;
	int r;

	if (!CryptAcquireContextW(&hProvider, 0, 0, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT | CRYPT_SILENT)) {
		fprintf(stderr, "error CryptAcquireContextW");
		exit(1);
	}

	if (!CryptGenRandom(hProvider, sizeof(r), (BYTE*)&r)) {
		fprintf(stderr, "error CryptGenRandom");
		exit(1);
	}

	CryptReleaseContext(hProvider, 0);

	return r;
}

#endif


/*
 * rdrand
 * 
 * Supports GCC x86 and X64
 * Does not support MSC (MSC x64 has no inline asm)
 */

#if HAS_X86_CPUID

static int has_rdrand()
{
	// CPUID.01H:ECX.RDRAND[bit 30] == 1
	int regs[4];
	x86_cpuid(regs, 1);
	return (regs[2] & (1 << 30)) != 0;
}

#endif

/* get_rdrand_seed - GCC x86 and X64 */

#if defined __GNUC__ && (defined __i386__ || defined __x86_64__)

#define HAVE_RDRAND 1

static uint32_t get_rdrand_seed()
{
	if (host_cpu::enable_debug) debug("get_rdrand_seed");

	int _eax;
	// rdrand eax
	__asm__ __volatile__("1: .byte 0x0F\n"
						 "   .byte 0xC7\n"
						 "   .byte 0xF0\n"
						 "   jnc 1b;\n"
						 : "=a" (_eax));
	return _eax;
}

#endif

#if defined _MSC_VER

#if _MSC_VER >= 1700
#define HAVE_RDRAND 1

/* get_rdrand_seed - Visual Studio 2012 and above */

static uint32_t get_rdrand_seed()
{
	if (host_cpu::enable_debug) debug("get_rdrand_seed");

	int r;
	while (_rdrand32_step(&r) == 0);
	return r;
}

#elif defined _M_IX86
#define HAVE_RDRAND 1

/* get_rdrand_seed - Visual Studio 2010 and below - x86 only */

static uint32_t get_rdrand_seed()
{
	if (host_cpu::enable_debug) debug("get_rdrand_seed");

	int _eax;
retry:
	// rdrand eax
	__asm _emit 0x0F __asm _emit 0xC7 __asm _emit 0xF0
	__asm jnc retry
	__asm mov _eax, eax
	return _eax;
}

#endif
#endif


/*
 * /dev/urandom
 */

#ifdef HAVE_DEV_URANDOM

static const char *dev_random_file = "/dev/urandom";

static uint32_t has_dev_urandom()
{
	struct stat buf;
	if (stat(dev_random_file, &buf)) {
		return 0;
	}
	return ((buf.st_mode & S_IFCHR) != 0);
}

static uint32_t get_dev_urandom_seed()
{
	if (host_cpu::enable_debug) debug("get_dev_urandom_seed");

	int fd = open(dev_random_file, O_RDONLY);
	if (fd < 0) {
		fprintf(stderr, "error opening %s: %s", dev_random_file, strerror(errno));
		exit(1);
	}

	uint32_t r;
	ssize_t nread = read(fd, &r, sizeof(r));
	if (nread != sizeof(r)) {
		fprintf(stderr, "error read %s: %s", dev_random_file, strerror(errno));
		exit(1);
	}
	else if (nread != sizeof(r)) {
		fprintf(stderr, "error short read %s", dev_random_file);
		exit(1);
	}
	close(fd);
	return r;
}

#endif


uint32_t host_cpu::get_random_seed()
{
#if HAVE_RDRAND
	if (has_rdrand()) return get_rdrand_seed();
#endif
#if HAVE_DEV_URANDOM
	if (has_dev_urandom()) return get_dev_urandom_seed();
#endif
#if HAVE_CRYPTGENRANDOM
	return get_cryptgenrandom_seed();
#endif
	panic("no random source present");
	return 5; /* woot */
}
