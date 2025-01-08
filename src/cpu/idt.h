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

typedef struct {
	uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; /* values pushed onto the stack by pushal */
	uint32_t int_num, err_num; /* interrupt num and error code if pushed by cpu */
	uint32_t eip, cs, eflags, useresp, ss; /* pushed by cpu */
} register_t;

static idt_gate_descriptor_t idt_table[IDT_FIELDS];
static idt_register_t idtr;

extern void isr0();
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void isr32();
extern void isr33();
extern void isr34();
extern void isr35();
extern void isr36();
extern void isr37();
extern void isr38();
extern void isr39();
extern void isr40();
extern void isr41();
extern void isr42();
extern void isr43();
extern void isr44();
extern void isr45();
extern void isr46();
extern void isr47();

extern void isr_handler(register_t registers);
extern void install_idt();

#endif
