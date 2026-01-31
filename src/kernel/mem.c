#include "mem.h"
#include "tty.h"
#include "multiboot.h"

uint32_t* mem_end_ptr = 0;
struct block_header* heap_start;

/*
 * Initialize physical memory manager
 * @end_of_kernel: Address marking the end of the kernel in memory. This address should be page aligned
 */

void mem_pmm_init(uint32_t end_of_kernel, uint32_t header_addr)
{
	struct multiboot_info* head = (struct multiboot_info*)(header_addr);
	int len = head->mmap_length;

	uint32_t* current_ptr = (uint32_t*)(end_of_kernel - 4);
	uint32_t current_page_addr = end_of_kernel;
	uint8_t is_first_alloc = 0;
	
	kprintf("%d\n", head->flags);

	for (int i = 0; i < len; i += sizeof(struct addr_range_descr_t)) {
		struct addr_range_descr_t* mem_map = (struct addr_range_descr_t*)(head->mmap_addr + i);

		uint64_t base_addr =
			(uint64_t)(mem_map->base_addr_high << 31) | mem_map->base_addr_low;
		uint64_t len_region = 
			(uint64_t)(mem_map->length_high << 31) | mem_map->length_low;
		
		/*kprintf("%d, addr: 0x%l, len: 0x%l, type: 0x%l\n"
				, i, base_addr, len_region, mem_map->type); */

		/* Skip reserved regions (type == 2 indicates reserved) */
		if (!(mem_map->type & 0x1)) {
			is_first_alloc = 1;
			continue;
		}

		/* Calculate the last address in the current region */
		uint64_t last_address_in_region = base_addr + len_region;
		last_address_in_region -= last_address_in_region % MEM_PAGE_SIZE;
		last_address_in_region -= MEM_PAGE_SIZE;
		
		/* Adjust the starting page */
		current_page_addr = (is_first_alloc) ? base_addr : end_of_kernel;
		
		/*kprintf("Current_addr: 0x%l, Last_reg: 0x%l\n"
				,(uint64_t)(current_page_addr), (uint64_t)(last_address_in_region));*/

		if (current_page_addr > last_address_in_region) {
			is_first_alloc = 1;
			continue;
		}
		
		/* Calculate the number of pages to allocate */
		uint64_t pages_to_allocate = last_address_in_region - current_page_addr;
		pages_to_allocate += MEM_PAGE_SIZE;
		pages_to_allocate /= MEM_PAGE_SIZE;
		
		/* Allocate pages in the region */
		for (uint64_t j = 0; j < pages_to_allocate; j++) {
			*current_ptr = current_page_addr;
			current_ptr = (uint64_t*)(current_page_addr);
			current_page_addr += MEM_PAGE_SIZE;
		}
		
		is_first_alloc = 1;
	}

	*current_ptr = 0x0;
	mem_end_ptr = current_ptr;
}

uint32_t mem_palloc(uint32_t* fr_ptr)
{
	if (*(fr_ptr) == 0x0) {
		kprintf("MEMORY ALLOCATION ERROR\n");
		return 0x0;
	
	} else {
		uint32_t* ptr_page = (uint32_t*)(*fr_ptr);
		uint32_t next_page = *ptr_page;
		uint32_t ret_page = *fr_ptr;
		*fr_ptr = next_page;
		return ret_page;
	}
}

uint32_t mem_free(uint32_t* page_addr)
{
	if (*(mem_end_ptr) == 0x0) {
		*mem_end_ptr = (uint32_t)(page_addr);
		mem_end_ptr = page_addr;
		*mem_end_ptr = 0x0;
		return (uint32_t)(mem_end_ptr);
	} else {
		kprintf("ERROR DURING FREEING\n");
	}

	return 0x0;
}

/*
 * Create Page Directory, at given address
 * @page_addr - Pointer to first address of page directory. Address must be 4096 bytes aligned
 */
void mem_pd_init(struct page_dir* page_addr)
{
	/* Check if address is aligned */
	if (((uint32_t)(page_addr) & 0xFFF)) {
		kprintf("ADDRESS IS NOT ALIGNED TO PAGE BOUNDARY\n");
		return;
	}
	
	/* Create 1024 Page Directory entries, where Present flag = 0 */
	for (int i = 0; i < 1024; i++)
		page_addr->entries[i].present = 0x0;
}

/*
 * Create Page Table at give address
 * @page_addr - Pointer to first address of page table. Address must be 4096 bytes aligned
 */
void mem_pt_init(struct page_table* page_addr)
{
	if (((uint32_t)(page_addr) & 0xFFF)) {
		kprintf("ADDRESS IS NOT ALIGNED TO PAGE BOUNDARY\n");
		return;
	}

	/* Create 1024 Page Table entries, where Present flag = 0 */
	for (int i = 0; i < 1024; i++)
		page_addr->entries[i].present = 0x0;
}

/*
 * Create Page Directory entry
 * @page_addr - Pointer to first address of page directory.
 * @index - Index of entry in page directory. Must be smaller than 1024
 * @flags - Flags for page directory
 * @phys_addr - Address of Page Table
 */
void mem_make_pde(struct page_dir* page_addr, uint16_t index, uint16_t flags, uint32_t phys_addr)
{
	if (index >= 1024) {
		kprintf("INDEX OUT OF RANGE\n");
		return;
	}

	page_addr->entries[index].present = 0x1;
	page_addr->entries[index].r_w = (flags & 0b10) ? 1 : 0;
	page_addr->entries[index].u_s = (flags & 0b100) ? 1 : 0;
	page_addr->entries[index].w_t = (flags & 0b1000) ? 1 : 0;
	page_addr->entries[index].c_d = (flags & 0b10000) ? 1 : 0;
	page_addr->entries[index].ps = (flags & 0b10000000) ? 1 : 0;
	page_addr->entries[index].global = (flags & 0b10000000) ? 1 : 0;
	page_addr->entries[index].addr = phys_addr;
}

/*
 * Create Page Table entry
 * @page_addr - Pointer to first address of page table
 * @index - Index of entry in page table. Must be smaller than 1024
 * @flags - Flags for page table
 * @phys_addr - Address of page frame
 */
void mem_make_pte(struct page_table* page_addr, uint16_t index, uint16_t flags, uint32_t phys_addr)
{
	if (index >= 1024) {
		kprintf("INDEX OUT OF RANGE\n");
		return;
	}

	page_addr->entries[index].present = 0x1;
	page_addr->entries[index].r_w = (flags & 0b10) ? 1 : 0;
	page_addr->entries[index].u_s = (flags & 0b100) ? 1 : 0;
	page_addr->entries[index].w_t = (flags & 0b1000) ? 1 : 0;
	page_addr->entries[index].c_d = (flags & 0b10000) ? 1 : 0;
	page_addr->entries[index].pat = (flags & 0b10000000) ? 1 : 0;
	page_addr->entries[index].global = (flags & 0b10000000) ? 1 : 0;
	page_addr->entries[index].addr = phys_addr;
}

void mem_en_paging(struct page_dir* page_addr)
{
	uint32_t kernel_start = 0x110000;
	uint16_t kernel_o = 0x08;

	asm("movl %0, %%eax" :: "Nd"(page_addr) : "memory");
	asm("movl %eax, %cr3");
	asm("movl %cr0, %eax");
	asm("orl $0x80000000, %eax");
	asm("movl %eax, %cr0");
	//asm("hlt");
}

void heap_init(uint32_t* ptr)
{
	heap_start = (struct block_header*)(ptr);
	heap_start->block_size = 0x0;
	heap_start->present = 0x0;
	heap_start->next_block = (struct block_header*)((uint32_t)(heap_start + 2));
	heap_start->next_block->block_size = (uint32_t)(HEAP_LIMIT - (uint32_t)(heap_start) - 4);
	heap_start->next_block->next_block = 0x0;
	heap_start->next_block->present = 0x0;
}

uint32_t* kmalloc(uint32_t block_size)
{
	struct block_header* list_start = heap_start->next_block;
	
	if(list_start->next_block == 0x0 && list_start->block_size < block_size) {
		kprintf("SIGMA");
		return 0;
	}

	while(list_start != 0x0)
	{
		if(list_start->block_size > block_size + 2) {
			struct block_header* next = (struct block_header*)((uint32_t)list_start + block_size + 2);
			next->present = 0x0;
			next->next_block = list_start->next_block;
			next->block_size = list_start->block_size - block_size - 2;
			list_start->block_size = block_size;
			list_start->present = 0x1;

			if(heap_start->next_block == list_start)
				heap_start->next_block = next;
			
			return (uint32_t*)(list_start);
		}

		list_start = list_start->next_block;
	}

	return 0x0;
}

void kfree(uint32_t* ptr)
{
	struct block_header* header = (struct block_header*)(ptr);
	struct block_header* list_start = (struct block_header*)(heap_start->next_block);
	struct block_header* prev_block = heap_start;
	
	while(list_start != NULL) {
		if (header < list_start) {
			prev_block->next_block = header;
			header->next_block = list_start;
			header->present = 0x0;
			
			kprintf("POINTER: %X\n", heap_start->next_block); 	
			if((uint32_t)(header) + header->block_size + 2 == (uint32_t)(header->next_block)) {
				header->block_size += header->next_block->block_size + 2;
				header->next_block = header->next_block->next_block;
			}
			
			if(heap_start == prev_block) {
				break;
			}

			if((uint32_t)(prev_block) + prev_block->block_size + 2 == (uint32_t)(header)) {
				prev_block->block_size += header->block_size + 2;
			        prev_block->next_block = header->next_block;
			}
				
			break;
		}
		
		prev_block = prev_block->next_block;
		list_start = list_start->next_block;
	}
}

