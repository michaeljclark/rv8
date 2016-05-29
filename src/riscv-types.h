//
//  riscv-types.h
//

#ifndef riscv_types_h
#define riscv_types_h

#include <cstdint>

/* Types */

typedef uintptr_t riscv_ptr;
typedef int8_t riscv_b;
typedef uint8_t riscv_bu;
typedef int16_t riscv_h;
typedef uint16_t riscv_hu;
typedef int32_t riscv_w;
typedef uint32_t riscv_wu;
typedef int64_t riscv_l;
typedef uint64_t riscv_lu;
typedef double riscv_d;
typedef float riscv_s;

/* Registers */

union riscv_i_reg
{
	riscv_l l;
	riscv_lu lu;
	riscv_w w;
	riscv_wu wu;
};

union riscv_f_reg
{
	riscv_d d;
	riscv_s s;
};

#endif