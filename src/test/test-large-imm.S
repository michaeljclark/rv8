.section .text
.globl _start
_start:

#if defined (__riscv64) || __riscv_xlen == 64
	li t0, 9999999999999999999
#endif
#if defined (__riscv32) || __riscv_xlen == 32
	li t0, 999999999
#endif
	li a0, 0
	li a1, 0
	li a2, 0
	li a3, 0
	li a7, 93                   # _NR_sys_exit
	ecall                       # system call

loop:
	j loop
