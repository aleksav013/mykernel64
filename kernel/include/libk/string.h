#ifndef STRING_H
#define STRING_H

#include <types.h>

size_t strlen(const char* s);
uint64_t stoi(const char *s);
void itos(uint64_t num, char* s);
void itoh(uint64_t num, char* s);
void strrev(char *s);
void memcpy(const void* destptr, const void* srcptr, size_t n);
void memset(const void* destptr, uint8_t value, size_t n);
int32_t memcmp(const void* aptr, const void* bptr);

#endif
