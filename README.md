# RISC-V Meta Compiler

A meta compiler that generates RISC-V instruction decoders, disassembler
meta data, opcode maps and documentation from instruction set meta data.
This is the starting point for a RISC-V binary translation investigation.

![ASCII map screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot.png)

*Example ASCII map output from make map*

## Performance Statistics

The follow table shows the performance of the switch based instruction
decoder compared to the riscv_isa_simulator instruction decoder.

```
decoder                 last_insn   insn_count  nanoseconds         code
decode_switch                 jal     33000000        11.36         RV64
decode_spike_nocache          jal     33000000        23.46         RV64
decode_spike_cache            jal     33000000         3.69         RV64
decode_switch                 c.j     32000000        10.14        RV64C
decode_spike_nocache          c_j     32000000        33.45        RV64C
decode_spike_cache            c_j     32000000         3.34        RV64C
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

To run concept emulator: ```make emulate```

## Disassembler

To run the disassembler:

```
make tests
./build/darwin_x86_64/bin/riscv-test-disasm test/hello-world-asm
```

## Parse Opcodes Utility

To print a colour opcode map for the RV32IMA ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV32IMA -m -r riscv-opcodes
```

To print a colour opcode map for the RV64IMAFDS ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV64IMAFDS -m -r riscv-opcodes
```

To output LaTeX for the RV32C ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV32C -l -r riscv-opcodes
```

To output LaTeX for the RV64G ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV64G -l -r riscv-opcodes
```
