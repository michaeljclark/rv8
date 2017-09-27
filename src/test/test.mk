#
# RISC-V Hello World
#

# Requires RISCV environment variable to be set
# RISCV = /opt/riscv/toolchain

# host
OS := $(shell uname -s | sed 's/ /_/' | tr A-Z a-z)
CPU := $(shell uname -m | sed 's/ /_/' | tr A-Z a-z)

# target
CC = ${RISCV}/bin/${TARGET}-gcc
CXX = ${RISCV}/bin/${TARGET}-g++
AS = ${RISCV}/bin/${TARGET}-as
LD = ${RISCV}/bin/${TARGET}-ld
STRIP = ${RISCV}/bin/${TARGET}-strip
PK = ${RISCV}/${TARGET}/bin/pk

TARGET_DIR = $(TARGET)

SRC_DIR = src/test
ASM_DIR = build/$(TARGET_DIR)/asm
OBJ_DIR = build/$(TARGET_DIR)/obj
BIN_DIR = build/$(TARGET_DIR)/bin
GEN_DIR = build/$(TARGET_DIR)/gen

HOST_BIN_DIR = build/$(OS)_$(CPU)/test
HOST_OBJ_DIR = build/$(OS)_$(CPU)/obj

# compiler function tests
check_opt = $(shell T=$$(mktemp /tmp/test.XXXX); echo 'int main() { return 0; }' > $$T.$(2) ; $(1) $(3) $$T.$(2) -o /dev/null >/dev/null 2>&1 ; echo $$?; rm $$T $$T.$(2))

ifeq ($(TARGET),riscv32-unknown-elf)
SPIKE = ${RISCV}/bin/spike --isa=$(ARCH) ${PK}
endif

ifeq ($(TARGET),riscv64-unknown-elf)
SPIKE = ${RISCV}/bin/spike --isa=$(ARCH) ${PK}
endif

CFLAGS = -O3 -g -march=$(ARCH) -Wall -fpie -ffunction-sections -fdata-sections

ifeq ($(EMULATOR),)
EMULATOR = $(SPIKE)
endif

ASSEMBLY = \
	$(ASM_DIR)/hello-world-libc.s \
	$(ASM_DIR)/test-jump-tables-yes.s \
	$(ASM_DIR)/test-jump-tables-no.s

PROGRAMS = \
	$(BIN_DIR)/hello-world-libc \
	$(BIN_DIR)/hello-world-libc-strip \
	$(BIN_DIR)/hello-world-abs \
	$(BIN_DIR)/hello-world-pcrel \
	$(BIN_DIR)/hello-world-pcrel-giga \
	$(BIN_DIR)/hello-world-pcrel-nano \
	$(BIN_DIR)/hello-world-pcrel-pico \
	$(BIN_DIR)/test-aes \
	$(BIN_DIR)/test-dhrystone \
	$(BIN_DIR)/test-miniz \
	$(BIN_DIR)/test-norx \
	$(BIN_DIR)/test-sha512 \
	$(BIN_DIR)/test-primes \
	$(BIN_DIR)/test-qsort \
	$(BIN_DIR)/test-args \
	$(BIN_DIR)/test-loop-1 \
	$(BIN_DIR)/test-loop-2 \
	$(BIN_DIR)/test-fusion-elision \
	$(BIN_DIR)/test-fusion-macro \
	$(BIN_DIR)/test-fusion-indirect \
	$(BIN_DIR)/test-fusion-vanilla \
	$(BIN_DIR)/test-open \
	$(BIN_DIR)/test-malloc \
	$(BIN_DIR)/test-nbody \
	$(BIN_DIR)/test-fpu-gen \
	$(BIN_DIR)/test-fpu-printf \
	$(BIN_DIR)/test-infinite-loop \
	$(BIN_DIR)/test-int-fib \
	$(BIN_DIR)/test-int-mul \
	$(BIN_DIR)/test-jump-tables-yes \
	$(BIN_DIR)/test-jump-tables-no \
	$(BIN_DIR)/test-large-imm \
	$(BIN_DIR)/test-reloc-imm \
	$(BIN_DIR)/test-sbi-info \
	$(BIN_DIR)/test-sbi-timer

HOST_PROGRAMS = \
	$(HOST_BIN_DIR)/test-aes \
	$(HOST_BIN_DIR)/test-dhrystone \
	$(HOST_BIN_DIR)/test-miniz \
	$(HOST_BIN_DIR)/test-norx \
	$(HOST_BIN_DIR)/test-sha512 \
	$(HOST_BIN_DIR)/test-primes \
	$(HOST_BIN_DIR)/test-qsort

ifeq ($(TARGET),riscv64-unknown-elf)
PROGRAMS += \
	$(BIN_DIR)/test-bswap \
	$(BIN_DIR)/test-m-ecall-trap \
	$(BIN_DIR)/test-m-hartid \
	$(BIN_DIR)/test-m-mret-user \
	$(BIN_DIR)/test-m-mmio-htif \
	$(BIN_DIR)/test-m-mmio-timer \
	$(BIN_DIR)/test-m-mmio-uart \
	$(BIN_DIR)/test-m-poll-uart \
	$(BIN_DIR)/test-m-sv39
endif

all: dirs $(ASSEMBLY) $(PROGRAMS) $(HOST_PROGRAMS)

dirs: ; @mkdir -p $(ASM_DIR) $(OBJ_DIR) $(BIN_DIR) $(GEN_DIR) $(HOST_BIN_DIR) $(HOST_OBJ_DIR)

clean:
	rm -f $(PROGRAMS) *.o

test-sim: all
	$(EMULATOR) $(BIN_DIR)/hello-world-pcrel
	$(EMULATOR) $(BIN_DIR)/hello-world-libc
	$(EMULATOR) $(BIN_DIR)/test-args Hello World
	$(EMULATOR) $(BIN_DIR)/test-malloc
	$(EMULATOR) $(BIN_DIR)/test-open README.md
	$(EMULATOR) $(BIN_DIR)/test-int-fib
	$(EMULATOR) $(BIN_DIR)/test-int-mul
	$(EMULATOR) $(BIN_DIR)/test-fpu-printf
	$(EMULATOR) $(BIN_DIR)/test-jump-tables-yes 11
	$(EMULATOR) $(BIN_DIR)/test-jump-tables-no 11

test-sys: all
	$(EMULATOR) $(BIN_DIR)/test-m-ecall-trap
	$(EMULATOR) $(BIN_DIR)/test-m-mmio-timer
	$(EMULATOR) $(BIN_DIR)/test-m-sv39

# host benchmarks

$(HOST_OBJ_DIR)/test-aes.o: $(SRC_DIR)/test-aes.c ; cc -O3 -c $^ -o $@
$(HOST_BIN_DIR)/test-aes: $(HOST_OBJ_DIR)/test-aes.o ; cc -O3 $^ -o $@

$(HOST_OBJ_DIR)/test-dhrystone.o: $(SRC_DIR)/test-dhrystone.c ; cc -O3 -DTIME -c $^ -o $@
$(HOST_BIN_DIR)/test-dhrystone: $(HOST_OBJ_DIR)/test-dhrystone.o ; cc -O3 $^ -o $@

$(HOST_OBJ_DIR)/test-miniz.o: $(SRC_DIR)/test-miniz.c ; cc -O3 -c $^ -o $@
$(HOST_BIN_DIR)/test-miniz: $(HOST_OBJ_DIR)/test-miniz.o ; cc -O3 $^ -lm -o $@

$(HOST_OBJ_DIR)/test-norx.o: $(SRC_DIR)/test-norx.c ; cc -O3 -c $^ -o $@
$(HOST_BIN_DIR)/test-norx: $(HOST_OBJ_DIR)/test-norx.o ; cc -O3 $^ -lm -o $@

$(HOST_OBJ_DIR)/test-sha512.o: $(SRC_DIR)/test-sha512.c ; cc -O3 -c $^ -o $@
$(HOST_BIN_DIR)/test-sha512: $(HOST_OBJ_DIR)/test-sha512.o ; cc -O3 $^ -o $@

$(HOST_OBJ_DIR)/test-primes.o: $(SRC_DIR)/test-primes.c ; cc -O3 -c $^ -o $@
$(HOST_BIN_DIR)/test-primes: $(HOST_OBJ_DIR)/test-primes.o ; cc -O3 $^ -o $@

$(HOST_OBJ_DIR)/test-qsort.o: $(SRC_DIR)/test-qsort.c ; cc -O3 -c $^ -o $@
$(HOST_BIN_DIR)/test-qsort: $(HOST_OBJ_DIR)/test-qsort.o ; cc -O3 $^ -o $@

$(HOST_OBJ_DIR)/test-args.o: $(SRC_DIR)/test-args.c ; cc -O3 -c $^ -o $@
$(HOST_BIN_DIR)/test-args: $(HOST_OBJ_DIR)/test-args.o ; cc -O3 $^ -o $@

# target benchmarks

$(OBJ_DIR)/test-aes.o: $(SRC_DIR)/test-aes.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-aes: $(OBJ_DIR)/test-aes.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-dhrystone.o: $(SRC_DIR)/test-dhrystone.c ; $(CC) $(CFLAGS) -DTIME -c $^ -o $@
$(BIN_DIR)/test-dhrystone: $(OBJ_DIR)/test-dhrystone.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-miniz.o: $(SRC_DIR)/test-miniz.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-miniz: $(OBJ_DIR)/test-miniz.o ; $(CC) $(CFLAGS) $^ -lm -o $@

$(OBJ_DIR)/test-norx.o: $(SRC_DIR)/test-norx.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-norx: $(OBJ_DIR)/test-norx.o ; $(CC) $(CFLAGS) $^ -lm -o $@

$(OBJ_DIR)/test-sha512.o: $(SRC_DIR)/test-sha512.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-sha512: $(OBJ_DIR)/test-sha512.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-primes.o: $(SRC_DIR)/test-primes.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-primes: $(OBJ_DIR)/test-primes.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-qsort.o: $(SRC_DIR)/test-qsort.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-qsort: $(OBJ_DIR)/test-qsort.o ; $(CC) $(CFLAGS) $^ -o $@

# target test programs

$(ASM_DIR)/hello-world-libc.s: $(SRC_DIR)/hello-world-libc.c ; $(CC) $(CFLAGS) -S -nostartfiles $^ -o $@
$(OBJ_DIR)/hello-world-libc.o: $(SRC_DIR)/hello-world-libc.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/hello-world-libc: $(OBJ_DIR)/hello-world-libc.o ; $(CC) $(CFLAGS) $^ -o $@
$(BIN_DIR)/hello-world-libc-strip: $(BIN_DIR)/hello-world-libc ; $(STRIP) $^ -o $@

$(OBJ_DIR)/hello-world-abs.o: $(SRC_DIR)/hello-world-abs.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/hello-world-abs: $(OBJ_DIR)/hello-world-abs.o ; $(LD) $^ -o $@

$(OBJ_DIR)/hello-world-pcrel.o: $(SRC_DIR)/hello-world-pcrel.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/hello-world-pcrel: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) $^ -o $@

$(BIN_DIR)/hello-world-pcrel-giga: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) -T $(SRC_DIR)/ld.$(TARGET).script $^ -o $@
$(BIN_DIR)/hello-world-pcrel-nano: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) --strip-all -T $(SRC_DIR)/ld.$(TARGET).script $^ -o $@
$(BIN_DIR)/hello-world-pcrel-pico: $(OBJ_DIR)/hello-world-pcrel.o ; $(LD) --nmagic --strip-all -T $(SRC_DIR)/ld.$(TARGET).script $^ -o $@

$(OBJ_DIR)/bswap32.o: $(SRC_DIR)/bswap32.s ; $(CC) $(CFLAGS) -c $^ -o $@
$(OBJ_DIR)/bswap64.o: $(SRC_DIR)/bswap64.s ; $(CC) $(CFLAGS) -c $^ -o $@

$(OBJ_DIR)/test-args.o: $(SRC_DIR)/test-args.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-args: $(OBJ_DIR)/test-args.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-bswap.o: $(SRC_DIR)/test-bswap.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-bswap: $(OBJ_DIR)/test-bswap.o $(OBJ_DIR)/bswap32.o $(OBJ_DIR)/bswap64.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-loop-1.o: $(SRC_DIR)/test-loop-1.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-loop-1: $(OBJ_DIR)/test-loop-1.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-loop-2.o: $(SRC_DIR)/test-loop-2.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-loop-2: $(OBJ_DIR)/test-loop-2.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-fusion-vanilla.o: $(SRC_DIR)/test-fusion.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fusion-vanilla: $(OBJ_DIR)/test-fusion-vanilla.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-fusion-macro.o: $(SRC_DIR)/test-fusion.c ; $(CC) -DMACRO_FUSION $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fusion-macro: $(OBJ_DIR)/test-fusion-macro.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-fusion-elision.o: $(SRC_DIR)/test-fusion.c ; $(CC) -DMACRO_FUSION_ELISION $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fusion-elision: $(OBJ_DIR)/test-fusion-elision.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-fusion-indirect.o: $(SRC_DIR)/test-fusion.c ; $(CC) -DMACRO_INDIRECT $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fusion-indirect: $(OBJ_DIR)/test-fusion-indirect.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-open.o: $(SRC_DIR)/test-open.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-open: $(OBJ_DIR)/test-open.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-malloc.o: $(SRC_DIR)/test-malloc.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-malloc: $(OBJ_DIR)/test-malloc.o ; $(CC) $(CFLAGS) $^ -lm -o $@

$(OBJ_DIR)/test-nbody.o: $(SRC_DIR)/test-nbody.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-nbody: $(OBJ_DIR)/test-nbody.o ; $(CC) $(CFLAGS) $^ -lm -o $@

$(OBJ_DIR)/test-fpu-gen.o: $(SRC_DIR)/test-fpu-gen.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fpu-gen: $(OBJ_DIR)/test-fpu-gen.o ; $(CC) $(CFLAGS) $^ -o $@

$(GEN_DIR)/test-fpu-assert.c: $(BIN_DIR)/test-fpu-gen ; $(SPIKE) $(BIN_DIR)/test-fpu-gen > $@
$(OBJ_DIR)/test-fpu-assert.o: $(GEN_DIR)/test-fpu-assert.c ; $(CC) $(CFLAGS) -I$(SRC_DIR) -c $^ -o $@
$(BIN_DIR)/test-fpu-assert: $(OBJ_DIR)/test-fpu-assert.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-fpu-printf.o: $(SRC_DIR)/test-fpu-printf.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-fpu-printf: $(OBJ_DIR)/test-fpu-printf.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-infinite-loop.o: $(SRC_DIR)/test-infinite-loop.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-infinite-loop: $(OBJ_DIR)/test-infinite-loop.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-int-fib.o: $(SRC_DIR)/test-int-fib.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-int-fib: $(OBJ_DIR)/test-int-fib.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-int-mul.o: $(SRC_DIR)/test-int-mul.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-int-mul: $(OBJ_DIR)/test-int-mul.o ; $(CC) $(CFLAGS) $^ -o $@

$(ASM_DIR)/test-jump-tables-yes.s: $(SRC_DIR)/test-jump-tables.c ; $(CC) $(CFLAGS) -S -nostartfiles $^ -o $@
$(OBJ_DIR)/test-jump-tables-yes.o: $(SRC_DIR)/test-jump-tables.c ; $(CC) $(CFLAGS) -c $^ -o $@
$(BIN_DIR)/test-jump-tables-yes: $(OBJ_DIR)/test-jump-tables-yes.o ; $(CC) $(CFLAGS) $^ -o $@

$(ASM_DIR)/test-jump-tables-no.s: $(SRC_DIR)/test-jump-tables.c ; $(CC) $(CFLAGS) -S -fno-jump-tables -nostartfiles $^ -o $@
$(OBJ_DIR)/test-jump-tables-no.o: $(SRC_DIR)/test-jump-tables.c ; $(CC) $(CFLAGS) -c -fno-jump-tables $^ -o $@
$(BIN_DIR)/test-jump-tables-no: $(OBJ_DIR)/test-jump-tables-no.o ; $(CC) $(CFLAGS) $^ -o $@

$(OBJ_DIR)/test-large-imm.o: $(SRC_DIR)/test-large-imm.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-large-imm: $(OBJ_DIR)/test-large-imm.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-reloc-imm.o: $(SRC_DIR)/test-reloc-imm.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-reloc-imm: $(OBJ_DIR)/test-reloc-imm.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-ecall-trap.o: $(SRC_DIR)/test-m-ecall-trap.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-ecall-trap: $(OBJ_DIR)/test-m-ecall-trap.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-hartid.o: $(SRC_DIR)/test-m-hartid.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-hartid: $(OBJ_DIR)/test-m-hartid.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-mmio-htif.o: $(SRC_DIR)/test-m-mmio-htif.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-mmio-htif: $(OBJ_DIR)/test-m-mmio-htif.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-mmio-timer.o: $(SRC_DIR)/test-m-mmio-timer.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-mmio-timer: $(OBJ_DIR)/test-m-mmio-timer.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-mmio-uart.o: $(SRC_DIR)/test-m-mmio-uart.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-mmio-uart: $(OBJ_DIR)/test-m-mmio-uart.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-poll-uart.o: $(SRC_DIR)/test-m-poll-uart.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-poll-uart: $(OBJ_DIR)/test-m-poll-uart.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-mret-user.o: $(SRC_DIR)/test-m-mret-user.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-mret-user: $(OBJ_DIR)/test-m-mret-user.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-m-sv39.o: $(SRC_DIR)/test-m-sv39.S ; $(CC) -c $^ -o $@
$(BIN_DIR)/test-m-sv39: $(OBJ_DIR)/test-m-sv39.o ; $(LD) $^ -o $@

$(OBJ_DIR)/test-sbi-info.o: $(SRC_DIR)/test-sbi-info.c ; $(CC) -fPIC -O3 -c $^ -o $@
$(BIN_DIR)/test-sbi-info: $(OBJ_DIR)/test-sbi-info.o ; $(CC) -Wl,--no-relax -nostartfiles $^ -o $@

$(OBJ_DIR)/test-sbi-timer.o: $(SRC_DIR)/test-sbi-timer.c ; $(CC) -fPIC -O3 -c $^ -o $@
$(BIN_DIR)/test-sbi-timer: $(OBJ_DIR)/test-sbi-timer.o ; $(CC) -Wl,--no-relax -nostartfiles $^ -o $@
