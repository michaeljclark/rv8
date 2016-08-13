//
//  riscv-host.h
//

#ifndef riscv_host_h
#define riscv_host_h

#if defined _MSC_VER
#include <intrin.h>
#define __func__  __FUNCTION__
#define snprintf _snprintf
#endif

#if defined(__GNUC__) && defined(__ARM_ARCH_7A__)
#include <arm_neon.h>
#endif

namespace riscv {

    /* cpu_cycle_clock - (lfence requires SSE, >= Pentium III)*/
    #if defined _MSC_VER
    #if defined _M_IX86
    #define HAVE_CPU_CYCLE_CLOCK 1
    inline uint64_t cpu_cycle_clock(void)
    {
        uint64_t c;
        __asm {
            lfence
            rdtsc
            mov dword ptr [c + 0], eax
            mov dword ptr [c + 4], edx
        }
        return c;
    }
    #elif defined _M_X64
    #define HAVE_CPU_CYCLE_CLOCK 1
    inline uint64_t cpu_cycle_clock() { _mm_lfence(); return __rdtsc(); }
    #endif
    #endif

    #if defined __GNUC__
    #if defined __i386__ || defined __x86_64__
    #define HAVE_CPU_CYCLE_CLOCK 1
    inline uint64_t __attribute__((__always_inline__)) cpu_cycle_clock()
    {
        uint32_t a, d;
        __asm__ volatile ("lfence\n"
                          "rdtsc\n" : "=a" (a), "=d" (d));
        return uint64_t(d)<<32 | a;
    }
    #endif
    #endif

    #if !defined(HAVE_CPU_CYCLE_CLOCK) && defined(__APPLE__)
    #define HAVE_CPU_CYCLE_CLOCK 1
    #include <mach/mach_time.h>
    inline uint64_t __attribute__((__always_inline__)) cpu_cycle_clock()
    {
        return mach_absolute_time();
    }
    #endif

    #if !HAVE_CPU_CYCLE_CLOCK
    inline uint64_t __attribute__((__always_inline__)) cpu_cycle_clock()
    {
        return 0;
    }
    #endif

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

    extern char x86_cpu_vendor[13];
    extern x86_cpuid_info x86_cpuid_h0;
    extern x86_cpuid_info x86_cpuid_h1;
    extern x86_cpuid_info x86_cpuid_h7_c0;
    extern x86_cpuid_info x86_cpuid_h80000001;
    extern std::map<std::string,uint32_t> x86_caps;

    void x86_cpu_init();

}

#endif
