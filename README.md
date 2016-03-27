# RISC-V Meta Compiler

Experiment with a tiny solver that write a RISC-V decoder and
disassembler using instruction set meta data.

Note: the riscv-isa-sim decoder is included (with and without caching
enabled). Pre-decode caching will eventually be added to the switch
decoder. The switch decoder performance should be compared to
spike_nocache for a fair comparison.

This is the starting point of a RISC-V binary translation investigation.

## Performance Statistics

```
decoder                 last_insn   insn_count  nanoseconds         code
decode_switch                 jal     33000000         9.92         RV64
decode_spike_nocache          jal     33000000        15.16         RV64
decode_spike_cache            jal     33000000         2.48         RV64
decode_switch                 c.j     32000000         7.55        RV64C
decode_spike_nocache          c_j     32000000        22.72        RV64C
decode_spike_cache            c_j     32000000         2.12        RV64C
```

## Build Instructions

```
make
```

To run the decoder benchmarks:
```
make bench
```

To print an opcode map:
```
make map
```

To print a static switch decoder:
```
make switch
```

To print decoder meta data table:
```
make meta
```

To print decoder opcode enum:
```
make enum
```
