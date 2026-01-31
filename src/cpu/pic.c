#include "pic.h"
#include "../drivers/ports.h"

void remap_pic(int irq1, int irq2)
{
    // Start ICW1 to initialize PIC
    outb(0x11, PIC1_COMMAND);
    io_wait();
    outb(0x11, PIC2_COMMAND);
    io_wait();

    // ICW2: Remap IRQ vectors
    outb(irq1, PIC1_DATA);  // Set PIC1 interrupt vector base
    io_wait();
    outb(irq2, PIC2_DATA);  // Set PIC2 interrupt vector base
    io_wait();

    // ICW3: Set connection between slave and master
    outb(0x4, PIC1_DATA);   // IRQ2 is connected to the slave (PIC2)
    io_wait();
    outb(0x2, PIC2_DATA);   // Slave PIC is at IRQ2 of master PIC
    io_wait();

    // ICW4: Set 8086 mode for both PICs
    outb(0x1, PIC1_DATA);   // Set 8086 mode for PIC1
    io_wait();
    outb(0x1, PIC2_DATA);   // Set 8086 mode for PIC2
    io_wait();

    // Mask all IRQs initially (disable all interrupts)
    outb(0xFF, PIC1_DATA);  // Mask all IRQs on PIC1
    io_wait();
    outb(0xFF, PIC2_DATA);  // Mask all IRQs on PIC2
    io_wait();

    // Unmask IRQ0 (timer) on PIC1 and IRQ1 (keyboard) on PIC1
    // IRQ0 is the timer, IRQ1 is the keyboard
    outb(0xFE, PIC1_DATA);  // Unmask IRQ0 (timer))
    io_wait();
    outb(0xFD, PIC1_DATA);  // Unmask IRQ1 (keyboard)
    io_wait();

    // Optionally, verify the current IRQ mask status:
    uint8_t mask1 = inb(PIC1_DATA);
    uint8_t mask2 = inb(PIC2_DATA);
    //printf("PIC1 IRQ mask: 0x%x\n", mask1);
    //printf("PIC2 IRQ mask: 0x%x\n", mask2);
}

