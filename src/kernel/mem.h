#ifndef __MEM_H
#define __MEM_H

#include <stdint.h>

#define MEM_PAGE_SIZE 4096

#define MEM_MAP_LEN_ADDR 0x8000 /* regions in memory detected by BIOS */
#define MEM_MAP_ADDR 0x8004 	/* start of memory map data structure */

typedef struct {
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint64_t type;
} addr_range_descr_t;

void mem_pmm_init(uint32_t);
uint32_t mem_palloc(uint32_t*);
uint32_t mem_free(uint32_t*);

#endif
