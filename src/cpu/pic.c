#include "../drivers/ports.h"
#include "pic.h"

void remap_pic(int irq1, int irq2)
{
	uint8_t a1, a2;

	a1 = inb(PIC1_DATA);
	a2 = inb(PIC2_DATA);
	
	/* start ICW1, to initialise PIC */
	outb(0x11, PIC1_COMMAND);
	io_wait();
	outb(0x11, PIC2_COMMAND);
	io_wait();

	/* ICW2 */
	outb(irq1, PIC1_DATA);
	io_wait();
	outb(irq2, PIC2_DATA);
	io_wait();

	/* ICW3, set connection between slave and master */
	outb(0x4, PIC1_DATA);
	io_wait();
	outb(0x2, PIC2_DATA);
	io_wait();

	/* ICW4 */
	outb(0x1, PIC1_DATA);
	io_wait();
	outb(0x1, PIC2_DATA);
	io_wait();

	outb(a1, PIC1_DATA);
	outb(a2, PIC2_DATA);
}


