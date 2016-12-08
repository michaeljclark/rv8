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

	sbi_console_putchar(sbi_console_getchar());
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

	/* TODO - boot ROM trap code needs to handle mideleg and hideleg */
	_puts("press any key to ebreak into M-mode trap handler, a0 <- 5\n");

	asm(
		"1:	 auipc   t0,     %pcrel_hi(_isr)\n "
		"    addi    t0, t0, %pcrel_lo(1b)\n "
		"    csrrw   zero, stvec, t0\n" /* set stvec = _isr */
		"    li      t0, 512\n"
		"    csrrs   zero, sie, t0\n"   /* set sie.SEIE=1 */
	);

	unsigned long ret = sbi_unmask_interrupt(3);
	if (ret < 0) {
		_puts("sbi_unmask_interrupt: error");
	}

	for(;;) asm volatile("wfi");
}
