ELF Relocations
====================

_work in progress_

The following table provides information of the RISC-V ELF relocations.

|ELF Reloc Type|Description|Assembler|Details|
|:-------------|:----------|:--------|:------|
|R_RISCV_NONE|None||||
|R_RISCV_32|Runtime relocation||word32 = S + A|
|R_RISCV_64|Runtime relocation||word64 = S + A|
|R_RISCV_RELATIVE|Runtime relocation||word32,64 = B + A|
|R_RISCV_COPY|Runtime relocation||symbol must be in executable; not allowed in shared library|
|R_RISCV_JUMP_SLOT|Runtime relocation||word32,64 = S (handled by PLT unless LD_BIND_NOW)|
|R_RISCV_TLS_DTPMOD32|TLS relocation||word32 = S->TLSINDEX|
|R_RISCV_TLS_DTPMOD64|TLS relocation||word64 = S->TLSINDEX|
|R_RISCV_TLS_DTPREL32|TLS relocation||word32 = TLS + S + A - TLS_TP_OFFSET|
|R_RISCV_TLS_DTPREL64|TLS relocation||word64 = TLS + S + A - TLS_TP_OFFSET|
|R_RISCV_TLS_TPREL32|TLS relocation||word32 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET|
|R_RISCV_TLS_TPREL64|TLS relocation||word64 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET|
|R_RISCV_BRANCH|PC-relative branch offset||SB-Type (beq,bne,blt,bge,bltu,bgeu)|
|R_RISCV_JAL|PC-relative jump offset||UJ-Type (jal)|
|R_RISCV_CALL|PC-relative function call||MACRO call (auipc/jalr)|
|R_RISCV_CALL_PLT|PC-relative function call||MACRO tail (auipc/jalr)|
|R_RISCV_GOT_HI20|PC-relative GOT offset||MACRO la|
|R_RISCV_TLS_GOT_HI20|PC-relative TLS IE GOT ref|%tls_ie_pcrel_hi(x)|Macro (la.tls.ie); -ftls-model=initial-exec|
|R_RISCV_TLS_GD_HI20|PC-relative TLS GD GOT ref|%tls_gd_pcrel_hi(x)|Macro (la.tls.gd); -ftls-model=global-dynamic|
|R_RISCV_PCREL_HI20|PC-relative reference|%pcrel_hi(symbol)|U-Type (auipc)|
|R_RISCV_PCREL_LO12_I|PC-relative reference|%pcrel_lo(label)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_PCREL_LO12_S|PC-relative reference|%pcrel_lo(label)|S-Type (sb,sh,sw)|
|R_RISCV_HI20|Absolute address|%hi(symbol)|U-Type (lui,auipc)|
|R_RISCV_LO12_I|Absolute address|%lo(symbol)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_LO12_S|Absolute address|%lo(symbol)|S-Type (sb,sh,sw)|
|R_RISCV_TPREL_HI20|TLS LE thread pointer offset|%tprel_hi(symbol)|U-Type (auipc);    -ftls-model=local-exec|
|R_RISCV_TPREL_LO12_I|TLS LE thread pointer offset|%tprel_lo(label)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_TPREL_LO12_S|TLS LE thread pointer offset|%tprel_lo(label)|S-Type (sb,sh,sw)|
|R_RISCV_TPREL_ADD|TLS LE thread pointer usage| %tprel_add(label)|internal assembler expansion|
|R_RISCV_ADD8|8-bit  add||word8 = S + A|label addition|
|R_RISCV_ADD16|16-bit add||word16 = S + A|label addition|
|R_RISCV_ADD32|32-bit add||word32 = S + A|label addition|
|R_RISCV_ADD64|64-bit add||word64 = S + A|label addition|
|R_RISCV_SUB8|8-bit  sub||word8 = S - A|label subtraction|
|R_RISCV_SUB16|16-bit sub||word16 = S - A|label subtraction|
|R_RISCV_SUB32|32-bit sub||word32 = S - A|label subtraction|
|R_RISCV_SUB64|64-bit sub||word64 = S - A|label subtraction|
|R_RISCV_GNU_VTINHERIT|GNU extension to record C++ vtable hierarchy|
|R_RISCV_GNU_VTENTRY|GNU extension to record C++ vtable member usage|
|R_RISCV_ALIGN|Alignment statement|||
|R_RISCV_RVC_BRANCH|PC-relative branch offset||SB-Type (c.beqz,c.bnez)|
|R_RISCV_RVC_JUMP|PC-relative jump offset||UJ-Type (c.j)|
|R_RISCV_RVC_LUI|Absolute address||CI-Type (c.lui)|
|R_RISCV_GPREL_I|PC-relative reference|%gprel(symbol)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_GPREL_S|PC-relative reference|%gprel(symbol)|S-Type (sb,sh,sw)|
