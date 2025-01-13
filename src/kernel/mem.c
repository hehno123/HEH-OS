#include "mem.h"
#include "tty.h"

void mem_PMM_init(uint32_t end_of_kernel)
{
	int* ptr = (short*)(0x8000);
	int len = *ptr;
	int* mem_struct_ptr = (int*)(0x8004);
	
	uint32_t* current_ptr = (uint64_t*)(end_of_kernel - 4);
	uint32_t current_page_addr = end_of_kernel;
	uint8_t is_first_alloc = 0;
	
	for(int i = 0; i < len; i++)
	{
		addr_range_descr_t* mem_map = (struct addr_range_descr_t*)(mem_struct_ptr);
		uint64_t base_addr = (uint64_t)(mem_map->base_addr_high << 31) | mem_map->base_addr_low;
		uint64_t len_region = (uint64_t)(mem_map->length_high << 31) | mem_map->length_low;
		
		kprintf("%d, addr: 0x%l, len: 0x%l, type: 0x%l\n", i, base_addr, len_region, mem_map->type);

		//if type == 2, this indicates that region is reserved
		if(!(mem_map->type & 0x1))
		{
			is_first_alloc = 1;
			mem_struct_ptr += 6;
			continue;
		}

		//check if end of the kernel, is in correct region
		uint64_t last_address_in_region = (uint64_t)(base_addr) + (uint64_t)(len_region);
		last_address_in_region = last_address_in_region - (last_address_in_region % MEM_PAGE_SIZE);
		last_address_in_region -= MEM_PAGE_SIZE;
		
		current_page_addr = (is_first_alloc) ? base_addr : end_of_kernel;
		
		kprintf("Current_addr: %X, Last_reg: %X\n", current_page_addr, last_address_in_region);
		if(current_page_addr > last_address_in_region)
		{
			mem_struct_ptr += 6;
			is_first_alloc = 1;
			continue;
		}

		uint64_t pages_to_allocate = (last_address_in_region - current_page_addr + MEM_PAGE_SIZE) / MEM_PAGE_SIZE;
		//kprintf
		for(uint64_t j = 0; j < pages_to_allocate; j++)
		{
			if(j < 2)
			{
				kprintf("Free_list_ptr %X, Page_addr %X\n", current_ptr, current_page_addr);
			}

			*current_ptr = current_page_addr; //store next free page addr in ptr
			current_ptr = (uint64_t*)(current_page_addr); //set ptr to next page
			current_page_addr += MEM_PAGE_SIZE; //set current page address to next page
		}
		
		kprintf("%l\n", current_ptr);
		is_first_alloc = 1;
		mem_struct_ptr += 6;
	}

	*current_ptr = NULL;
}


