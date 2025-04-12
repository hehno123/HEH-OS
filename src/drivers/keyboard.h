#include "ports.h"

#ifndef __KEYBOARD_H
#define __KEYBOARD_H

#define KEY_DATA_PORT 0x60	/* keyboard data port */
#define KEY_STAT_PORT 0x64	/* keyboard status register */
#define KEY_COMM_PORT 0x64 	/* keyboard command register */

void keyboard_init();
extern char keyboard_buffer[256];

#endif
