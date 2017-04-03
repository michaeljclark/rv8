.section .text
.globl __bswap64

__bswap64:
1:	auipc   a4, %pcrel_hi(__bswap64_c1)
	ld      a4, %pcrel_lo(1b)(a4)
	slli    a5, a0, 8
	and     a5, a5, a4
	srli    a0, a0, 8
	srli    a4, a4, 8
	and     a0, a0, a4
	or      a5, a5, a0
1:	auipc   a4, %pcrel_hi(__bswap64_c2)
	ld      a4, %pcrel_lo(1b)(a4)
	slli    a0, a5, 16
	and     a0, a0, a4
	srli    a5, a5, 16
	srli    a4, a4, 16
	and     a5, a5, a4
	or      a5, a5, a0
	slli    a0, a5, 32
	srli    a5, a5, 32
	or      a0, a0, a5
	ret

.section .rodata
__bswap64_c1: .8byte 0xFF00FF00FF00FF00ULL
__bswap64_c2: .8byte 0xFFFF0000FFFF0000ULL
