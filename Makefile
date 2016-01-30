all: bin asm bin/riscv-parse-opcodes bin/riscv-test-decoder asm/riscv-test-decoder.s

clean: ;rm -rf bin/* asm/*

bin: ; mkdir -p bin

asm: ; mkdir -p asm

bin/riscv-parse-opcodes: src/riscv-parse-opcodes.cc
	c++ -g -std=c++11 -O3 $< -o $@

bin/riscv-test-decoder: src/riscv-test-decoder.cc
	c++ -g -std=c++11 -O3 $< -o $@

asm/riscv-test-decoder.s: src/riscv-test-decoder.cc
	c++ -g -std=c++11 -O3 -S -masm=intel $< -o $@

map: bin/riscv-parse-opcodes
	bin/riscv-parse-opcodes -m \
		-r ../riscv-opcodes/opcodes \
		-r ../riscv-opcodes/opcodes-rvc

switch: bin/riscv-parse-opcodes
	bin/riscv-parse-opcodes -s \
		-r ../riscv-opcodes/opcodes \
		-r ../riscv-opcodes/opcodes-rvc

dsm: bin/riscv-parse-opcodes
	bin/riscv-parse-opcodes -d \
		-r ../riscv-opcodes/opcodes \
		-r ../riscv-opcodes/opcodes-rvc
