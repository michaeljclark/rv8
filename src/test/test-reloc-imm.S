.section .text
.globl _start
_start:

	lui t0, %hi(foo)
	lw  t0, %lo(foo)(t0)

	lui t0, %hi(bar)
	lw  t0, %lo(bar)(t0)

	li a0, 0
	li a1, 0
	li a2, 0
	li a3, 0
	li a7, 93                   # _NR_sys_exit
	ecall                       # system call

.section .data
foo:	.word 99
bar:	.word 101
