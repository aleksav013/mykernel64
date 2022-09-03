#include <tss.h>

tss_type tss;

void load_tss()
{
	__asm__ volatile ("mov $0x28, %ax; ltr %ax;");
}

void init_tss()
{
	tss.iopb = sizeof(tss_type);

	load_tss();
}
