ELF Relocations
====================

The following table provides information of the RISC-V ELF relocations.

|ELF Reloc Type|Number|Size|Bits|Description|Assembler|Details|
|:-------------|:-----|----|----|:----------|:--------|:------|
|R_RISCV_NONE|0||xx|None||||
|R_RISCV_32|1|32||Runtime relocation||word32 = S + A|
|R_RISCV_64|2|64||Runtime relocation||word64 = S + A|
|R_RISCV_RELATIVE|3|32||Runtime relocation||word32,64 = B + A|
|R_RISCV_COPY|4|0||Runtime relocation||symbol must be in executable; not allowed in shared library|
|R_RISCV_JUMP_SLOT|5|64||Runtime relocation||word32,64 = S (handled by PLT unless LD_BIND_NOW)|
|R_RISCV_TLS_DTPMOD32|6|32||TLS relocation||word32 = S->TLSINDEX|
|R_RISCV_TLS_DTPMOD64|7|64||TLS relocation||word64 = S->TLSINDEX|
|R_RISCV_TLS_DTPREL32|8|32||TLS relocation||word32 = TLS + S + A - TLS_TP_OFFSET|
|R_RISCV_TLS_DTPREL64|9|64||TLS relocation||word64 = TLS + S + A - TLS_TP_OFFSET|
|R_RISCV_TLS_TPREL32|10|32||TLS relocation||word32 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET|
|R_RISCV_TLS_TPREL64|11|64||TLS relocation||word64 = TLS + S + A + S->TLS_OFFSET - TLS_DTV_OFFSET|
|R_RISCV_BRANCH|16|32|12:1|PC-relative branch offset||SB-Type (beq,bne,blt,bge,bltu,bgeu)|
|R_RISCV_JAL|17|32|20:1|PC-relative jump offset||UJ-Type (jal)|
|R_RISCV_CALL|18|64|31:0|PC-relative function call||MACRO call (auipc/jalr)|
|R_RISCV_CALL_PLT|19|64|31:0|PC-relative function call||MACRO tail (auipc/jalr)|
|R_RISCV_GOT_HI20|20|32|31:12|PC-relative GOT offset||MACRO la|
|R_RISCV_TLS_GOT_HI20|21|32|31:12|PC-relative TLS IE GOT ref|%tls_ie_pcrel_hi(x)|Macro (la.tls.ie); -ftls-model=initial-exec|
|R_RISCV_TLS_GD_HI20|22|32|31:12|PC-relative TLS GD GOT ref|%tls_gd_pcrel_hi(x)|Macro (la.tls.gd); -ftls-model=global-dynamic|
|R_RISCV_PCREL_HI20|23|32|31:12|PC-relative reference|%pcrel_hi(symbol)|U-Type (auipc)|
|R_RISCV_PCREL_LO12_I|24|32|11:0|PC-relative reference|%pcrel_lo(label)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_PCREL_LO12_S|25|32|11:0|PC-relative reference|%pcrel_lo(label)|S-Type (sb,sh,sw)|
|R_RISCV_HI20|26|32|31:12|Absolute address|%hi(symbol)|U-Type (lui,auipc)|
|R_RISCV_LO12_I|27|32|11:0|Absolute address|%lo(symbol)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_LO12_S|28|32|11:0|Absolute address|%lo(symbol)|S-Type (sb,sh,sw)|
|R_RISCV_TPREL_HI20|29|32|31:12|TLS LE thread pointer offset|%tprel_hi(symbol)|U-Type (auipc);    -ftls-model=local-exec|
|R_RISCV_TPREL_LO12_I|30|32|11:0|TLS LE thread pointer offset|%tprel_lo(label)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_TPREL_LO12_S|31|32|11:0|TLS LE thread pointer offset|%tprel_lo(label)|S-Type (sb,sh,sw)|
|R_RISCV_TPREL_ADD|32|32|31:0|TLS LE thread pointer usage| %tprel_add(label)|internal assembler expansion|
|R_RISCV_ADD8|33|8||8-bit  add||word8 = S + A|label addition|
|R_RISCV_ADD16|34|16||16-bit add||word16 = S + A|label addition|
|R_RISCV_ADD32|35|32||32-bit add||word32 = S + A|label addition|
|R_RISCV_ADD64|36|64||64-bit add||word64 = S + A|label addition|
|R_RISCV_SUB8|37|8||8-bit  sub||word8 = S - A|label subtraction|
|R_RISCV_SUB16|38|16||16-bit sub||word16 = S - A|label subtraction|
|R_RISCV_SUB32|39|32||32-bit sub||word32 = S - A|label subtraction|
|R_RISCV_SUB64|40|64||64-bit sub||word64 = S - A|label subtraction|
|R_RISCV_GNU_VTINHERIT|41|32||GNU extension to record C++ vtable hierarchy|
|R_RISCV_GNU_VTENTRY|42|32||GNU extension to record C++ vtable member usage|
|R_RISCV_ALIGN|43|xx||Alignment statement|||
|R_RISCV_RVC_BRANCH|44|xx|8:1|PC-relative branch offset||SB-Type (c.beqz,c.bnez)|
|R_RISCV_RVC_JUMP|45|xx|11:1|PC-relative jump offset||UJ-Type (c.j)|
|R_RISCV_RVC_LUI|46|xx|17:12|Absolute address||CI-Type (c.lui)|
|R_RISCV_GPREL_I|47|32|11:0|PC-relative reference|%gprel(symbol)|I-Type (lb,lbu,lh,lhu,lw,lwu,addi,addiw)|
|R_RISCV_GPREL_S|48|32|11:|PC-relative reference|%gprel(symbol)|S-Type (sb,sh,sw)|
