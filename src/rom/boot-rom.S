#
# boot-rom.S
#
# 1). The assembly code uses lx and xlenb to load and
#     calculate pointer offsets to allow for RV32
#
# 2). temporary code until we have an SMP boot protocol
#     eventually will use hart_id and LR/SC to choose
#     boot processor and application processors go into
#     wfi loop
#
# 3). handle ecall traps without full register save
#

.eqv xlenb, 8

.macro lx rd, rs1
	ld \rd, \rs1
.endm

.macro sx rd, rs1
	sd \rd, \rs1
.endm

# MMIO definitions

.equ UART_MMIO_BASE, 0x40002000

.equ CONFIG_MMIO_BASE, 0x40001000

.equ CONFIG_NUM_HARTS, xlenb * 0
.equ CONFIG_TIME_BASE, xlenb * 1
.equ CONFIG_ROM_BASE,  xlenb * 2
.equ CONFIG_ROM_ENTRY, xlenb * 3
.equ CONFIG_RAM_BASE,  xlenb * 4
.equ CONFIG_RAM_SIZE,  xlenb * 5

# Partial register save macro (skips a0-a1,t0-t6)

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
.option norvc
.globl _start
_start:

reset:

	# setup mcall trap address
1:	auipc   t0, %pcrel_hi(mtvec)        # load mtvec(hi)
	addi    t0, t0, %pcrel_lo(1b)       # load mtvec(lo)
	csrrw   zero, mtvec, t0

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
	# check cause is in bounds
	slti    a0, a1, 12
	beqz    a0, bad_trap

	# computa cause function address from trap table
1:	auipc   a0, %pcrel_hi(trap_table)  # load trap_table(hi)
	addi    a0, a0, %pcrel_lo(1b)      # load trap_table(lo)
	slli    a1, a1, 2
	add     a0, a0, a1
	lw      a0, (a0)

	# jump to trap table
	jalr    zero, a0

s_ecall_trap:
h_ecall_trap:
m_ecall_trap:
	# check mcall is in bounds
	slti    a0, a7, 16
	beqz    a0, bad_trap
	slti    a0, a7, 16

	# computa cause function address from trap table
1:	auipc   a0, %pcrel_hi(sbi_table)   # load sbi_table(hi)
	addi    a0, a0, %pcrel_lo(1b)      # load sbi_table(lo)
	slli    a7, a7, 2
	add     a0, a0, a7
	lw      a0, (a0)

	# advance mepc
	csrrs   a1, mepc, x0
	addi    a1, a1, 4
	csrrw   zero, mepc, a1

	# jump to SBI table
	jalr    zero, a0

mcall_hart_id:
	li      a1, CONFIG_MMIO_BASE
	lx      a0, CONFIG_NUM_HARTS(a1)
	csrrw   sp, mscratch, sp
	mret

mcall_console_putchar:
mcall_console_getchar:
mcall_htif_syscall:
mcall_send_ipi:
mcall_clear_ipi:
mcall_shutdown:
mcall_set_timer:
mcall_remote_sfence_vm:
mcall_remote_fence_i:
mcall_num_harts:
mcall_query_memory:
mcall_timebase:
mcall_mask_interrupt:
mcall_unmask_interrupt:
mcall_remote_sfence_vm_range:
	# load diagnostic register
	li      a0, 5
	# we can't handle M-mode SBI yet
	ebreak;

bad_trap:
	# can't handle bad trep
	ebreak;

wait:
	wfi
	j wait

save_reg:
	TRAP_REG_SP_OP sx
	ret

restore_reg:
	TRAP_REG_SP_OP lx
	ret

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

sbi_table:
	.word mcall_hart_id,                 0
	.word mcall_console_putchar,         1
	.word mcall_console_getchar,         2
	.word mcall_htif_syscall,            3
	.word mcall_send_ipi,                4
	.word mcall_clear_ipi,               5
	.word mcall_shutdown,                6
	.word mcall_set_timer,               7
	.word mcall_remote_sfence_vm,        8
	.word mcall_remote_fence_i,          9
	.word mcall_num_harts,               10
	.word mcall_query_memory,            11
	.word mcall_timebase,                12
	.word mcall_mask_interrupt,          13
	.word mcall_unmask_interrupt,        14
	.word mcall_remote_sfence_vm_range,  15

.p2align 12
