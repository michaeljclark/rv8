//
//  riscv-regs.h
//

#ifndef riscv_regs_h
#define riscv_regs_h

extern const char* riscv_i_registers[];
extern const char* riscv_f_registers[];

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

enum riscv_ireg_name
{
 	riscv_ireg_zero, /* x0 - Hard-wired zero */
	riscv_ireg_ra,   /* x1 - Return address Caller */
	riscv_ireg_sp,   /* x2 - Stack pointer Callee */
	riscv_ireg_gp,   /* x3 - Global pointer */
	riscv_ireg_tp,   /* x4 - Thread pointer Callee */
	riscv_ireg_t0,   /* x5 - Temporaries Caller */
	riscv_ireg_t1,   /* x6 - Temporaries Caller */
	riscv_ireg_t2,   /* x7 - Temporaries Caller */
	riscv_ireg_s0,   /* x8 - Saved register/frame pointer Callee */
	riscv_ireg_s1,   /* x9 - Saved registers Callee */
	riscv_ireg_a0,   /* x10 - Function arguments Caller */
	riscv_ireg_a1,   /* x11 - Function arguments Caller */
	riscv_ireg_a2,   /* x12 - Function arguments Caller */
	riscv_ireg_a3,   /* x13 - Function arguments Caller */
	riscv_ireg_a4,   /* x14 - Function arguments Caller */
	riscv_ireg_a5,   /* x15 - Function arguments Caller */
	riscv_ireg_a6,   /* x16 - Function arguments Caller */
	riscv_ireg_a7,   /* x17 - Function arguments Caller */
	riscv_ireg_s2,   /* x18 - Saved registers Callee */
	riscv_ireg_s3,   /* x19 - Saved registers Callee */
	riscv_ireg_s4,   /* x20 - Saved registers Callee */
	riscv_ireg_s5,   /* x21 - Saved registers Callee */
	riscv_ireg_s6,   /* x22 - Saved registers Callee */
	riscv_ireg_s7,   /* x23 - Saved registers Callee */
	riscv_ireg_s8,   /* x24 - Saved registers Callee */
	riscv_ireg_s9,   /* x25 - Saved registers Callee */
	riscv_ireg_s10,  /* x26 - Saved registers Callee */
	riscv_ireg_s11,  /* x27 - Saved registers Callee */
	riscv_ireg_t3,   /* x28 - Temporaries Caller */
	riscv_ireg_t4,   /* x29 - Temporaries Caller */
	riscv_ireg_t5,   /* x30 - Temporaries Caller */
	riscv_ireg_t6    /* x31 - Temporaries Caller */
};

enum riscv_freg_name
{
	riscv_freg_ft0,  /* f0 - FP temporaries Caller */
	riscv_freg_ft1,  /* f1 - FP temporaries Caller */
	riscv_freg_ft2,  /* f2 - FP temporaries Caller */
	riscv_freg_ft3,  /* f3 - FP temporaries Caller */
	riscv_freg_ft4,  /* f4 - FP temporaries Caller */
	riscv_freg_ft5,  /* f5 - FP temporaries Caller */
	riscv_freg_ft6,  /* f6 - FP temporaries Caller */
	riscv_freg_ft7,  /* f7 - FP temporaries Caller */
	riscv_freg_fs0,  /* f8 - FP saved registers Callee */
	riscv_freg_fs1,  /* f9 - FP saved registers Callee */
	riscv_freg_fa0,  /* f10 - FP arguments Caller */
	riscv_freg_fa1,  /* f11 - FP arguments Caller */
	riscv_freg_fa2,  /* f12 - FP arguments Caller */
	riscv_freg_fa3,  /* f13 - FP arguments Caller */
	riscv_freg_fa4,  /* f14 - FP arguments Caller */
	riscv_freg_fa5,  /* f15 - FP arguments Caller */
	riscv_freg_fa6,  /* f16 - FP arguments Caller */
	riscv_freg_fa7,  /* f17 - FP arguments Caller */
	riscv_freg_fs2,  /* f18 - FP saved registers Callee */
	riscv_freg_fs3,  /* f19 - FP saved registers Callee */
	riscv_freg_fs4,  /* f20 - FP saved registers Callee */
	riscv_freg_fs5,  /* f21 - FP saved registers Callee */
	riscv_freg_fs6,  /* f22 - FP saved registers Callee */
	riscv_freg_fs7,  /* f23 - FP saved registers Callee */
	riscv_freg_fs8,  /* f24 - FP saved registers Callee */
	riscv_freg_fs9,  /* f25 - FP saved registers Callee */
	riscv_freg_fs10, /* f26 - FP saved registers Callee */
	riscv_freg_fs11, /* f27 - FP saved registers Callee */
	riscv_freg_ft8,  /* f28 - FP temporaries Caller */
	riscv_freg_ft9,  /* f29 - FP temporaries Caller */
	riscv_freg_ft10, /* f30 - FP temporaries Caller */
	riscv_freg_ft11  /* f31 - FP temporaries Caller */
};

#endif