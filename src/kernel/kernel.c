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
	//terminal_init();
	//kprintf("HEJ TO JA");
	
	void (*mem_pmm_init_ptr)(uint32_t) = (&mem_pmm_init - 0x100000);
	uint32_t (*mem_palloc_ptr)(uint32_t*) = (&mem_palloc - 0x100000);
	void (*mem_pd_init_ptr)(struct page_dir*) = (&mem_pd_init - 0x100000);
	void (*mem_pt_init_ptr)(struct page_table*) = (&mem_pt_init - 0x100000);
	void (*mem_make_pde_ptr)(struct page_dir*, uint16_t, uint16_t, uint32_t) = (&mem_make_pde - 0x100000);
	void (*mem_make_pte_ptr)(struct page_table*, uint16_t, uint16_t, uint32_t) = (&mem_make_pte - 0x100000);
	void (*mem_en_paging_ptr)(struct page_dir*) = (&mem_en_paging - 0x100000);

	struct page_dir* kernel_page_dir = (struct page_dir*)(mem_palloc_ptr((uint32_t*)(&_end - 0x40000 - 1)));
		struct page_table* kernel_page_table = (struct page_table*)(mem_palloc_ptr((uint32_t*)(&_end - 0x40000 - 1)));
	
	mem_pmm_init_ptr((uint32_t)(&_end - 0x100000 - 1));
	mem_pd_init_ptr(kernel_page_dir);
	mem_pt_init_ptr(kernel_page_table);

	mem_make_pde_ptr(kernel_page_dir, 0x0, PAGE_RW, (uint32_t)((uint32_t)(kernel_page_table) >> 12));
	
	for (int i = 0; i < 1024; i++)
		mem_make_pte_ptr(kernel_page_table, i, PAGE_RW, (uint32_t)((uint32_t)((MEM_PAGE_SIZE * i) >> 12)));

	mem_en_paging(kernel_page_dir);

	/* initalize PMM manager */
	/*mem_pmm_init((uint32_t)(&_end));
	struct page_dir* kernel_page_dir = (struct page_dir*)(mem_palloc((uint32_t*)(&_end - 1)));
	struct page_table* kernel_page_table = (struct page_table*)(mem_palloc((uint32_t*)(&_end - 1)));
	mem_pd_init(kernel_page_dir);
	mem_pt_init(kernel_page_table);
	*/

	/* Initalize Page Directory and Page table entries */
	/* uint16_t flags = PAGE_RW;
	mem_make_pde(kernel_page_dir, 0x0, flags, (uint32_t)((uint32_t)(kernel_page_table) >> 12));
	flags = 0x0;	
	flags = PAGE_RW;

	for (int i = 0; i < 1024; i++)
		mem_make_pte(kernel_page_table, i, flags, ((uint32_t)((MEM_PAGE_SIZE * i) >> 12)));
		
	mem_en_paging(kernel_page_dir);
	terminal_change_memory(0x1b8000);

	keyboard_init();
	remap_pic(0x20, 0x28);
	install_idt();
	kprintf("ASDFASDFASDF");

	__asm__ volatile("sti");*/
}

