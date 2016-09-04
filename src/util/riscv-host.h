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

    struct host_cpu
    {
        static bool enable_debug;

        static host_cpu& get_instance();

        std::map<std::string,uint32_t> caps;

        host_cpu();

        uint32_t get_random_seed();
    };
}

#endif
