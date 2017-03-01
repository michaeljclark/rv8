.section .text
.globl _start
_start:

	call func
	ebreak

func:
	ret
