.section .text
.globl _start
_start:

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

# set mstatus.MPRV=1
	csrrsi  a0, mstatus, 0
	li      a1, 1
	slli    a1, a1, 17
	or      a0, a0, a1
	csrrw   zero, mstatus, a0

# write 0xff to 0x1000
	li      a0, 0x1000
	li      a1, 0xff
	sb      a1, 0(a0)

# break
	ebreak

.section .rodata
.p2align 12
sv39_pte_0:
	.dword 0x0          # 0x0000 -> INVALID
	.dword 0x20000017   # 0x1000 -> 0x8000_0000 flags=URWV
