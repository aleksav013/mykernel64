#include <io.h>
#include <serial.h>

void init_serial()
{
	outb(PORT + 1, 0x00); /* Disable all interrupts */
	outb(PORT + 3, 0x80); /* Enable DLAB (set baud rate divisor) */
	outb(PORT + 0, 0x03); /* Set divisor to 3 (lo byte) 38400 baud */
	outb(PORT + 1, 0x00); /* (hi byte) */
	outb(PORT + 3, 0x03); /* 8 bits, no parity, one stop bit */
	outb(PORT + 2,
	     0xC7); /* Enable FIFO, clear them, with 14-byte threshold */
	outb(PORT + 4, 0x0B); /* IRQs enabled, RTS/DSR set */
}

uint8_t serial_received()
{
	return inb(PORT + 5) & 1;
}

char read_serial()
{
	while (serial_received() == 0)
		;

	return (char)inb(PORT);
}

uint8_t is_transmit_empty()
{
	return inb(PORT + 5) & 0x20;
}

void write_serial(char c)
{
	while (is_transmit_empty() == 0)
		;

	outb(PORT, (uint8_t)c);
}
