.section .text
.globl _start

.equ HTIF_TOHOST, 0x40008000

_start:
1:	auipc a0, %pcrel_hi(msg)    # load msg(hi)
	addi a0, a0, %pcrel_lo(1b)  # load msg(lo)
	jal ra, puts
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
msg:
	.string "HTIF Console Output Demo\n"
