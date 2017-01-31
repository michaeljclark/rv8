Assembler
==============

_work in progress_

Directives
-----------------

The following table lists assembler directives:

Directive    | Arguments                    | Description
:----------- | :-------------               | :---------------
.align       | integer                      | align to power of 2 (alias for .p2align)
.file        | "filename"                   | emit filename FILE LOCAL symbol table
.globl       | symbol_name                  | emit symbol_name to GLOBAL to symbol table
.ident       | "string"                     | ignore
.section     | [{.text,.data,.rodata,.bss}] | emit section (if not present, default .text) and make current
.size        | symbol, symbol               | ignore
.text        |                              | emit .text section (if not present) and make current
.data        |                              | emit .data section (if not present) and make current
.rodata      |                              | emit .rodata section (if not present) and make current
.bss         |                              | emit .bss section (if not present) and make current
.string      | "string"                     | emit string
.asciz       | "string"                     | emit string (alias for .string)
.equ         | name, value                  | constant definition
.macro       | name arg1 [, argn]           | begin macro definition \argname to substitute
.endm        |                              | end macro definition
.type        | symbol, @function            | ignore
.option      | {rvc,norvc,push,pop}         | RISC-V options
.byte        |                              | 8-bit comma separated words
.half        |                              | 16-bit comma separated words
.word        |                              | 32-bit comma separated words
.dword       |                              | 64-bit comma separated words
.dtprelword  |                              | 32-bit thread local word
.dtpreldword |                              | 64-bit thread local word
.p2align     | p2,[pad_val=0],max           | align to power of 2
.balign      | b,[pad_val=0]                | byte align
.zero        | integer                      | zero bytes

Pseudo expansions
----------------------

The following table lists assembler pseudo expansions:

Assembler Notation       | Description                 | Instruction / Macro
:----------------------  | :---------------            | :-------------------
%hi(symbol)              | Absolute (HI20)             | lui
%lo(symbol)              | Absolute (LO12)             | load, store, add
%pcrel_hi(symbol)        | PC-relative (HI20)          | auipc
%pcrel_lo(label)         | PC-relative (LO12)          | load, store, add
%tls_ie_pcrel_hi(symbol) | TLS IE GOT "Initial Exec"   | la.tls.ie
%tls_gd_pcrel_hi(symbol) | TLS GD GOT "Global Dynamic" | la.tls.gd
%tprel_hi(symbol)        | TLS LE "Local Exec"         | auipc
%tprel_lo(label)         | TLS LE "Local Exec"         | load, store, add
%tprel_add(offset)       | TLS LE "Local Exec"         | add
%gprel(symbol)           | GP-relative                 | load, store, add

Labels
------------

Text labels are used as branch, unconditional jump targets and symbol offsets.
Text labels are added to the symbol table of the compiled module.

```
loop:
    j loop
```

Numeric labels are used for local references. References to local labels are
suffixed with 'f' for a forward reference or 'b' for a backwards reference.

```
1:
    j 1b
```

Absolute addressing
------------------------

```
.section .text
	lui a1,       %hi(msg)       # load msg(hi)
	addi a1, a1,  %lo(msg)       # load msg(lo)
	jalr ra, puts

.section .rodata
msg:
	.string "Hello World"
```

Relative addressing
--------------------------------

```
.section .text
1:	auipc a1,     %pcrel_hi(msg) # load msg(hi)
	addi  a1, a1, %pcrel_lo(1b)  # load msg(lo)
	jalr ra, puts

.section .rodata
msg:
	.string "Hello World"
```
