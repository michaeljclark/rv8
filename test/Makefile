#
# RISC-V Hello World
#

all: hello-world-libc hello-world-asm

clean:
	rm -f hello-world-asm hello-world-asm.o hello-world-libc

hello-world-libc: hello-world.c
	riscv64-unknown-elf-gcc hello-world.c -o hello-world-libc

hello-world-asm.o:
	riscv64-unknown-elf-as hello-world.S -o hello-world-asm.o

hello-world-asm: hello-world-asm.o
	riscv64-unknown-elf-ld hello-world-asm.o -o hello-world-asm

dump:
	riscv64-unknown-elf-readelf -a hello-world-asm
	riscv64-unknown-elf-objdump -d hello-world-asm

strip:
	riscv64-unknown-elf-strip hello-world-libc
	riscv64-unknown-elf-strip hello-world-asm
