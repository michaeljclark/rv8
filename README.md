rv8
===========

RISC-V simulator for x86-64

[![Build Status](https://travis-ci.org/rv8-io/rv8.svg?branch=master)](https://travis-ci.org/rv8-io/rv8)

Date    : January 15, 2020

Version : (under version control)

About
-------------

The rv8 suite contains decoupled libraries and command line tools
for creating instruction opcode maps, C headers and source containing
instruction set metadata, instruction decoders, a JIT assembler, LaTeX
documentation, a metadata based RISC-V disassembler, a histogram tool
for generating statistics on RISC-V ELF executables, a RISC-V proxy
syscall simulator, a RISC-V full system emulator that implements the
RISC-V 1.9.1 privileged specification and an x86-64 binary translator.

The future goals of the rv8 project are:

- Concise metadata representing the RISC-V ISA
- Tools for metadata-based generation of source and documentation 
- Full color disassembler
- ELF Binary compressor, decompressor
- High performance emulation, sandboxing and binary translation
- RISC-V-(n) → RISC-V-(n+1)
- RISC-V-(n) → Intel i7 / AMD64 + AVX-512
- RISC-V Linux ABI emulation on MacOS, Windows, Linux and *BSD
- RISC-V Linux ABI randomisation and entropy coding
- RISC-V Specification undefined behaviour investigation
- RISC-V Virtualization and memory protection investigation

See [RISC-V Instruction Set Listing](/doc/pdf/riscv-instructions.pdf) and
[RISC-V Instruction Types](/doc/pdf/riscv-types.pdf) for sample LaTeX output.

Screenshots
----------------

![ASCII map screenshot](/doc/images/screenshot-1.png)

*Example ASCII map output from make map*

![Histogram screenshot](/doc/images/screenshot-2.png)

*Example Instructions Histogram from `rv-bin histogram`*

![Histogram screenshot](/doc/images/screenshot-3.png)

*Example Registers Histogram from `rv-bin histogram`*

![RISC-V disassembly screenshot](/doc/images/screenshot-4.png)

*Example Disassembly output from `rv-bin dump`*

Tools
-----------------

The following table shows the RISC-V Meta tools:

|Name    | Description
|:------ | :-----------------
|rv-asm  | Assembler
|rv-meta | Code and documentation generator
|rv-bin  | ELF dump, disassmble, compress and histogram
|rv-jit  | Simulator for x86-64
|rv-sim  | ABI Proxy Simulator
|rv-sys  | Privileged System Emulator

Libraries
-----------------

The following table shows the RISC-V Meta libraries:

| Name                | Description                             | Scale
| :------------------ | :-------------------------------------- | :----
| `libriscv_asm.a`    | ISA metadata and disassembly formatting | micro
| `libriscv_elf.a`    | ELF parser                              | micro
| `libriscv_gen.a`    | Source and documentation generators     | macro
| `libriscv_model.a`  | Instruction set metamodel               | macro
| `libriscv_util.a`   | Utility functions for tools             | mini

Project Structure
-----------------------

| Directory    | Description
| :----------- | :---------------
| `meta`       | Instruction set metadata
| `src/abi`    | Application binary interface
| `src/app`    | Test programs and utilities
| `src/asm`    | Assembler metadata library
| `src/elf`    | ELF parser library
| `src/emu`    | ISA simulator and emulator headers
| `src/mem`    | Memory allocator library
| `src/model`  | ISA metamodel library
| `src/rom`    | Boot ROM generator
| `src/test`   | ISA simulator and emulator tests
| `src/util`   | Miscellaneous utilities library
| `doc/pdf`    | Generated documentation

Dependencies
-----------------

- gmake
- gcc-5.4 or clang-3.4
- ragel (required to regenerate config grammar)
- riscv-gnu-toolchain (required for `make test-sim`)
- Note: Set RISCV environment variable to point to toolchain

Supported Platforms
-----------------------------

- Linux
- macOS
- FreeBSD
- Windows Services for Linux

Build Instructions
-----------------------------

The meta compiler has been tested on Linux, Darwin and FreeBSD.

To checkout submodules: ```git submodule update --init --recursive```

To build the utilities, simulator, emulator and tests: ```make all test-build```

To print a colour opcode map: ```make map```

To output a LaTeX opcode tex: ```make latex```

To output a LaTeX opcode pdf: ```make pdf```

To run the ABI Proxy Simulator tests with _`spike`_: ```make test-spike```

To run the ABI Proxy Simulator tests with _`rv-sim`_: ```make test-sim```

To run the QEMU tests with _`rv-sim`_: ```make qemu-tests```

To run the Privileged System Emulator tests with _`rv-sys`_: ```make test-sys```

To bootstrap bbl, linux kernel and busybox image: ```make linux```

To install to `/usr/local/bin`: ```make && sudo make install```

**Notes**

- The test-build target requires the RISC-V GNU Compiler Toolchain

### Ubuntu 14.04LTS Dependencies

The compiler in Ubuntu 14.04LTS doesn't support C++14. These
instructions will install g++6 from the ubuntu toolchain repository
and build the project using g++6.

```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update
sudo apt-get install g++-6 libncurses5-dev
make CXX=g++-6 CC=gcc-6
```

RV x86-64 Simulator
-----------------------------

RISC-V x86-64 JIT Simulator command line options:

```
$ rv-jit -h
usage: rv-jit [<options>] <elf_file> [<options>]
            --log-instructions, -l            Log Instructions
                --log-operands, -o            Log Instructions and Operands
              --log-memory-map, -m            Log Memory Map Information
               --log-registers, -r            Log Registers (defaults to integer registers)
               --log-jit-trace, -T            Log JIT trace
    --register-usage-histogram, -H            Record register usage
          --pc-usage-histogram, -P            Record program counter usage
                       --debug, -d            Start up in debugger CLI
                   --no-pseudo, -x            Disable Pseudoinstruction decoding
                    --no-trace, -t            Disable JIT tracer
                       --audit, -a            Enable JIT audit
                 --trace-iters, -I <string>   Trace iterations
                --trace-length, -L <string>   Trace length
                        --help, -h            Show help
```

**Notes**

- Currently only the Linux syscall ABI proxy is implemented for the JIT simulator


RV Proxy Simulator
-----------------------------

The ABI Proxy Simulator command line options:

```
$ rv-sim -h
usage: rv-sim [<options>] <elf_file> [<options>]
                         --isa, -i <string>   ISA Extensions (IMA, IMAC, IMAFD, IMAFDC)
            --log-instructions, -l            Log Instructions
                --log-operands, -o            Log Instructions and Operands
              --log-memory-map, -m            Log Memory Map Information
               --log-registers, -r            Log Registers (defaults to integer registers)
                       --debug, -d            Start up in debugger CLI
                   --no-pseudo, -x            Disable Pseudoinstruction decoding
                        --seed, -s <string>   Random seed
                        --help, -h            Show help
```

To run the simple Hello World program (Proxy Mode):

```
rv-sim build/riscv64-unknown-elf/bin/hello-world-libc
```


RV System Emulator
-----------------------------

The Privilged ISA System Emulator command line options:

```
$ rv-sys -h
usage: rv-sys [<options>] <elf_file>
            --log-instructions, -l            Log Instructions
                --log-operands, -o            Log Instructions and Operands
                    --log-mmio, -O            Log Memory Mapped IO
              --log-memory-map, -m            Log Memory Map Information
               --log-mmode-csr, -M            Log Machine Control and Status Registers
               --log-hmode-csr, -H            Log Hypervisor Control and Status Registers
               --log-smode-csr, -S            Log Supervisor Control and Status Registers
               --log-umode-csr, -U            Log User Control and Status Registers
               --log-registers, -r            Log Registers (defaults to integer registers)
               --log-pagewalks, -v            Log Pagewalks
                   --log-traps, -t            Log Traps
    --register-usage-histogram, -H            Record register usage
          --pc-usage-histogram, -J            Record program counter usage
                       --debug, -d            Start up in debugger
                  --debug-trap, -T            Start up in debugger and enter debugger on trap
                   --no-pseudo, -x            Disable Pseudoinstruction decoding
                --map-physical, -p <string>   Map execuatable at physical address
                         --bbl, -b <string>   BBL Boot ( 32, 64 )
                        --seed, -s <string>   Random seed
                        --help, -h            Show help
```

To run the privilged UART echo program (Privileged Mode):

```
rv-sys build/riscv64-unknown-elf/bin/test-m-mmio-uart
```


RV ELF Dump Utility
-----------------------------

ELF Dump usage command line options:

```
$ rv-bin dump -h
usage: dump [<options>] <elf_file>
                       --color, -c            Enable Color
            --print-elf-header, -e            Print ELF header
        --print-elf-header-ext, -x            Print ELF header (extended)
       --print-section-headers, -s            Print Section headers
       --print-program-headers, -p            Print Program headers
          --print-symbol-table, -t            Print Symbol Table
           --print-relocations, -r            Print Relocations
           --print-disassembly, -d            Print Disassembly
                      --pseudo, -P            Decode Pseudoinstructions
               --print-headers, -h            Print All Headers
                   --print-all, -a            Print All Headers and Disassembly
                        --help, -h            Show help
```

To run the ELF parser and disassembler:

```
rv-bin dump -c -a build/riscv64-unknown-elf/bin/hello-world-pcrel
```

**Notes**

- The ELF dissassembler output requires 125 column terminal window


RV ELF Histogram Utility
-------------------------------------

The ELF Histogram Utility usage command line options:

```
$ rv-bin histogram -h
usage: histogram [<options>] <elf_file>
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
rv-bin histogram -I -b -c █ linux/vmlinux | head -20
```

To print the top 20 registers in a RISC-V ELF binary:

```
rv-bin histogram -R -b -c █ linux/vmlinux | head -20
```


RV Metadata Utility
-----------------------------

The RV source and documentation generator usage command line options:

```
$ rv-meta -h
usage: rv-meta [<options>]
                        --help, -h            Show help
                  --isa-subset, -I <string>   ISA subset (e.g. RV32IMA, RV32G, RV32GSC, RV64IMA, RV64G, RV64GSC)
                    --read-isa, -r <string>   Read instruction set metadata from directory
                  --no-comment, -N            Don't emit comments in generated source
           --numeric-constants, -0            Use numeric constants in generated source
         --print-constraints-h, -XC           Print constraints header
            --print-fpu-test-h, -FH           Print FPU test header
            --print-fpu-test-c, -FC           Print FPU test source
              --print-interp-h, -V            Print interpreter header
                 --print-jit-h, -J            Print jit header
                --print-jit-cc, -K            Print jit source
   --substitute-question-marks, -?            Substitute question marks for zeros in LaTeX output
                 --print-latex, -l            Print LaTeX instruction listing
                       --color, -c            Enable ANSI color map
                   --print-map, -m            Print instruction map
      --print-map-pseudocode-c, -mc           Print instruction map with C pseudocode
    --print-map-pseudocode-alt, -ma           Print instruction map with alternate pseudocode
                --print-meta-h, -H            Print metadata header
               --print-meta-cc, -C            Print metadata source
            --print-operands-h, -A            Print operands header
             --print-strings-h, -SH           Print strings header
            --print-strings-cc, -SC           Print strings source
              --print-switch-h, -S            Print switch header
```

To print a colour opcode map for the RV32IMA ISA subset:

```
rv-meta -I RV32IMA -m -c -r meta
```

To print a colour opcode map for the RV64IMAFDS ISA subset:

```
rv-meta -I RV64IMAFDS -m -c -r meta
```

To output LaTeX for the RV32C ISA subset:

```
rv-meta -I RV32C -l -r meta
```

To output LaTeX for the RV64G ISA subset:

```
rv-meta -I RV64G -l -r meta
```


References
----------------

- [lowRISC project](http://www.lowrisc.org/)
- [Native Client x86-64 Sandbox](https://developer.chrome.com/native-client/reference/sandbox_internals/x86-64-sandbox)
- [Native Client ARM 32-bit Sandbox](https://developer.chrome.com/native-client/reference/sandbox_internals/arm-32-bit-sandbox)
- [Design of the RISC-V Instruction Set Architecture](http://www.eecs.berkeley.edu/~waterman/papers/phd-thesis.pdf)
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
- [Bit Twiddling Hacks](https://graphics.stanford.edu/~seander/bithacks.html)
- [The Netwide Assembler](http://repo.or.cz/nasm.git)
- [Complete x86/x64 JIT and Remote Assembler for C++](https://github.com/kobalicek/asmjit/)
- [Optimizing Binary Translation of Dynamically Generated Code](http://plrg.eecs.uci.edu/publications/cgo15.pdf)
- [Live Range Hole Allocation in Dynamic Binary Translation](http://amas-bt.cs.virginia.edu/2011proceedings/amasbt2011-p2.pdf)
- [Dynamic Re-compilation of Binary RISC Code for CISC Architectures](https://www.weihenstephan.org/~michaste/down/steil-recompilation.pdf)
- [Using Dynamic Binary Translation to Fuse Dependent Instructions](http://www.cgo.org/cgo2004/papers/17_61_HU_S.pdf)
- [librando: Transparent Code Randomization for Just-in-Time Compilers](https://www.ics.uci.edu/~ahomescu/ccs13librando_printed.pdf)
- [Practical Control Flow Integrity & Randomization for Binary Executables](http://bitblaze.cs.berkeley.edu/papers/CCFIR-oakland-CR.pdf)
- [Abstractions for Practical Virtual Machine Replay](https://www.cs.utah.edu/~aburtsev/doc/vee16-xentt.pdf)
- [A Secure Processor Architecture for Encrypted Computation on Untrusted Programs](https://people.csail.mit.edu/devadas/pubs/ascend-stc12.pdf)
- [Jump Over ASLR: Attacking Branch Predictors to Bypass ASLR](http://www.cs.binghamton.edu/~dima/micro16.pdf)
- [SoK: Eternal War in Memory](https://www.cs.berkeley.edu/~dawnsong/papers/Oakland13-SoK-CR.pdf)
- [SoK: Introspections on Trust and the Semantic Gap](http://ieeexplore.ieee.org/stamp/stamp.jsp?arnumber=6956590)
- [Hacking Blind](http://www.scs.stanford.edu/~abelay/pdf/bittau:brop.pdf)
- [Code Pointer Integrity](http://dslab.epfl.ch/pubs/cpi.pdf)
- [Geometric Memory Management](http://arxiv.org/pdf/1512.09358.pdf)
- [Two-Level Segregated Fit memory allocator implementation](https://github.com/mattconte/tlsf/)
- [TLSF: a New Dynamic Memory Allocator for Real-Time Systems](http://www.gii.upv.es/tlsf/files/ecrts04_tlsf.pdf)
- [A Precise Memory Model for Low-Level Bounded Model Checking](https://www.usenix.org/legacy/event/ssv10/tech/full_papers/Sinz.pdf)
- [IEEE Std 1003.1™, 2013 Edition](http://pubs.opengroup.org/onlinepubs/9699919799/)
- [Virtual IO Device Specification](http://docs.oasis-open.org/virtio/virtio/v1.0/virtio-v1.0.html)
- [Standard C++](https://isocpp.org/std/the-standard/)
- [Exception Handling in LLVM](http://llvm.org/docs/ExceptionHandling.html)
- [AMD64 System V Application Binary Interface](http://refspecs.linuxfoundation.org/elf/x86-64-abi-0.99.pdf)
- [ELF Handling for Thread-Local Storage](https://www.akkadia.org/drepper/tls.pdf)
- [DWARF Debugging Information Format Version 4](http://dwarfstd.org/doc/DWARF4.pdf)
- [The Error Model](http://joeduffyblog.com/2016/02/07/the-error-model/) and other essays
- [No sane compiler would optimize atomics](https://github.com/jfbastien/no-sane-compiler/)
- A Guide to Undefined Behavior in C and C++ ([part 1](http://blog.regehr.org/archives/213) [part 2](http://blog.regehr.org/archives/226) [part 3](http://blog.regehr.org/archives/232))
