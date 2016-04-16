//
//  riscv-regs.h
//

#ifndef riscv_regs_h
#define riscv_regs_h

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