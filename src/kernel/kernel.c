#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"
#include "../drivers/vga.h"
#include "../drivers/ports.h"

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_init();
	remap_pic(0x20, 0x28);
	keyboard_init();
	install_idt();

	__asm__ volatile("sti");
	
	/* Newline support is left as an exercise. */
	terminal_printstring("hejo\n");
	terminal_printstring("pozdrawiam Wojtka\n");
	//int x = 2 / 0;

	terminal_printstring("SIEMA");
}

