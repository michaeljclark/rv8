#
# test-m-mmio-timer
#

.equ RTC_BASE,      0x40000000
.equ TIMER_BASE,    0x40004000
.equ HTIF_TOHOST,   0x40008000
.equ UART_BASE,     0x40003000
.equ REG_RBR, 0
.equ REG_TBR, 0
.equ REG_IIR, 2
.equ IIR_TX_RDY, 2
.equ IIR_RX_RDY, 4

.section .text
.globl _start
_start:

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
	li      a0, RTC_BASE
	ld      a1, 0(a0)

# write to mtimecmp
	li      a0, TIMER_BASE
	li      t0, 1000000000
	add     a1, a1, t0
	sd      a1, 0(a0)

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
