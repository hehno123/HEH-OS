.extern isr_handler

.global isr_wrapper
.align 4

isr_wrapper:
	pushal
	cld
	call isr_handler
	popal
	iret

.global isr0
.global isr1
.global isr2
.global isr3
.global isr4
.global isr5
.global isr6
.global isr7
.global isr8
.global isr9
.global isr10
.global isr11
.global isr12
.global isr13
.global isr14
.global isr15
.global isr16
.global isr17
.global isr18
.global isr19
.global isr20
.global isr21
.global isr22
.global isr23
.global isr24
.global isr25
.global isr26
.global isr27
.global isr28
.global isr29
.global isr30
.global isr31

isr0: 
 cli
 pushl $0
 pushl $0
 jmp isr_wrapper

isr1: 
 cli
 pushl $0
 pushl $1
 jmp isr_wrapper

isr2: 
 cli
 pushl $0
 pushl $2
 jmp isr_wrapper

isr3: 
 cli
 pushl $0
 pushl $3
 jmp isr_wrapper

isr4: 
 cli
 pushl $0
 pushl $4
 jmp isr_wrapper

isr5: 
 cli
 pushl $0
 pushl $5
 jmp isr_wrapper

isr6: 
 cli
 pushl $0
 pushl $6
 jmp isr_wrapper

isr7: 
 cli
 pushl $0
 pushl $7
 jmp isr_wrapper

isr8: 
 cli
 pushl $8
 jmp isr_wrapper

isr9: 
 cli
 pushl $0
 pushl $9
 jmp isr_wrapper

isr10: 
 cli
 pushl $10
 jmp isr_wrapper

isr11: 
 cli
 pushl $11
 jmp isr_wrapper

isr12: 
 cli
 pushl $12
 jmp isr_wrapper

isr13: 
 cli
 pushl $13
 jmp isr_wrapper

isr14: 
 cli
 pushl $14
 jmp isr_wrapper

isr15: 
 cli
 pushl $0
 pushl $15
 jmp isr_wrapper

isr16: 
 cli
 pushl $0
 pushl $16
 jmp isr_wrapper

isr17: 
 cli
 pushl $17
 jmp isr_wrapper

isr18: 
 cli
 pushl $0
 pushl $18
 jmp isr_wrapper

isr19: 
 cli
 pushl $0
 pushl $19
 jmp isr_wrapper

isr20: 
 cli
 pushl $0
 pushl $20
 jmp isr_wrapper

isr21: 
 cli
 pushl $21
 jmp isr_wrapper

isr22: 
 cli
 pushl $0
 pushl $22
 jmp isr_wrapper

isr23: 
 cli
 pushl $0
 pushl $23
 jmp isr_wrapper

isr24: 
 cli
 pushl $0
 pushl $24
 jmp isr_wrapper

isr25: 
 cli
 pushl $0
 pushl $25
 jmp isr_wrapper

isr26: 
 cli
 pushl $0
 pushl $26
 jmp isr_wrapper

isr27: 
 cli
 pushl $0
 pushl $27
 jmp isr_wrapper

isr28: 
 cli
 pushl $0
 pushl $28
 jmp isr_wrapper

isr29: 
 cli
 pushl $29
 jmp isr_wrapper

isr30: 
 cli
 pushl $30
 jmp isr_wrapper

isr31: 
 cli
 pushl $0
 pushl $31
 jmp isr_wrapper	
