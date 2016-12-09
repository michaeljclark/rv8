#include "../rom/sbi.h"

void _puts(const char* str)
{
	void(*sbi_console_putchar)(char) = (void*)-2000;
	while(*str != '\0') sbi_console_putchar(*str++);
}

void _isr()
{
	static int counter = 0;

	unsigned long(*sbi_timebase)(void) = (void*)-1920;
	void(*sbi_set_timer)(unsigned long long stime_value) = (void*)-1888;

	_puts(counter++ % 2 ? "beep\r\n" : "boop\r\n");

	sbi_set_timer(sbi_timebase());

	asm(
		"    li      t0, 32\n"
		"    csrrc   zero, sip, t0\n"      /* set sie.STIP=0 */
		"    sret\n"
	);
}

void _start()
{
	unsigned long(*sbi_timebase)(void) = (void*)-1920;
	void(*sbi_set_timer)(unsigned long long stime_value) = (void*)-1888;

	asm(
		"1:	 auipc   t0,     %pcrel_hi(_isr)\n "
		"    addi    t0, t0, %pcrel_lo(1b)\n "
		"    csrrw   zero, stvec, t0\n"    /* set stvec = _isr */
		"    csrrsi  zero, sstatus, 2\n"   /* set sstatus.SIE=1 */
		"    li      t0, 32\n"
		"    csrrs   zero, sie, t0\n"      /* set sie.STIE=1 */
	);

	sbi_set_timer(sbi_timebase());

	for(;;) asm volatile("wfi");
}
