#ifndef SERIAL_H
#define SERIAL_H

#define COM1 0x3F8
#define PORT COM1

#include <types.h>

void init_serial(void);
uint8_t serial_received(void);
char read_serial(void);
uint8_t is_transmit_empty(void);
void write_serial(char c);

#endif
