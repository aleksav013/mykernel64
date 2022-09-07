#include <libk/stdio.h>
#include <syscall.h>

void syscall_handler()
{
	uint64_t rax;
	__asm__ volatile ("mov %%rax, %0;" : "=r"(rax) : :);
	printf("syscall invoked with rax=0x%x\n", rax);
}
