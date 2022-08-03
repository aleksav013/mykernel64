#include <stdint.h>

void bochs_breakpoint(void)
{
	__asm__ volatile ("xchgw %bx, %bx;");
}

void put_in_r8(uint64_t value)
{
	__asm__ volatile ("movq %0, %%r8;" : : "r"(value) : "%r8");
}

void put_in_r9(uint64_t value)
{
	__asm__ volatile ("movq %0, %%r9;" : : "r"(value) : "%r9");
}

void put_in_r10(uint64_t value)
{
	__asm__ volatile ("movq %0, %%r10;" : : "r"(value) : "%r10");
}
