#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <types.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CMD_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

void keyboard_handler(void);
void init_keyboard(void);

#endif
