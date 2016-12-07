#include "../rom/sbi.h"

void _start()
{
	void(*sbi_console_putchar)(char) = (void*)-2000;

	sbi_console_putchar('H');
	sbi_console_putchar('e');
	sbi_console_putchar('l');
	sbi_console_putchar('l');
	sbi_console_putchar('o');
	sbi_console_putchar('\n');

	for (volatile int i = 0; i < 100000; i++) {}

	asm volatile ("ebreak");
}
