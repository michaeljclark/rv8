# RISC-V Meta

[![Build Status](https://travis-ci.org/michaeljclark/riscv-meta.svg?branch=master)](https://travis-ci.org/michaeljclark/riscv-meta)

A meta compiler that generates RISC-V instruction decoders, disassembler
meta data, opcode maps and documentation from instruction set meta data.
This is the starting point for a RISC-V binary translation investigation.

![ASCII map screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot-1.png)

*Example ASCII map output from make map*

![Histogram screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot-2.png)

*Example Histogram from riscv-histogram-elf*

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

To run concept emulator: ```make emulate```

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
