#include "ports.h"

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEY_DATA_PORT 0x60	/* keyboard data port */
#define KEY_STAT_PORT 0x64	/* keyboard status register */
#define KEY_COMM_PORT 0x64 	/* keyboard command register */

void keyboard_init();
uint8_t read_keyboard();
uint8_t code_to_ascii(uint8_t);

extern int kbd_wrt_ptr;
extern int kbd_rd_ptr;
extern uint8_t keyboard_buffer[256];

static uint8_t kbd_US [128] = {
    27, 49, 50, 51, 52, 53, 54, 55, 56, 57, 48, /* Numbers */
    45, 61, 8, 9, 113, 119, 101, 114, 116, 121, 117, 105, 111, 112, 	91, 93, 10, 129, 97, 115, 100, 102, 103, 104, 106, 107, 108, 59, 	39, 96, 130, 92, 122, 120, 99, 118, 98, 110, 109, 44, 46, 47

};

#endif
