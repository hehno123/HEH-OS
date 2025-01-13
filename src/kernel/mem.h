#ifndef __MEM_H
#define __MEM_H

#include <stdint.h>

#define MEM_PAGE_SIZE 4096
 
typedef struct {
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint64_t type;
} addr_range_descr_t;

void mem_PMM_init(uint32_t);

#endif
