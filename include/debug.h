#ifndef DEBUG_H
#define DEBUG_H

#include <stdint.h>

void bochs_breakpoint(void);
void put_in_r8(uint64_t value);
void put_in_r9(uint64_t value);
void put_in_r10(uint64_t value);

#endif
