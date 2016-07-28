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
CXXFLAGS = -std=c++1y $(CFLAGS)

PROGRAMS = \
	hello-world-libc \
	hello-world-libc-strip \
	hello-world-abs \
	hello-world-pcrel \
	hello-world-pcrel-giga \
	hello-world-pcrel-nano \
	hello-world-pcrel-pico \
	test-int-fib \
	test-fpu \
	test-fpu-assert \
	test-fpu-printf \
	test-sieve

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) *.o

test: all
	${SPIKE} ${PK} hello-world-pcrel
	${SPIKE} ${PK} hello-world-libc
	${SPIKE} ${PK} test-int-fib
	${SPIKE} ${PK} test-fpu-printf
	${SPIKE} ${PK} test-fpu-assert
	${SPIKE} ${PK} test-sieve

test-int-fib.o: test-int-fib.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-int-fib: test-int-fib.o ; $(CC) $(CFLAGS) $^ -o $@

test-fpu.o: test-fpu.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-fpu: test-fpu.o ; $(CC) $(CFLAGS) $^ -o $@

test-fpu-assert.c: test-fpu ; ${SPIKE} ${PK} test-fpu | egrep -v '(nan|inf)' > $@
test-fpu-assert.o: test-fpu-assert.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-fpu-assert: test-fpu-assert.o ; $(CC) $(CFLAGS) $^ -o $@

test-printf.o: test-printf.c ; $(CC) $(CFLAGS) -c $^ -o $@
test-printf: test-printf.o ; $(CC) $(CFLAGS) $^ -o $@

test-sieve.o: test-sieve.cc ; $(CXX) $(CXXFLAGS) -c $^ -o $@
test-sieve: test-sieve.o ; $(CXX) $(CXXFLAGS) $^ -o $@

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
