#ifndef KCPUID_H
#define KCPUID_H

#include <types.h>

struct CPUIDinfo
{
	uint32_t EAX;
	uint32_t EBX;
	uint32_t ECX;
	uint32_t EDX;
};
typedef struct CPUIDinfo CPUIDinfo;

void kcpuid(uint32_t func, uint32_t subfunc, CPUIDinfo* info);

#endif
