Assembler
==============

_work in progress_

Directives
-----------------

The following table lists assembler directives:

|Directive|Arguments|Description|
|:--------|:--------|:----------|
|.align      |integer             |section alignment|
|.file       |"filename"          |emit filename FILE LOCAL symbol table|
|.globl      |symbol_name         |emit symbol_name NOTYPE? GLOBAL to symbol table|
|.ident      |"string"            |ignore|
|.section    |[{.text,.data,.rodata,.bss}]|emit section header in argument (if not present, default .text) and make current|
|.size       |symbol, symbol      |ignore|
|.text       |                    |emit .text section (if not present) and make current|
|.data       |                    |emit .data section (if not present) and make current|
|.rodata     |                    |emit .rodata section (if not present) and make current|
|.bss        |                    |emit .bss section (if not present) and make current|
|.string     |"string"            |emit string|
|.type       |symbol, @function   |ignore|
|.option     |{rvc,norvc,push,pop}|RISC-V options|
|.half       |                    |16-bit word|
|.word       |                    |32-bit word|
|.dword      |                    |64-bit word|
|.dtprelword |                    |32-bit thread local word|
|.dtpreldword|                    |64-bit thread local word|
|.p2align    |p2,[pad_val=0],max  |power of 2 align|
|.balign     |b,[pad_val=0]       |byte align|


Addressing pseudo expansions
----------------------------------

The following table lists assembler pseudo expansions:

|Expansion|Description|Instructions/Macros|
|:--------|:----------|:------------------|
|%hi(symbol)                    |Absolute                    |lui                 |
|%lo(symbol)                    |Absolute                    |load, store, add    |
|%pcrel_hi(symbol)              |PC-relative                 |auipc               |
|%pcrel_lo(label)               |PC-relative                 |load, store, add    |
|%tls_ie_pcrel_hi(symbol)       |TLS IE GOT "Initial Exec"   |la.tls.ie           |
|%tls_gd_pcrel_hi(symbol)       |TLS GD GOT "Global Dynamic" |la.tls.gd           |
|%tprel_hi(symbol)              |TLS LE "Local Exec"         |auipc               |
|%tprel_lo(label)               |TLS LE "Local Exec"         |load, store, add    |
|%tprel_add(x)                  |TLS LE "Local Exec"         |assembler expansion |
|%gprel(symbol)                 |GP-relative                 |load, store, add    |

Labels
------------

text labels:

```
labels:
```

numeric labels for `1f`, `1b` type expressions:

```
1:
```

Absolute addressing
------------------------

```
	lui a1, %hi(msg)            # load msg(hi)
	addi a1, a1, %lo(msg)       # load msg(lo)
```

Relative addressing example
------------------------------------

```
1:	auipc a1, %pcrel_hi(msg)    # load msg(hi)
	addi a1, a1, %pcrel_lo(1b)  # load msg(lo)
```
