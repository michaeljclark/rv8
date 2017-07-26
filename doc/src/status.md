Status
====================

_work in progress_

Item                     | Status
:------------------      | :---------------
RV32IMAFDC               | Complete
RV64IMAFDC               | Complete
Floating Point           | Testing (NaN and RM)
ABI Proxy Model          | Implement more syscalls
Soft MMU Model           | 1.9.1 complete, 1.10 pending
Privilege 1.9 Paging     | 1.9.1 complete, 1.10 pending
Privilege 1.9 Timer      | 1.9.1 complete
Privilege 1.9 PLIC       | RISCVEMU compat, SiFive compat pending
Privilege 1.9 Interrupts | Testing (Revisit {m,h,s,u}{t,e,s}ip delivery)
Privilege 1.9 PMAs       | In progress
MMIO UART                | Testing
VirtIO Input             | Not Started
VirtIO Framebuffer       | Not Started
VirtIO Net               | Not Started
VirtIO Block             | Not Started
GDB server               | Not Started
Multi-core               | Started
Disassembler             | Complete
Disassembler (pseudos)   | Complete
Disassembler (objdump)   | Option compatibility not started
x86 Floating Point       | Begun mapping to MXCSR
x86 Simple bintrans      | RV64IM complete, FD not started
x86 Optimizing bintrans  | Needs register allocator
x86 Shadow paging        | Exploratory stage
RVC Compressor           | In progress
Boot Protocol            | Document ELF Auxv AT_BASE Proposal
Assembler                | RV64IMAFD Mostly complete (in assembler branch)
ELF                      | Started documenting
RV128                    | Encoder, Decoder and Disassembler complete
GUI                      | Exploratory stage

## rv-meta

`rv-meta` is a generator framework that uses ISA metadata to construct LaTeX output, the instruction decoder, the interpreter and contains formats and operand types for the assembler and disassembler. It reads RISC-V ISA metadata from the (/meta/) directory. The metadata is originally derived from riscv-opcodes but it has been expanded to include compression and pseudo instruction constraint metadata and pseudo code for the ISA itself. Here is sample LaTeX output: (/doc/pdf/riscv-instructions.pdf) and (/doc/pdf/riscv-types.pdf)

## rv-bin

`rv-bin` is a little like obdump however it supports colour disassembly output `rv-bin dump -a -c -d <file>`. `rv-bin` can create histograms of static register usage and static instruction usage. The compress feature is alpha quality and only works on simple static binaries. All features with the exception of "compress" are production grade.

## rv-sys

`rv-sys` is a full system emulator that implements RV32IMAFDC and RV64IMAFDC. It has a soft MMU and implements the 1.9.1 privileged spec. It can boot linux (/doc/src/linux.md) and is similar to spike in many ways. `rv-sys` differs from spike in that it implements interpreted hard float vs interpreted soft float. Some simple tests can be invoked with "make test-sys". Running "make test-build" outputs some interactive privileged tests such as `build/riscv64-unknown-elf/test-m-mmio-uart`. The memory map is described in (/doc/src/mmio.md)

## rv-sim

`rv-sim` is a user level ABI proxy simulator. `rv-sim` is similar to running `spike` with the proxy kernel however the Linux ABI syscall proxy is built into the simulator. `rv-sim` is currently able to interpret simple static riscv binaries. Some simple tests can be invoked with "make test-sim". It passes the `riscv-qemu-tests` user level conformance tests (the QEMU version of the RISC-V conformance tests ported from `riscv-tests` to run as user-mode programs). Running the tests requires building the tests in the `riscv-qemu-tests` submodule and then running "make qemu-tests" in the toplevel `riscv-meta` directory.

## rv-jit

`rv-jit` is a user level ABI proxy simulator that contains the x86_64 JIT engine. `rv-jit` is fast. `rv-jit` is faster than QEMU (riscv-qemu-user) on some benchmarks. `rv-jit` supports RV64IMAFDC however only IM are accelerated and AFDC are interpreted. The engine is currently similar to a hotspot JIT engine in that it first interprets the code, then when a code path becomes hot (e.g. loops), it then JIT compiles while tracing execution and stores the native code in a trace cache for native execution on the next loop iteration. The intention is to merge the JIT engine back into the full system emulator and ABI proxy simulator when time permits i.e. merge into `rv-sys` and `rv-sim`. `rv-jit` is the testing ground for the JIT engine. `rv-jit` has an audit mode (`--audit`) that executes JIT for an instruction and compares the output against the interpreter. This mode is required as any single RISC-V instruction can translate to many combinations of x86_64 instructions depending on the operand combination and whether the register is in memory or in an x86 register (12 of the 31 RISC-V registers are stored in x86 registers and the remaining 4 x86 registers are reserved for access to the translator and for temporaries for some of the translated instructions).

## rv-asm

`rv-asm` is quick and dirty assembler that reads RISC-V assembly input and writes ELF object files. `rv-asm` technically supports RV32IMAFDC, RV64IMAFDC and RF128IMAFD although some command line options still need to be wired up. `rv-asm` can assemble the output from gcc for simple programs. `rv-asm` is not complete but it is useful for assembling RV128 code. The intention is to use `rv-asm` to test the internal assembler API so it should be regarded a test program. `rv-asm` is able to assemble many of the assembly programs in the (/src/test/) directory. There are plans to implement a RISC-V JIT API for generation of dynamic code and the assembler is to test the internal APIs.
