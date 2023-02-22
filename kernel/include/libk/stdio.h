#ifndef STDIO_H
#define STDIO_H

#include <types.h>
#include <atomic.h>

void print_char(char c);
void print_string(const char* s);
void print_int(uint64_t num);
void print_hex(uint64_t num);
void printf(const char *s, ...);
void vprintf(const char *s, va_list list);

extern mutex_t stdio_lock;

#endif
