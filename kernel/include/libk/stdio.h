#ifndef STDIO_H
#define STDIO_H

#include <types.h>

extern uint32_t curr_x;
extern uint32_t curr_y;

void print_char(char c);
void print_string(const char* s);
void print_int(uint64_t num);
void print_hex(uint64_t num);
void printf(const char *s, ...);

#endif
