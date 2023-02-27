#include <libk/stdio.h>
#include <panic.h>
#include <regs.h>
#include <graphics.h>
#include <idt.h>

__attribute__((noreturn)) void panic(uint64_t rsp, const char *s, ...)
{
	set_color(&main_fb, RED, BLACK);
	printf("KERNEL PANIC\n");
	/*	set_color(&main_fb, WHITE, BLACK); */
	va_list list;
	va_start(list, s);
	vprintf(s, list);
	va_end(list);

	if (rsp) {
		print_regs_from_rsp(rsp);
	}

	disable_interrupts();
	for (;;) {
		__asm__ __volatile__("hlt;");
	}
}
