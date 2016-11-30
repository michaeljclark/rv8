# MMIO

The following document outlines (unofficial) layout for the riscv-meta
emulator devices:

- TIME
- MIPI
- PLIC
- UART

The UART MMIO layout is based on a 16550, the TIME device is based on
version 1.9.1 of the RISC-V Privileged Specification. The PLIC device
has a custom scalable layout based on principles outlined in the
RISC-V Privileged Specification. The MIPI device is a simple bitfield
with one bit per hart, which when set will raise a software interrupt
on the destiniation hart.

_Note: the addresses here are just examples and it is expected that
the configuration string will contain the base addresses for each
device MMIO aperture._

## TIME MMIO Layout

The TIME device is based on the priv-1.9.1 specification and has
an `mtime` register which contains a clock and an `mtimecmp` register
which when timer interrupts are enabled, will raise a timer interrupt.

Example TIME device at offset `0x40000000`.

`0000000040000000-0000000040000010 TIME (0x0000-0x0010) +IO+R+W`

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | u64  | mtime            | Machine Timer Register
8                | u64  | mtimecmp         | Machine Timer Compare Register

## MIPI MMIO Layout

The MIPI device is simply a bitfield with one bit per hart, which
when set will raise a software interrupt on the destiniation hart.

Example MIPI device at offset `0x40001000`.

`0000000040001000-0000000040001008 MIPI (0x0000-0x0008) +IO+R+W`

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | ux   | hart             | hart_words * sizeof(ux)

The following constants describe the length of the MMIO apeture:

Constant         | Expression
:--------------- | :----------------
hart_words       | num_harts / bits_per_word

_Note: The MIPI device needs support for multliple nodes, or
potentially there may be one MIPI device per node._

## PLIC MMIO Layout

The PLIC device has a custom scalable layout based on principles
outlined in the RISC-V Privileged Specification. The PLIC MMIO
apeture is designed to be scalable. It can be configured with
variables for number of nodes, hardware threads and IRQs.

Example PLIC device aperture at offset `0x40002000`, with 1 node,
32 harts, 64 irqs.

`0000000040002000-0000000040002078 PLIC (0x0000-0x0078) +IO+R+W`

Offset           | Type | Name             | Value
:--------------- | :--- | :--------------  | :-----
0                | u32  | num_irqs         | NUM_IRQS
4                | u32  | num_nodes        | NUM_NODES
8                | u32  | num_harts        | NUM_HARTS
12               | u32  | num_priorities   | 4
16               | u32  | pending_offset   | 12 * sizeof(u32)
20               | u32  | priority0_offset | pending_offset + pending_length
24               | u32  | priority1_offset | priority0_offset + priority0_length
28               | u32  | enabled_offset   | priority1_offset + priority1_length
pending_offset   | u32  | pending          | irq_words * sizeof(u32)
priority0_offset | u32  | priority0        | irq_words * sizeof(u32)
priority1_offset | u32  | priority1        | irq_words * sizeof(u32)
enabled_offset   | u32  | enabled          | enabled_words * sizeof(u32)

The following constants describe the length of the MMIO apetures:

Constant         | Expression
:--------------- | :----------------
irq_words        | num_irqs / bits_per_word
pending_length   | irq_words * sizeof(u32)
priority0_length | irq_words * sizeof(u32)
priority1_length | irq_words * sizeof(u32)
enabled_words    | num_irqs * num_nodes * num_harts / bits_per_word
enabled_length   | enabled_words * sizeof(u32)

### PLIC MMIO Registers

The offset into the pending, priority0 and priority1 bit fields
is based on the least significant IRQ (e.g. irq 0) being in the
least significant bit of the first word:

	pending[irq >> word_shift] & (1ULL << (irq & (bits_per_word-1)))

where word_shift = 5 and bits_per_word = 32.

The enabled bit field order is `node_id, hart_id, irq_word` so that
the offset into the enabled bit field can be calculated using the
node_id and hart_id, which locates the start of the bit field that
is encoded in the same order as the pending and priority bit fields.

The enabled bit field for a given node_id and hart_id combination
starts at an offset based on the node_id and hart_id and the
encoding then follows the same scheme as the pending bit field:

	(node_id * NUM_HARTS * irq_words) + (hart_id * irq_words)

### PLIC Priority Encoding

implements 4 priority levels using 2 bits ([p0][p1])

Priority | Mode | Enabled ([p0][p1])
:------  | ---- | :------
highest  | M    | (11)
         | H    | (10,11)
         | S    | (01,10,11)
lowest   | U    | (00,01,10,11)

## UART MMIO Layout

The UART MMIO layout is based on the 16550. See
[Serial UART information](https://www.lammertbies.nl/comm/info/serial-uart.html)
for more information.

Example UART device at offset `0x40003000`.

`0000000040003000-0000000040003008 UART (0x0000-0x0008) +IO+R+W`

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | u8   | rbr              | (R) Recieve Buffer Register
0                | u8   | thr              | (W) Transmit Holding Register
1                | u8   | ier              | (RW) Interrupt Enable Register
2                | u8   | iir              | (R) Interrupt Identity Register
2                | u8   | fcr              | (W) FIFO Control Register
3                | u8   | lcr              | (RW) Line Control Register
4                | u8   | mcr              | (RW) MODEM Control Register
5                | u8   | lsr              | (RW) Line Status Register
6                | u8   | msr              | (RW) MODEM Status Register
7                | u8   | scr              | (RW) Scratch Register
0                | u8   | dll              | (RW) Divisor Latch LSB (LCR.DLAB=1)
1                | u8   | dlm              | (RW) Divisor Latch MSB (LCR.DLAB=1)
