# RISC-V Meta

[![Build Status](https://travis-ci.org/michaeljclark/riscv-meta.svg?branch=master)](https://travis-ci.org/michaeljclark/riscv-meta)

RISC-V Meta is a suite of tools that operate on RISC-V ISA (Instruction
Set Architecture) Metadata. The RISC-V Meta suite of tools can generate
RISC-V instruction opcode maps, C headers and source containing instruction
set metadata, instruction decoders, JIT encoder, instruction interpreters
and LaTeX documentation (`riscv-parse-meta`).

The suite also contains simple decoupled libraries with an ELF parser, a
functional metadata based RISC-V disassembler (`riscv-parse-elf`), a
RVC compressor / decompressor (`riscv-compress-elf`) plus library routines
and tools for generating statistics on RISC-V executables (`riscv-histogram-elf`).

RISC-V Meta is the starting point for a RISC-V binary translation and security
sandboxing investigation.

The future goals of the RISC-V Meta project are:

- Concise metadata representing the RISC-V ISA
- Tools for metadata-based generation of source and documentation 
- Full color disassembler
- ELF Binary compressor, decompressor
- High performance emulation, sandboxing and binary translation
- RISC-V-(n) → RISC-V-(n+1)
- RISC-V-(n) → Intel i786 + AVX-512
- RISC-V Linux ABI emulation on MacOS, Windows, Linux and *BSD
- RISC-V Linux ABI randomisation and entropy coding
- RISC-V Specification undefined behaviour investigation
- RISC-V Virtualization and memory protection investigation

See [RISC-V Instruction Set Listing](/doc/pdf/riscv-instructions.pdf) for sample LaTeX output.

## Screenshots

![ASCII map screenshot](/doc/images/screenshot-1.png)

*Example ASCII map output from make map*

![Histogram screenshot](/doc/images/screenshot-2.png)

*Example Instructions Histogram from riscv-histogram-elf*

![Histogram screenshot](/doc/images/screenshot-3.png)

*Example Registers Histogram from riscv-histogram-elf*

![RISC-V disassembly screenshot](/doc/images/screenshot-4.png)

*Example Disassembly output from riscv-parse-elf*

## References

- [Native Client ARM 32-bit Sandbox](https://developer.chrome.com/native-client/reference/sandbox_internals/arm-32-bit-sandbox)
- [Native Client x86-64 Sandbox](https://developer.chrome.com/native-client/reference/sandbox_internals/x86-64-sandbox)
- [lowRISC project](http://www.lowrisc.org/)
- [RISC-V ISA Specification](http://riscv.org/specifications/)
- [RISC-V Compressed ISA Specification](http://riscv.org/specifications/compressed-isa/)
- [RISC-V Privileged ISA Specification](http://riscv.org/specifications/privileged-isa/)
- [RISC-V ISA Simulator](https://github.com/riscv/riscv-isa-sim/)
- [RISC-V Opcodes](https://github.com/riscv/riscv-opcodes/)
- [RISC-V LLVM](https://github.com/riscv/riscv-llvm/)
- [RISC-V Clang](https://github.com/riscv/riscv-clang/)
- [Clang SafeStack](http://clang.llvm.org/docs/SafeStack.html)
- [musl libc](https://www.musl-libc.org/)
- [Yocto Project](https://www.yoctoproject.org/)
- [Two-Level Segregated Fit memory allocator implementation](https://github.com/mattconte/tlsf/)
- [Complete x86/x64 JIT and Remote Assembler for C++](https://github.com/kobalicek/asmjit/)
- [Abstractions for Practical Virtual Machine Replay](https://www.cs.utah.edu/~aburtsev/doc/vee16-xentt.pdf)
- [A Secure Processor Architecture for Encrypted Computation on Untrusted Programs](https://people.csail.mit.edu/devadas/pubs/ascend-stc12.pdf)
- [SoK: Eternal War in Memory](https://www.cs.berkeley.edu/~dawnsong/papers/Oakland13-SoK-CR.pdf)
- [SoK: Introspections on Trust and the Semantic Gap](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=6956590)
- [Hacking Blind](http://www.scs.stanford.edu/~abelay/pdf/bittau:brop.pdf)
- [Code Pointer Integrity](http://dslab.epfl.ch/pubs/cpi.pdf)
- [Geometric Memory Management](http://arxiv.org/pdf/1512.09358.pdf)
- [TLSF: a New Dynamic Memory Allocator for Real-Time Systems](http://www.gii.upv.es/tlsf/files/ecrts04_tlsf.pdf)
- [A Precise Memory Model for Low-Level Bounded Model Checking](https://www.usenix.org/legacy/event/ssv10/tech/full_papers/Sinz.pdf)
- [Design of the RISC-V Instruction Set Architecture](http://www.eecs.berkeley.edu/~waterman/papers/phd-thesis.pdf)
- [IEEE Std 1003.1™, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/)
- [Standard C++](https://isocpp.org/std/the-standard/)
- [Exception Handling in LLVM](http://llvm.org/docs/ExceptionHandling.html)
- [The Error Model](http://joeduffyblog.com/2016/02/07/the-error-model/) and other essays
- [No sane compiler would optimize atomics](https://github.com/jfbastien/no-sane-compiler/)
- A Guide to Undefined Behavior in C and C++ ([part 1](http://blog.regehr.org/archives/213) [part 2](http://blog.regehr.org/archives/226) [part 3](http://blog.regehr.org/archives/232))

## Project Structure

|Directory|Description|
|:---|:---------------|
|`meta`|Instruction set metadata|
|`src/abi`|Application binary interface|
|`src/app`|Test programs and utilities|
|`src/asm`|Assembler metadata library|
|`src/elf`|ELF parser library|
|`src/emulator`|ISA emulator headers|
|`src/model`|ISA metamodel library|
|`src/util`|Miscellaneous utilities library|
|`doc/pdf`|Generated documentation|

## Libraries

The following table shows the RISC-V Meta libraries:

|Name|Description|Scale|
|:---|:----------|-----|
|libriscv_asm.a|disassembly formatting|micro|
|libriscv_elf.a|ELF parser|micro|
|libriscv_meta.a|instruction set metadata|nano|
|libriscv_model.a|instruction set metamodel|macro|
|libriscv_util.a|utility functions for tools|mini|

## Tools

The following table shows the RISC-V Meta tools:

|Name|Description|
|:---|:----------|
|riscv-parse-meta|generates source, headers and documentation|
|riscv-parse-elf|ELF parser and disassmbler tool|
|riscv-compress-elf|ELF compressor tool (work in progress)|
|riscv-histogram-elf|ELF histogram tool|
|riscv-test-emulate|Emulator Proof of Concept|

## Dependencies

- gmake
- gcc-5.4 or clang-3.4
- ragel (required to regenerate config grammar)
- riscv-gnu-toolchain (required for `make test-emulate`)
- Note: Set RISCV environment variable to point to toolchain

## Supported Platforms

- Linux
- macOS
- FreeBSD

## Build Instructions

The meta compiler has been tested on Linux, Darwin and FreeBSD.

To print a colour opcode map: ```make map```

To output a LaTeX opcode tex: ```make latex```

To output a LaTeX opcode pdf: ```make pdf```

To run the proof of concept emulator: ```make test-emulate```

## Parse ELF Utility

Parse ELF usage command line options:

```
usage: riscv-parse-elf [<options>] <elf_file>
                       --color, -c            Enable Color
            --print-elf-header, -e            Print ELF header
       --print-section-headers, -s            Print Section headers
       --print-program-headers, -p            Print Program headers
          --print-symbol-table, -t            Print Symbol Table
           --print-disassembly, -d            Print Disassembly
                   --print-all, -a            Print All
                        --help, -h            Show help
```

To run the ELF parser and disassembler:

```
# Requires RISC-V GNU Compiler Toolchain to build tests
make test
./build/darwin_x86_64/bin/riscv-parse-elf -c -a test/hello-world-pcrel
```

## Parse Meta Utility

Parse Meta usage command line options:

```
usage: riscv-parse-meta [<options>]
                       --color, -c            Enable Color
                  --isa-subset, -I <string>   ISA subset (e.g. RV32IMA, RV32G, RV32GSC, RV64IMA, RV64G, RV64GSC)
                    --read-isa, -r <string>   Read instruction set metadata from directory
                 --print-latex, -l            Print LaTeX
   --substitute-question-marks, -?            Substitute question marks for zeros in LaTeX output
                   --print-map, -m            Print map
      --print-map-instructions, -x            Print map with instructions
                  --no-comment, -N            Don't emit comments in generated source
           --numeric-constants, -0            Use numeric constants in generated source
              --include-pseudo, -p            Include pseudo opcodes in switch decoder
              --print-interp-h, -V            Print interpreter header
                --print-meta-h, -H            Print metadata header
               --print-meta-cc, -C            Print metadata source
             --print-strings-h, -SH           Print strings header
            --print-strings-cc, -SC           Print strings source
                --print-args-h, -A            Print args header
              --print-switch-h, -S            Print switch header
                 --print-jit-h, -J            Print jit header
                --print-jit-cc, -K            Print jit source
                        --help, -h            Show help
```

To print a colour opcode map for the RV32IMA ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-meta -I RV32IMA -m -r meta
```

To print a colour opcode map for the RV64IMAFDS ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-meta -I RV64IMAFDS -m -r meta
```

To output LaTeX for the RV32C ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-meta -I RV32C -l -r meta
```

To output LaTeX for the RV64G ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-meta -I RV64G -l -r meta
```

## RISC-V ELF Histogram Utility

ELF Histogram usage command line options:

```
usage: riscv-histogram-elf [<options>] <elf_file>
                        --help, -h            Show help
                        --char, -c <string>   Character to use in bars
                        --bars, -b            Print bars next to counts
                --instructions, -I            Instruction Usage Histogram
                   --registers, -R            Register Usage Histogram
          --registers-operands, -P            Register Usage Histogram (with operand positions)
                   --max-chars, -m <string>   Maximum number of characters for bars
                --reverse-sort, -r            Sort in Reverse
```

To print the top 20 instructions in a RISC-V ELF binary:

```
./build/darwin_x86_64/bin/riscv-histogram-elf -b -c █ linux/vmlinux | head -20
```
