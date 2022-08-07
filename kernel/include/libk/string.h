#ifndef STRING_H
#define STRING_H

#include <types.h>

size_t strlen(const char* s);
uint64_t stoi(const char *s);
void itos(uint64_t num, char* s);
void strrev(char *s);

#endif
