#include "../rom/sbi.h"

void _start()
{
	void(*sbi_console_putchar)(char) = (void*)-2000;
	unsigned long(*sbi_query_memory)(unsigned long id, unsigned long *) = (void*)-2016;

	unsigned long block[3];
	sbi_query_memory(0, (unsigned long*)&block);

	sbi_console_putchar('m');
	sbi_console_putchar('e');
	sbi_console_putchar('m');
	sbi_console_putchar('_');
	sbi_console_putchar('b');
	sbi_console_putchar('a');
	sbi_console_putchar('s');
	sbi_console_putchar('e');
	sbi_console_putchar(' ');
	sbi_console_putchar('0' + (block[0] / 1000000000) % 10);
	sbi_console_putchar('0' + (block[0] / 100000000) % 10);
	sbi_console_putchar('0' + (block[0] / 10000000) % 10);
	sbi_console_putchar('0' + (block[0] / 1000000) % 10);
	sbi_console_putchar('0' + (block[0] / 100000) % 10);
	sbi_console_putchar('0' + (block[0] / 10000) % 10);
	sbi_console_putchar('0' + (block[0] / 1000) % 10);
	sbi_console_putchar('0' + (block[0] / 100) % 10);
	sbi_console_putchar('0' + (block[0] / 10) % 10);
	sbi_console_putchar('0' + block[0] % 10);
	sbi_console_putchar('\n');

	sbi_console_putchar('m');
	sbi_console_putchar('e');
	sbi_console_putchar('m');
	sbi_console_putchar('_');
	sbi_console_putchar('s');
	sbi_console_putchar('i');
	sbi_console_putchar('z');
	sbi_console_putchar('e');
	sbi_console_putchar(' ');
	sbi_console_putchar('0' + (block[1] / 1000000000) % 10);
	sbi_console_putchar('0' + (block[1] / 100000000) % 10);
	sbi_console_putchar('0' + (block[1] / 10000000) % 10);
	sbi_console_putchar('0' + (block[1] / 1000000) % 10);
	sbi_console_putchar('0' + (block[1] / 100000) % 10);
	sbi_console_putchar('0' + (block[1] / 10000) % 10);
	sbi_console_putchar('0' + (block[1] / 1000) % 10);
	sbi_console_putchar('0' + (block[1] / 100) % 10);
	sbi_console_putchar('0' + (block[1] / 10) % 10);
	sbi_console_putchar('0' + block[1] % 10);
	sbi_console_putchar('\n');

	for (volatile int i = 0; i < 100000; i++) {}

	asm volatile ("ebreak");
}
