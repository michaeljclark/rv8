OPCODES_DIR=riscv

all: bin asm bin/riscv-mc bin/riscv-parse-opcodes bin/riscv-test-decoder asm/riscv-test-decoder.s

clean: ;rm -rf bin/* asm/*

bin: ; mkdir -p bin

asm: ; mkdir -p asm

bin/riscv-mc: src/riscv-mc.cc
	c++ -g -std=c++11 -O3 $< -o $@

bin/riscv-parse-opcodes: src/riscv-parse-opcodes.cc
	c++ -g -std=c++11 -O3 $< -o $@

bin/riscv-test-decoder: src/riscv-test-decoder.cc
	c++ -g -std=c++11 -O3 $< -o $@

asm/riscv-test-decoder.s: src/riscv-test-decoder.cc
	c++ -g -std=c++11 -O3 -S -masm=intel $< -o $@

map: bin/riscv-parse-opcodes
	bin/riscv-parse-opcodes -m \
		-r $(OPCODES_DIR)/opcodes \
		-r $(OPCODES_DIR)/opcodes-rvc

meta: bin/riscv-parse-opcodes
	bin/riscv-parse-opcodes -c \
		-r $(OPCODES_DIR)/opcodes \
		-r $(OPCODES_DIR)/opcodes-rvc

switch: bin/riscv-parse-opcodes
	bin/riscv-parse-opcodes -s \
		-r $(OPCODES_DIR)/opcodes \
		-r $(OPCODES_DIR)/opcodes-rvc

dsm: bin/riscv-parse-opcodes
	bin/riscv-parse-opcodes -d \
		-r $(OPCODES_DIR)/opcodes \
		-r $(OPCODES_DIR)/opcodes-rvc

bench: bin/riscv-test-decoder
	bin/riscv-test-decoder
