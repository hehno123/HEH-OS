#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"
#include "../drivers/vga.h"
#include "../drivers/ports.h"
#include "mem.h"

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_init();
	keyboard_init();
	remap_pic(0x20, 0x28);
	install_idt();

	__asm__ volatile("sti");
	   	
	int* ptr = (short*)(0x8000);
	int len = *ptr;
	
	int* mem_struct_ptr = (int*)(0x8004);
	
	for(int i = 0; i < len; i++)
	{
		addr_range_descr_t* mem_desc = (struct addr_range_descr_t*)(mem_struct_ptr);
		uint64_t base_addr = ((uint64_t)(mem_desc->base_addr_high) << 31) | mem_desc->base_addr_low;
		uint64_t len = ((uint64_t)(mem_desc->length_high) << 31) | mem_desc->length_low;

	        kprintf("%d, addr: 0x%l, len: 0x%l, type: 0x%l\n", i, base_addr, len, mem_desc->type);
		mem_struct_ptr += 6;
	}
	
}

