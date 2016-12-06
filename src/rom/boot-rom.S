#
# boot-rom.S
#

# RV64 definitions
#
# The assembly code uses lx and xlenb to load and
# calculate pointer offsets to allow RV32 support

.eqv xlenb, 8

.macro lx rd, rs1
	ld \rd, \rs1
.endm

.macro sx rd, rs1
	sd \rd, \rs1
.endm

# Condig MMIO definitions

.equ CONFIG_MMIO_BASE, 0x40001000

.equ CONFIG_NUM_HARTS, xlenb * 0
.equ CONFIG_TIME_BASE, xlenb * 1
.equ CONFIG_ROM_BASE,  xlenb * 2
.equ CONFIG_ROM_ENTRY, xlenb * 3
.equ CONFIG_RAM_BASE,  xlenb * 4
.equ CONFIG_RAM_SIZE,  xlenb * 5

# Partial register save macro (skips a0,a2,t0-t6)

.macro TRAP_REG_SP_OP OP
	\OP ra, 2*xlenb(sp)
	\OP sp, 3*xlenb(sp)
	\OP gp, 4*xlenb(sp)
	\OP tp, 5*xlenb(sp)
	\OP s0, 6*xlenb(sp)
	\OP s1, 7*xlenb(sp)
	\OP a2, 8*xlenb(sp)
	\OP a3, 9*xlenb(sp)
	\OP a4, 10*xlenb(sp)
	\OP a5, 11*xlenb(sp)
	\OP a6, 12*xlenb(sp)
	\OP a7, 13*xlenb(sp)
	\OP s2, 14*xlenb(sp)
	\OP s3, 15*xlenb(sp)
	\OP s4, 16*xlenb(sp)
	\OP s5, 17*xlenb(sp)
	\OP s6, 18*xlenb(sp)
	\OP s7, 19*xlenb(sp)
	\OP s8, 20*xlenb(sp)
	\OP s9, 21*xlenb(sp)
	\OP s10, 22*xlenb(sp)
	\OP s11, 23*xlenb(sp)
.endm

.section .text
.globl _start
_start:

reset:

	# setup mcall trap address
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

	# temporary code until we have an SMP boot protocol
	# eventually will use hart_id and LR/SC to choose
	# boot processor and application processors go into
	# wfi loop

	# load ROM address from config MMIO region
	li      gp, CONFIG_MMIO_BASE

	# set stack to register save area at the top of RAM
	lx      t0, CONFIG_RAM_BASE(gp)
	lx      t1, CONFIG_RAM_SIZE(gp)
	add     sp, t0, t1
	addi    sp, sp, -(xlenb*32)

	# set mstatus.MPP = 0b11 (Machine mode)
	csrrs   t1, mstatus, zero
	li      t0, 3
	slli    t0, t0, 11
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

	# return to the ROM
	LX      ra, CONFIG_ROM_ENTRY(gp)
	csrrw   sp, mscratch, sp
	csrrw   zero, mepc, ra
	mret

mtvec:
	# save stack in scratch
	csrrw   sp, mscratch, sp

	# save two working registers
	sx      a0, 0*xlenb(sp)
	sx      a1, 1*xlenb(sp)

	# determine trap or interrupt from sign
	csrrs   a1, mcause, zero
	bgez    a1, trap

intr:
	# we can't handle interrupts yet
	li      a0, 4
	ebreak

trap:
	# computa cause function address from trap table
	slti    a0, a1, 12
	beqz    a0, bad_trap
1:	auipc   a0, %pcrel_hi(trap_table)  # load trap_table(hi)
	addi    a0, a0, %pcrel_lo(1b)      # load trap_table(lo)
	slli    a1, a1, 2
	add     a0, a0, a1
	lw      a0, (a0)

	# TODO - handle simple trap requests without full
	#        register save and perhaps use subroutine

	# save regs
	TRAP_REG_SP_OP sx

	# advance mepc
	csrrs   t0, mepc, x0               # increment mepc
	addi    t0, t0, 4
	csrrw   zero, mepc, t0

	# jump to trap table
	jalr    ra, a0
	LX      a1, 1*xlenb(sp)               # a0 holds result

	# restore registers and return
	TRAP_REG_SP_OP lx
	mret

s_ecall_trap:
	li      a0, 5
	# we can't handle S-mode ecall yet
	ebreak;

h_ecall_trap:
	li      a0, 6
	# we can't handle H-mode ecall yet
	ebreak;

m_ecall_trap:
	li      a0, 7
	# we can't handle M-mode ecall yet
	ebreak;

bad_trap:
	li      a0, 8
	# we can't handle bad yet
	ebreak;

wait:
	wfi
	j wait

trap_table:
  .word bad_trap
  .word bad_trap
  .word bad_trap
  .word bad_trap
  .word bad_trap
  .word bad_trap
  .word bad_trap
  .word bad_trap
  .word bad_trap
  .word s_ecall_trap
  .word h_ecall_trap
  .word m_ecall_trap

.p2align 12
