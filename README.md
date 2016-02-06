# RISC-MMXV Meta Compiler

Experiment with a tiny solver that writes C code and simple
state machine tables to decode RISC-V instructions.

 * Static switch decoder (suitable for compile time generation)
 * Dynamic state machine decoder (suitable for runtime generation)
 * Dynamic state machine decoder with native JIT - TODO

Note: the riscv-isa-sim decoder is included (with and without hash).
Hash caching can be added to any of these decoders. The switch
decoder has approximately equal worst and best case decode times
and does not require any warm up. The state machine decoder can be
used to create opcode tables at runtime (and mapped read only).
The dynamic state machine table is statically embedded in the demo
however the code supports creating decoder tables at runtime.
The decoder numbers should be compared to spike_nohash.

This is a component of a RISC-V binary translation investigation.

## Performance Statistics

```
decoder                 last_insn   insn_count  nanoseconds         code
decode_dsm_sw                 jal     33000000        27.33         RV64
decode_switch                 jal     33000000        11.64         RV64
decode_spike_nohash           jal     33000000        15.59         RV64
decode_spike_hash             jal     33000000         2.64         RV64
decode_dsm_sw                 c.j     32000000        26.52        RV64C
decode_switch                 c.j     32000000         9.95        RV64C
decode_spike_nohash           c_j     32000000        23.43        RV64C
decode_spike_hash             c_j     32000000         2.08        RV64C
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
