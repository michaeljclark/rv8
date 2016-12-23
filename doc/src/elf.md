ELF
========

_work in progress_

Notes on RISC-V ELF mapping (unofficial).


## ELF Headers

The following is a list of ELF header fields and potential RISC-V mappings.

### e_ident[EI_DATA]

- ELFDATA2LSB for little-endian
- ELFDATA2MSB for big-endian (not applicable to RISC-V)

### e_flags

MISA could potentially be stored in `e_flags` i.e. `-march` compiler flags.
This would be similar to the SPARC ELF Headers.

- EF_RISCV_EXT_MASK = 0x03ffffff (Processor extension mask from `misa` CSR)

ARM is using e_flags (`EF_ARM_BE8` and `EF_ARM_LE8`) for endianness but this
should be in `e_ident[EI_DATA]`. ARM is using e_flags for ABI version
(`EF_ARM_EABIMASK`) but this should be in `e_ident[EI_OSABI]` and/or
`e_ident[EI_ABIVERSION]`.

### e_ident[EI_OSABI]

- ELFOSABI_SYSV = 0  

### e_ident[EI_ABIVERSION]

- RVABI_HARD = 0
- RVABI_V1 = 1 (soft float via library dispatch, 64-bit fill in function on RV32)


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
