#
# RISC-V Hello World
#

# Requires RISCV environment variable to be set
# RISCV = /opt/riscv/toolchain

TARGET = riscv64-unknown-elf

CC = ${RISCV}/bin/${TARGET}-gcc
CXX = ${RISCV}/bin/${TARGET}-g++
AS = ${RISCV}/bin/${TARGET}-as
LD = ${RISCV}/bin/${TARGET}-ld
STRIP = ${RISCV}/bin/${TARGET}-strip
SPIKE = ${RISCV}/bin/spike
PK = ${RISCV}/${TARGET}/bin/pk

CFLAGS = -Os -Wall -fpie -ffunction-sections -fdata-sections

PROGRAMS = \
	hello-world-libc \
	hello-world-libc-strip \
	hello-world-abs \
	hello-world-pcrel \
	hello-world-pcrel-giga \
	hello-world-pcrel-nano \
	hello-world-pcrel-pico \
	test-fib \
	test-fpu \
	test-fpu-assert \
	test-printf \
	test-sieve \
	test-sieve-cpp

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) *.o

test: all
	${SPIKE} ${PK} hello-world-libc
	${SPIKE} ${PK} hello-world-libc-strip
	${SPIKE} ${PK} hello-world-abs
	${SPIKE} ${PK} hello-world-pcrel
	${SPIKE} ${PK} test-fib
	${SPIKE} ${PK} test-sieve
	${SPIKE} ${PK} test-printf

test-fib.o: test-fib.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-fib: test-fib.o ; $(CC) $(CFLAGS) $^ -o $@

test-fpu.o: test-fpu.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-fpu: test-fpu.o ; $(CC) $(CFLAGS) $^ -o $@

test-fpu-assert.c: test-fpu ; ${SPIKE} ${PK} test-fpu | egrep -v '(nan|inf)' > $@
test-fpu-assert.o: test-fpu-assert.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-fpu-assert: test-fpu-assert.o ; $(CC) $(CFLAGS) $^ -o $@

test-printf.o: test-printf.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-printf: test-printf.o ; $(CC) $(CFLAGS) $^ -o $@

test-sieve.o: test-sieve.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-sieve: test-sieve.o ; $(CC) $(CFLAGS) $^ -o $@

test-sieve-cpp.o: test-sieve-cpp.cc ; $(CXX) -std=c++11 $(CFLAGS) -c $^ -o $@
test-sieve-cpp: test-sieve-cpp.o ; $(CXX) $(CFLAGS) $^ -o $@

hello-world-libc.o: hello-world-libc.c ; $(CC) $(CFLAGS) -c $^ -o $@
hello-world-libc: hello-world-libc.o ; $(CC) $(CFLAGS) $^ -o $@
hello-world-libc-strip: hello-world-libc ; $(STRIP) $^ -o $@

hello-world-abs.o: hello-world-abs.S ; $(AS) $^ -o $@
hello-world-abs: hello-world-abs.o ; $(LD) $^ -o $@
hello-world-pcrel.o: hello-world-pcrel.S ; $(AS) $^ -o $@
hello-world-pcrel: hello-world-pcrel.o ; $(LD) $^ -o $@

hello-world-pcrel-giga: hello-world-pcrel.o ; $(LD) -T ld.script $^ -o $@
hello-world-pcrel-nano: hello-world-pcrel.o ; $(LD) --strip-all -T ld.script $^ -o $@
hello-world-pcrel-pico: hello-world-pcrel.o ; $(LD) --nmagic --strip-all -T ld.script $^ -o $@
