Bit Manipulation
=====================

_work in progress_

Candidate instructions for Bit Manipulation (unofficial)

|Opcode                     |Long Name                       |Description
|:--------------------------|:-------------------------------|:------------------------------------------------------------------------|
| RLL.{w,d,q} rd,rs1,rs2    | Rotate Left Logical            | Rotate bits in rs1 left by the amount in rs2                            |
| RRL.{w,d,q} rd,rs1,rs2    | Rotate Right Logical           | Rotate bits in rs1 right by the amount in rs2                           |
| RLLI.{w,d,q} rd,rs1,shamt | Rotate Left Logical Immediate  | Rotate bits in rs1 left by the immediate                                |
| RRLI.{w,d,q} rd,rs1,shamt | Rotate Right Logical Immediate | Rotate bits in rs1 right by the immediate                               |
| BX.{w,d,q} rd,rs1,rs2     | Bit Extend                     | Copy the bit in rs1 at the position in rs2 and extend to the MSB        |
| BXI.{w,d,q} rd,rs1,imm    | Bit Extend Immediate           | Copy the bit in rs1 at the position in imm and extend to the MSB        |
| CLZB.{w,d,q} rd,rs1       | Count Leading Zero Bits        | Count leading zero bits in rs1                                          |
| CTZB.{w,d,q} rd,rs1       | Count Trailing Zero Bits       | Count trailing zero bits in rs1                                         |
| PBC.{w,d,q} rd,rs1        | Parallel Bit Count             | Count number of bits set in rs1                                         |
| PBE.{w,d,q} rd,rs1,rs2    | Parallel Bit Extract           | Gather bits from rs1 using mask in rs2 to LSB justified contiguous bits |
| PBD.{w,d,q} rd,rs1,rs2    | Parallel Bit Deposit           | Scatter LSB justified contiguous bits from rs1 using mask in rs2        |

Notes
==========
- RV32B, RV64B and RV128B
- Bit Extend and Bit Extend Immediate (see below)
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
- Bit Extend Immediate for arbitrary width sign and zero extension

References
================
- [Fast Bit Gather, Bit Scatter and Bit Permutation Instructions for Commodity Microprocessors](http://palms.princeton.edu/system/files/Hilewitz_JSPS_08.pdf)
- [Fast Bit Compression and Expansion with Parallel Extract and Parallel Deposit Instructions](http://palms.ee.princeton.edu/PALMSopen/hilewitz06FastBitCompression.pdf)
