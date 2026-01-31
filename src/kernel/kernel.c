#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"
#include "../drivers/vga.h"
#include "../drivers/ports.h"
#include "mem.h"
#include "../fs/fs.h"

extern uint32_t _end;
#define VIRT_BASE 0xC0000000

void command_control()
{
}

void kernel_main(uint32_t multiboot_struct) 
{	
	void (*mem_pmm_init_ptr)(uint32_t, uint32_t) = (&mem_pmm_init - VIRT_BASE);
	uint32_t (*mem_palloc_ptr)(uint32_t*) = (&mem_palloc - VIRT_BASE);
	void (*mem_pd_init_ptr)(struct page_dir*) = (&mem_pd_init - VIRT_BASE);
	void (*mem_pt_init_ptr)(struct page_table*) = (&mem_pt_init - VIRT_BASE);
	void (*mem_make_pde_ptr)(struct page_dir*, uint16_t, uint16_t, uint32_t) = (&mem_make_pde - VIRT_BASE);
	void (*mem_make_pte_ptr)(struct page_table*, uint16_t, uint16_t, uint32_t) = (&mem_make_pte - VIRT_BASE);
	void (*mem_en_paging_ptr)(struct page_dir*) = (&mem_en_paging - VIRT_BASE);
	
	mem_pmm_init_ptr((uint32_t)(&_end) - VIRT_BASE, multiboot_struct);
	struct page_dir* kernel_page_dir = (struct page_dir*)(mem_palloc_ptr((uint32_t*)(&_end - (VIRT_BASE / 4) - 1)));
	struct page_table* kernel_page_table = (struct page_table*)(mem_palloc_ptr((uint32_t*)(&_end - (VIRT_BASE / 4) - 1)));
	struct page_table* identity_page_table = (struct page_table*)(mem_palloc_ptr((uint32_t*)(&_end - (VIRT_BASE / 4) - 1)));
	
	mem_pd_init_ptr((uint32_t)(kernel_page_dir));
	mem_pt_init_ptr((uint32_t)(kernel_page_table));
	mem_pt_init_ptr((uint32_t)(identity_page_table));

	mem_make_pde_ptr(kernel_page_dir, VIRT_BASE >> 22, PAGE_RW, (uint32_t)((uint32_t)(kernel_page_table) >> 12));
	mem_make_pde_ptr(kernel_page_dir, 0x0, PAGE_RW, (uint32_t)((uint32_t)(identity_page_table) >> 12));

	for (int i = 0; i < 1024; i++) {
		mem_make_pte_ptr(kernel_page_table, i, PAGE_RW, (uint32_t)((uint32_t)((MEM_PAGE_SIZE * i) >> 12)));
		mem_make_pte_ptr(identity_page_table, i, PAGE_RW, (uint32_t)((uint32_t)(MEM_PAGE_SIZE * i) >> 12));
	}

	mem_en_paging_ptr((uint32_t)(kernel_page_dir));
	terminal_init();
	
	heap_init((uint32_t*)(mem_palloc((uint32_t*)(&_end - 1))));
	uint16_t* super_block = (uint16_t *)(kmalloc(1024));
	read_ata(2050, 2, super_block);
	
	struct fs_superblock* block = (struct fs_superblock*)(super_block);

	kprintf("SUPER_BLOCK addr: %X\n", (uint32_t)(super_block));	
	kprintf("EXT VERSION: %X\n", block->rev_level);

	install_idt();
	keyboard_init();
	remap_pic(0x20, 0x28);

	__asm__ volatile("sti");
	__asm__ volatile("int $0");
	
	while(1);	
}

