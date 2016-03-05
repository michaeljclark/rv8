# RISC-MMXVI Meta Compiler

Experiment with a tiny solver that writes C code and simple
state machine tables to decode RISC-V instructions.

 * Static switch decoder (suitable for compile time generation)
 * Dynamic state machine decoder (suitable for runtime generation)
 * Dynamic state machine decoder with native JIT - TODO

Note: the riscv-isa-sim decoder is included (with and without caching
enabled). Pre-decode caching will eventually be added to all of the
experimental decoders. The dynamic state machine decoder table is
statically embedded in the demo decoder however the decoder generator
supports creating decoder tables at runtime. The decoder numbers should
be compared to spike_nocache for a fair comparison.

This is the starting point of a RISC-V binary translation investigation.

## Performance Statistics

```
decoder                 last_insn   insn_count  nanoseconds         code
decode_dsm_sw                 jal     33000000        27.33         RV64
decode_switch                 jal     33000000        11.64         RV64
decode_spike_nocache          jal     33000000        15.59         RV64
decode_spike_cache            jal     33000000         2.64         RV64
decode_dsm_sw                 c.j     32000000        26.52        RV64C
decode_switch                 c.j     32000000         9.95        RV64C
decode_spike_nocache          c_j     32000000        23.43        RV64C
decode_spike_cache            c_j     32000000         2.08        RV64C
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

To print a dynamic state machine decoder table:
```
make dsm
```

To print decoder meta data table:
```
make meta
```

To print decoder opcode enum
```
make enum
```

To print decoder opcode strings
```
make strings
```
