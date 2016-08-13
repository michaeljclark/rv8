//
//  riscv-host.cc
//

#include <cstdio>
#include <cstdint>
#include <cstring>
#include <string>
#include <map>

#include "riscv-host.h"

using namespace riscv;

/*
 * Intel CPUID capabilities
 *
 * See: http://en.wikipedia.org/wiki/CPUID
 *
 * CPUID.EAX=1:EDX.SSE[bit 25] = 1
 * CPUID.EAX=1:EDX.SSE2[bit 26] = 1
 * CPUID.EAX=1:ECX.SSE3[bit 0] = 1
 * CPUID.EAX=1:ECX.SSSE3[bit 9] = 1
 * CPUID.EAX=1:ECX.FMA[bit 12] = 1
 * CPUID.EAX=1:ECX.CX16[bit 13] = 1               CMPXCHG16B instruction (128-bit cmpxchg)
 * CPUID.EAX=1:ECX.SSE4_1[bit 19] = 1
 * CPUID.EAX=1:ECX.SSE4_2[bit 20] = 1
 * CPUID.EAX=1:ECX.MOVBE[Bit 22] = 1              Big-endian byteswap move instruction
 * CPUID.EAX=1:ECX.POPCNT[Bit 23] = 1
 * CPUID.EAX=1:ECX.AES[Bit 25] = 1
 * CPUID.EAX=1:ECX.AVX[Bit 28] = 1
 * CPUID.EAX=1:ECX.RDRAND[bit 30] = 1
 * CPUID.EAX=7.ECX=0:EBX.BMI1[bit 3] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX2[bit 5] = 1
 * CPUID.EAX=7.ECX=0:EBX.BMI2[bit 8] = 1
 * CPUID.EAX=7.ECX=0:EBX.MPX[bit 14] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX512F[bit 16] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX512DQ[bit 17] = 1
 * CPUID.EAX=7.ECX=0:EBX.RDSEED[bit 18] = 1
 * CPUID.EAX=7.ECX=0:EBX.ADX[bit 19] = 1          Multi-Precision Add-Carry Instruction Extensions
 * CPUID.EAX=7.ECX=0:EBX.AVX512IFMA[bit 21] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX512PF[bit 26] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX512ER[bit 27] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX512CD[bit 28] = 1
 * CPUID.EAX=7.ECX=0:EBX.SHA[bit 29] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX512BW[bit 30] = 1
 * CPUID.EAX=7.ECX=0:EBX.AVX512VL[bit 31] = 1
 * CPUID.EAX=7.ECX=0:ECX.PREFETCHWT2[bit 0] = 1
 * CPUID.EAX=7.ECX=0:ECX.AVX512VBMI[bit 1] = 1
 * CPUID.EAX=80000001H:ECX.LAHF_LM[bit 0] = 1     LAHF/SAHF
 * CPUID.EAX=80000001H:ECX.ABM[bit 5] = 1         LZCNT
 * CPUID.EAX=80000001H:ECX.PREFETCHW[bit 8] = 1
 *
 * Intel® 64 and IA-32 Architectures Software Developer’s Manual, Vol. 2A, 3-86
 * The result of lzcnt is 31 minus the result of bsr (bit scan reverse), except when the input is 0
 *
 * CPU topology enumeration
 *
 * https://software.intel.com/en-us/articles/intel-64-architecture-processor-topology-enumeration
 * http://developer.amd.com/resources/documentation-articles/articles-whitepapers/processor-and-core-enumeration-using-cpuid/
 */

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

#define HAS_X86_CPUID 1
#define x86_cpuid __cpuid
#define x86_cpuid_count __cpuidex

#endif

char riscv::x86_cpu_vendor[13];
x86_cpuid_info riscv::x86_cpuid_h0;
x86_cpuid_info riscv::x86_cpuid_h1;
x86_cpuid_info riscv::x86_cpuid_h7_c0;
x86_cpuid_info riscv::x86_cpuid_h80000001;
std::map<std::string,uint32_t> riscv::x86_caps;

void riscv::x86_cpu_init()
{
    /* zero x86 capabilities */
    memset(&x86_cpuid_h0, 0, sizeof(x86_cpuid_info));
    memset(&x86_cpuid_h1, 0, sizeof(x86_cpuid_info));
    memset(&x86_cpuid_h7_c0, 0, sizeof(x86_cpuid_info));
    memset(&x86_cpuid_h80000001, 0, sizeof(x86_cpuid_info));

#if HAS_X86_CPUID
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
    x86_caps["SSE"] = X86_HAS_SSE;
    x86_caps["SSE2"] = X86_HAS_SSE2;
    x86_caps["SSE3"] = X86_HAS_SSE3;
    x86_caps["SSSE3"] = X86_HAS_SSSE3;
    x86_caps["FMA"] = X86_HAS_FMA;
    x86_caps["CX16"] = X86_HAS_CX16;
    x86_caps["SSE4_1"] = X86_HAS_SSE4_1;
    x86_caps["SSE4_2"] = X86_HAS_SSE4_2;
    x86_caps["MOVBE"] = X86_HAS_MOVBE;
    x86_caps["POPCNT"] = X86_HAS_POPCNT;
    x86_caps["AES"] = X86_HAS_AES;
    x86_caps["AVX"] = X86_HAS_AVX;
    x86_caps["RDRAND"] = X86_HAS_RDRAND;
    x86_caps["LAHF_LM"] = X86_HAS_LAHF_LM;
    x86_caps["PREFETCHW"] = X86_HAS_PREFETCHW;

    /* >= Haswell capabilities */
    x86_caps["ABM"] = X86_HAS_ABM;
    x86_caps["BMI1"] = X86_HAS_BMI1;
    x86_caps["AVX2"] = X86_HAS_AVX2;
    x86_caps["BMI2"] = X86_HAS_BMI2;
    x86_caps["MPX"] = X86_HAS_MPX;
    x86_caps["AVX512F"] = X86_HAS_AVX512F;
    x86_caps["AVX512DQ"] = X86_HAS_AVX512DQ;
    x86_caps["RDSEED"] = X86_HAS_RDSEED;
    x86_caps["ADX"] = X86_HAS_ADX;
    x86_caps["AVX512IFMA"] = X86_HAS_AVX512IFMA;
    x86_caps["AVX512PF"] = X86_HAS_AVX512PF;
    x86_caps["AVX512ER"] = X86_HAS_AVX512ER;
    x86_caps["AVX512CD"] = X86_HAS_AVX512CD;
    x86_caps["SHA"] = X86_HAS_SHA;
    x86_caps["AVX512BW"] = X86_HAS_AVX512BW;
    x86_caps["AVX512VL"] = X86_HAS_AVX512VL;
    x86_caps["PREFETCHWT2"] = X86_HAS_PREFETCHWT2;
    x86_caps["AVX512VBMI"] = X86_HAS_AVX512VBMI;

    /* print x86 capabilities */
    printf("%s ", x86_cpu_vendor);
    for (auto cap : x86_caps) {
        if (cap.second) printf("%s ", cap.first.c_str());
    }
    printf("\n");
#endif
}
