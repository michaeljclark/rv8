.section .text
.globl __bswap32

__bswap32:
1:	auipc   a4, %pcrel_hi(__bswap32_c1)
	ld      a4, %pcrel_lo(1b)(a4)
	slli    a5, a0, 8
	and     a5, a5, a4
	srli    a0, a0, 8
	srli    a4, a4, 8
	and     a0, a0, a4
	or      a5, a5, a0
	slliw   a0, a5, 16
	srliw   a5, a5, 16
	or      a0, a5, a0
	sext.w	a0,a0
	ret

.section .rodata
__bswap32_c1: .4byte 0xFF00FF00ULL
