#include "boot.h"

.section .text
.globl _start
_start:

reset:

	# setup mcall trap address
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

	# temporary code until we have an SMP boot protocol
	# use hart_id and LR/SC to choose boot processor
	# application processors go into a wfi loop

	# set mstatus.MPP = 0b11 (Machine mode)
	csrrs   t1, mstatus, zero
	li      t0, 3
	slli    t0, t0, 11
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

	# load ROM address from config MMIO region
	li      t0, CONFIG_ROM_ENTRY
	ld      ra, 0(t0)
	csrrw   zero, mepc, ra

	# return to the ROM
	mret

mtvec:
	csrrw sp, mscratch, sp
	ebreak

wait:
	wfi
	j wait

.p2align 12
