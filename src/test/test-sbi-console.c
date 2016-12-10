#include "../rom/sbi.h"

void _puts(const char* str)
{
	void(*sbi_console_putchar)(char) = (void*)-2000;
	while(*str != '\0') sbi_console_putchar(*str++);
}

void _isr()
{
	int(*sbi_console_getchar)(void) = (void*)-1984;
	void(*sbi_console_putchar)(char) = (void*)-2000;

	sbi_console_putchar('A');
	asm(
		"    li      t0, 512\n"
		"    csrrc   zero, sip, t0\n" /* set sip.SEIP=0 */
		"    sret\n"
	);
}

void _start()
{
	void(*sbi_set_timer)(unsigned long long stime_value) = (void*)-1888;
	unsigned long(*sbi_mask_interrupt)(unsigned long which) = (void*)-1872;
	unsigned long(*sbi_unmask_interrupt)(unsigned long which) = (void*)-1856;

	_puts("work in progress\n");

	asm(
		"1:	 auipc   t0,     %pcrel_hi(_isr)\n "
		"    addi    t0, t0, %pcrel_lo(1b)\n "
		"    csrrw   zero, stvec, t0\n"    /* set stvec = _isr */
		"    csrrsi  zero, sstatus, 2\n"   /* set sstatus.SIE=1 */
		"    li      t0, 512\n"
		"    csrrs   zero, sie, t0\n"      /* set sie.SEIE=1 */
	);

	unsigned long ret = sbi_unmask_interrupt(3);
	if (ret < 0) {
		_puts("sbi_unmask_interrupt: error");
	}

	for(;;) asm volatile("wfi");
}
