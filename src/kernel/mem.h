#ifndef __MEM_H
#define __MEM_H

#include <stdint.h>

/* Memory Managment constants */
#define MEM_PAGE_SIZE 4096
#define MEM_MAP_LEN_ADDR 0x8000 /* regions in memory detected by BIOS */
#define MEM_MAP_ADDR 0x8004 	/* start of memory map data structure */

#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024

struct addr_range_descr_t {
	uint32_t base_addr_low;
	uint32_t base_addr_high;
	uint32_t length_low;
	uint32_t length_high;
	uint64_t type;
};

struct __attribute__((packed)) pd_entry {
    uint32_t present : 1;   /* Is Page present in physical memory */
    uint32_t r_w : 1;       /* Read-Only (0) or Read-Write (1) */
    uint32_t u_s : 1;       /* Supervisor mode (0) or User Mode (1) */
    uint32_t w_t : 1;       /* Write back cache (0) or Write through (0) */
    uint32_t c_d : 1;       /* Cache disabled (0) or cache enabled (1) */
    uint32_t accessed : 1;  /* Set by processor to 1 if page was accessed */
    uint32_t res : 1;       /* Reserved by intel */
    uint32_t ps : 1;        /* Page size 4KB(0), 4 MB(1) */
    uint32_t global : 1;    /* If global, don't evict from TLB */
    uint32_t available : 3;    /* Available */
    uint32_t addr : 20;     /* Most significant 20 bits of Physical address */
};

struct __attribute__((packed)) pt_entry {
    uint32_t present : 1;   /* Is Page present in physical memory */
    uint32_t r_w : 1;       /* Read-Only (0) or Read-Write (1) */
    uint32_t u_s : 1;       /* Supervisor mode (0) or User Mode (1) */
    uint32_t w_t : 1;       /* Write back cache (0) or Write through (0) */
    uint32_t c_d : 1;       /* Cache disabled (0) or cache enabled (1) */
    uint32_t accessed : 1;  /* Set by processor to 1 if page was accessed */
    uint32_t dirty : 1;     /* Set by MMU if write, cleare by software */
    uint32_t pat: 1;        /* Page attribute table */
    uint32_t global : 1;    /* If global, don't evict from TLB */
    uint32_t available : 3;    /* Available */
    uint32_t addr : 20;     /* Most significant 20 bits of Physical address */
};

struct page_dir {
	struct pd_entry entries[PAGE_DIRECTORY_ENTRIES];
};

struct page_table {
	struct pt_entry entries[PAGE_TABLE_ENTRIES];
};

void mem_pmm_init(uint32_t);
uint32_t mem_palloc(uint32_t*);
uint32_t mem_free(uint32_t*);
void mem_pd_init(struct page_dir*);

#endif
