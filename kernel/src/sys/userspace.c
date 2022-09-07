#include <types.h>
#include <userspace.h>

void begin_userspace()
{
	__asm__ volatile ("mov $0x1, %rax; syscall;");
	__asm__ volatile ("mov $0x11, %rax; syscall;");
	__asm__ volatile ("mov $0x111, %rax; syscall;");
	while(true) {
	}
}
