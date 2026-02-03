#include "../cpu/idt.h"
#include "../cpu/pic.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "tty.h"
#include "../drivers/vga.h"
#include "../drivers/ports.h"
#include "../drivers/keyboard.h"
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

	initialize_fs();	
	struct inode_t root_inode;
	inode_read(2, &root_inode);

	uint8_t* root_name = (uint8_t*)(kmalloc(1024));
	kprintf("OKI: %X\n", root_inode.block_ptr[0]);
	read_ata(2048 + (root_inode.block_ptr[0] * 2), 2, root_name);
	
	uint32_t offset = 0;
	int count = 0;

	while (offset < 1024 && count < 6) {
    		struct dir_entry_t* entry = (struct dir_entry_t*)(root_name + offset);

    		char name[256];
    		memcpy(name, entry->name, entry->name_len);
    		name[entry->name_len] = 0;

    		kprintf("Inode %d: ", entry->inode);
		kprintf(name);
		kprintf("\n");
		
    		offset += entry->size;
		count++;
	}

	install_idt();
	keyboard_init();
	remap_pic(0x20, 0x28);
	
	kprintf("SIZEOF: %d\n", sizeof(struct inode_t));
	__asm__ volatile("sti");
	__asm__ volatile("int $0");
	
	/*kprintf("%c", 32);

	while(1) {
		kprintf("%c", read_keyboard());
	}*/

	while(1);
			
}

