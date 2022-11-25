#include <types.h>
#include <libk/serial_stdio.h>

struct registers_t {
	uint64_t rax;
	uint64_t rbx;
	uint64_t rcx;
	uint64_t rdx;
	uint64_t rsi;
	uint64_t rdi;
	uint64_t rsp;
	uint64_t rbp;
	uint64_t r8;
	uint64_t r9;
	uint64_t r10;
	uint64_t r11;
	uint64_t r12;
	uint64_t r13;
	uint64_t r14;
	uint64_t r15;
	uint64_t rip;
	uint16_t cs;
	uint16_t ds;
	uint16_t ss;
	uint16_t es;
	uint16_t fs;
	uint16_t gs;
};
typedef struct registers_t registers_t;

void print_reg(registers_t* registers)
{
	printf("printing registers:");
	printf("rax: %x", registers->rax);
	printf("rbx: %x", registers->rbx);
	printf("rcx: %x", registers->rcx);
	printf("rdx: %x", registers->rdx);
	printf("rsi: %x", registers->rsi);
	printf("rdi: %x", registers->rdi);
	printf("rsp: %x", registers->rsp);
	printf("rbp: %x", registers->rbp);
	printf("r8: %x", registers->r8);
	printf("r9: %x", registers->r9);
	printf("r10: %x", registers->r10);
	printf("r11: %x", registers->r11);
	printf("r12: %x", registers->r12);
	printf("r13: %x", registers->r13);
	printf("r14: %x", registers->r14);
	printf("r15: %x", registers->r15);
	printf("rip: %x", registers->rip);
	printf("cs: %x", registers->cs);
	printf("ds: %x", registers->ds);
	printf("ss: %x", registers->ss);
	printf("es: %x", registers->es);
	printf("fs: %x", registers->fs);
	printf("gs: %x", registers->gs);
}
