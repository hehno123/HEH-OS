#include "mem.h"
#include "tty.h"

uint32_t* mem_end_ptr = 0;

/*
 * Initialize physical memory manager
 * @end_of_kernel: Address marking the end of the kernel in memory. This address should be page aligned
 */

void mem_pmm_init(uint32_t end_of_kernel)
{
	int* ptr = (short*)(MEM_MAP_LEN_ADDR);
	int len = *ptr;
	addr_range_descr_t* mem_map = (struct addr_range_descr_t*)(MEM_MAP_ADDR);
	
	uint32_t* current_ptr = (uint32_t*)(end_of_kernel - 4);
	uint32_t current_page_addr = end_of_kernel;
	uint8_t is_first_alloc = 0;
	
	for (int i = 0; i < len; i++) {
		uint64_t base_addr =
			(uint64_t)(mem_map->base_addr_high << 31) | mem_map->base_addr_low;
		uint64_t len_region = 
			(uint64_t)(mem_map->length_high << 31) | mem_map->length_low;
		
		kprintf("%d, addr: 0x%l, len: 0x%l, type: 0x%l\n"
				, i, base_addr, len_region, mem_map->type);

		/* Skip reserved regions (type == 2 indicates reserved) */
		if (!(mem_map->type & 0x1)) {
			is_first_alloc = 1;
			mem_map++;
			continue;
		}

		/* Calculate the last address in the current region */
		uint64_t last_address_in_region = base_addr + len_region;
		last_address_in_region -= last_address_in_region % MEM_PAGE_SIZE;
		last_address_in_region -= MEM_PAGE_SIZE;
		
		/* Adjust the starting page */
		current_page_addr = (is_first_alloc) ? base_addr : end_of_kernel;
		
		kprintf("Current_addr: 0x%l, Last_reg: 0x%l\n"
				,(uint64_t)(current_page_addr), (uint64_t)(last_address_in_region));

		if (current_page_addr > last_address_in_region) {
			mem_map++;
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
		mem_map++;
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
