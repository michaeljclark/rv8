#
# RISC-V Hello World
#

RISCV_TOOLCHAIN_ROOT = /opt/riscv/toolchain
RISCV_TOOLCHAIN_PREFIX = riscv64-unknown-elf

CC = $(shell which ${RISCV_TOOLCHAIN_PREFIX}-gcc || ${RISCV_TOOLCHAIN_ROOT}/bin/${RISCV_TOOLCHAIN_PREFIX}-gcc)
AS = $(shell which ${RISCV_TOOLCHAIN_PREFIX}-as || ${RISCV_TOOLCHAIN_ROOT}/bin/${RISCV_TOOLCHAIN_PREFIX}-as)
LD = $(shell which ${RISCV_TOOLCHAIN_PREFIX}-ld || ${RISCV_TOOLCHAIN_ROOT}/bin/${RISCV_TOOLCHAIN_PREFIX}-ld)
SPIKE = $(shell which spike || ${RISCV_TOOLCHAIN_ROOT}/bin/riscv64-unknown-elf-ld)
PK = ${RISCV_TOOLCHAIN_ROOT}/${RISCV_TOOLCHAIN_PREFIX}/bin/pk

CFLAGS = -Os -Wall -fpie -ffunction-sections -fdata-sections

PROGRAMS = \
	hello-world-libc \
	hello-world-abs \
	hello-world-pcrel \
	hello-world-pcrel-giga \
	hello-world-pcrel-nano \
	hello-world-pcrel-pico

all: $(PROGRAMS)

clean:
	rm -f $(PROGRAMS) *.o

test: $(PROGRAMS)
	${SPIKE} ${PK} hello-world-libc
	${SPIKE} ${PK} hello-world-abs
	${SPIKE} ${PK} hello-world-pcrel

hello-world-libc.o: hello-world-libc.c ; $(CC) $(CFLAGS) -c $^ -o $@
hello-world-libc: hello-world-libc.o ; $(CC) $(CFLAGS) $^ -o $@
hello-world-abs.o: hello-world-abs.S ; $(AS) $^ -o $@
hello-world-abs: hello-world-abs.o ; $(LD) $^ -o $@
hello-world-pcrel.o: hello-world-pcrel.S ; $(AS) $^ -o $@
hello-world-pcrel: hello-world-pcrel.o ; $(LD) $^ -o $@

hello-world-pcrel-giga: hello-world-pcrel.o ; $(LD) -T ld.script $^ -o $@
hello-world-pcrel-nano: hello-world-pcrel.o ; $(LD) --strip-all -T ld.script $^ -o $@
hello-world-pcrel-pico: hello-world-pcrel.o ; $(LD) --nmagic --strip-all -T ld.script $^ -o $@
