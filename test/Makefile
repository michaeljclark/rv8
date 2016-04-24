#
# RISC-V Hello World
#

CC = riscv64-unknown-elf-gcc
AS = riscv64-unknown-elf-as
LD = riscv64-unknown-elf-ld

all: hello-world-libc hello-world-abs hello-world-pcrel

clean:
	rm -f hello-world-libc hello-world-abs hello-world-pcrel *.o

hello-world-libc: hello-world-libc.c ; $(CC) $^ -o $@
hello-world-abs.o: hello-world-abs.S ; $(AS) $^ -o $@
hello-world-abs: hello-world-abs.o ; $(LD) $^ -o $@
hello-world-pcrel.o: hello-world-pcrel.S ; $(AS) $^ -o $@
hello-world-pcrel: hello-world-pcrel.o ; $(LD) $^ -o $@
