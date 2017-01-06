#
# test-m-mmio-htif
#

.equ UART_BASE, 0x40003000
.equ REG_RBR, 0
.equ REG_TBR, 0
.equ REG_IIR, 2
.equ IIR_TX_RDY, 2
.equ IIR_RX_RDY, 4

.equ HTIF_TOHOST,   0x40008000

.section .text
.globl _start
_start:

1:	auipc a0, %pcrel_hi(msg)    # load msg(hi)
	addi a0, a0, %pcrel_lo(1b)  # load msg(lo)
2:	jal ra, puts

3:	jal ra, getc
	beqz a0, 3b
	jal ra, putc
	j 3b

getc:
	li a2, UART_BASE
1:	lbu a3, REG_IIR(a2)
	andi a3, a3, IIR_RX_RDY
	beqz a3, 1b
	lbu a1, REG_RBR(a2)
	ret

putc:
	li a2, UART_BASE
1:	lbu a3, REG_IIR(a2)
	andi a3, a3, IIR_TX_RDY
	beqz a3, 1b
	sb a1, REG_TBR(a2)
	ret

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
msg:
	.string "UART Console Poll Demo v0.0.0\n$ "
