#include <types.h>
#include <libk/serial_stdio.h>
#include <stdarg.h>
#include <libk/string.h>
#include <graphics.h>
#include <serial.h>

mutex_t serial_stdio_lock;

void serial_print_char(char c)
{
	write_serial(c);
}

void serial_print_string(const char *s)
{
	size_t i;
	for (i = 0; i < strlen(s); i++) {
		serial_print_char(s[i]);
	}
}

void serial_print_int(uint64_t num)
{
	char a[100];
	itos(num, a);
	serial_print_string(a);
}

void serial_print_hex(uint64_t num)
{
	char a[100];
	itoh(num, a);
	serial_print_string(a);
}

void serial_printf(const char *s, ...)
{
	va_list list;
	va_start(list, s);

	serial_vprintf(s, list);

	va_end(list);
}

void serial_vprintf(const char *s, va_list list)
{
	lock(serial_stdio_lock);
	size_t count = 0;
	size_t i;
	for (i = 0; i < strlen(s); i++)
		if (s[i] == '%')
			count++;

	for (i = 0; i < strlen(s); i++) {
		if (s[i] == '%') {
			i++;
			if (s[i] == 'c')
				serial_print_char((char)va_arg(list, uint32_t));
			else if (s[i] == 's')
				serial_print_string(va_arg(list, char *));
			else if (s[i] == 'd')
				serial_print_int(
					(uint64_t)va_arg(list, uint64_t));
			else if (s[i] == 'x')
				serial_print_hex(
					(uint64_t)va_arg(list, uint64_t));
			else {
				serial_print_string(
					"Wrong format using printf\n");
				return;
			}
		} else
			serial_print_char(s[i]);
	}
	unlock(serial_stdio_lock);
}
