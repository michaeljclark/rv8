.section .text
.globl _start
_start:

.equ HTIF_TOHOST, 0x40008000

# RAM is at 0x8000_0000 - 0xc000_0000

# setup mtvec
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

#
# test load and store to RW gigapage
#

# copy pte to RAM
1:	auipc   t0, %pcrel_hi(sv39_pte_l1_rw)  # load sv39_pte_l1_rw(hi)
	addi    t0, t0, %pcrel_lo(1b)          # load sv39_pte_l1_rw(lo)
	li      t1, 0x80000000
	ld      t2, 0(t0)
	sd      t2, 0(t1)

# load sptbr ppn
	li      t1, 0x80000
	csrrw   zero, sptbr, t1
	sfence.vm

# set mstatus.VM=sv39
	csrrsi  t1, mstatus, 0
	li      t0, 9        # VM.val
	slli    t0, t0, 24
	li      t2, 15       # VM.mask
	slli    t2, t2, 24
	neg     t2, t2
	and     t1, t1, t2   # & mask
	or      t1, t1, t0   # | val
	csrrw   zero, mstatus, t1

# set mstatus.MPRV=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 17
	csrrs   zero, mstatus, t0

# write 0xcc to 0x1000
	li      a0, 0x1000
	li      a2, 0xcc
	sb      a2, 0(a0)

# read from 0x1000
	lbu     a3, 0(a0)
	bne     a2, a3, fail

# set mstatus.PUM=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 18
	csrrs   zero, mstatus, t0

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
# test reading from physical memory
#

# read from 0x80001000 and compare to 0xcc
	li      a0, 0x80001000
	lbu     a1, 0(a0)
	bne     a1, a2, fail

#
# test store fault on R gigapage
#

# copy pte to RAM
1:	auipc   t0, %pcrel_hi(sv39_pte_l1_r)   # load sv39_pte_l1_r(hi)
	addi    t0, t0, %pcrel_lo(1b)          # load sv39_pte_l1_r(lo)
	li      t1, 0x80000000
	ld      t2, 0(t0)
	sd      t2, 0(t1)

# load sptbr ppn
	li      t1, 0x80000
	csrrw   zero, sptbr, t1
	sfence.vm

# set mstatus.MPRV=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 17
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

# write 0xcc to 0x1000
	li      a0, 0x1000
	li      a2, 0xcc
	sb      a2, 0(a0)   # instruction should cause store fault

# compare a7 to fault_store (7)
	li      t0, 7
	bne     a7, t0, fail

# read from 0x1000
	lbu     a3, 0(a0)
	bne     a2, a3, fail

# set mstatus.MPRV=0 (Disable M mode Paging)
	li      t1, 1
	slli    t1, t1, 17
	or      t0, t0, t1
	csrrc   zero, mstatus, t0

# check PTE accessed and dirty bits are set
	li      t1, 0x80000000
	ld      t2, 0(t1)
	li      t3, 192
	and     t4, t3, t2
	bne     t4, t3, fail

#
# test a load fault on X gigapage
#

# copy pte to RAM
1:	auipc   t0, %pcrel_hi(sv39_pte_l1_x)   # load sv39_pte_l1_x(hi)
	addi    t0, t0, %pcrel_lo(1b)          # load sv39_pte_l1_x(lo)
	li      t1, 0x80000000
	ld      t2, 0(t0)
	sd      t2, 0(t1)

# load sptbr ppn
	li      t1, 0x80000
	csrrw   zero, sptbr, t1
	sfence.vm

# set mstatus.MPRV=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 17
	csrrs   zero, mstatus, t0

# read from 0x1000
	lbu     a3, 0(a0)   # instruction should cause load fault

# compare a7 to fault_load (5)
#	li      t0, 5
#	bne     a7, t0, fail

# set mstatus.MXR=1 (Mmake eXecute Readable)
	li      t0, 1
	slli    t0, t0, 19
	csrrs   zero, mstatus, t0

# read from 0x1000
	li      a7, 0
	lbu     a3, 0(a0)   # instruction should not fault

# compare a7 to (0)
	li      t0, 0
	bne     a7, t0, fail

# set mstatus.MXR=0 (Mmake eXecute Readable)
	li      t0, 1
	slli    t0, t0, 19
	csrrc   zero, mstatus, t0

# set mstatus.MPRV=0 (Disable M mode Paging)
	li      t0, 1
	slli    t0, t0, 17
	csrrc   zero, mstatus, t0

# check PTE accessed bit is set
	li      t1, 0x80000000
	ld      t2, 0(t1)
	li      t3, 192
	and     t4, t3, t2
	li      t3, 64
	bne     t4, t3, fail

#
# test load and store to RW 2MiB page
#

# copy pte to RAM
1:	auipc   t0, %pcrel_hi(sv39_pte_l2)  # load sv39_pte_l2(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load sv39_pte_l2(lo)
	li      t1, 0x80000000
	ld      t2, 0(t0)
	sd      t2, 0(t1)
	li      t1, 0x80001000
	ld      t2, 8(t0)
	sd      t2, 0(t1)

# load sptbr ppn
	li      t1, 0x80000
	csrrw   zero, sptbr, t1
	sfence.vm

# set mstatus.MPRV=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 17
	csrrs   zero, mstatus, t0

# write 0xcc to 0x2000
	li      a0, 0x2000
	li      a2, 0xcc
	sb      a2, 0(a0)

# read from 0x2000
	lbu     a3, 0(a0)
	bne     a2, a3, fail

# set mstatus.MPRV=0 (Disable M mode Paging)
	li      t1, 1
	slli    t1, t1, 17
	or      t0, t0, t1
	csrrc   zero, mstatus, t0

#
# test reading from physical memory
#

# read from 0x80202000 and compare to 0xcc
	li      a0, 0x80202000
	lbu     a1, 0(a0)
	bne     a1, a2, fail

#
# test load and store to RW 4 KiB page
#

# copy pte to RAM
1:	auipc   t0, %pcrel_hi(sv39_pte_l3)  # load sv39_pte_l3(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load sv39_pte_l3(lo)
	li      t1, 0x80000000
	ld      t2, 0(t0)
	sd      t2, 0(t1)
	li      t1, 0x80001000
	ld      t2, 8(t0)
	sd      t2, 0(t1)
	li      t1, 0x80002000
	ld      t2, 16(t0)
	sd      t2, 0(t1)
	ld      t2, 24(t0)
	sd      t2, 8(t1)

# load sptbr ppn
	li      t1, 0x80000
	csrrw   zero, sptbr, t1
	sfence.vm

# set mstatus.MPRV=1 (enable M mode paging)
	li      t0, 1
	slli    t0, t0, 17
	csrrs   zero, mstatus, t0

# write 0xcc to 0x1000
	li      a0, 0x1000
	li      a2, 0xcc
	sb      a2, 0(a0)

# read from 0x2000
	lbu     a3, 0(a0)
	bne     a2, a3, fail

# set mstatus.MPRV=0 (Disable M mode Paging)
	li      t1, 1
	slli    t1, t1, 17
	or      t0, t0, t1
	csrrc   zero, mstatus, t0

#
# test reading from physical memory
#

# read from 0x80003000 and compare to 0xcc
	li      a0, 0x80003000
	lbu     a1, 0(a0)
	bne     a1, a2, fail

# success
	j pass

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

pass:
# set mstatus.MPRV=0 (Disable M mode Paging)
	li      t1, 1
	slli    t1, t1, 17
	or      t0, t0, t1
	csrrc   zero, mstatus, t0
	la a0, pass_msg
	jal puts
	j shutdown

fail:
# set mstatus.MPRV=0 (Disable M mode Paging)
	li      t1, 1
	slli    t1, t1, 17
	or      t0, t0, t1
	csrrc   zero, mstatus, t0
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

.section .rodata
.balign 8

# 1 GiB gigapage mapping (URW)
sv39_pte_l1_rw:
	# 0x8000_0000
	# rv-bin pte sv39 0x80000000 URWV
	.dword 0x20000017   # 0x0000 -> 0x8000_0000 - 0xC000_0000

# gigapage mapping (UR)
sv39_pte_l1_r:
	# 0x8000_0000
	# rv-bin pte sv39 0x80000000 URV
	.dword 0x20000013   # 0x0000 -> 0x8000_0000 - 0xC000_0000

# gigapage mapping (UX)
sv39_pte_l1_x:
	# 0x8000_0000
	# rv-bin pte sv39 0x80000000 UXV
	.dword 0x20000019   # 0x0000 -> 0x8000_0000 - 0xC000_0000

# 2 MiB megapage mapping (URW)
sv39_pte_l2:
	# 0x8000_0000
	# rv-bin pte sv39 0x80001000 U
	.dword 0x20000410   # 0x8000_0000 -> 0x8000_1000
	# 0x8000_1000
	# rv-bin pte sv39 0x80200000 URWV
	.dword 0x20080017   # 0x0000 -> 0x8020_0000 - 0x8040_0000

# 4 KiB megapage mapping (URW)
sv39_pte_l3:
	# 0x8000_0000
	# rv-bin pte sv39 0x80001000 U
	.dword 0x20000410   # 0x8000_0000 -> 0x8000_1000
	# 0x8000_1000
	# rv-bin pte sv39 0x80002000 U
	.dword 0x20000810   # 0x8000_1000 -> 0x8000_2000
	# 0x8000_2000
	# rv-bin pte sv39 0x80003000 URWV
	.dword 0x0         # 0x0000 -> invalid
	.dword 0x20000c17  # 0x1000 -> 0x8000_3000 - 0x8000_4000
