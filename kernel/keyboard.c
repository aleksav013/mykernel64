#include <stdint.h>
#include <pic.h>
#include <io.h>
#include <graphics.h>
#include <debug.h>

#define KEYBOARD_DATA_PORT	0x60
#define KEYBOARD_STATUS_PORT	0x64

uint8_t i = 0;

void keyboard_handler(void)
{
	uint8_t status = inb(KEYBOARD_STATUS_PORT);
	if (!(status & 0x1)) {
		return;
	}
	uint8_t keycode = inb(KEYBOARD_DATA_PORT);
	fb_draw_character(fb, keycode, 0, 0, RED, BLACK);
}
