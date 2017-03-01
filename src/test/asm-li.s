.section .text
.globl _start
_start:

	li a0, 0x0102030405060708
	slli a0, a1, 3
	lui a0, 0x100000
	ebreak
