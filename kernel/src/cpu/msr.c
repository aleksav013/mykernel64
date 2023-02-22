#include <msr.h>

uint64_t read_msr(uint32_t addr)
{
	uint64_t eax;
	uint64_t edx;
	uint64_t ecx = addr;

	__asm__ __volatile__ ("rdmsr;" : "=a"(eax), "=d"(edx) : "c"(ecx) :);

	return (edx << 32) | eax;
}

void write_msr(uint32_t addr, uint64_t value)
{
	uint64_t eax = value & 0xFFFFFFFF;
	uint64_t edx = value >> 32;
	uint64_t ecx = addr;

	__asm__ __volatile__ ("wrmsr;" : : "a"(eax), "d"(edx), "c"(ecx) :);
}
