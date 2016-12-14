.section .text
.globl _start
_start:

#ifdef __riscv64
	li t0, 9999999999999999999
#endif
#ifdef __riscv32
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
