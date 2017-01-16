Bit Manipulation
=====================

_work in progress_

Candidate instructions for Bit Manipulation (unofficial)

Opcode                    | Long Name                      | Description
:----------               | :-------------                 | :---------------
RLL.{w,d,q} rd,rs1,rs2    | Rotate Left Logical            | Rotate bits in rs1 left by the amount in rs2 
RRL.{w,d,q} rd,rs1,rs2    | Rotate Right Logical           | Rotate bits in rs1 right by the amount in rs2
RLLI.{w,d,q} rd,rs1,shamt | Rotate Left Logical Immediate  | Rotate bits in rs1 left by the immediate
RRLI.{w,d,q} rd,rs1,shamt | Rotate Right Logical Immediate | Rotate bits in rs1 right by the immediate
BCLZ.{w,d,q} rd,rs1       | Bit Count Leading Zero         | Count leading zero bits in rs1
BCTZ.{w,d,q} rd,rs1       | Bit Count Trailing Zero        | Count trailing zero bits in rs1
BSWAP.{w,d,q} rd,rs1      | Byte Swap                      | Swap byte order in rs1
PBC.{w,d,q} rd,rs1        | Parallel Bit Count             | Count number of bits set in rs1
PBE.{w,d,q} rd,rs1,rs2    | Parallel Bit Extract           | Gather bits from rs1 using mask in rs2 to LSB justified contiguous bits
PBD.{w,d,q} rd,rs1,rs2    | Parallel Bit Deposit           | Scatter LSB justified contiguous bits from rs1 using mask in rs2

Notes
==========
- RV32B, RV64B and RV128B
- Parallel Bit Extract could potentially be named Parallel Bit Gather
- Parallel Bit Deposit could potentially be named Parallel Bit Scatter
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
