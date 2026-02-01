#include "keyboard.h"
#include "../kernel/tty.h"

/*
 * Initalize PS/2 controller
 */

//char keyboard_buffer[256];

int kbd_wrt_ptr = 0;
int kbd_rd_ptr = 0;
uint8_t keyboard_buffer[256];

void keyboard_init()
{
    	/* Disable devices connected to PS/2 */
	outb(0xAD, KEY_COMM_PORT);
	io_wait();
	outb(0xA7, KEY_COMM_PORT);
	io_wait();

	/* Flush the output buffer */
	while (inb(KEY_STAT_PORT) & 0x01) {
        	(void)inb(KEY_DATA_PORT);
    	}

    	io_wait();

    	/* Read the current configuration byte */
    	outb(0x20, KEY_COMM_PORT);
    	while (!(inb(KEY_STAT_PORT) & 0x01));
    	uint8_t config_byte = inb(KEY_DATA_PORT);

    	/* Modify PS/2 configuration bytes and enable interrupts */
    	config_byte &= ~0x20;   
    	config_byte |= 0x01;    
    	outb(0x60, KEY_COMM_PORT);
    	
	/* Write configuration bytes */
    	while(inb(KEY_STAT_PORT) & 0x02);
    	outb(config_byte, KEY_DATA_PORT);
	
    	outb(0xAE, KEY_COMM_PORT); 
}

uint8_t code_to_ascii(uint8_t code)
{
	if (code == 57) {
		kprintf("SPACJA WYKRYTA!!!!\n");
		return 32;
	}
	
	if (code >= 0x81) {
		return 0;
	}

	return kbd_US[code];
}

uint8_t read_keyboard()
{
	if(kbd_wrt_ptr != kbd_rd_ptr) {
		if(kbd_rd_ptr > 255) {
			kbd_rd_ptr = 0;
		}

		uint8_t val = keyboard_buffer[kbd_rd_ptr];
		kbd_rd_ptr++;
		return val;
	}

	return 0;
}


