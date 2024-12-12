/* defines structure of interrupt descriptor table */

#ifndef IDT_H
#define IDT_H

#include <stdint.h>

#define IDT_FIELDS 256

void set_idt_gate(uint8_t entry_num, uint32_t handler, uint8_t type);
void set_idt();

typedef struct {
	uint16_t offset_one;
	uint16_t segment;
	uint8_t reserved;
	uint8_t flags;
	uint16_t offset_two;
} __attribute__((packed)) idt_gate_descriptor_t;

typedef struct {
	uint16_t limit;
	uint32_t base_addr;
} __attribute__((packed)) idt_register_t;

static idt_gate_descriptor_t idt_table[IDT_FIELDS];
static idt_register_t idtr;

#endif
