#ifndef __MEM_H
#define __MEM_H

#include <stdint.h>

/* Memory Managment constants */
#define MEM_PAGE_SIZE 4096
#define MEM_MAP_LEN_ADDR 0x8000 /* regions in memory detected by BIOS */
#define MEM_MAP_ADDR 0x8004 	/* start of memory map data structure */

#define PAGE_DIRECTORY_ENTRIES 1024
#define PAGE_TABLE_ENTRIES 1024

/* Define Page Flags */
#define PAGE_RW 0b10
#define PAGE_RO 0x0

#define PAGE_SUP 0x0
#define PAGE_USR 0b100

#define PAGE_WB 0x0
#define PAGE_WT 0b1000

#define PAGE_CD 0x0
#define PAGE_CE 0b10000

#define PAGE_KB 0x0
#define PAGE_MB 0b10000000

#define HEAP_LIMIT 0x800000

struct addr_range_descr_t {
	uint32_t size;
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
    uint32_t w_t : 1;       /* Write back cache (0) or Write through (1) */
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

struct __attribute__((packed)) block_header {
	uint32_t block_size : 29;
	uint32_t present : 3;
	struct block_header* next_block;
};

void mem_pmm_init(uint32_t, uint32_t);
uint32_t mem_palloc(uint32_t*);
uint32_t mem_free(uint32_t*);
void mem_pd_init(struct page_dir*);
void mem_pt_init(struct page_table*);
void mem_make_pde(struct page_dir*, uint16_t, uint16_t, uint32_t);
void mem_make_pte(struct page_table*, uint16_t, uint16_t, uint32_t);
void mem_en_paging(struct page_dir*);

void kfree(uint32_t *);
uint32_t* kmalloc(uint32_t);
void heap_init(uint32_t*);

#endif
