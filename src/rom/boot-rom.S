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

.include "src/rom/boot-def.S"

.section .text
.option norvc
.globl _start
_start:

#
# Reset Vector
#

reset:
	j boot;
	nop
	nop
	.word config

boot:
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
	addi    sp, sp, -xlenb*32

	# set mstatus.MPP = 0b11 (Machine mode)
	csrrs   t1, mstatus, zero
	li      t0, 3
	slli    t0, t0, 11
	or      t1, t1, t0
	csrrs   zero, mstatus, t1

	# program uart to raise interrupts on recieved data
	li      t0, UART_MMIO_BASE
	li      t1, IER_ERBDA     # Load IER_ERBDA bit
	sb      t1, REG_IER(t0)   # Enable IER_ERBDA in IER reg

	# Enable machine mode external interrupts
	li      t0, 2048          # set mie.MEIE=1
	csrrs   zero, mie, t0
	li      t0, 128           # set mstatus.MPIE=1
	csrrs   zero, mstatus, t0

	# return to the ROM
	lx      ra, CONFIG_ROM_ENTRY(gp)
	csrrw   sp, mscratch, sp
	csrrw   zero, mepc, ra
	mret                      # MPIE -> MIE after mret

#
# Trap Entry
#

mtvec:
	# save stack in scratch
	csrrw   sp, mscratch, sp

	# save two working registers
	sx      t0, 0*xlenb(sp)
	sx      t1, 1*xlenb(sp)

	# determine trap or interrupt from sign
	csrrs   t1, mcause, zero
	bgez    t1, trap

	# discard MSB
	sll     t1, t1, 1
	srl     t1, t1, 1

	# dispatch
	li      t0, M_EXTERNAL
	beq     t0, t1, m_external
	li      t0, M_TIMER
	beq     t0, t1, m_timer

unhandled_trap:
	ebreak

m_external:
	# TODO implement delegation using mideleg, hideleg, sideleg
	# NOTE privilege spec is buggy - if we don't aknowledge the
	#      level exception, then MEIP will be raised again and
	#      SEIP interrupt won't be delivered due to the priority.
	#      if we acknowledge the interrupt the IRQ will be lost.
	# NOTE if we implemented simulated hardware delegation using
	#      mideleg, hideleg, sideleg we can solve this issue.
	li t0, MIP_MEIP_MASK
	csrrc  zero, mip, t0           # clear mstatus.MEIP
	li t0, MIP_SEIP_MASK
	csrrs  zero, mip, t0           # set mstatus.SEIP
	j trap_ret

m_timer:
	# TODO implement delegation using mideleg, hideleg, sideleg
	# NOTE Timer interrupt is edge triggered so does not suffer
	#      from the issue with M_EXTERNAL software-bsed delegation
	li t0, MIP_MTIP_MASK
	csrrc  zero, mip, t0           # clear mstatus.MTIP
	li t0, MIP_STIP_MASK
	csrrs  zero, mip, t0           # set mstatus.STIP
	j trap_ret

trap_ret:
	lx      t0, 0*xlenb(sp)        # interrupt handlers need
	lx      t1, 1*xlenb(sp)        # to restore all registers
	csrrw   sp, mscratch, sp
	mret                           # return from interrupt

trap:
	# check cause is in bounds
	slti    t0, t1, 12
	beqz    t0, bad_trap

	# computa cause function address from trap table
1:	auipc   t0, %pcrel_hi(trap_table)  # load trap_table(hi)
	addi    t0, t0, %pcrel_lo(1b)      # load trap_table(lo)
	slli    t1, t1, 2
	add     t0, t0, t1
	lw      t0, (t0)

	# jump to trap table
	jalr    zero, t0

s_ecall_trap:
h_ecall_trap:
m_ecall_trap:
	# check mcall is in bounds
	slti    t0, a7, 16
	beqz    t0, bad_trap

	# computa cause function address from trap table
1:	auipc   t0, %pcrel_hi(sbi_table)   # load sbi_table(hi)
	addi    t0, t0, %pcrel_lo(1b)      # load sbi_table(lo)
	slli    a7, a7, 2
	add     t0, t0, a7
	lw      t0, (t0)

	# advance mepc
	csrrs   t1, mepc, zero
	addi    t1, t1, 4
	csrrw   zero, mepc, t1

	# jump to SBI table
	jalr    zero, t0

bad_trap:
	# can't handle bad trep
	ebreak

wait:
	wfi
	j wait

#
# SBI Interface
#
# The SBI PLIC routines only touch a0-a7 and t0-t6 which
# are caller saved registers in the C ABU and thus can be
# clobbered by SBI ecall routines.
#

mcall_hart_id:
	csrrs   a0, mhartid, zero
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

mcall_console_putchar:
	li      t1, UART_MMIO_BASE     # load UART MMIO base register
	sb      a0, REG_RBR(t1)        # write byte
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

mcall_console_getchar:
	li      t1, UART_MMIO_BASE     # load UART MMIO base register
	lb      t0, REG_IIR(t1)        # check if a byte is readable
	andi    t0, t0, IIR_RX_RDY
	beqz    t0, fail               # return -1 if no data
	lb      a0, REG_RBR(a1)        # read byte
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

mcall_htif_syscall:
	# unimplemented
	ebreak

mcall_send_ipi:
	# unimplemented
	ebreak

mcall_clear_ipi:
	# unimplemented
	ebreak

mcall_shutdown:
	li      t1, GPIO_MMIO_BASE
	li      t0, GPIO_OUT_POWER
	sw      t0, GPIO_OUTPUT_BUF(t1)
	j       wait

mcall_set_timer:
	li      t0, TIMER_MMIO_BASE
	ld      t1, TIMER_MTIME(t0)    # read from mtime
	add     t2, t1, a0             # add arg0 to current time
	sd      t2, TIMER_MTIMECMP(t0) # write to mtimecmp
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret                           # MPIE -> MIE after mret

mcall_remote_sfence_vm:
mcall_remote_sfence_vm_range:
	# unimplemented
	ebreak

mcall_remote_fence_i:
	# unimplemented
	ebreak

mcall_num_harts:
	li      a1, CONFIG_MMIO_BASE
	lx      a0, CONFIG_NUM_HARTS(a1)
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

mcall_query_memory:
	bnez    a0, fail               # only one segment
	li      t0, CONFIG_MMIO_BASE
	lx      t1, CONFIG_RAM_BASE(t0)
	lx      t2, CONFIG_RAM_SIZE(t0)
	li      t3, M_MODE_STACK_SIZE  # reserve M-mode space
	sub     t2, t2, t3
	li      a0, 0
	sx      t1, 0*xlenb(a1)
	sx      t2, 1*xlenb(a1)
	sx      a0, 2*xlenb(a1)
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

mcall_timebase:
	li      t0, CONFIG_MMIO_BASE
	lx      a0, CONFIG_TIME_BASE(t0)
	lx      t0, 0*xlenb(sp)        # restore regs (avoid info leak)
	lx      t1, 1*xlenb(sp)
	csrrw   sp, mscratch, sp
	mret

mcall_mask_interrupt:
	slti    t1, a0, 32             # can handle irqs 0-31
	beqz    t1, fail
	li      t1, 1                  # set irq bit
	sll     t5, t1, a0             # a5 contains shifted mask
	li      t4, -1
	xor     t5, t5, t4             # invert irq mask

	li      t0, PLIC_MMIO_BASE

	lw      t1, 28(t0)             # enabled offset
	add     t2, t0, t1             # enabled node_id=0, hart_id=0
	lw      t4, 0(t2)              # load enabled word from PLIC
	and     t4, t4, t5
	sw      t4, 0(t2)              # store enabled word to PLIC

	li      a0, 0                  # success
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

mcall_unmask_interrupt:
	slti    t1, a0, 32             # can handle irqs 0-31
	beqz    t1, fail
	li      t1, 1                  # set irq bit
	sll     t5, t1, a0             # a5 contains shifted mask

	li      t0, PLIC_MMIO_BASE

	lw      t1, 20(t0)             # priority0 offset
	add     t2, t0, t1             # priority0 node_id=0, hart_id=0
	lw      t4, 0(t2)              # load priority0 word from PLIC
	or      t4, t4, t5
	sw      t4, 0(t2)              # store priority0 word to PLIC

	lw      t1, 24(t0)             # priority1 offset
	add     t2, t0, t1             # priority1 node_id=0, hart_id=0
	lw      t4, 0(t2)              # load priority1 word from PLIC
	or      t4, t4, t5
	sw      t4, 0(t2)              # store priority1 word to PLIC

	lw      t1, 28(t0)             # enabled offset
	add     t2, t0, t1             # enabled node_id=0, hart_id=0
	lw      t4, 0(t2)              # load enabled word from PLIC
	or      t4, t4, t5
	sw      t4, 0(t2)              # store enabled word to PLIC

	li      a0, 0                  # success
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

fail:
	li      a0, -1
	lx      t0, 0*xlenb(sp)        # restore regs to avoid
	lx      t1, 1*xlenb(sp)        # information leakage
	csrrw   sp, mscratch, sp
	mret

#
# Exception jump table
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
# SBI jump table
#

sbi_table:
	.word mcall_hart_id                #  0 <- SBI 0  (-2048)
	.word mcall_console_putchar        #  1 <- SBI 3  (-2000)
	.word mcall_console_getchar        #  2 <- SBI 4  (-1984)
	.word mcall_htif_syscall           #  3 <- SBI 5  (-1968) deprecated
	.word mcall_send_ipi               #  4 <- SBI 6  (-1952)
	.word mcall_clear_ipi              #  5 <- SBI 7  (-1936)
	.word mcall_shutdown               #  6 <- SBI 9  (-1904)
	.word mcall_set_timer              #  7 <- SBI 10 (-1888)
	.word mcall_remote_sfence_vm       #  8 <- SBI 13 (-1840)
	.word mcall_remote_fence_i         #  9 <- SBI 15 (-1808)
	.word mcall_num_harts              # 10 <- SBI 1  (-2032)
	.word mcall_query_memory           # 11 <- SBI 2  (-2016)
	.word mcall_timebase               # 12 <- SBI 8  (-1920)
	.word mcall_mask_interrupt         # 13 <- SBI 11 (-1872)
	.word mcall_unmask_interrupt       # 14 <- SBI 12 (-1856)
	.word mcall_remote_sfence_vm_range # 15 <- SBI 14 (-1824)

config:
	.incbin "src/rom/config.rv"

.p2align 12
