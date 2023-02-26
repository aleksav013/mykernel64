#ifndef PANIC_H
#define PANIC_H

#include <types.h>

extern uint64_t panic_rsp;

__attribute__((noreturn)) void panic(uint64_t rsp, const char *s, ...);

#endif
