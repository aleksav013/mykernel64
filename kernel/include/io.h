#ifndef IO_H
#define IO_H

#include <types.h>

uint8_t inb(uint32_t port);
void outb(uint32_t port, uint8_t value);
void io_wait(void);

#endif
