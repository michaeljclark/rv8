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

# set mie.MEIE=1 (enable M mode external interrupts)
	li      t0, 1
	slli    t0, t0, 11
	csrrs   zero, mie, t0

# program uart to raise interrupts on recieved data
	li      a0, 0x40003000
	li      a1, 1           # Load IER_ERBDA bit
	sb      a1, 1(a0)       # Set IER_ERBDA bit in IER register

# write msg to uart
1:	auipc   a1, %pcrel_hi(msg)     # load msg(hi)
	addi    a1, a1, %pcrel_lo(1b)  # load msg(lo)
print:
	lb      t0, 0(a1)
	beqz    t0, loop
	sb      t0, 0(a0)
	addi    a1, a1, 1
	j       print

# loop
loop:
	wfi
	j loop


# break on interrupt
mtvec:
# echo character from uart
	li      a0, 0x40003000
	lb      a1, 0(a0)
	sb      a1, 0(a0)

# clear pending interrupt in the PLIC
	li      a0, 0x40002000
	ld      a1, 0(a0)       # read interrupt
	sd      a1, 0(a0)       # acknowledge
	mret                    # return to faulting instruction

.section .rodata
msg:
        .string "UART echo demo v0.0.0\n$ "
