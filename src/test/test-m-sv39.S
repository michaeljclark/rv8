.section .text
.globl _start
_start:

# RAM is at 0x8000_0000 - 0xc000_0000

# setup mtvec
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

#
# test load and store to RW page
#

# load sptbr
1:	auipc   t0, %pcrel_hi(sv39_pte_rw)  # load sv39_pte_rw(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load sv39_pte_rw(lo)
	csrrw   zero, sptbr, t0
	sfence.vm

# set mstatus.VM=sv39
	csrrsi  t1, mstatus, 0
	li      t0, 9
	slli    t0, t0, 24
	or      t1, t1, t0
	csrrw   zero, mstatus, t1

# set mstatus.MPRV=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 17
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

# write 0xff to 0x1000
	li      a0, 0x1000
	li      a2, 0xcc
	sb      a2, 0(a0)

# read from 0x1000
	lbu     a3, 0(a0)
	bne     a2, a3, fail

# set mstatus.PUM=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 18
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

# read from 0x1000
	lbu     a3, 0(a0)
	bne     a2, a3, fail   # instruction should cause load fault

# compare a7 to fault_load (5)
	li      t0, 5
	bne     a7, t0, fail

# set mstatus.PUM=0 (Disable M mode Paging)
	li      t1, 1
	slli    t1, t1, 18
	or      t0, t0, t1
	csrrc   zero, mstatus, t0

# set mstatus.MPRV=0 (Disable M mode Paging)
	li      t1, 1
	slli    t1, t1, 17
	or      t0, t0, t1
	csrrc   zero, mstatus, t0

#
# test a reading back data from physical memory
#

# read from 0x80001000 and compare to 0xcc
	li      a0, 0x80001000
	lbu     a1, 0(a0)
	bne     a1, a2, fail

#
# test a store fault on R page
#

# load sptbr
1:	auipc   t0, %pcrel_hi(sv39_pte_r)   # load sv39_pte_r(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load sv39_pte_r(lo)
	csrrw   zero, sptbr, t0
	sfence.vm

# set mstatus.MPRV=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 17
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

# write 0xff to 0x1000
	li      a0, 0x1000
	li      a2, 0xcc
	sb      a2, 0(a0)   # instruction should cause store fault

# compare a7 to fault_store (7)
	li      t0, 7
	bne     a7, t0, fail

# read from 0x1000
	lbu     a3, 0(a0)
	bne     a2, a3, fail

#
# test a load fault on X page
#

# load sptbr
1:	auipc   t0, %pcrel_hi(sv39_pte_x)   # load sv39_pte_x(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load sv39_pte_x(lo)
	csrrw   zero, sptbr, t0
	sfence.vm

# read from 0x1000
	lbu     a3, 0(a0)   # instruction should cause load fault

# compare a7 to fault_load (5)
	li      t0, 5
	bne     a7, t0, fail

# set mstatus.MXR=1 (Mmake eXecute Readable)
	li      t0, 1
	slli    t0, t0, 19
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

# read from 0x1000
	li      a7, 0
	lbu     a3, 0(a0)   # instruction should not fault

# compare a7 to (0)
	li      t0, 0
	bne     a7, t0, fail

# success
	li      a0, 0
	ebreak

# compare failure
fail:
	li      a0, 1
	ebreak

# trap failure
mtvec:
	# set a7 to cause
	csrrsi  a7, mcause, 0

	# increment past the trapping instruction
	csrrsi  t0, mepc, 0
	addi    t0, t0, 4
	csrrw   zero, mepc, t0

	# return
	mret

.section .rodata
.p2align 12
sv39_pte_rw:
	# riscv-pte-util sv39 0x80000000 URWV
	.dword 0x20000017   # 0x0000 -> 0x8000_0000 - 0xC000_0000

.p2align 12
sv39_pte_r:
	# riscv-pte-util sv39 0x80000000 URV
	.dword 0x20000013   # 0x0000 -> 0x8000_0000 - 0xC000_0000

.p2align 12
sv39_pte_x:
	# riscv-pte-util sv39 0x80000000 UXV
	.dword 0x20000019   # 0x0000 -> 0x8000_0000 - 0xC000_0000

