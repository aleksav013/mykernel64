#ifndef MSR_H
#define MSR_H

#include <types.h>

#define APIC_BASE_MSR 0x0000001B

uint64_t read_msr(uint32_t addr);
void write_msr(uint32_t addr, uint64_t value);

#endif
