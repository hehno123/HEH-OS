#include "../cpu/idt.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"
#include "../drivers/vga.h"

void ok()
{
	__asm__("hlt");
}

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_init();
 	
	set_idt_gate(0, ok, 0x8E);
	set_idt();

	/* Newline support is left as an exercise. */
	terminal_printstring("hejo\n");
	terminal_printstring("pozdrawiam Wojtka\n");

	//int x = 2 / 0;

	terminal_printstring("SIEMA");
}

