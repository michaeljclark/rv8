.section .text
.globl _start
_start:

.equ UART_BASE, 0x40003000
.equ REG_RBR, 0
.equ REG_TBR, 0
.equ REG_IIR, 2
.equ IIR_TX_RDY, 2
.equ IIR_RX_RDY, 4

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
	li a2, UART_BASE
1:	lbu a1, (a0)
	beqz a1, 3f
2:	lbu a3, REG_IIR(a2)
	andi a3, a3, IIR_TX_RDY
	beqz a3, 2b
	sb a1, REG_TBR(a2)
	addi a0, a0, 1
	j 1b
3:	ret

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
