# MMIO

This document outlines the memory map for the `rv-sys` emulator
devices:

- RTC (Real Time Clock)
- IPI (Inter-processor Interrupt)
- PLIC (Platform Level Interrupt Controller)
- UART (Universal Asychronous Receiver / Transmitter)
- HTIF (Host Target Interface)

The UART MMIO layout is based on a 8250. The RTC device is based on
version 1.9.1 of the RISC-V Privileged Specification. The PLIC  has
a layout based on [RISCVEMU](http://bellard.org/riscvemu/). The IPI
device is a bitfield with one bit per hart, which when set will raise
a software interrupt on each destiniation hart. The HTIF device
emulates the console protocol used by BBL.

## Memory layout

This tables shows an overview of the `rv-sys` emulator memory map:

Memory Range                      | Device | Attributes
:------------------               | :----- | :---------
0000000080000000-00000000c0000000 | RAM    | +MAIN+R+W+X
0000000040000000-0000000040000010 | RTC    | +IO+R+W
0000000040001000-0000000040001008 | IPI    | +IO+R+W
0000000040002000-0000000040002008 | LIC    | +IO+R+W
0000000040003000-0000000040003008 | UART   | +IO+R+W
0000000040008000-0000000040008010 | HTIF   | +IO+R+W


## RTC (Real Time Clock)

The RTC device is based on the priv-1.9.1 specification and has
an `mtime` register which contains a clock and a one-shot timer
programmed via the `mtimecmp` register. When timer interrupts are
enabled, a timer interrupt will be raised when `mtimecmp == mtime`.

Example RTC MMIO device at offset `0x40000000`.

`0000000040000000-0000000040000010 (0x0000-0x0010) RTC +IO+R+W`

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | u64  | mtime            | Machine Timer Register
8                | u64  | mtimecmp         | Machine Timer Compare Register


## IPI (Inter-Processor Interrupt)

The IPI device is simply a bitfield with one bit per hart, which
when set will raise a software interrupt on the destiniation hart.
The least signigicant bit corresponds to hart_id 0 and the most
significant bit corresponds to hart_id 63.

Example IPI device at offset `0x40001000`.

```
0000000040001000-0000000040001008 (0x0000-0x0008) IPI +IO+R+W
```

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | u64  | hart             | Per hart bitfield


## PLIC (Platform Level Interrupt Controller)

The PLIC is the Platform Level Interrupt Controller which provides
an interface to receive and acknowledge external interrupts.

Example PLIC MMIO device aperture at offset `0x40002000`.

```
0000000040002000-0000000040002008 (0x0000-0x0008) PLIC +IO+R+W
```

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | u64  | claim            | Read IRQ, Write IRQ EOI

There is a single 64-bit read/write register:

- Read returns an IRQ number for the highest priority interrupt
- Zero is returned if there are no active interrupts
- Interrupt service can read multiple times until zero is returned
- Write claims an IRQ number and signals EOI (End Of Interrupt)


## UART (Universal Asychronous Receiver / Transmitter)

The UART MMIO layout is based on the 8250 (16550 with no FIFO). See
[Serial UART information](https://www.lammertbies.nl/comm/info/serial-uart.html)
for more information.

Example UART device at offset `0x40003000`.

```
0000000040003000-0000000040003008 (0x0000-0x0008) UART +IO+R+W
```

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


## HTIF (Host Target Interface)

The HTIF MMIO device is a MMIO version of the RISC-V host
target interface altered to use a predefined memory location.

Example UART device at offset `0x40008000`.

```
0000000040008000-0000000040008010 (0x0000-0x0010) HTIF +IO+R+W
```

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | u64  | tohost           | To Host Register
8                | u64  | fromhost         | From Host Register

The only interface that is implemented is the shutdown and console IO.

- Bits 63:56 bits of `tohost` and `fromhost` encodes the device
- Bits 55:48 bits of `tohost` and `fromhost` encodes the command
- Console device is 1
- Console input request command is 0
- Console output request command is 1

To write an output character to the console, clear `fromhost`, then
write to `tohost` with device 1 command 1 with the output character
in the low 8 bits. Finally, read `fromhost` and check for the device
and command for acknowledgement.

```
fromhost <- 0
tohost <- ((uint64_t)1 << 56) | ((uint64_t)1 << 48) | ch /* putchar request */
fromhost -> ((uint64_t)1 << 56) | ((uint64_t)1 << 48)    /* putchar acknowledge */
```

To poll the console for an input character, clear `fromhost`, then
write to `tohost` with device 1 command 0. Read `fromhost` and check
for the device, command and console input character. If there is console
input available, a non zero value will be encoded in the low 8 bits.

```
fromhost <- 0
tohost <- ((uint64_t)1 << 56) | ((uint64_t)0 << 48)        /* getchar request */
fromhost -> ((uint64_t)1 << 56) | ((uint64_t)0 << 48) | ch /* getchar acknowledge */
```

To send the shutdown command, write 1 to `tohost`:

```
tohost <- 1 /* shutdown */
```
