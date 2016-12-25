# MMIO

The following document outlines (unofficial) layout for the riscv-meta
emulator devices:

- TIME
- MIPI
- PLIC
- UART
- HTIF

The UART MMIO layout is based on a 16550, the TIME device is based on
version 1.9.1 of the RISC-V Privileged Specification. The PLIC device
has a custom scalable layout based on principles outlined in the
RISC-V Privileged Specification. The MIPI device is a simple bitfield
with one bit per hart, which when set will raise a software interrupt
on the destiniation hart.

## Example memory layout for `rv-sys`

```
soft-mmu :0000000080000000-00000000c0000000 (0x4e697000-0x8e697000) RAM +MAIN+R+W+X
soft-mmu :fffffffffffff000-0000000000000000 (0x0000-0x1000) SBI +MAIN+R+X
soft-mmu :0000000040000000-0000000040000010 (0x0000-0x0010) TIME +IO+R+W
soft-mmu :0000000040001000-0000000040001008 (0x0000-0x0008) MIPI +IO+R+W
soft-mmu :0000000040002000-0000000040002008 (0x0000-0x0008) PLIC +IO+R+W
soft-mmu :0000000040003000-0000000040003008 (0x0000-0x0008) UART +IO+R+W
soft-mmu :0000000040008000-0000000040008010 (0x0000-0x0010) HTIF +IO+R+W
```

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

`0000000040001000-0000000040001008 (0x0000-0x0008) MIPI +IO+R+W`

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | ux   | hart             | Per hard bitfield


## PLIC MMIO

Example PLIC device aperture at offset `0x40002000`

`soft-mmu :0000000040002000-0000000040002008 (0x0000-0x0008) PLIC +IO+R+W`

There is a single 64-bit read/write register:

- when read, returns an IRQ number for the highest priority interrupt
- when IRW number is written, claims/acknowledges end of interrupt
- zero is returned if there are no active interrupts


## UART MMIO

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


## HTIF MMIO

The HTIF MMIO device is a MMIO version of the RISC-V host
target interface altered to use a predefined memory location.

Example UART device at offset `0x40008000`.

```
soft-mmu :0000000040008000-0000000040008010 (0x0000-0x0010) HTIF +IO+R+W
```

Offset           | Type | Name             | Description
:--------------- | :--- | :--------------  | :--------------
0                | u64  | tohost           | To Host Register
8                | u64  | fromhost         | From Host Register

The only interface that is implemented is the shutdown and console IO.

- Bits 63:56 bits of `tohost` and `fromhost` host is the device
- Bits 55:48 bits of `tohost` and `fromhost` host is the command

To post a single character to the console, write to `tohost` with the
cdevice 1 and the command 1 and the character in the low order bits:

```
tohost <- ((uint64_t)1 << 56) | ((uint64_t)0 << 48) | ch /* putchar */
```

To poll for keyboard read `fromhost`. If there is keyboard input, where c
is the character, the device is 1, and the command is 0. If there is no
data available the result will be zero.

```
fromhost -> ((uint64_t)1 << 56) | ((uint64_t)0 << 48) | ch /* getchar */
```

To send the shutdown command:

```
tohost <- 1 /* shutdown */
```
