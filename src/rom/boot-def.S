
.eqv xlenb, 8

.macro lx rd, rs1
	ld \rd, \rs1
.endm

.macro sx rd, rs1
	sd \rd, \rs1
.endm

# M-Mode constants

.equ M_MODE_STACK_SIZE, 2 * 1024 * 1024

.equ MIP_MEIP_MASK,    2048
.equ MIP_HEIP_MASK,    1024
.equ MIP_SEIP_MASK,    512
.equ MIP_UEIP_MASK,    256
.equ MIP_MTIP_MASK,    128
.equ MIP_HTIP_MASK,    62
.equ MIP_STIP_MASK,    32
.equ MIP_UTIP_MASK,    16
.equ MIP_MSIP_MASK,    8
.equ MIP_HSIP_MASK,    4
.equ MIP_SSIP_MASK,    2
.equ MIP_USIP_MASK,    1

.equ U_SOFTWARE,       0
.equ S_SOFTWARE,       1
.equ H_SOFTWARE,       2
.equ M_SOFTWARE,       3
.equ U_TIMER,          4
.equ S_TIMER,          5
.equ H_TIMER,          6
.equ M_TIMER,          7
.equ U_EXTERNAL,       8
.equ S_EXTERNAL,       9
.equ H_EXTERNAL,       10
.equ M_EXTERNAL,       11

# MMIO Base registers

.equ RTC_MMIO_BASE,    0x40000000
.equ MIPI_MMIO_BASE,   0x40001000
.equ PLIC_MMIO_BASE,   0x40002000
.equ UART_MMIO_BASE,   0x40003000
.equ CONFIG_MMIO_BASE, 0x40004000
.equ GPIO_MMIO_BASE,   0x40005000
.equ TIMER_MMIO_BASE,  0x40007000

# Config MMIO register offsets

.equ CONFIG_NUM_HARTS, xlenb * 0
.equ CONFIG_TIME_BASE, xlenb * 1
.equ CONFIG_ROM_BASE,  xlenb * 2
.equ CONFIG_ROM_SIZE,  xlenb * 3
.equ CONFIG_ROM_ENTRY, xlenb * 4
.equ CONFIG_RAM_BASE,  xlenb * 5
.equ CONFIG_RAM_SIZE,  xlenb * 6

# UART MMIO register offets

.equ REG_RBR,          0
.equ REG_THR,          0
.equ REG_IER,          1
.equ REG_IIR,          2

.equ IIR_RX_RDY,       4
.equ IER_ERBDA,        1

# GPIO MMIO register offsets

.equ GPIO_INTR_ENABLE, 0
.equ GPIO_INTR_PEND,   4
.equ GPIO_INPUT_BUF,   8
.equ GPIO_OUTPUT_BUF,  12

.equ GPIO_OUT_POWER,   1 # GPIO output pin 1 wired to power
.equ GPIO_OUT_RESET,   2 # GPIO output pin 2 wired to reset

# Partial register save macro (skips t0,t1)

.macro TRAP_REG_SP_OP OP
	\OP ra, 2*xlenb(sp)
	\OP sp, 3*xlenb(sp)
	\OP gp, 4*xlenb(sp)
	\OP tp, 5*xlenb(sp)
	\OP s0, 6*xlenb(sp)
	\OP s1, 7*xlenb(sp)
	\OP a0, 8*xlenb(sp)
	\OP a1, 9*xlenb(sp)
	\OP a2, 10*xlenb(sp)
	\OP a3, 11*xlenb(sp)
	\OP a4, 12*xlenb(sp)
	\OP a5, 13*xlenb(sp)
	\OP a6, 14*xlenb(sp)
	\OP a7, 15*xlenb(sp)
	\OP s2, 16*xlenb(sp)
	\OP s3, 17*xlenb(sp)
	\OP s4, 18*xlenb(sp)
	\OP s5, 19*xlenb(sp)
	\OP s6, 20*xlenb(sp)
	\OP s7, 21*xlenb(sp)
	\OP s8, 22*xlenb(sp)
	\OP s9, 23*xlenb(sp)
	\OP s10, 24*xlenb(sp)
	\OP s11, 25*xlenb(sp)
	\OP t2, 26*xlenb(sp) # t0,t1 are saved early
	\OP t3, 27*xlenb(sp)
	\OP t4, 28*xlenb(sp)
	\OP t5, 29*xlenb(sp)
	\OP t6, 30*xlenb(sp)
.endm
