.section .text
.globl _start
_start:

reset:

	# temporary code until we have an SMP boot protocol

1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

	# use hart_id and LR/SC to choose boot processor
	# application processors go into a wfi loop

mtvec:
	ebreak

wait:
	wfi
	j wait

.p2align 12
