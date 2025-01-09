#include "keyboard.h"

void keyboard_init()
{
    // Disable devices connected to PS/2
    outb(0xAD, 0x64); // Disable first PS/2 port
    io_wait();
    outb(0xA7, 0x64); // Disable second PS/2 port
    io_wait();

    // Flush the output buffer (0x60) if status indicates it is full
    while (inb(0x64) & 0x01) {
        (void)inb(0x60); // Read and discard the data
    }
    io_wait();

    // Read the current configuration byte
    outb(0x20, 0x64);       // Request configuration byte
    while(!(inb(0x64) & 0x01));
    uint8_t config_byte = inb(0x60);

    // Modify the configuration byte to disable translation and enable interrupts
    config_byte &= ~0x20;   // Clear translation bit
    config_byte |= 0x01;    // Enable interrupts for the first PS/2 port
    outb(0x60, 0x64);       // Write configuration byte command
    
    while(inb(0x64) & 0x02);
    outb(config_byte, 0x60);// Write modified configuration byte
	
    outb(0xAE, 0x64); 
}

