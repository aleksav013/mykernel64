#include <tss.h>
#include <heap.h>
#include <libk/stdio.h>
#include <libk/string.h>

tss_type tss;

void load_tss()
{
	__asm__ __volatile__("push %rax; mov $0x28, %ax; ltr %ax; pop %rax;");
}

void init_tss()
{
	memset(&tss, 0, sizeof(tss_type));
	tss.iopb = sizeof(tss_type);
	uint32_t stack_size = 1024 * 16;
	uint64_t stack = (uint64_t)kalloc(stack_size) + stack_size - 8;
	tss.rsp0_low = (uint32_t)stack;
	tss.rsp0_high = (uint32_t)(stack >> 32);

	uint32_t ist_stack_size = 1024 * 32;
	uint64_t ist_stack =
		(uint64_t)kalloc(ist_stack_size) + ist_stack_size - 8;
	tss.ist1_low = (uint32_t)ist_stack;
	tss.ist1_high = (uint32_t)(ist_stack >> 32);

	load_tss();
}
