#include <regs.h>
#include <libk/stdio.h>

void print_regs_from_rsp(uint64_t rsp)
{
	regs_t *regs = (regs_t *)rsp;

	printf("info regs:\n");
	printf("rax: 0x%x; ", regs->rax);
	printf("rbx: 0x%x; ", regs->rbx);
	printf("rcx: 0x%x; ", regs->rcx);
	printf("rdx: 0x%x; ", regs->rdx);
	printf("rdi: 0x%x; ", regs->rdi);
	printf("rsi: 0x%x; ", regs->rsi);
	printf("rsp: 0x%x; ", regs->rsp);
	printf("rbp: 0x%x; ", regs->rbp);
	printf("r8: 0x%x; ", regs->r8);
	printf("r9: 0x%x; ", regs->r9);
	printf("r10: 0x%x; ", regs->r10);
	printf("r11: 0x%x; ", regs->r11);
	printf("r12: 0x%x; ", regs->r12);
	printf("r13: 0x%x; ", regs->r13);
	printf("r14: 0x%x; ", regs->r14);
	printf("r15: 0x%x; ", regs->r15);
	printf("rip: 0x%x; ", regs->rip);
	printf("cs: 0x%x; ", regs->cs);
	printf("ds: 0x%x; ", regs->seg >> 48);
	printf("ss: 0x%x; ", regs->ss);
	printf("es: 0x%x; ", (regs->seg >> 32) & 0xFFFF);
	printf("fs: 0x%x; ", (regs->seg >> 16) & 0xFFFF);
	printf("gs: 0x%x; ", regs->seg & 0xFFFF);
	printf("rflags: 0x%x; ", regs->rflags);
}
