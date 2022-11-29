#include <tss.h>
#include <heap.h>
#include <libk/stdio.h>
#include <libk/string.h>

tss_type tss;

void load_tss()
{
	__asm__ volatile ("push %rax; mov $0x28, %ax; ltr %ax; pop %rax;");
}

void init_tss()
{
	tss.iopb = sizeof(tss_type);
	uint64_t stack = (uint64_t)kalloc(4096*4);
	stack = (uint64_t)kalloc(4096*4) + 8;
	tss.rsp0_low = (uint32_t)stack;
	tss.rsp0_high = (uint32_t)(stack >> 32);

	load_tss();
}
