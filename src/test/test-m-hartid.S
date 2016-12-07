.section .text
.globl _start
_start:

	csrrs   a0, mvendorid, zero
	csrrs   a1, marchid, zero
	csrrs   a2, mimpid, zero
	csrrs   a3, mhartid, zero
