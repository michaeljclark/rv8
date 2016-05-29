# RISC-V Meta

[![Build Status](https://travis-ci.org/michaeljclark/riscv-meta.svg?branch=master)](https://travis-ci.org/michaeljclark/riscv-meta)

RISC-V Meta is a suite of tools that operate on RISC-V ISA (Instruction
Set Architecture) Metadata. The RISC-V Meta suite of tools can generate
RISC-V instruction opcode maps, C headers and source containing instruction
set metadata, instruction decoders and LaTeX documentation (`riscv-parse-meta`).

The suite also contains simple decoupled libraries with an ELF parser, a
functional metadata based RISC-V disassembler (`riscv-parse-elf`), a
RVC compressor / decompressor (`riscv-compress-elf`) plus library routines
and tools for benchmarking software decoders and generating statistics on
RISC-V executables (`riscv-histogram-elf`).

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

## Screenshots

![ASCII map screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot-1.png)

*Example ASCII map output from make map*

![Histogram screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot-2.png)

*Example Histogram from riscv-histogram-elf*

![RISC-V disassembly screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot-3.png)

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
- [SoK: Eternal War in Memory](https://www.cs.berkeley.edu/~dawnsong/papers/Oakland13-SoK-CR.pdf)
- [Hacking Blind](http://www.scs.stanford.edu/~abelay/pdf/bittau:brop.pdf)
- [Code Pointer Integrity](http://dslab.epfl.ch/pubs/cpi.pdf)
- [TLSF: a New Dynamic Memory Allocator for Real-Time Systems](http://www.gii.upv.es/tlsf/files/ecrts04_tlsf.pdf)
- [A Precise Memory Model for Low-Level Bounded Model Checking](https://www.usenix.org/legacy/event/ssv10/tech/full_papers/Sinz.pdf)
- [Design of the RISC-V Instruction Set Architecture](http://www.eecs.berkeley.edu/~waterman/papers/phd-thesis.pdf)
- [IEEE Std 1003.1™, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/)
- [Standard C++](https://isocpp.org/std/the-standard/)
- [The Error Model](http://joeduffyblog.com/2016/02/07/the-error-model/) and other essays
- [No sane compiler would optimize atomics](https://github.com/jfbastien/no-sane-compiler/)
- A Guide to Undefined Behavior in C and C++ ([part 1](http://blog.regehr.org/archives/213) [part 2](http://blog.regehr.org/archives/226) [part 3](http://blog.regehr.org/archives/232))

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
|riscv-test-decoder|Decoder Benchmarks|
|riscv-test-emulate|Emulator Proof of Concept|

## Dependencies

- gcc-5 or clang-3.7
- riscv-gnu-toolchain (required for `make test emulate`)

## Performance Statistics

The follow table shows the performance of the switch based instruction
decoder compared to the riscv_isa_simulator instruction decoder.
The code is compiled with `Apple LLVM version 7.3.0 (clang-703.0.29)`

##### Micro benchmark 33 RV64 instructions (1000000 iterations)
|Decoder|Instruction Count|Time (ns)|Architecture|
|:------|----------------:|--------:|:-----------|
|meta_switch   | 33000000  |   2.30 | RV64       |
|spike_nocache | 33000000  |  15.66 | RV64       |
|spike_cache   | 33000000  |   2.97 | RV64       |

##### Micro benchmark 32 RV64C instructions (1000000 iterations)
|Decoder|Instruction Count|Time (ns)|Architecture|
|:------|----------------:|--------:|:-----------|
|meta_switch   | 32000000 |   2.36  | RV64C      |
|spike_nocache | 32000000 |  25.20  | RV64C      |
|spike_cache   | 32000000 |   2.52  | RV64C      |

##### ELF benchmark vmlinux RV64 .init.text (10 iterations)
|Decoder|Instruction Count|Time (ns)|Architecture|
|:------|----------------:|--------:|:-----------|
|meta_switch   |   161970 |   2.59  | RV64       |
|spike_nocache |   161970 |  21.42  | RV64       |
|spike_cache   |   161970 |  14.47  | RV64       |

##### ELF benchmark vmlinux RV64 .text (10 iterations)
|Decoder|Instruction Count|Time (ns)|Architecture|
|:------|----------------:|--------:|:-----------|
|meta_switch   | 4871200  |   2.50  | RV64       |
|spike_nocache | 4871200  |  24.87  | RV64       |
|spike_cache   | 4871200  |  18.31  | RV64       |

##### ELF benchmark vmlinux RV64C .init.text (10 iterations)
|Decoder|Instruction Count|Time (ns)|Architecture|
|:------|----------------:|--------:|:-----------|
|meta_switch   |   158550 |   2.72  | RV64C      |
|spike_nocache |   158550 |  63.59  | RV64C      |
|spike_cache   |   158550 |  23.25  | RV64C      |

##### ELF benchmark vmlinux RV64C .text (10 iterations)
|Decoder|Instruction Count|Time (ns)|Architecture|
|:------|----------------:|--------:|:-----------|
|meta_switch   | 4848690  |   2.48  | RV64C      |
|spike_nocache | 4848690  |  74.17  | RV64C      |
|spike_cache   | 4848690  |  27.66  | RV64C      |

## Build Instructions

The meta compiler has been tested on Linux, Darwin and FreeBSD.

To run the decoder benchmarks: ```make bench```

To print a colour opcode map: ```make map```

To output a LaTeX opcode tex: ```make latex```

To output a LaTeX opcode pdf: ```make pdf```

To print decoder C switch: ```make c_switch```

To print opcodes C header: ```make c_header```

To print opcodes C source: ```make c_source```

To run the proof of concept emulator: ```make emulate```

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
usage: riscv-parse-meta
                       --color, -c            Enable Color
                  --isa-subset, -I <string>   ISA subset (e.g. RV32IMA, RV32G, RV32GSC, RV64IMA, RV64G, RV64GSC)
                    --read-isa, -r <string>   Read instruction set metadata from directory
                 --print-latex, -l            Print LaTeX
                   --print-map, -m            Print map
                  --no-comment, -N            Don't emit comments in generated source
                 --zero-not-oh, -0            Use numeric constants in generated source
             --print-opcodes-h, -H            Print C header
             --print-opcodes-c, -C            Print C source
              --print-switch-c, -S            Print C switch
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
                   --max-chars, -m <string>   Maximum number of characters for bars
                --reverse-sort, -r            Sort in Reverse
```

To print the top 20 instructions in a RISC-V ELF binary:

```
./build/darwin_x86_64/bin/riscv-histogram-elf -b -c █ linux/vmlinux | head -20
```
