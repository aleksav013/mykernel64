#ifndef SERIAL_STDIO_H
#define SERIAL_STDIO_H

#include <types.h>

void serial_print_char(char c);
void serial_print_string(const char* s);
void serial_print_int(uint64_t num);
void serial_print_hex(uint64_t num);
void serial_printf(const char *s, ...);
void serial_vprintf(const char *s, va_list list);

#endif
