#
# RISC-V Hello World
#

CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld

CFLAGS = -Wall -fpie -ffunction-sections -fdata-sections

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

hello-world-libc.o: hello-world-libc.c ; $(CC) $(CFLAGS) -c $^ -o $@
hello-world-libc: hello-world-libc.o ; $(CC) $(CFLAGS) $^ -o $@
hello-world-abs.o: hello-world-abs.S ; $(AS) $^ -o $@
hello-world-abs: hello-world-abs.o ; $(LD) $^ -o $@
hello-world-pcrel.o: hello-world-pcrel.S ; $(AS) $^ -o $@
hello-world-pcrel: hello-world-pcrel.o ; $(LD) $^ -o $@
hello-world-pcrel-giga: hello-world-pcrel.o ; $(LD) -T ld.script $^ -o $@
hello-world-pcrel-nano: hello-world-pcrel.o ; $(LD) --strip-all -T ld.script $^ -o $@
hello-world-pcrel-pico: hello-world-pcrel.o ; $(LD) --nmagic --strip-all -T ld.script $^ -o $@
