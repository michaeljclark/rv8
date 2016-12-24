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
1    | `ELFDATA2MSB`               |

### e_ident[EI_VERSION]

Enum | Name
:--- | :---
1    | `EV_CURRENT`

### e_ident[EI_OSABI]

Enum | Name                        | RISC-V Description
:--- | :---                        | :---
0    | `ELFOSABI_SYSV`             | `riscv-unknown-elf`
3    | `ELFOSABI_LINUX`            | `riscv-linux-gnu`
6    | `ELFOSABI_SOLARIS`          | `riscv-oracle-solaris`
9    | `ELFOSABI_FREEBSD`          | `riscv-unknown-freebsd`

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

ELF Reloc Type       | Description                | Assembler           | Details
:------------------  | :---------------           | :-------------      | :-----------
R_RISCV_NONE         | None                       |                     |
R_RISCV_32           | Runtime relocation         |                     | word32 = S + A
R_RISCV_64           | Runtime relocation         |                     | word64 = S + A
R_RISCV_RELATIVE     | Runtime relocation         |                     | word32,64 = B + A
R_RISCV_COPY         | Runtime relocation         |                     | must be in executable. not allowed in shared library
R_RISCV_JUMP_SLOT    | Runtime relocation         |                     | word32,64 = S ;handled by PLT unless LD_BIND_NOW
R_RISCV_TLS_DTPMOD32 | TLS relocation             |                     | word32 = S->TLSINDEX
R_RISCV_TLS_DTPMOD64 | TLS relocation             |                     | word64 = S->TLSINDEX
R_RISCV_TLS_DTPREL32 | TLS relocation             |                     | word32 = TLS + S + A - TLS_TP_OFFSET
R_RISCV_TLS_DTPREL64 | TLS relocation             |                     | word64 = TLS + S + A - TLS_TP_OFFSET
R_RISCV_TLS_TPREL32  | TLS relocation             |                     | word32 = TLS + S + A + S_TLS_OFFSET - TLS_DTV_OFFSET
R_RISCV_TLS_TPREL64  | TLS relocation             |                     | word64 = TLS + S + A + S_TLS_OFFSET - TLS_DTV_OFFSET
R_RISCV_BRANCH       | PC-relative branch offset  |                     | SB-Type (beq,bne,blt,bge,bltu,bgeu)
R_RISCV_JAL          | PC-relative jump offset    |                     | UJ-Type (jal)
R_RISCV_CALL         | PC-relative function call  |                     | MACRO call,tail (auipc+jalr)
R_RISCV_CALL_PLT     | PC-relative function call  |                     | MACRO call,tail (auipc+jalr)
R_RISCV_GOT_HI20     | PC-relative GOT offset     |                     | MACRO la
R_RISCV_TLS_GOT_HI20 | PC-relative TLS IE GOT ref | %tls_ie_pcrel_hi(x) | MACRO la.tls.ie
R_RISCV_TLS_GD_HI20  | PC-relative TLS GD GOT ref | %tls_gd_pcrel_hi(x) | MACRO la.tls.gd
R_RISCV_PCREL_HI20   | PC-relative reference      | %pcrel_hi(symbol)   | U-Type (auipc)
R_RISCV_PCREL_LO12_I | PC-relative reference      | %pcrel_lo(label)    | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
R_RISCV_PCREL_LO12_S | PC-relative reference      | %pcrel_lo(label)    | S-Type (sb,sh,sw,sd,fsw,fsd)
R_RISCV_HI20         | Absolute address           | %hi(symbol)         | U-Type (lui)
R_RISCV_LO12_I       | Absolute address           | %lo(symbol)         | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
R_RISCV_LO12_S       | Absolute address           | %lo(symbol)         | S-Type (sb,sh,sw,sd,fsw,fsd)
R_RISCV_TPREL_HI20   | TLS LE thread offset       | %tprel_hi(symbol)   | U-Type (auipc)
R_RISCV_TPREL_LO12_I | TLS LE thread offset       | %tprel_lo(label)    | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
R_RISCV_TPREL_LO12_S | TLS LE thread offset       | %tprel_lo(label)    | S-Type (sb,sh,sw,sd,fsw,fsd)
R_RISCV_TPREL_ADD    | TLS LE thread usage        | %tprel_add(label)   | I-Type (addi)
R_RISCV_ADD8         | 8-bit label addition       |                     | word8 = S + A
R_RISCV_ADD16        | 16-bit label addition      |                     | word16 = S + A
R_RISCV_ADD32        | 32-bit label addition      |                     | word32 = S + A
R_RISCV_ADD64        | 64-bit label addition      |                     | word64 = S + A
R_RISCV_SUB8         | 8-bit label subtraction    |                     | word8 = S - A
R_RISCV_SUB16        | 16-bit label subtraction   |                     | word16 = S - A
R_RISCV_SUB32        | 32-bit label subtraction   |                     | word32 = S - A
R_RISCV_SUB64        | 64-bit label subtraction   |                     | word64 = S - A
R_RISCV_GNU_VTINHERIT| GNU C++ vtable hierarchy   |                     |
R_RISCV_GNU_VTENTRY  | GNU C++ vtable member usage|                     |
R_RISCV_ALIGN        | Alignment statement        |                     |
R_RISCV_RVC_BRANCH   | PC-relative branch offset  |                     | CB-Type (c.beqz,c.bnez)
R_RISCV_RVC_JUMP     | PC-relative jump offset    |                     | CJ-Type (c.j)
R_RISCV_RVC_LUI      | Absolute address           |                     | CI-Type (c.lui)
R_RISCV_GPREL_I      | GP-relative reference      | %gprel(symbol)      | I-Type (lb,lbu,lh,lhu,lw,lwu,ld,flw,fld,addi,addiw)
R_RISCV_GPREL_S      | GP-relative reference      | %gprel(symbol)      | S-Type (sb,sh,sw,sd,fsw,fsd)

## Glossary

- TLS LE (Thread Local Storage - Local Exec), Compiler flag: `-ftls-model=local-exec`
- TLS IE (Thread Local Storage - Initial Exec), Compiler flag: `-ftls-model=initial-exec`
- TLS GD (Thread Local Storage - Global Dynamic), Compiler flag: `-ftls-model=global-dynamic`

## References

- TIS Portable Formats Specification, Version 1.1 ELF: Executable and Linkable Format
- ELF-64 Object File Format, Version 1.5 Draft 2
- System V Application Binary Interface AMD64 Architecture Processor Supplement Draft 0.99.6
