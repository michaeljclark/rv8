#
# RISC-V Hello World
#

# Requires RISCV environment variable to be set
# RISCV = /opt/riscv/toolchain

CC = ${RISCV}/bin/${TARGET}-gcc
CXX = ${RISCV}/bin/${TARGET}-g++
AS = ${RISCV}/bin/${TARGET}-as
LD = ${RISCV}/bin/${TARGET}-ld
STRIP = ${RISCV}/bin/${TARGET}-strip
PK = ${RISCV}/${TARGET}/bin/pk

ifeq ($(TARGET),riscv32-unknown-elf)
SPIKE = ${RISCV}/bin/spike --isa=RV32IMAFD ${PK}
endif

ifeq ($(TARGET),riscv64-unknown-elf)
SPIKE = ${RISCV}/bin/spike --isa=RV64IMAFD ${PK}
endif

ifeq ($(EMULATOR),)
EMULATOR = $(SPIKE)
endif

SRC_DIR = src/test
OBJ_DIR = build/$(TARGET)/obj
BIN_DIR = build/$(TARGET)/bin
GEN_DIR = build/$(TARGET)/gen

CFLAGS = -Os -Wall -fpie -ffunction-sections -fdata-sections
CXXFLAGS = -std=c++1y $(CFLAGS)

PROGRAMS = \
	$(BIN_DIR)/hello-world-libc \
	$(BIN_DIR)/hello-world-libc-strip \
	$(BIN_DIR)/hello-world-abs \
	$(BIN_DIR)/hello-world-pcrel \
	$(BIN_DIR)/hello-world-pcrel-giga \
	$(BIN_DIR)/hello-world-pcrel-nano \
	$(BIN_DIR)/hello-world-pcrel-pico \
	$(BIN_DIR)/test-int-fib \
	$(BIN_DIR)/test-fpu-printf \
	$(BIN_DIR)/test-sieve \
	$(BIN_DIR)/test-fpu \
	$(BIN_DIR)/test-fpu-assert

all: dirs $(PROGRAMS)

dirs: ; @mkdir -p $(OBJ_DIR) $(BIN_DIR) $(GEN_DIR)

clean:
	rm -f $(PROGRAMS) *.o

test: all
	$(EMULATOR) $(BIN_DIR)/hello-world-pcrel
	$(EMULATOR) $(BIN_DIR)/hello-world-libc
	$(EMULATOR) $(BIN_DIR)/test-int-fib
	$(EMULATOR) $(BIN_DIR)/test-fpu-printf
	$(EMULATOR) $(BIN_DIR)/test-fpu-assert
	$(EMULATOR) $(BIN_DIR)/test-sieve

$(OBJ_DIR)/test-int-fib.o: $(SRC_DIR)/test-int-fib.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-int-fib: $(OBJ_DIR)/test-int-fib.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-fpu.o: $(SRC_DIR)/test-fpu.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fpu: $(OBJ_DIR)/test-fpu.o ; $(CC) $(CFLAGS) $^ -o $@

$(GEN_DIR)/test-fpu-assert.c: $(BIN_DIR)/test-fpu ; $(SPIKE) $(BIN_DIR)/test-fpu | egrep -v '(nan|inf)' > $@
$(OBJ_DIR)/test-fpu-assert.o: $(GEN_DIR)/test-fpu-assert.c ; $(CC) $(CFLAGS) -I$(SRC_DIR) -c $^ -o $@
$(BIN_DIR)/test-fpu-assert: $(OBJ_DIR)/test-fpu-assert.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-fpu-printf.o: $(SRC_DIR)/test-fpu-printf.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fpu-printf: $(OBJ_DIR)/test-fpu-printf.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-sieve.o: $(SRC_DIR)/test-sieve.cc ; $(CXX) $(CXXFLAGS) -c $^ -o $@
$(BIN_DIR)/test-sieve: $(OBJ_DIR)/test-sieve.o ; $(CXX) $(CXXFLAGS) $^ -o $@

$(OBJ_DIR)/hello-world-libc.o: $(SRC_DIR)/hello-world-libc.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/hello-world-libc: $(OBJ_DIR)/hello-world-libc.o ; $(CC) $(CFLAGS) $^ -o $@
$(BIN_DIR)/hello-world-libc-strip: $(BIN_DIR)/hello-world-libc ; $(STRIP) $^ -o $@

$(OBJ_DIR)/hello-world-abs.o: $(SRC_DIR)/hello-world-abs.S ; $(AS) $^ -o $@
$(BIN_DIR)/hello-world-abs: $(OBJ_DIR)/hello-world-abs.o ; $(LD) $^ -o $@
$(OBJ_DIR)/hello-world-pcrel.o: $(SRC_DIR)/hello-world-pcrel.S ; $(AS) $^ -o $@
$(BIN_DIR)/hello-world-pcrel: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) $^ -o $@

$(BIN_DIR)/hello-world-pcrel-giga: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) -T $(SRC_DIR)/ld.$(TARGET).script $^ -o $@
$(BIN_DIR)/hello-world-pcrel-nano: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) --strip-all -T $(SRC_DIR)/ld.$(TARGET).script $^ -o $@
$(BIN_DIR)/hello-world-pcrel-pico: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) --nmagic --strip-all -T $(SRC_DIR)/ld.$(TARGET).script $^ -o $@
