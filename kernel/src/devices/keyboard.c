#include <types.h>
#include <keyboard.h>

#include <pic.h>
#include <io.h>
#include <keymap.h>
#include <libk/stdio.h>
#include <libk/math.h>

#define KEYBOARD_DATA_PORT	0x60
#define KEYBOARD_STATUS_PORT	0x64

bool is_pressed[128];

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
			printf(" ");

		} else if (keycode == KEY_BACKSPACE) {
			printf("\b \b");
		} else if (keycode == KEY_ENTER) {
			printf("%c", keymap[keycode]);
		} else {
			if (keymap[keycode] == ' ') return;
			if (is_pressed[KEY_LSHIFT] || is_pressed[KEY_RSHIFT])
				printf("%c", shift_keymap[keycode]);
			else
				printf("%c", keymap[keycode]);
		}
	} else {
		is_pressed[keycode - 128] = false;
	}
}
