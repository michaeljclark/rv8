.section .text
.globl _start
_start:

	j reset
	nop
	nop

.word config

reset:
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0
	ecall

mtvec:
	ebreak

config:
	.incbin "src/rom/config.rv"

	.p2align 12
