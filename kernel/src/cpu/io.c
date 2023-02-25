#include <types.h>
#include <io.h>

uint8_t inb(uint32_t port)
{
	uint8_t ret;
	__asm__ __volatile__("inb %%dx, %%al;" : "=a"(ret) : "d"(port));
	return ret;
}

void outb(uint32_t port, uint8_t value)
{
	__asm__ __volatile__("outb %%al, %%dx;" : : "d"(port), "a"(value));
}

void io_wait(void)
{
	outb(0x80, 0);
}
