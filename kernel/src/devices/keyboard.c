#include <types.h>

#include <pic.h>
#include <io.h>
#include <graphics.h>
#include <debug.h>
#include <keymap.h>
#include <libk/string.h>

#define KEYBOARD_DATA_PORT	0x60
#define KEYBOARD_STATUS_PORT	0x64

static uint32_t x;
static uint32_t y;
bool is_pressed[128];

void print_char(char c)
{

	if (c == '\n') {
		x = 0;
		y++;
		return;
	}
	if (x * 8 >= fb.width) {
		x = 0;
		y++;
	}
	if (y * 16 >= fb.height) {
		x = 0;
		y = 0;
	}
	fb_draw_character(fb, c, x * 8, y * 16, WHITE, BLACK);
	x++;
}

void print_string(char* s)
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

void keyboard_handler(void)
{
	uint8_t status = inb(KEYBOARD_STATUS_PORT);
	if (!(status & 0x1)) {
		return;
	}
	uint8_t keycode = inb(KEYBOARD_DATA_PORT);
	if (keycode < keymap_len) {
		is_pressed[keycode] = true;
		if (keycode == KEY_SPACE) {
			print_char(' ');

		} else if (keycode == KEY_BACKSPACE) {
			if (!x) return;
			x--;
			print_char(' ');
			x--;
		} else if (keycode == KEY_ENTER) {
			print_char(keymap[keycode]);
		} else {
			if (keymap[keycode] == ' ') return;
			if (is_pressed[KEY_LSHIFT] || is_pressed[KEY_RSHIFT])
				print_char(shift_keymap[keycode]);
			else
				print_char(keymap[keycode]);
		}
	} else {
		is_pressed[keycode - 128] = false;
	}
}
