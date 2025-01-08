#include "keyboard.h"

void keyboard_init()
{
    // Disable devices connected to PS/2
    outb(0xAD, 0x64); // Disable first PS/2 port
    outb(0xA7, 0x64); // Disable second PS/2 port

    // Flush the output buffer (0x60) if status indicates it is full
    while (inb(0x64) & 0x01) {
        (void)inb(0x60); // Read and discard the data
    }

    // Read the current configuration byte
    outb(0x20, 0x64);       // Request configuration byte
    uint8_t config_byte = inb(0x60);

    // Modify the configuration byte to disable translation and enable interrupts
    config_byte &= ~0x20;   // Clear translation bit
    config_byte |= 0x01;    // Enable interrupts for the first PS/2 port
    outb(0x60, 0x64);       // Write configuration byte command
    outb(config_byte, 0x60);// Write modified configuration byte

    // Perform controller self-test
    outb(0xAA, 0x64); // Send self-test command
    while (!(inb(0x64) & 0x01)); // Wait for output buffer to be full
    if (inb(0x60) != 0x55) {
        terminal_printstring("Self-test failed");
        return;
    }
    terminal_printstring("Self-test passed");

    // Test first PS/2 port
    outb(0xAB, 0x64); // Send test command
    while (!(inb(0x64) & 0x01)); // Wait for output buffer to be full
    if (inb(0x60) != 0x00) {
        terminal_printstring("First port test failed");
        return;
    }
    terminal_printstring("First port test passed");

    // Re-enable the first PS/2 port
    outb(0xAE, 0x64);

    // Send reset command to keyboard
    outb(0xFF, 0x60); // Send reset command to keyboard
    while (!(inb(0x64) & 0x01)); // Wait for response
    if (inb(0x60) != 0xFA) {
        terminal_printstring("Keyboard reset failed");
        return;
    }
    terminal_printstring("Keyboard reset successful");

    // Enable interrupts for the first PS/2 port
    config_byte = inb(0x60);   // Read current configuration byte again
    config_byte |= 0x01;       // Enable interrupt for first PS/2 port
    outb(0x60, 0x64);          // Write configuration byte command
    outb(config_byte, 0x60);   // Write modified configuration byte
}

