#include "ports.h"
#include "../kernel/tty.h"

#define ATA_PORT_IO_0 0x1F0
#define ATA_PORT_IO_1 0x1F1
#define ATA_PORT_IO_2 0x1F2
#define ATA_PORT_IO_3 0x1F3
#define ATA_PORT_IO_4 0x1F4
#define ATA_PORT_IO_5 0x1F5
#define ATA_PORT_IO_6 0x1F6
#define ATA_PORT_IO_7 0x1F7

#define ATA_CONTROL_BLOCK_0 0x3F6
#define ATA_CONTROL_BLOCK_1 0x3F6

void polling()
{
	while(1) {
		uint8_t status = inb(ATA_PORT_IO_7);

		if (status & 0x80) continue;

		if (status & 0x08) break;
	}
}

void read_ata(uint32_t lba_address, uint8_t count, uint16_t* buffer)
{
	outb(0xE0 | ((lba_address >> 24) & 0x0F), ATA_PORT_IO_6);
	outb(0x00, ATA_PORT_IO_1);
	outb(count, ATA_PORT_IO_2);
	outb((uint8_t)(lba_address), ATA_PORT_IO_3);
	outb((uint8_t)(lba_address >> 8), ATA_PORT_IO_4);
	outb((uint8_t)(lba_address >> 16), ATA_PORT_IO_5);
	outb(0x20, ATA_PORT_IO_7);
	
	for(int i = 0; i < count; i++) {
		polling();
		
		char ok[513];
		ok[512] = '\0';
		
		for(int j = 0; j < 256; j++) {
			uint16_t data = inw(0x1F0);
			ok[j * 2] = (char)(data);
			ok[(j * 2) + 1] = (char)(data >> 8);
		}

		terminal_printstring(ok);
	}
}

			
		

