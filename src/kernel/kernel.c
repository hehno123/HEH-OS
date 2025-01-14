#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"
#include "../drivers/vga.h"
#include "../drivers/ports.h"
#include "mem.h"

extern uint32_t _end;

void kernel_main(void) 
{
	/* Initialize terminal interface */
	terminal_init();
	keyboard_init();
	remap_pic(0x20, 0x28);
	install_idt();

	__asm__ volatile("sti");
	   		
	/* initalize PMM manager */
	mem_pmm_init((uint32_t)(&_end));
	kprintf("FIRST FREE PAGE: %X\n", (int32_t)(*(&_end - 1)));
	kprintf("SECOND FREE PAGE %X\n", (int32_t)(*(&_end + MEM_PAGE_SIZE)));
	kprintf("PAGE AT 0x100000 %X\n", (int32_t)(*(uint32_t*)(0x100000)));
	kprintf("PAGE AT 0x9E000 %X\n", (int32_t)(*(uint32_t*)(0x9E000)));
	
	for (int i = 0; i < 5; i++)	
		kprintf("%d PAGE ALLOCATED: %X\n", i + 1, mem_palloc((uint32_t*)(&_end - 1)));
	
	//for (int i = 0; i < 5; i++)
	kprintf("PAGE FREED: %X\n", mem_free((uint32_t*)(0x14000)));
}

