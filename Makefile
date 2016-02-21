OPCODES_DIR=riscv

all: bin obj asm bin/riscv-test-disasm bin/riscv-parse-opcodes bin/riscv-test-decoder asm/riscv-test-decoder.s asm/riscv-decode-dsm.s asm/riscv-decode-switch.s

clean: ;rm -rf bin/* obj/* asm/*

bin: ; mkdir -p bin
obj: ; mkdir -p obj
asm: ; mkdir -p asm


obj/riscv-test-disasm.o: src/riscv-test-disasm.cc src/riscv-types.h src/riscv-regs.h src/riscv-opcodes.h src/riscv-dsm.h src/riscv-util.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-opcodes.o: src/riscv-opcodes.cc src/riscv-opcodes.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-regs.o: src/riscv-regs.cc src/riscv-regs.h src/riscv-types.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-compression.o: src/riscv-compression.cc src/riscv-compression.h src/riscv-opcodes.h src/riscv-types.h src/riscv-decode.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-csr.o: src/riscv-csr.cc src/riscv-csr.h src/riscv-types.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-decode-dsm.o: src/riscv-decode-dsm.cc src/riscv-dsm-table.h src/riscv-dsm.h src/riscv-types.h src/riscv-opcodes.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-decode-switch.o: src/riscv-decode-switch.cc src/riscv-types.h src/riscv-opcodes.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-dsm-table.o: src/riscv-dsm-table.cc src/riscv-dsm-table.h src/riscv-dsm.h src/riscv-types.h src/riscv-opcodes.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-dsm.o: src/riscv-dsm.cc src/riscv-dsm.h src/riscv-types.h src/riscv-opcodes.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-format.o: src/riscv-format.cc src/riscv-format.h src/riscv-types.h src/riscv-opcodes.h src/riscv-decode.h src/riscv-regs.h src/riscv-csr.h src/riscv-processor.h src/riscv-compression.h src/riscv-util.h src/riscv-format.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-parse-opcodes.o: src/riscv-parse-opcodes.cc src/riscv-types.h src/riscv-opcodes.h src/riscv-util.h
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-test-decoder.o: src/riscv-test-decoder.cc src/riscv-types.h src/riscv-opcodes.h src/riscv-dsm.h src/riscv-dsm-table.h 
	c++ -g -std=c++11 -O3 -c $< -o $@

obj/riscv-util.o: src/riscv-util.cc src/riscv-util.h
	c++ -g -std=c++11 -O3 -c $< -o $@


bin/riscv-test-disasm: obj/riscv-test-disasm.o obj/riscv-opcodes.o obj/riscv-regs.o obj/riscv-csr.o obj/riscv-dsm.o obj/riscv-dsm-table.o obj/riscv-decode-dsm.o obj/riscv-decode-switch.o obj/riscv-format.o obj/riscv-compression.o obj/riscv-util.o
	c++ -g -std=c++11 -O3 $^ -o $@

bin/riscv-parse-opcodes: obj/riscv-parse-opcodes.o
	c++ -g -std=c++11 -O3 $^ -o $@

bin/riscv-test-decoder: obj/riscv-test-decoder.o obj/riscv-opcodes.o obj/riscv-dsm.o obj/riscv-dsm-table.o 
	c++ -g -std=c++11 -O3 $^ -o $@


asm/riscv-test-decoder.s: src/riscv-test-decoder.cc
	c++ -std=c++11 -O3 -S -masm=intel $< -o $@

asm/riscv-decode-dsm.s: src/riscv-decode-dsm.cc
	c++ -std=c++11 -O3 -S -masm=intel $< -o $@

asm/riscv-decode-switch.s: src/riscv-decode-switch.cc
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
