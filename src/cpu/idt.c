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

void install_idt()
{
	set_idt_gate(0, (uint32_t)isr0, 0x8E);
	set_idt_gate(1, (uint32_t)isr1, 0x8E);
	set_idt_gate(2, (uint32_t)isr2, 0x8E);
	set_idt_gate(3, (uint32_t)isr3, 0x8E);
	set_idt_gate(4, (uint32_t)isr4, 0x8E);
	set_idt_gate(5, (uint32_t)isr5, 0x8E);
	set_idt_gate(6, (uint32_t)isr6, 0x8E);
	set_idt_gate(7, (uint32_t)isr7, 0x8E);
	set_idt_gate(8, (uint32_t)isr8, 0x8E);
	set_idt_gate(9, (uint32_t)isr9, 0x8E);
	set_idt_gate(10, (uint32_t)isr10, 0x8E);
	set_idt_gate(11, (uint32_t)isr11, 0x8E);
	set_idt_gate(12, (uint32_t)isr12, 0x8E);
	set_idt_gate(13, (uint32_t)isr13, 0x8E);
	set_idt_gate(14, (uint32_t)isr14, 0x8E);
	set_idt_gate(15, (uint32_t)isr15, 0x8E);
	set_idt_gate(16, (uint32_t)isr16, 0x8E);
	set_idt_gate(17, (uint32_t)isr17, 0x8E);
	set_idt_gate(18, (uint32_t)isr18, 0x8E);
	set_idt_gate(19, (uint32_t)isr19, 0x8E);
	set_idt_gate(20, (uint32_t)isr20, 0x8E);
	set_idt_gate(21, (uint32_t)isr21, 0x8E);
	set_idt_gate(22, (uint32_t)isr22, 0x8E);
	set_idt_gate(23, (uint32_t)isr23, 0x8E);
	set_idt_gate(24, (uint32_t)isr24, 0x8E);
	set_idt_gate(25, (uint32_t)isr25, 0x8E);
	set_idt_gate(26, (uint32_t)isr26, 0x8E);
	set_idt_gate(27, (uint32_t)isr27, 0x8E);
	set_idt_gate(28, (uint32_t)isr28, 0x8E);
	set_idt_gate(29, (uint32_t)isr29, 0x8E);
	set_idt_gate(30, (uint32_t)isr30, 0x8E);
	set_idt_gate(31, (uint32_t)isr31, 0x8E);
	set_idt_gate(32, (uint32_t)isr32, 0x8E);
	set_idt_gate(33, (uint32_t)isr33, 0x8E);
	set_idt_gate(34, (uint32_t)isr34, 0x8E);
	set_idt_gate(35, (uint32_t)isr35, 0x8E);
	set_idt_gate(36, (uint32_t)isr36, 0x8E);
	set_idt_gate(37, (uint32_t)isr37, 0x8E);
	set_idt_gate(38, (uint32_t)isr38, 0x8E);
	set_idt_gate(39, (uint32_t)isr39, 0x8E);
	set_idt_gate(40, (uint32_t)isr40, 0x8E);
	set_idt_gate(41, (uint32_t)isr41, 0x8E);
	set_idt_gate(42, (uint32_t)isr42, 0x8E);
	set_idt_gate(43, (uint32_t)isr43, 0x8E);
	set_idt_gate(44, (uint32_t)isr44, 0x8E);
	set_idt_gate(45, (uint32_t)isr45, 0x8E);
	set_idt_gate(46, (uint32_t)isr46, 0x8E);
	set_idt_gate(47, (uint32_t)isr47, 0x8E);

	set_idt();
}
