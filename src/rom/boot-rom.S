#
# boot-rom.S
#
# portions derived from riscv-pk BBL
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

# MMIO Base registers

.equ TIME_MMIO_BASE,     0x40000000
.equ CONFIG_MMIO_BASE,   0x40001000
.equ PLIC_MMIO_BASE,     0x40002000
.equ UART_MMIO_BASE,     0x40003000
.equ MIPI_MMIO_BASE,     0x40004000
.equ GPIO_MMIO_BASE,     0x40005000

# Config MMIO register offsets

.equ CONFIG_NUM_HARTS, xlenb * 0
.equ CONFIG_TIME_BASE, xlenb * 1
.equ CONFIG_ROM_BASE,  xlenb * 2
.equ CONFIG_ROM_ENTRY, xlenb * 3
.equ CONFIG_RAM_BASE,  xlenb * 4
.equ CONFIG_RAM_SIZE,  xlenb * 5

# GPIO MMIO register offsets
.equ GPIO_INTR_ENABLE, 0
.equ GPIO_INTR_PEND,   4
.equ GPIO_INPUT_BUF,   8
.equ GPIO_OUTPUT_BUF,  12

.equ GPIO_OUT_POWER,   1 # GPIO output pin 1 wired to power
.equ GPIO_OUT_RESET,   2 # GPIO output pin 2 wired to reset

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

#
# Reset Vector
#

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

#
# Trap Entry
#

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
	csrrs   a1, mepc, zero
	addi    a1, a1, 4
	csrrw   zero, mepc, a1

	# jump to SBI table
	jalr    zero, a0

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

#
# SBI Interface
#
# The SBI PLIC routines only touch a0-a7 and t0-t6 which
# are caller saved registers in the C ABU and thus can be
# clobbered by SBI ecall routines.

mcall_hart_id:
	csrrs   a0, mhartid, zero
	csrrw   sp, mscratch, sp
	mret

mcall_console_putchar:
	li      a1, UART_MMIO_BASE
	lx      a0, 0*xlenb(sp)
	sb      a0, 0(a1)
	csrrw   sp, mscratch, sp
	mret

mcall_console_getchar:
	li      a1, UART_MMIO_BASE
	lx      a0, 0*xlenb(sp)
	lb      a0, 0(a1)
	csrrw   sp, mscratch, sp
	mret

mcall_htif_syscall:
	# unimplemented
	ebreak;

mcall_send_ipi:
	# unimplemented
	ebreak;

mcall_clear_ipi:
	# unimplemented
	ebreak;

mcall_shutdown:
	li      a1, GPIO_MMIO_BASE
	li      a0, GPIO_OUT_POWER
	sw      a0, GPIO_OUTPUT_BUF(a1)
	j       wait

mcall_set_timer:
	# unimplemented
	ebreak;

mcall_remote_sfence_vm:
	# unimplemented
	ebreak;

mcall_remote_fence_i:
	# unimplemented
	ebreak;

mcall_num_harts:
	li      a1, CONFIG_MMIO_BASE
	lx      a0, CONFIG_NUM_HARTS(a1)
	csrrw   sp, mscratch, sp
	mret

mcall_query_memory:
	lx      a0, 0*xlenb(sp)
	lx      a1, 1*xlenb(sp)
	bnez    a0, fail
	li      t0, CONFIG_MMIO_BASE
	lx      t1, CONFIG_RAM_BASE(t0)
	lx      t2, CONFIG_RAM_SIZE(t0)
	li      t3, 65536    # reserve 64K for M-mode stack
	sub     t2, t2, t3
	li      a0, 0
	sx      t1, 0*xlenb(a1)
	sx      t2, 1*xlenb(a1)
	sx      a0, 2*xlenb(a1)
	csrrw   sp, mscratch, sp
	mret

mcall_timebase:
	li      a1, CONFIG_MMIO_BASE
	lx      a0, CONFIG_TIME_BASE(a1)
	csrrw   sp, mscratch, sp
	mret

mcall_mask_interrupt:
	lx      a0, 0*xlenb(sp)
	slti    a0, a0, 32      # can handle irqs 0-31
	bnez    a0, fail
	li      a3, 1           # set irq bit
	sll     a3, a3, a0
	li      a4, -1          # invert irq mask
	xor     a3, a4, a4
	li      a0, PLIC_MMIO_BASE

	lw      a1, 28(a0)      # enabled offset
	add     a2, a0, a1      # enabled node_id=0, hart_id=0
	lw      a4, 0(a2)       # load enabled word from PLIC
	and     a4, a5, a3
	sw      a4, 0(a2)       # store enabled word to PLIC

	li      a0, 0           # success
	csrrw   sp, mscratch, sp
	mret

mcall_unmask_interrupt:
	lx      a0, 0*xlenb(sp)
	slti    a0, a0, 32      # can handle irqs 0-31
	bnez    a0, fail
	li      a3, 1           # set irq bit
	sll     a3, a3, a0
	li      a0, PLIC_MMIO_BASE

	lw      a1, 20(a0)      # priority0 offset
	add     a2, a0, a1      # priority0 node_id=0, hart_id=0
	lw      a4, 0(a2)       # load priority0 word from PLIC
	or      a4, a5, a3
	sw      a4, 0(a2)       # store priority0 word to PLIC

	lw      a1, 24(a0)      # priority1 offset
	add     a2, a0, a1      # priority1 node_id=0, hart_id=0
	lw      a4, 0(a2)       # load priority1 word from PLIC
	or      a4, a5, a3
	sw      a4, 0(a2)       # store priority1 word to PLIC

	lw      a1, 28(a0)      # enabled offset
	add     a2, a0, a1      # enabled node_id=0, hart_id=0
	lw      a4, 0(a2)       # load enabled word from PLIC
	or      a4, a5, a3
	sw      a4, 0(a2)       # store enabled word to PLIC

	li      a0, 0           # success
	csrrw   sp, mscratch, sp
	mret

mcall_remote_sfence_vm_range:
	# unimplemented
	ebreak;

fail:
	li      a0, -1
	csrrw   sp, mscratch, sp
	mret

#
# Exception dispatch table
#

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

#
# SBI dispatch table
#

sbi_table:
	.word mcall_hart_id
	.word mcall_console_putchar
	.word mcall_console_getchar
	.word mcall_htif_syscall
	.word mcall_send_ipi
	.word mcall_clear_ipi
	.word mcall_shutdown
	.word mcall_set_timer
	.word mcall_remote_sfence_vm
	.word mcall_remote_fence_i
	.word mcall_num_harts
	.word mcall_query_memory
	.word mcall_timebase
	.word mcall_mask_interrupt
	.word mcall_unmask_interrupt
	.word mcall_remote_sfence_vm_range

.p2align 12
