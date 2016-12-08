#include "../rom/sbi.h"

void _puts(const char* str)
{
	void(*sbi_console_putchar)(char) = (void*)-2000;
	while(*str != '\0') sbi_console_putchar(*str++);
}

void _putul(unsigned long val, unsigned long ndigs)
{
	void(*sbi_console_putchar)(char) = (void*)-2000;
	if (ndigs > 9) sbi_console_putchar('0' + (val / 1000000000) % 10);
	if (ndigs > 8) sbi_console_putchar('0' + (val / 100000000) % 10);
	if (ndigs > 7) sbi_console_putchar('0' + (val / 10000000) % 10);
	if (ndigs > 6) sbi_console_putchar('0' + (val / 1000000) % 10);
	if (ndigs > 5) sbi_console_putchar('0' + (val / 100000) % 10);
	if (ndigs > 4) sbi_console_putchar('0' + (val / 10000) % 10);
	if (ndigs > 3) sbi_console_putchar('0' + (val / 1000) % 10);
	if (ndigs > 2) sbi_console_putchar('0' + (val / 100) % 10);
	if (ndigs > 1) sbi_console_putchar('0' + (val / 10) % 10);
	if (ndigs > 0) sbi_console_putchar('0' + val % 10);
}

void _putval(const char* name, unsigned long val, unsigned long ndigs)
{
	_puts(name);
	_putul(val, ndigs);
	_puts("\r\n");
}

void _start()
{
	unsigned long(*sbi_hart_id)(void) = (void*)-2048;
	unsigned long(*sbi_num_harts)(void) = (void*)-2032;
	unsigned long(*sbi_query_memory)(unsigned long id, unsigned long *) = (void*)-2016;
	void(*sbi_shutdown)(void) = (void*)-1904;

	/* 
	 * Note: This C code is designed to run in Machine mode without
	 * any initial setup of stack, thread or global pointer registers.
	 *
	 * For this reason, the code is linked with --no-relax to avoid
	 * GP-relative constant references
	 */

	/* print hart_id and num_harts */
	_putval("hart_id   ", sbi_hart_id(), 4);
	_putval("num_harts ", sbi_num_harts(), 4);

	/* get memory info */
	unsigned long block[3];
	sbi_query_memory(0, (unsigned long*)&block);

	/* print memory base and size */
	_putval("mem_base  ", block[0], 10);
	_putval("mem_size  ", block[1], 10);

	/* wait for IO buffers to quench */
	for (volatile int i = 0; i < 100000; i++) {}

	/* shutdown */
	sbi_shutdown();
}
