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
		uint64_t seg = *(uint64_t *)(rsp + SEG_OFF);

		printf("info regs:\n");
		printf("rax: 0x%x; ", *(uint64_t *)(rsp + RAX_OFF));
		printf("rbx: 0x%x; ", *(uint64_t *)(rsp + RBX_OFF));
		printf("rcx: 0x%x; ", *(uint64_t *)(rsp + RCX_OFF));
		printf("rdx: 0x%x; ", *(uint64_t *)(rsp + RDX_OFF));
		printf("rdi: 0x%x; ", *(uint64_t *)(rsp + RDI_OFF));
		printf("rsi: 0x%x; ", *(uint64_t *)(rsp + RSI_OFF));
		printf("rsp: 0x%x; ", *(uint64_t *)(rsp + RSP_OFF));
		printf("rbp: 0x%x; ", *(uint64_t *)(rsp + RBP_OFF));
		printf("r8: 0x%x; ", *(uint64_t *)(rsp + R8_OFF));
		printf("r9: 0x%x; ", *(uint64_t *)(rsp + R9_OFF));
		printf("r10: 0x%x; ", *(uint64_t *)(rsp + R10_OFF));
		printf("r11: 0x%x; ", *(uint64_t *)(rsp + R11_OFF));
		printf("r12: 0x%x; ", *(uint64_t *)(rsp + R12_OFF));
		printf("r13: 0x%x; ", *(uint64_t *)(rsp + R13_OFF));
		printf("r14: 0x%x; ", *(uint64_t *)(rsp + R14_OFF));
		printf("r15: 0x%x; ", *(uint64_t *)(rsp + R15_OFF));
		printf("rip: 0x%x; ", *(uint64_t *)(rsp + RIP_OFF));
		printf("cs: 0x%x; ", *(uint64_t *)(rsp + CS_OFF));
		printf("ds: 0x%x; ", seg >> 48);
		printf("ss: 0x%x; ", *(uint64_t *)(rsp + SS_OFF));
		printf("es: 0x%x; ", (seg >> 32) & 0xFFFF);
		printf("fs: 0x%x; ", (seg >> 16) & 0xFFFF);
		printf("gs: 0x%x; ", seg & 0xFFFF);
		printf("rflags: 0x%x; ", *(uint64_t *)(rsp + RFLAGS_OFF));
	}

	disable_interrupts();
	for (;;) {
		__asm__ __volatile__("hlt;");
	}
}
