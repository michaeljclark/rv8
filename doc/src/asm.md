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

The following example shows how to load an absolute address:

```
.section .text
.globl _start
_start:
	    lui a1,       %hi(msg)       # load msg(hi)
	    addi a1, a1,  %lo(msg)       # load msg(lo)
	    jalr ra, puts
2:	    j2b

.section .rodata
msg:
	.string "Hello World\n"
```

Relative addressing
--------------------------------

The following example shows how to load a PC-relative address:

```
.section .text
.globl _start
_start:
1:	    auipc a1,     %pcrel_hi(msg) # load msg(hi)
	    addi  a1, a1, %pcrel_lo(1b)  # load msg(lo)
	    jalr ra, puts
2:	    j2b

.section .rodata
msg:
	.string "Hello World\n"
```

Constants
-------------------

The following example shows the use of constants. The example
writes a string using polled IO to a UART:

```
.equ UART_BASE, 0x40003000
.equ REG_RBR, 0
.equ REG_TBR, 0
.equ REG_IIR, 2
.equ IIR_TX_RDY, 2
.equ IIR_RX_RDY, 4

1:      auipc a0, %pcrel_hi(msg)    # load msg(hi)
        addi a0, a0, %pcrel_lo(1b)  # load msg(lo)
2:      jal ra, puts
3:      j 3b

puts:
        li a2, UART_BASE
1:      lbu a1, (a0)
        beqz a1, 3f
2:      lbu a3, REG_IIR(a2)
        andi a3, a3, IIR_TX_RDY
        beqz a3, 2b
        sb a1, REG_TBR(a2)
        addi a0, a0, 1
        j 1b
3:      ret

.section .rodata
msg:
	.string "Hello World\n"
```