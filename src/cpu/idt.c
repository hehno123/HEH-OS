#include "idt.h"

void set_idt_gate(uint8_t entry_num, uint32_t handler, uint8_t type)
{
	idt_gate_descriptor_t temp_gate;

	temp_gate.offset_one = (uint16_t)(handler & 0xFFFF);
	temp_gate.offset_two = (uint16_t)((handler >> 16) & 0xFFFF);
	temp_gate.reserved = 0;
	temp_gate.segment = 0x8;
	temp_gate.flags = type;

	idt_table[entry_num] = temp_gate;
}

void set_idt()
{
	idtr.base_addr = (uint32_t)(&idt_table);
	idtr.limit = IDT_FIELDS * sizeof(idt_gate_descriptor_t) - 1;

	__asm__ __volatile__("lidtl (%0)" : : "r" (&idtr));
}
