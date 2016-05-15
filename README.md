# RISC-V Meta

[![Build Status](https://travis-ci.org/michaeljclark/riscv-meta.svg?branch=master)](https://travis-ci.org/michaeljclark/riscv-meta)

RISC-V Meta is a suite of tools that operate on RISC-V ISA (Instruction
Set Architecture) Metadata. The RISC-V Meta suite of tools can generate
RISC-V instruction opcode maps, C headers and source containing instruction
set metadata, instruction decoders and LaTex documentation (`riscv-parse-opcodes`).

The suite also contains simple decoupled libraries with an ELF parser, a
functional meta data based RISC-V disassembler (`riscv-parse-elf`), a
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
- RISC-V Linux ABI entropy coding - Runtime ABI randomisation
- RISC-V Specification undefined behaviour investigation
- RISC-V Virtualization and memory protection investigation

## Screenshots

![ASCII map screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot-1.png)

*Example ASCII map output from make map*

![Histogram screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot-2.png)

*Example Histogram from riscv-histogram-elf*

## Refernences

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
- [Two-Level Segregated Fit memory allocator implementation](https://github.com/mattconte/tlsf/)
- [Complete x86/x64 JIT and Remote Assembler for C++](https://github.com/kobalicek/asmjit/)
- [SoK: Eternal War in Memory](https://www.cs.berkeley.edu/~dawnsong/papers/Oakland13-SoK-CR.pdf)
- [Hacking Blind](http://www.scs.stanford.edu/~abelay/pdf/bittau:brop.pdf)
- [Code Pointer Integrity](http://dslab.epfl.ch/pubs/cpi.pdf)
- [Design of the RISC-V Instruction Set Architecture](http://www.eecs.berkeley.edu/~waterman/papers/phd-thesis.pdf)
- [The Error Model](http://joeduffyblog.com/2016/02/07/the-error-model/) and other essays
- A Guide to Undefined Behavior in C and C++ ([part 1](http://blog.regehr.org/archives/213) [part 2](http://blog.regehr.org/archives/226) [part 3](http://blog.regehr.org/archives/232))
- [No sane compiler would optimize atomics](https://github.com/jfbastien/no-sane-compiler/)
- [IEEE Std 1003.1™, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/)
- [Standard C++](https://isocpp.org/std/the-standard/)

## Dependencies

- gcc-5 or clang-3.7
- riscv-gnu-toolchain (required for `make test emulate`)

## Performance Statistics

The follow table shows the performance of the switch based instruction
decoder compared to the riscv_isa_simulator instruction decoder.

### Micro benchmark 33 RV64 instructions (1000000 iterations)
```
#decoder                  insn_count     nanoseconds            code
decode_switch               33000000            2.22            RV64
decode_spike_nocache        33000000           15.98            RV64
decode_spike_cache          33000000            3.06            RV64
```

### Micro benchmark 32 RV64C instructions (1000000 iterations)
```
#decoder                  insn_count     nanoseconds            code
decode_switch               32000000            2.30           RV64C
decode_spike_nocache        32000000           25.20           RV64C
decode_spike_cache          32000000            2.91           RV64C
```

### ELF benchmark vmlinux RV64C .init.text (10 iterations)
```
#decoder                  insn_count     nanoseconds            code
decode_switch                 158550            3.34           RV64C
decode_spike_nocache          158550           52.22           RV64C
decode_spike_cache            158550           20.45           RV64C
```

### ELF benchmark vmlinux RV64C .text (10 iterations)
```
#decoder                  insn_count     nanoseconds            code
decode_switch                4848690            2.48           RV64C
decode_spike_nocache         4848690           51.57           RV64C
decode_spike_cache           4848690           24.40           RV64C
```

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

To run the ELF parser and disassembler:

```
# Requires RISC-V GNU Compiler Toolchain to build tests
make test
./build/darwin_x86_64/bin/riscv-parse-elf -a test/hello-world-pcrel
```

## Parse Opcodes Utility

To print a colour opcode map for the RV32IMA ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV32IMA -m -r meta
```

To print a colour opcode map for the RV64IMAFDS ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV64IMAFDS -m -r meta
```

To output LaTeX for the RV32C ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV32C -l -r meta
```

To output LaTeX for the RV64G ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV64G -l -r meta
```

## RISC-V ELF Histogram Utility

To print the top 20 instructions in a RISC-V ELF binary:

```
./build/darwin_x86_64/bin/riscv-histogram-elf -b -c █ linux/vmlinux | head -20
```
