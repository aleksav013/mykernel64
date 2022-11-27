#include <libk/stdio.h>
#include <panic.h>

__attribute__((noreturn))
void panic()
{
	printf("PANIC MODE ENETERED");
	__asm__ volatile ("cli;");
	for(;;) {
		__asm__ volatile ("hlt;");
	}
}
