.section .text
.globl _start
_start:

# setup mepc
1:	auipc   t0, %pcrel_hi(user)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mepc, t0

# return to user mode
	mret

user:
	ebreak
