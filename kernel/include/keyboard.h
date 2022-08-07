#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_DATA_PORT	0x60
#define KEYBOARD_STATUS_PORT	0x64

uint32_t stoi(const char *s);
void itos(uint32_t num, char** s);
void print_char(char c);
void print_string(char* s);
void keyboard_handler(void);

#endif
