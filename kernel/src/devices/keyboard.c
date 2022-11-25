#include <types.h>
#include <keyboard.h>
#include <pic.h>
#include <io.h>
#include <keymap.h>
#include <libk/stdio.h>
#include <libk/math.h>
#include <libk/string.h>

bool is_pressed[128];

#include <stdbuff.h>
#include <heap.h>
#define BUFFER_SIZE 10000
stdbuff* keyboard_buffer;

void keyboard_handler()
{
	if (keyboard_buffer == NULL) {
		keyboard_buffer = init_buff(BUFFER_SIZE);
		memset(keyboard_buffer->data, ' ', BUFFER_SIZE);
	}

	uint8_t status = inb(KEYBOARD_STATUS_PORT);
	if (!(status & 0x1)) {
		return;
	}

	uint8_t keycode = inb(KEYBOARD_DATA_PORT);

	if (keycode < keymap_len) {
		is_pressed[keycode] = true;
		if (keycode == KEY_SPACE) {
			write_buff(keyboard_buffer, " ", 1);
		} else if (keycode == KEY_BACKSPACE) {
			if (curr_x != 0)
				write_buff(keyboard_buffer, "\b \b", 3);
		} else if (keycode == KEY_ENTER) {
			char* output = kalloc(sizeof(char) + 1);
			output[0] = keymap[keycode];
			write_buff(keyboard_buffer, output, 1);
			kfree(output);
		} else {
			char* output = kalloc(sizeof(char) + 1);
			if (keymap[keycode] == ' ') return;
			if (is_pressed[KEY_LSHIFT] || is_pressed[KEY_RSHIFT])
				output[0] = shift_keymap[keycode];
			else
				output[0] = keymap[keycode];
			write_buff(keyboard_buffer, output, 1);
			kfree(output);
		}
	} else {
		is_pressed[keycode - 128] = false;
	}

	uint32_t len = (uint32_t)(keyboard_buffer->head - keyboard_buffer->tail);
	char* print_buff = kalloc(len + 1);
	read_buff(keyboard_buffer, print_buff, len);
	printf("%s", print_buff);
	kfree(print_buff);
}
