#ifndef SYSCALL_H
#define SYSCALL_H

#define SYSCALL_READ 0
#define SYSCALL_WRITE 1

#include <types.h>

void syscall_handler(void);
void syscall_read(void);
void syscall_write(char *buff, size_t count);

#endif
