.section .text
.globl _start
_start:

# RAM is at 0x8000_0000 - 0xc000_0000
# mtime is at 0x4000_0000
# mtimecmp is at 0x4000_0008

# setup trap stack at top of memory
	li      sp, 0xbffffff8
	csrrw   zero, mscratch, sp

# setup stack 1MB below top of memory
	li      sp, 0xbfeffff8

# setup mtvec
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

# set mstatus.MIE=1 (enable M mode interrupt)
	li      t0, 1
	slli    t0, t0, 3
	csrrs   zero, mstatus, t0

# set mie.MTIE=1 (enable M mode timer interrupts)
	li      t0, 1
	slli    t0, t0, 7
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
	li      a0, 0xdabadabad00
	ebreak
