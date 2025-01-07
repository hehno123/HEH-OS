#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"
#include "../drivers/vga.h"

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_init();
	remap_pic(0x20, 0x28);
	install_idt();
		
	/* Newline support is left as an exercise. */
	terminal_printstring("hejo\n");
	terminal_printstring("pozdrawiam Wojtka\n");
	terminal_printstring("SIEMA");
}

