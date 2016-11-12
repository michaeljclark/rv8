.section .text
.globl _start
_start:

# RAM is at 0x8000_0000 - 0xc000_0000

# setup trap stack at top of memory
	li      sp, 0xbffffff8
	csrrw   zero, mscratch, sp

# setup stack 1MB below top of memory
	li      sp, 0xbfeffff8

# setup mtvec
1:	auipc   a0, %pcrel_hi(mtvec)       # load mtvec(hi)
	addi    a0, a0, %pcrel_lo(1b)      # load mtvec(lo)
	csrrw   zero, mtvec, sp

# load sptbr
1:	auipc   a0, %pcrel_hi(sv39_pte_0)  # load sv39_pte_0(hi)
	addi    a0, a0, %pcrel_lo(1b)      # load sv39_pte_0(lo)
	csrrw   zero, sptbr, a0

# set mstatus.VM=sv39
	csrrsi  a0, mstatus, 0
	li      a1, 9
	slli    a1, a1, 24
	or      a0, a0, a1
	csrrw   zero, mstatus, a0

# set mstatus.MPRV=1 (enable M mode paging)
	csrrsi  a0, mstatus, 0
	li      a1, 1
	slli    a1, a1, 17
	or      a0, a0, a1
	csrrw   zero, mstatus, a0

# write 0xff to 0x1000
	li      a0, 0x1000
	li      a2, 0xcc
	sb      a2, 0(a0)

# set mstatus.MPRV=0 (disable M mode paging)
	li      a1, 1
	slli    a1, a1, 17
	or      a0, a0, a1
	csrrc   zero, mstatus, a0

# read from 0x80001000 and compare to 0xff
	li      a0, 0x80001000
	lbu     a1, 0(a0)
	bne     a1, a2, fail

# success
	li      a0, 0
	ebreak

# failure
fail:
	li      a0, 1
	ebreak

# trap vector
mtvec:
	csrrw sp, mscratch, sp
	ebreak

.section .rodata
.p2align 12
sv39_pte_0:
	# riscv-pte-util sv39 0x80000000 URWV
	.dword 0x20000017   # 0x0000 -> 0x8000_0000 - 0xC000_0000
