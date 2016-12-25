.section .text
.globl _start
_start:

.equ HTIF_TOHOST, 0x40008000

# mtime is at 0x4000_0000
# mtimecmp is at 0x4000_0008

# setup mtvec
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

# set mstatus.MIE=1 (enable M mode interrupt)
	li      t0, 8
	csrrs   zero, mstatus, t0

# set mie.MTIE=1 (enable M mode timer interrupts)
	li      t0, 128
	csrrs   zero, mie, t0

# read from mtime
	li      a0, 0x40000000
	ld      a1, 0(a0)

# write to mtimecmp
	li      t0, 1000000000
	add     a1, a1, t0
	sd      a1, 8(a0)

# loop
loop:
	wfi
	j loop

# break on interrupt
mtvec:
	csrrc  t0, mcause, zero
	bgez t0, fail       # interrupt causes are less than zero
	slli t0, t0, 1      # shift off high bit
	srli t0, t0, 1
	li t1, 7            # check this is an m_timer interrupt
	bne t0, t1, fail
	j pass

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
