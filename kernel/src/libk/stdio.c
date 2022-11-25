#include <types.h>
#include <libk/stdio.h>
#include <libk/serial_stdio.h>
#include <stdarg.h>
#include <libk/string.h>
#include <graphics.h>
#include <serial.h>

uint32_t curr_x;
uint32_t curr_y;

void print_char(char c)
{
	if (curr_x * 8 >= main_fb.width) {
		curr_x = 0;
		curr_y++;
	}
	if (curr_y * 16 >= main_fb.height) {
		curr_x = 0;
		curr_y = 0;
	}
	if (c == '\n') {
		write_serial('\n');
		curr_x = 0;
		curr_y++;
		return;
	}
	if (c == '\b') {
		if (curr_x != 0) {
			curr_x--;
		}
		return;
	}
	fb_draw_character(main_fb, c, (int32_t)curr_x * 8, (int32_t)curr_y * 16, WHITE, BLACK);
	curr_x++;

	write_serial(c);
}

void print_string(const char* s)
{
	for (size_t i = 0; i < strlen(s); i++) {
		print_char(s[i]);
	}
}

void print_int(uint64_t num)
{
	char a[100];
	itos(num, a);
	print_string(a);
}

void print_hex(uint64_t num)
{
	char a[100];
	itoh(num, a);
	print_string(a);
}

void printf(const char *s, ...)
{
	size_t count = 0;
	for(size_t i = 0; i < strlen(s); i++) if(s[i] == '%') count++;

	va_list list;
	va_start(list, s);

	for(size_t i = 0; i < strlen(s); i++)
	{
		if(s[i] == '%')
		{
			i++;
			if(s[i] == 'c') print_char((char)va_arg(list, uint32_t));
			else if(s[i] == 's') print_string(va_arg(list, char*));
			else if(s[i] == 'd') print_int((uint64_t)va_arg(list, uint64_t));
			else if(s[i] == 'x') print_hex((uint64_t)va_arg(list, uint64_t));
			else
			{
				print_string("Wrong format using printf\n");
				return;
			}
		}
		else print_char(s[i]);
	}

	va_end(list);
}
