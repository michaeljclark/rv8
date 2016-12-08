#include "../rom/sbi.h"

#define PUTC sbi_console_putchar

void _start()
{
	unsigned long(*sbi_hart_id)(void) = (void*)-2048;
	unsigned long(*sbi_num_harts)(void) = (void*)-2032;
	void(*sbi_console_putchar)(char) = (void*)-2000;
	unsigned long(*sbi_query_memory)(unsigned long id, unsigned long *) = (void*)-2016;
	void(*sbi_shutdown)(void) = (void*)-1904;

	/* 
	 * Note: This C code is designed to run in Machine mode without
	 * any initial setup of stack, thread or global pointer registers.
	 *
	 * For this reason, this simple test code avoids using constant
	 * strings as the compiler will generate GP-relative references.

	 * TODO: Figure out how to switch off GP relative constants
	 *       and instead use PC-relative access for constants.
	 */

	/* print hart_id */
	unsigned long hart_id = sbi_hart_id();
	PUTC('h');
	PUTC('a');
	PUTC('r');
	PUTC('t');
	PUTC('_');
	PUTC('i');
	PUTC('d');
	PUTC(' ');
	PUTC(' ');
	PUTC(' ');
	PUTC('0' + (hart_id / 1000) % 10);
	PUTC('0' + (hart_id / 100) % 10);
	PUTC('0' + (hart_id / 10) % 10);
	PUTC('0' + hart_id % 10);
	PUTC('\r');
	PUTC('\n');

	/* print num_harts */
	unsigned long num_harts = sbi_num_harts();
	PUTC('n');
	PUTC('u');
	PUTC('m');
	PUTC('_');
	PUTC('h');
	PUTC('a');
	PUTC('r');
	PUTC('t');
	PUTC('s');
	PUTC(' ');
	PUTC('0' + (num_harts / 1000) % 10);
	PUTC('0' + (num_harts / 100) % 10);
	PUTC('0' + (num_harts / 10) % 10);
	PUTC('0' + num_harts % 10);
	PUTC('\r');
	PUTC('\n');

	/* get memory info */
	unsigned long block[3];
	sbi_query_memory(0, (unsigned long*)&block);

	/* print memory base */
	PUTC('m');
	PUTC('e');
	PUTC('m');
	PUTC('_');
	PUTC('b');
	PUTC('a');
	PUTC('s');
	PUTC('e');
	PUTC(' ');
	PUTC(' ');
	PUTC('0' + (block[0] / 1000000000) % 10);
	PUTC('0' + (block[0] / 100000000) % 10);
	PUTC('0' + (block[0] / 10000000) % 10);
	PUTC('0' + (block[0] / 1000000) % 10);
	PUTC('0' + (block[0] / 100000) % 10);
	PUTC('0' + (block[0] / 10000) % 10);
	PUTC('0' + (block[0] / 1000) % 10);
	PUTC('0' + (block[0] / 100) % 10);
	PUTC('0' + (block[0] / 10) % 10);
	PUTC('0' + block[0] % 10);
	PUTC('\r');
	PUTC('\n');

	/* print memory size */
	PUTC('m');
	PUTC('e');
	PUTC('m');
	PUTC('_');
	PUTC('s');
	PUTC('i');
	PUTC('z');
	PUTC('e');
	PUTC(' ');
	PUTC(' ');
	PUTC('0' + (block[1] / 1000000000) % 10);
	PUTC('0' + (block[1] / 100000000) % 10);
	PUTC('0' + (block[1] / 10000000) % 10);
	PUTC('0' + (block[1] / 1000000) % 10);
	PUTC('0' + (block[1] / 100000) % 10);
	PUTC('0' + (block[1] / 10000) % 10);
	PUTC('0' + (block[1] / 1000) % 10);
	PUTC('0' + (block[1] / 100) % 10);
	PUTC('0' + (block[1] / 10) % 10);
	PUTC('0' + block[1] % 10);
	PUTC('\r');
	PUTC('\n');

	/* wait for IO buffers to quench */
	for (volatile int i = 0; i < 100000; i++) {}

	/* shutdown */
	sbi_shutdown();
}
