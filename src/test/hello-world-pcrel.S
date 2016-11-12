.section .text
.globl _start
_start:

	li a0, 0                    # stdout
1:	auipc a1, %pcrel_hi(msg)    # load msg(hi)
	addi a1, a1, %pcrel_lo(1b)  # load msg(lo)
	li a2, 12                   # length
	li a3, 0
	li a7, 64                   # _NR_sys_write
	ecall                       # system call

	li a0, 0
	li a1, 0
	li a2, 0
	li a3, 0
	li a7, 93                   # _NR_sys_exit
	ecall                       # system call

loop:
	j loop

.section .rodata
msg:
	.string "Hello World\n"
