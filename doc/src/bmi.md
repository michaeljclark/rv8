Bit Manipulation
=====================

Candidate instructions for B extension (Bit Manipulation)

Opcode               | Long Name                      | Description
:----------          | :-------------                 | :---------------
RLL[W] rd,rs1,rs2    | Rotate Left Logical            | Rotate bits in rs1 left by the amount in rs2 
RRL[W] rd,rs1,rs2    | Rotate Right Logical           | Rotate bits in rs1 right by the amount in rs2
RLLI[W] rd,rs1,shamt | Rotate Left Logical Immediate  | Rotate bits in rs1 left by the immediate
RRLI[W] rd,rs1,shamt | Rotate Right Logical Immediate | Rotate bits in rs1 right by the immediate
BCLZ[W] rd,rs1       | Bit Count Leading Zeros        | Count leading zero bits in rs1
BCTZ[W] rd,rs1       | Bit Count Trailing Zeros       | Count trailing zero bits in rs1
BCNT[W] rd,rs1       | Bit Count                      | Count number of bits set in rs1
BREV[W] rd,rs1       | Bit Reverse                    | Reverse bits in rs1
BSWAP[W] rd,rs1      | Byte Swap                      | Swap byte order in rs1
BEXT[W] rd,rs1,rs2   | Bit Extract                    | Gather LSB justified bits to rd from rs1 using extract mask in rs2
BDEP[W] rd,rs1,rs2   | Bit Deposit                    | Scatter LSB justified bits from rs2 to rd using deposit mask in rs2

## Bit Extract

Gather LSB justified bits to rd from rs1 using extract mask in rs2.
Bits are extracted from the source register using bit positions
enabled in the mask register and are placed in the result as
popcount(mask) LSB contiguous bits.

Register | Value
:--      | :--
source   | 0b11110100
mask     | 0b01100011
result   | 0b00001100

## Bit Deposit

Scatter LSB justified bits from rs2 to rd using deposit mask in rs2.
Bits are deposited from the source register using popcount(mask)
LSB contiguous bits and are placed in the result at the bit positions
enabled in the mask register.

Register | Value
:--      | :--
source   | 0b11110100
mask     | 0b01100011
result   | 0b00100000

## Count leading and trailing zeros

Count leading and trailing zeros can be constructed using popcount (`BCNT`)
and bit reverse (`BREV`) instructions or functional units. Given the length
of the instruction sequences and the possibility to multiplex via shared bit
reverse and popcount functional units, the count leading and trailing zeros
instructions are included.

One area saving implementation approach involves multiplexing functional units in
a pipeline containing bit reverse, neg+and+sub (leading/traling) and popcount.
The critical path the length for these bit manipulation instructions would be the
fanout of the bit reverse circuit, the neg+and+sub circuit for (`BCLZ`) and (`BCTZ`)
and the popcount circuit. The popcount instruction (`BCNT`) would have an input
bypass and bit reverse instruction (`BREV`) would have an output bypass.

### C
```
#include <stdint.h>

uint32_t bctz_w(uint32_t x) { return __builtin_popcount((x & -x) - 1); }
uint64_t bctz_d(uint64_t x) { return __builtin_popcountll((x & -x) - 1); }
uint32_t bclz_w(uint32_t x) { uint32_t y = __builtin_bitreverse32(x); return __builtin_popcount((y & -y) - 1); }
uint64_t bclz_d(uint64_t x) { uint64_t y = __builtin_bitreverse64(x); return __builtin_popcountll((y & -y) - 1); }
```

### Asssembler
```
# count trailing zeros
.macro BCTZ.X  rd, rs`
	neg     \rd, \rs
	and     \rd, \rd, \rs
	addi    \rd, \rd, -1
	bcnt    \rd, \rd
.endm
```

```
# count leading zeros
.macro BCLZ.X  rd, rs`
	brev    t0,  \rs
	neg     \rd, t0
	and     \rd, \rd, t0
	addi    \rd, \rd, -1
	bcnt    \rd, \rd
.endm
```

Notes
==========
- Candidate Bit Manipulation Instructions should require at least 4 Base ISA instructions to be considered
- Rotate instructions are used frequenty in cyptographic ciphers and hashing algorithms
- Bit Extract could potentially be named Parallel Bit Extract or Parallel Bit Gather
- Bit Deposit could potentially be named Parallel Bit Deposit or Parallel Bit Scatter
- Investigate static versus dynamic bit extraction (implementation dependent)
- Investigate Butterfly and Inverse Butterfly bit permutation instructions

RISC-V Immediates
=======================

There are a number of costly operations for decoding RISC-V instructions in software.
As an effort to virtualise the ISA both form the Popek and Goldberg model and from
a fast translation model, specific instructions are proposed that will aid in fast
decoding of RISC-V instructions in software.

- Parallel Bit Extract and Parallel Bit Deposit for accelerated immediate extraction
- Bit Extend Immediate for sign and zero extension can be accomplished with SLL and SRA

References
================
- [Fast Bit Gather, Bit Scatter and Bit Permutation Instructions for Commodity Microprocessors](http://palms.princeton.edu/system/files/Hilewitz_JSPS_08.pdf)
- [Fast Bit Compression and Expansion with Parallel Extract and Parallel Deposit Instructions](http://palms.ee.princeton.edu/PALMSopen/hilewitz06FastBitCompression.pdf)
