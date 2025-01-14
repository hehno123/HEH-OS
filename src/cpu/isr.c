#include "../kernel/tty.h"
#include "../drivers/ports.h"
#include "pic.h"
#include "idt.h"

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Clock",
    "Keyboard"
};

void isr_handler(register_t registers)
{
	if (registers.int_num >= 32) {
		if (registers.err_num == 1) {	
			
			if (inb(0x64) & 0x01) {
				io_wait();
				uint8_t oki = inb(0x60);
				io_wait();
				
				if(oki == 0x04)
					terminal_printstring("3");
			}
		}

		if (registers.err_num >= 8) {
			outb(0x20, PIC2_COMMAND);
		}

		outb(0x20, PIC1_COMMAND);
		io_wait();
	}
	
}
