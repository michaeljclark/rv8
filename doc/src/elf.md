ELF
========

_work in progress_

Notes on RISC-V ELF mapping (unofficial).


## ELF Headers

The following is a list of ELF header fields and potential RISC-V mappings.

### e_ident[EI_DATA]

Enum | Name                        | RISC-V Description
:--- | :---                        | :---
0    | `ELFDATANONE`               |
1    | `ELFDATA2LSB`               | RISC-V little-endian
2    | `ELFDATA2MSB`               |

### e_ident[EI_VERSION]

Enum | Name
:--- | :---
1    | `EV_CURRENT`

### e_ident[EI_OSABI]

Enum | Name                        | RISC-V Description
:--- | :---                        | :---
0    | `ELFOSABI_SYSV`             | e.g. `riscv64-unknown-elf`
3    | `ELFOSABI_LINUX`            | e.g. `riscv64-linux-gnu`
9    | `ELFOSABI_FREEBSD`          | e.g. `riscv64-generic-freebsd`

_Note: Host triples are in the form:_ `machine-vendor-operatingsystem`

### e_ident[EI_ABIVERSION]

Enum | Name
:--- | :---
0    | `ELFABIVERSION_NONE`

### e_flags

Enum | Name                        | RISC-V Description
:--- | :---                        | :---
1    | `EF_RISCV_RVC`              | RVC Compression
2    | `EF_RISCV_FLOAT_ABI_SINGLE` | Single-precision float ABI
4    | `EF_RISCV_FLOAT_ABI_DOUBLE` | Double-precision float ABI
6    | `EF_RISCV_FLOAT_ABI_QUAD`   | Quad-precision float ABI
8    | `EF_RISCV_RVE`              |

### e_version

Enum | Name                        | RISC-V Description
:--- | :---                        | :---
1    | `EV_CURRENT`                | (same as `e_ident[EI_VERSION]`)

## Address calculation

The following table provides details on the variables used in address calculation:

Variable       | Description
:------------- | :----------------
A              | Addend field in the relocation entry associated with the symbol
B              | Base address of a shared object loaded into memory
G              | Offset of the symbol into the GOT (Global Offset Table)
S              | Value of the symbol in the symbol table
GP             | Global Pointer register (x3)

## Relocations

The following table provides information of the RISC-V ELF relocations:

Enum | ELF Reloc Type       | Description                | Assembler           | Details
:--- | :------------------  | :---------------           | :-------------      | :-----------
0    | R_RISCV_NONE         | None                       |                     |
1    | R_RISCV_32           | Runtime relocation         |                     | word32 = S + A
2    | R_RISCV_64           | Runtime relocation         |                     | word64 = S + A
3    | R_RISCV_RELATIVE     | Runtime relocation         |                     | word32,64 = B + A
4    | R_RISCV_COPY         | Runtime relocation         |                     | must be in executable. not allowed in shared library
5    | R_RISCV_JUMP_SLOT    | Runtime relocation         |                     | word32,64 = S ;handled by PLT unless LD_BIND_NOW
6    | R_RISCV_TLS_DTPMOD32 | TLS relocation             |                     | word32 = S->TLSINDEX
7    | R_RISCV_TLS_DTPMOD64 | TLS relocation             |                     | word64 = S->TLSINDEX
8    | R_RISCV_TLS_DTPREL32 | TLS relocation             |                     | word32 = TLS + S + A - TLS_TP_OFFSET
9    | R_RISCV_TLS_DTPREL64 | TLS relocation             |                     | word64 = TLS + S + A - TLS_TP_OFFSET
10   | R_RISCV_TLS_TPREL32  | TLS relocation             |                     | word32 = TLS + S + A + S_TLS_OFFSET - TLS_DTV_OFFSET
11   | R_RISCV_TLS_TPREL64  | TLS relocation             |                     | word64 = TLS + S + A + S_TLS_OFFSET - TLS_DTV_OFFSET
16   | R_RISCV_BRANCH       | PC-relative branch offset  |                     | SB-Type (beq,bne,blt,bge,bltu,bgeu)
17   | R_RISCV_JAL          | PC-relative jump offset    |                     | UJ-Type (jal)
18   | R_RISCV_CALL         | PC-relative function call  |                     | MACRO call,tail (auipc+jalr)
19   | R_RISCV_CALL_PLT     | PC-relative function call  |                     | MACRO call,tail (auipc+jalr) PIC
20   | R_RISCV_GOT_HI20     | PC-relative GOT offset     |                     | MACRO la
21   | R_RISCV_TLS_GOT_HI20 | PC-relative TLS IE GOT ref | %tls_ie_pcrel_hi(x) | MACRO la.tls.ie
22   | R_RISCV_TLS_GD_HI20  | PC-relative TLS GD GOT ref | %tls_gd_pcrel_hi(x) | MACRO la.tls.gd
23   | R_RISCV_PCREL_HI20   | PC-relative reference      | %pcrel_hi(symbol)   | U-Type (auipc)
24   | R_RISCV_PCREL_LO12_I | PC-relative reference      | %pcrel_lo(label)    | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
25   | R_RISCV_PCREL_LO12_S | PC-relative reference      | %pcrel_lo(label)    | S-Type (sb,sh,sw,sd,fsw,fsd)
26   | R_RISCV_HI20         | Absolute address           | %hi(symbol)         | U-Type (lui)
27   | R_RISCV_LO12_I       | Absolute address           | %lo(symbol)         | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
28   | R_RISCV_LO12_S       | Absolute address           | %lo(symbol)         | S-Type (sb,sh,sw,sd,fsw,fsd)
29   | R_RISCV_TPREL_HI20   | TLS LE thread offset       | %tprel_hi(symbol)   | U-Type (auipc)
30   | R_RISCV_TPREL_LO12_I | TLS LE thread offset       | %tprel_lo(label)    | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
31   | R_RISCV_TPREL_LO12_S | TLS LE thread offset       | %tprel_lo(label)    | S-Type (sb,sh,sw,sd,fsw,fsd)
32   | R_RISCV_TPREL_ADD    | TLS LE thread usage        | %tprel_add(label)   | I-Type (addi)
33   | R_RISCV_ADD8         | 8-bit label addition       |                     | word8 = S + A
34   | R_RISCV_ADD16        | 16-bit label addition      |                     | word16 = S + A
35   | R_RISCV_ADD32        | 32-bit label addition      |                     | word32 = S + A
36   | R_RISCV_ADD64        | 64-bit label addition      |                     | word64 = S + A
37   | R_RISCV_SUB8         | 8-bit label subtraction    |                     | word8 = S - A
38   | R_RISCV_SUB16        | 16-bit label subtraction   |                     | word16 = S - A
39   | R_RISCV_SUB32        | 32-bit label subtraction   |                     | word32 = S - A
40   | R_RISCV_SUB64        | 64-bit label subtraction   |                     | word64 = S - A
41   | R_RISCV_GNU_VTINHERIT| GNU C++ vtable hierarchy   |                     |
42   | R_RISCV_GNU_VTENTRY  | GNU C++ vtable member usage|                     |
43   | R_RISCV_ALIGN        | Alignment statement        |                     |
44   | R_RISCV_RVC_BRANCH   | PC-relative branch offset  |                     | CB-Type (c.beqz,c.bnez)
45   | R_RISCV_RVC_JUMP     | PC-relative jump offset    |                     | CJ-Type (c.j)
46   | R_RISCV_RVC_LUI      | Absolute address           |                     | CI-Type (c.lui)
47   | R_RISCV_GPREL_I      | GP-relative reference      |                     | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
48   | R_RISCV_GPREL_S      | GP-relative reference      |                     | S-Type (sb,sh,sw,sd,fsw,fsd)
49   | R_RISCV_TPREL_I      | TP-relative TLS LE load    |                     | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld)
50   | R_RISCV_TPREL_S      | TP-relative TLS LE store   |                     | S-Type (sb,sh,sw,sd,fsw,fsd)
51   | R_RISCV_RELAX        | Reloc pair can be relaxed  |                     | 
52   | R_RISCV_SUB6         | Local label subtraction    |                     | 
53   | R_RISCV_SET6         | Local label subtraction    |                     | 
54   | R_RISCV_SET8         | Local label subtraction    |                     | 
55   | R_RISCV_SET16        | Local label subtraction    |                     | 
56   | R_RISCV_SET32        | Local label subtraction    |                     | 


### Position Independent Code

R_RISCV_PCREL_HI20, and R_RISCV_PCREL_LO12_I or R_RISCV_PCREL_LO12_S pairs are relocations used in position independent code.

The R_RISCV_PCREL_HI20 relocation associates a symbol with an AUIPC instruction where the high 20-bits are relocated to point to a symbol relative to the PC of the instruction.

The AUIPC instruction and its relocation are followed by an I-Type instruction (add immediate, load) and R_RISCV_PCREL_LO12_I relocation or an S-Type instruction (store) and R_RISCV_PCREL_LO12_S relocation.

The R_RISCV_PCREL_LO12_I or R_RISCV_PCREL_LO12_S relocations contains a (typically private) local label symbol that points to the AUIPC instruction with the R_RISCV_PCREL_HI20 relocation entry that then points to the target symbol. The relocation has three steps:

 - R_RISCV_PCREL_LO12_I relocation entry ⟶ label
 - label ⟶ labeled instruction R_RISCV_PCREL_HI20 reloc
 - labeled instruction R_RISCV_PCREL_HI20 reloc ⟶ symbol

To get the symbol address to perform the calculation to fill the 12-bit immediate on the add, load or store instruction the linker finds the R_RISCV_PCREL_HI20 relocation entry associated with the prior AUIPC instruction offset pointed to by the R_RISCV_PCREL_LO12_I or R_RISCV_PCREL_LO12_S symbol.

## Glossary

- TLS LE (Thread Local Storage - Local Exec), Compiler flag: `-ftls-model=local-exec`
- TLS IE (Thread Local Storage - Initial Exec), Compiler flag: `-ftls-model=initial-exec`
- TLS GD (Thread Local Storage - Global Dynamic), Compiler flag: `-ftls-model=global-dynamic`

## References

- TIS Portable Formats Specification, Version 1.1 ELF: Executable and Linkable Format
- ELF-64 Object File Format, Version 1.5 Draft 2
- System V Application Binary Interface AMD64 Architecture Processor Supplement Draft 0.99.6
