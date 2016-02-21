OPCODES_DIR=riscv

all: bin obj asm bin/riscv-mc bin/riscv-parse-opcodes bin/riscv-test-decoder asm/riscv-mc.s asm/riscv-test-decoder.s

clean: ;rm -rf bin/* obj/* asm/*

bin: ; mkdir -p bin
obj: ; mkdir -p obj
asm: ; mkdir -p asm

obj/riscv-mc.o: src/riscv-mc.cc src/riscv-mc.h src/riscv-util.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-parse-opcodes.o: src/riscv-parse-opcodes.cc
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-test-decoder.o: src/riscv-test-decoder.cc
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-util.o: src/riscv-util.cc src/riscv-util.h
	c++ -g -std=c++11 -O3 -c $< -o $@


bin/riscv-mc: obj/riscv-mc.o obj/riscv-util.o
	c++ -g -std=c++11 -O3 $^ -o $@

bin/riscv-parse-opcodes: obj/riscv-parse-opcodes.o
	c++ -g -std=c++11 -O3 $^ -o $@

bin/riscv-test-decoder: obj/riscv-test-decoder.o
	c++ -g -std=c++11 -O3 $^ -o $@


asm/riscv-mc.s: src/riscv-mc.cc
	c++ -std=c++11 -O3 -S -masm=intel $< -o $@

asm/riscv-test-decoder.s: src/riscv-test-decoder.cc
	c++ -std=c++11 -O3 -S -masm=intel $< -o $@


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
