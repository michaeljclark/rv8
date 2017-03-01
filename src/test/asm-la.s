.section .text
.globl _start
_start:

	la a0, msg
	ebreak

.rodata
msg:
	.string "hello"
