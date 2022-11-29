#include <libk/stdio.h>
#include <panic.h>
#include <graphics.h>

uint64_t panic_rsp;

__attribute__((noreturn))
void panic(const char *s, ...)
{
	set_color(&main_fb, RED, BLACK);
	printf("KERNEL PANIC\n");
//	set_color(&main_fb, WHITE, BLACK);
	va_list list;
	va_start(list, s);
	vprintf(s, list);
	va_end(list);

	if (panic_rsp != 0) {
		printf("info:\n");
		uint64_t i = 0;
		printf("ss: 0x%x   ", (uint16_t)*((uint64_t*)panic_rsp + i++));
		printf("ds: 0x%x   ", (uint16_t)*((uint64_t*)panic_rsp + i++));
		printf("es: 0x%x   ", (uint16_t)*((uint64_t*)panic_rsp + i++));
		printf("r15: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("r14: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("r13: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("r12: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("r11: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("r10: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("r9: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("r8: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rdi: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rsi: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rdx: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rcx: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rbx: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rax: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		if (*((uint64_t*)panic_rsp + i) <= 0xffffffff)
			printf("err code: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rip: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("cs: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rflags: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("rsp: 0x%x   ", *((uint64_t*)panic_rsp + i++));
		printf("ss: 0x%x   ", *((uint64_t*)panic_rsp + i++));
	}

	__asm__ volatile ("cli;");
	for(;;) {
		__asm__ volatile ("hlt;");
	}
}
