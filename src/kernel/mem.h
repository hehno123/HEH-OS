#ifndef __MEM_H
#define __MEM_H

#include <stdint.h>

typedef struct {
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint64_t type;
} addr_range_descr_t;

#endif
