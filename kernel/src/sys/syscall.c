#include <libk/stdio.h>
#include <syscall.h>
#include <libk/string.h>
#include <heap.h>

void syscall_handler()
{
	uint64_t rax;
	uint64_t rdi;
	uint64_t rsi;
	__asm__ __volatile__("mov %%rax, %0;" : "=r"(rax) : :);
	__asm__ __volatile__("mov %%rdi, %0;" : "=r"(rdi) : :);
	__asm__ __volatile__("mov %%rsi, %0;" : "=r"(rsi) : :);
	switch (rax) {
	case SYSCALL_READ:
		syscall_read();
		break;
	case SYSCALL_WRITE:
		syscall_write((char *)rdi, rsi);
		break;
	default:
		printf("invalid syscall (%d)\n", rax);
	}
}

void syscall_read()
{
	printf("syscall_read()\n");
}

void syscall_write(char *buff, size_t count)
{
	printf("syscall_write()\n");
	printf("buff = 0x%x, count = %d\n", buff, count);
}
