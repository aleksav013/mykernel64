#include <types.h>
#include <libk/stdio.h>

#include <stdarg.h>
#include <libk/string.h>
#include <graphics.h>

int32_t x;
int32_t y;

inline void print_char(char c)
{
	if (c == '\n') {
		x = 0;
		y++;
		return;
	}
	if (c == '\b') {
		if (x != 0) {
			x--;
		}
		return;
	}
	if (x * 8 >= (int32_t)main_fb.width) {
		x = 0;
		y++;
	}
	if (y * 16 >= (int32_t)main_fb.height) {
		x = 0;
		y = 0;
	}
	fb_draw_character(main_fb, c, x * 8, y * 16, WHITE, BLACK);
	x++;
}

inline void print_string(const char* s)
{
	for (size_t i = 0; i < strlen(s); i++) {
		print_char(s[i]);
	}
}

inline void print_int(uint64_t num)
{
	char a[100];
	itos(num, a);
	print_string(a);
}

inline void print_hex(uint64_t num)
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
