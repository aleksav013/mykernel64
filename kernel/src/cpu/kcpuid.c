#include <types.h>
#include <kcpuid.h>

void kcpuid(uint32_t func, uint32_t subfunc, CPUIDinfo *info)
{
	__asm__ __volatile__("cpuid"
			     : "=a"(info->EAX), "=b"(info->EBX),
			       "=c"(info->ECX), "=d"(info->EDX)
			     : "a"(func), "c"(subfunc)
			     :);
}
