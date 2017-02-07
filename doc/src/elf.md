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

# Relocations

The following table provides details of the RISC-V ELF relocations:

Enum | ELF Reloc Type       | Description                         | Details
:--- | :------------------  | :---------------                    | :-----------
0    | R_RISCV_NONE         | None                                |
1    | R_RISCV_32           | Runtime relocation                  | word32 = S + A
2    | R_RISCV_64           | Runtime relocation                  | word64 = S + A
3    | R_RISCV_RELATIVE     | Runtime relocation                  | word32,64 = B + A
4    | R_RISCV_COPY         | Runtime relocation                  | must be in executable. not allowed in shared library
5    | R_RISCV_JUMP_SLOT    | Runtime relocation                  | word32,64 = S ;handled by PLT unless LD_BIND_NOW
6    | R_RISCV_TLS_DTPMOD32 | TLS relocation                      | word32 = S->TLSINDEX
7    | R_RISCV_TLS_DTPMOD64 | TLS relocation                      | word64 = S->TLSINDEX
8    | R_RISCV_TLS_DTPREL32 | TLS relocation                      | word32 = TLS + S + A - TLS_TP_OFFSET
9    | R_RISCV_TLS_DTPREL64 | TLS relocation                      | word64 = TLS + S + A - TLS_TP_OFFSET
10   | R_RISCV_TLS_TPREL32  | TLS relocation                      | word32 = TLS + S + A + S_TLS_OFFSET - TLS_DTV_OFFSET
11   | R_RISCV_TLS_TPREL64  | TLS relocation                      | word64 = TLS + S + A + S_TLS_OFFSET - TLS_DTV_OFFSET
16   | R_RISCV_BRANCH       | PC-relative branch offset           | SB-Type
17   | R_RISCV_JAL          | PC-relative jump offset             | UJ-Type
18   | R_RISCV_CALL         | PC-relative function call           | MACRO call,tail (auipc+jalr pair)
19   | R_RISCV_CALL_PLT     | PC-relative function call           | MACRO call,tail (auipc+jalr pair) PIC
20   | R_RISCV_GOT_HI20     | PC-relative GOT offset              | MACRO la
21   | R_RISCV_TLS_GOT_HI20 | PC-relative TLS IE GOT reference    | MACRO la.tls.ie %tls_ie_pcrel_hi(x)
22   | R_RISCV_TLS_GD_HI20  | PC-relative TLS GD GOT reference    | MACRO la.tls.gd %tls_gd_pcrel_hi(x)
23   | R_RISCV_PCREL_HI20   | PC-relative reference  (U-Type)     | %pcrel_hi(symbol)
24   | R_RISCV_PCREL_LO12_I | PC-relative reference (I-Type)      | %pcrel_lo(symbol)
25   | R_RISCV_PCREL_LO12_S | PC-relative reference (S-Type)      | %pcrel_lo(symbol)
26   | R_RISCV_HI20         | Absolute address (U-Type)           | %hi(symbol)
27   | R_RISCV_LO12_I       | Absolute address (I-Type)           | %lo(symbol)
28   | R_RISCV_LO12_S       | Absolute address (S-Type)           | %lo(symbol)
29   | R_RISCV_TPREL_HI20   | TLS LE thread offset (U-Type)       | %tprel_hi(symbol)
30   | R_RISCV_TPREL_LO12_I | TLS LE thread offset (I-Type)       | %tprel_lo(symbol)
31   | R_RISCV_TPREL_LO12_S | TLS LE thread offset (S-Type)       | %tprel_lo(symbol)
32   | R_RISCV_TPREL_ADD    | TLS LE thread usage                 | %tprel_add(symbol)
33   | R_RISCV_ADD8         | 8-bit label addition                | word8 = S + A
34   | R_RISCV_ADD16        | 16-bit label addition               | word16 = S + A
35   | R_RISCV_ADD32        | 32-bit label addition               | word32 = S + A
36   | R_RISCV_ADD64        | 64-bit label addition               | word64 = S + A
37   | R_RISCV_SUB8         | 8-bit label subtraction             | word8 = S - A
38   | R_RISCV_SUB16        | 16-bit label subtraction            | word16 = S - A
39   | R_RISCV_SUB32        | 32-bit label subtraction            | word32 = S - A
40   | R_RISCV_SUB64        | 64-bit label subtraction            | word64 = S - A
41   | R_RISCV_GNU_VTINHERIT| GNU C++ vtable hierarchy            |
42   | R_RISCV_GNU_VTENTRY  | GNU C++ vtable member usage         |
43   | R_RISCV_ALIGN        | Alignment statement                 |
44   | R_RISCV_RVC_BRANCH   | PC-relative branch offset (CB-Type) |
45   | R_RISCV_RVC_JUMP     | PC-relative jump offset (CJ-Type)   |
46   | R_RISCV_RVC_LUI      | Absolute address (CI-Type)          |
47   | R_RISCV_GPREL_I      | GP-relative reference (I-Type)      |
48   | R_RISCV_GPREL_S      | GP-relative reference (S-Type)      |
49   | R_RISCV_TPREL_I      | TP-relative TLS LE load (I-Type)    |
50   | R_RISCV_TPREL_S      | TP-relative TLS LE store (S-Type)   |
51   | R_RISCV_RELAX        | Instruction pair can be relaxed     | (auipc+jalr pair)
52   | R_RISCV_SUB6         | Local label subtraction             |
53   | R_RISCV_SET6         | Local label subtraction             |
54   | R_RISCV_SET8         | Local label subtraction             |
55   | R_RISCV_SET16        | Local label subtraction             |
56   | R_RISCV_SET32        | Local label subtraction             |

The following table provides details on the variables used in address calculation:

Variable       | Description
:------------- | :----------------
A              | Addend field in the relocation entry associated with the symbol
B              | Base address of a shared object loaded into memory
G              | Offset of the symbol into the GOT (Global Offset Table)
S              | Value of the symbol in the symbol table
GP             | Global Pointer register (x3)

While the linker can make relocations on arbitrary memory locations, many
relocations are designed for use with specific instructions or instruction
sequences. For clarity, the description of those relocations assumes they
are used in the intended context.


### Absolute Addresses

32-bit absolute addresses in position dependent code are loaded with a pair
of instructions which have an associated pair of relocations:
`R_RISCV_HI20` plus `R_RISCV_LO12_I` or `R_RISCV_LO12_S`.

The `R_RISCV_HI20` refers to an `LUI` instruction containing the high
20-bits to be relocated to an absolute symbol address. The `LUI` instruction
is followed by an I-Type instruction (add immediate or load) with an
`R_RISCV_LO12_I` relocation or an S-Type instruction (store) and an
`R_RISCV_LO12_S` relocation. The addresses for pair of relocations are
calculated like this:

 - `hi20 = ((symbol_address + 0x800) >> 12);`
 - `lo12 = symbol_address - hi20;`


### Global Offset Table

For position independent code in dynamically linked objects, each shared
object contains a GOT (Global Offset Table) which contains addresses of
global symbols (objects and functions) referred to by the dynamically
linked shared object.

...


### Program Linkage Table

The PLT (Program Linkage Table) exists to allow function calls between
dynamically linked shared objects. Each dynamic object has its own
GOT (Global Offset Table) and PLT (Program Linkage Table).

The first entry of a shared objects PLT is a special entry that calls
`_dl_runtime_resolve` to resolve the GOT offset for the called function.
The ``_dl_runtime_resolve` function in the dynamic loader resolves the
GOT offsets lazily on the first call to the function except when
`LD_BIND_NOW` is set in which case the GOT entries are populated by the
dynamic linker before the exutable is started. Lazy resolution of GOT
entries is intended to speed up program loading by deferring the symbol
resolution to the first time the function is called. The first entry in
the PLT which corresponds to an unpopulated GOT entry for the symbol is
a resolver stub entry which takes up 32 bytes (two 16 byte entries):

```
1:   auipc  t2, %pcrel_hi(.got.plt)
     sub    t1, t1, t3               # shifted .got.plt offset + hdr size + 12
     l[w|d] t3, %pcrel_lo(1b)(t2)    # _dl_runtime_resolve
     addi   t1, t1, -(hdr size + 12) # shifted .got.plt offset
     addi   t0, t2, %lo(.got.plt)    # &.got.plt
     srli   t1, t1, log2(16/PTRSIZE) # .got.plt offset
     l[w|d] t0, PTRSIZE(t0)          # link map
     jr     t3
```

Subsequent function entry stubs in the PLT take up 16 bytes and load
the function pointer from the GOT. On first call the GOT entry is zero
and the first PLT entry is called which calls `_dl_runtime_resolve`
and fills in the GOT entry for subsequent calls to the function:

```
1:  auipc   t3, %pcrel_hi(function@.got.plt)
    l[w|d]  t3, %pcrel_lo(1b)(t3)
    jalr    t1, t3
    nop
```


### Procedure Calls

`R_RISCV_CALL` or `R_RISCV_CALL_PLT` and `R_RISCV_RELAX` relocations are
associated with pairs of instructions (`AUIPC+JALR`) generated by the `CALL`
or `TAIL` pseudo instructions.

In position dependent code (`-fno-pic`) the `AUIPC` instruction in the
`AUIPC+JALR` pair has both a `R_RISCV_CALL` relocation and a `R_RISCV_RELAX`
relocation indicating the instruction sequence can be relaxed during linking.

In position independent code (`-fPIC`, `-fpic` or `-fpie`) the `AUIPC`
instruction in the `AUIPC+JALR` pair has both a `R_RISCV_CALL_PLT` relocation
and a `R_RISCV_RELAX` relocation indicating the instruction sequence can be
relaxed during linking.

Procedure call linker relaxation allows the `AUIPC+JALR` pair to be relaxed
to the `JAL` instruction when the prodecure or PLT entry is within (-2MiB to
+2MiB-1) of the instruction pair.


### PC-Relative Jumps and Branches

Unconditional jump (UJ-Type) instructions have a `R_RISCV_JAL` relocation
that can represent an even signed 21-bit offset (-2MiB to +2MiB-1).

Branch (SB-Type) instructions have a `R_RISCV_BRANCH` relocation that
can represent an even signed 13-bit offset (-4096 to +4095).


### PC-Relative Symbol Addresses

32-bit PC-relative relocations for symbol addresses on sequences of
instructions such as the `AUIPC+ADDI` instruction pair expanded from
the `la` pseudo-instruction, in position independent code typically
have an associated pair of relocations: `R_RISCV_PCREL_HI20` plus
`R_RISCV_PCREL_LO12_I` or `R_RISCV_PCREL_LO12_S`.

The `R_RISCV_PCREL_HI20` relocation refers to an `AUIPC` instruction
containing the high 20-bits to be relocated to a symbol relative to the
program counter address of the `AUIPC` instruction. The `AUIPC`
instruction is followed by an I-Type instruction (add immediate or load)
with an `R_RISCV_PCREL_LO12_I` relocation or an S-Type instruction (store)
and an `R_RISCV_PCREL_LO12_S` relocation.

The `R_RISCV_PCREL_LO12_I` or `R_RISCV_PCREL_LO12_S` relocations contain
a label pointing to an instruction with a `R_RISCV_PCREL_HI20` relocation
entry that points to the target symbol:

 - At label: `R_RISCV_PCREL_HI20` relocation entry ⟶ symbol
 - `R_RISCV_PCREL_LO12_I` relocation entry ⟶ label

To get the symbol address to perform the calculation to fill the 12-bit
immediate on the add, load or store instruction the linker finds the
`R_RISCV_PCREL_HI20` relocation entry associated with the `AUIPC`
instruction. The addresses for pair of relocations are calculated like this:

 - `hi20 = ((symbol_address - hi20_reloc_offset + 0x800) >> 12);`
 - `lo12 = symbol_address - hi20_reloc_offset - hi20;`

The successive instruction has a signed 12-bit immediate so the value of the
preceding high 20-bit relocation may have 1 added to it.

Note the compiler emitted instructions for PC-relative symbol addresses are
not necessarily sequential or in pairs. There is a constraint is that the
instruction with the `RISCV_R_PCREL_LO12_I` or `RISCV_R_PCREL_LO12_S`
relocation label points to a valid HI20 PC-relative relocation pointing to
the symbol. e.g.

```
label:
   auipc t0, %pcrel_hi(sym)      # t0 := label + (sym - label + 0x800)[31:12]
   lui t1, 1
   lw t2, t0, %pcrel_lo(label)   # t0 := t0 + (sym - label)[11:0]
   add t2, t2, t1
   sw t2, t0, %pcrel_lo(label)   # t0 := t0 + (sym - label)[11:0]
```

## Thread Local Storage

RISC-V adopts the ELF Thread Local Storage Model in which ELF objects define
`.tbss` and `.tdata` sections and `PT_TLS` program headers that contain the
TLS "initialization images" for new threads. The `.tbss` and `.tdata` sections
are not referenced directly like regular segments, rather they are copied or
allocated to the thread local storage space of newly created threads.
See [https://www.akkadia.org/drepper/tls.pdf](https://www.akkadia.org/drepper/tls.pdf).

In The ELF Thread Local Storage Model, TLS offsets are used instead of pointers.
The ELF TLS sections are initialization images for the thread local variables of
each new thread. A TLS offset defines an offset into the dynamic thread vector
which is pointed to by the TCB (Thread Control Block) held in the `tp` register.

There are various thread local storage models for statically allocated or
dynamically allocated thread local storage. The following table lists the
thread local storage models:

Mnemonic | Model          | Compiler flags
:------- | :---------     | :-------------------
TLS LE   | Local Exec     | `-ftls-model=local-exec`
TLS IE   | Initial Exec   | `-ftls-model=initial-exec`
TLS LD   | Local Dynamic  | `-ftls-model=local-dynamic`
TLS GD   | Global Dynamic | `-ftls-model=global-dynamic`

The program linker in the case of static TLS or the dynamic linker in the case
of dynamic TLS allocate TLS offsets for storage of thread local variables.


### Local Exec

Local exec is a form of static thread local storage. This model is used
when static linking as the TLS offsets are resolved during program linking.

- Compiler flag `-ftls-model=local-exec`
- Variable attribute: `__thread int i __attribute__((tls_model("local-exec")));`

Example assembler load and store of a thread local variable `i` using the
`%tprel_hi`, `%tprel_add` and `%tprel_lo` assembler functions. The emitted
relocations are in comments.

```
  lui  a5,%tprel_hi(i)         # R_RISCV_TPREL_HI20
  add  a5,a5,tp,%tprel_add(i)  # R_RISCV_TPREL_ADD
  lw   t0,%tprel_lo(i)(a5)     # R_RISCV_TPREL_LO12_I
  addi t0,a0,1
  sw   t0,%tprel_lo(i)(a5)     # R_RISCV_TPREL_LO12_S
```

The `%tprel_add` assembler function does not return a value and is used purely
to associate the `R_RISCV_TPREL_ADD` relocation with the `add` instruction.


### Initial Exec

Initial exec is is a form of static thread local storage that can be used in
shared libraries that use thread local storage. TLS relocations are performed
at load time. `dlopen` calls to libraries that use thread local storage may fail
when using the initial exec thread local storage model as TLS offsets must all
be resolved at load time. This model uses the GOT to resolve TLS offsets.

- Compiler flag `-ftls-model=initial-exec`
- Variable attribute: `__thread int i __attribute__((tls_model("initial-exec")));`
- ELF flags: DF_STATIC_TLS

Example assembler load and store of a thread local variable `i` using the
`la.tls.ie` psuedo-instruction, with the emitted TLS relocations in comments:

```
  la.tls.ie a5,i    # auipc+{ld,lw}  R_RISCV_TLS_GOT_HI20, R_RISCV_PCREL_LO12_I
  add  a5,a5,tp
  lw   t0,0(a5)
  addi t0,a0,1
  sw   t0,0(a5)
```


### Global Dynamic

RISC-V local dynamic and global dynamic TLS models generate equivalent object code.
The Global dynamic thread local storage model is used for PIC Shared libraries and
handles the case where more than one library uses thread local variables, and
additionally allows libraries may be loaded and unloaded at runtime using `dlopen`.
In the global dynamic model, application code calls the dynamic linker function
`__tls_get_addr` to locate TLS offsets into the dynamic thread vector at runtime.

- Compiler flag `-ftls-model=global-dynamic`
- Variable attribute: `__thread int i __attribute__((tls_model("global-dynamic")));`

Example assembler load and store of a thread local variable `i` using the
`la.tls.gd` pseudo-instruction, with the emitted TLS relocations in comments:

```
  la.tls.gd a0,i    # auipc+addi  R_RISCV_TLS_GD_HI20,  R_RISCV_PCREL_LO12_I
  call  __tls_get_addr@plt
  mv   a5,a0
  lw   t0,0(a5)
  addi t0,a0,1
  sw   t0,0(a5)
```

In the Global Dynamic model, the runtime library provides the `__tls_get_addr` function:

```
extern void *__tls_get_addr (tls_index *ti);
```

where the type tls index are defined as:

```
typedef struct
{
  unsigned long int ti_module;
  unsigned long int ti_offset;
} tls_index;
```

## References

- TIS Portable Formats Specification, Version 1.1 ELF: Executable and Linkable Format
- ELF-64 Object File Format, Version 1.5 Draft 2
- System V Application Binary Interface AMD64 Architecture Processor Supplement Draft 0.99.6
- ELF Handling for Thread-Local Storage
