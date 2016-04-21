# RISC-V Meta

A meta compiler that generates RISC-V instruction decoders, disassembler
meta data, opcode maps and documentation from instruction set meta data.
This is the starting point for a RISC-V binary translation investigation.

![ASCII map screenshot](https://raw.githubusercontent.com/michaeljclark/riscv-mc/master/screenshot.png)

*Example ASCII map output from make map*

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

## Disassembler

To run the disassembler:

```
# Requires RISC-V GNU Compiler Toolchain to build tests
make test
./build/darwin_x86_64/bin/riscv-test-disasm test/hello-world-asm
```

## Parse Opcodes Utility

To print a colour opcode map for the RV32IMA ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV32IMA -m -r opcodes
```

To print a colour opcode map for the RV64IMAFDS ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV64IMAFDS -m -r opcodes
```

To output LaTeX for the RV32C ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV32C -l -r opcodes
```

To output LaTeX for the RV64G ISA subset:

```
./build/darwin_x86_64/bin/riscv-parse-opcodes -I RV64G -l -r opcodes
```
