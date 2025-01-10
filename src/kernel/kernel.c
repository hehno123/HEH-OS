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
	keyboard_init();
	remap_pic(0x20, 0x28);
	install_idt();

	__asm__ volatile("sti");
		
	/* Newline support is left as an exercise. */
	terminal_printstring("hejo\nsiema to ja hehno 123\nasdf ok to znowu ja\n");
	kprintf("%d", 123);	
}

