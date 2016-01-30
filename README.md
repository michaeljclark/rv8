# RISC-V Meta Compiler

Experiment with a tiny solver that writes C code and simple
state machine tables to decode RISC-V instructions.

 * Static switch decoder (suitable for compile time generation)
 * Dynamic state machine decoder (suitable for runtime generation)
 * Dynamic state machine decoder with native JIT - TODO

Note: the riscv-isa-sim decoder is included (hash cache disabled).
Hash caching can be added to any of these decoders. The switch
decoder has approximately equal worst and best case decode times
and does not require any warm up. The state machine decoder can be
used to create opcode tables at runtime (and mapped read only).
The dynamic state machine table is statically embedded in the demo
however the code supports creating decoder tables at runtime.

This is a component of a RISC-V binary translation investigation.

## Performance Statistics

```
decoder            last_insn   insn_count  nanoseconds  format
decode_dsm_sw            jal     33000000        26.71  RV64
decode_switch            jal     33000000        12.83  RV64
decode_spike             jal     33000000        28.17  RV64
decode_dsm_sw            c.j     32000000        26.43  RV64C
decode_switch            c.j     32000000         9.65  RV64C
decode_spike             c_j     32000000        37.88  RV64C
```

## Build Instructions

Check out in a subdirectory of ```riscv-tools``` and build:
```
cd riscv-mc
make
```

To print an opcode map:
```
make map
```

To print a static switch decoder:
```
make switch
```

To print a dynamic state machine decoder:
```
make dsm
```

To run the decoder benchmarks:
```
./bin/riscv-test-decoder
