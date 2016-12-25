.section .text
.globl _start
_start:

.equ HTIF_TOHOST, 0x40008000

# setup mtvec
1:	auipc   a0, %pcrel_hi(mtvec)       # load mtvec(hi)
	addi    a0, a0, %pcrel_lo(1b)      # load mtvec(lo)
	csrrw   zero, mtvec, a0

# trap test
	ecall
	j pass

# simple trap vector example
mtvec:
	csrrc  t0, mcause, zero
	li     t1, 11
	bne    t0, t1, fail

	# increment past the trapping instruction
	csrrsi  a2, mepc, 0
	addi    a2, a2, 4
	csrrw   zero, mepc, a2

	# return
	mret

pass:
	la a0, pass_msg
	jal puts
	j shutdown

fail:
	la a0, fail_msg
	jal puts
	j shutdown

puts:
	li a2, HTIF_TOHOST
	li a3, 0x01010000
1:	lbu a1, (a0)
	beqz a1, 2f
	sw a1, 0(a2)
	sw a3, 4(a2)
3:	lw a1, 0(a2)
	lw a4, 4(a2)
	or a1, a1, a4
	bnez a1, 3b
	addi a0, a0, 1
	j 1b
2:	ret

shutdown:
	li a2, HTIF_TOHOST
	li a1, 1
	sw a1, 0(a2)
	sw zero, 4(a2)
1: 	wfi
	j 1b

.section .data

pass_msg:
	.string "PASS\n"

fail_msg:
	.string "FAIL\n"
