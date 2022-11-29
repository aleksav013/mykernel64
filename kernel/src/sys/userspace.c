#include <types.h>
#include <userspace.h>
#include <libk/stdio.h>
#include <libk/string.h>

void begin_userspace()
{
	// read
	__asm__ volatile ("mov $0x0, %rax; syscall;");
	// write
	__asm__ volatile ("mov $0x1, %rax; syscall;");

	while(true) {
	}
}
