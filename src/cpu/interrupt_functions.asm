.extern isr_handler

.global isr_wrapper
.align 4

isr_wrapper:
	pushal
	cld
	call isr_handler
	popal
	addl $8, %esp
	sti
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
.global isr32
.global isr33
.global isr34
.global isr35
.global isr36
.global isr37
.global isr38
.global isr39
.global isr40
.global isr41
.global isr42
.global isr43
.global isr44
.global isr45
.global isr46
.global isr47

isr0: 
 pushl $0
 pushl $0
 jmp isr_wrapper

isr1: 
 pushl $0
 pushl $1
 jmp isr_wrapper

isr2: 
 pushl $0
 pushl $2
 jmp isr_wrapper

isr3: 
 pushl $0
 pushl $3
 jmp isr_wrapper

isr4: 
 pushl $0
 pushl $4
 jmp isr_wrapper

isr5: 
 pushl $0
 pushl $5
 jmp isr_wrapper

isr6: 
 pushl $0
 pushl $6
 jmp isr_wrapper

isr7: 
 pushl $0
 pushl $7
 jmp isr_wrapper

isr8: 
 pushl $8
 jmp isr_wrapper

isr9: 
 pushl $0
 pushl $9
 jmp isr_wrapper

isr10: 
 pushl $10
 jmp isr_wrapper

isr11: 
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

isr32: 
    pushl $0 
    pushl $32 
    jmp isr_wrapper
isr33:
    pushl $1 
    pushl $33 
    jmp isr_wrapper
isr34:  
    pushl $2 
    pushl $34 
    jmp isr_wrapper
isr35:  
    pushl $3 
    pushl $35 
    jmp isr_wrapper
isr36:  
    pushl $4 
    pushl $36 
    jmp isr_wrapper
isr37:  
    pushl $5 
    pushl $37 
    jmp isr_wrapper
isr38:  
    pushl $6 
    pushl $38 
    jmp isr_wrapper
isr39:  
    pushl $7 
    pushl $39 
    jmp isr_wrapper
isr40: 
    cli 
    pushl $8 
    pushl $40 
    jmp isr_wrapper
isr41: 
    cli 
    pushl $9 
    pushl $41 
    jmp isr_wrapper
isr42: 
    cli 
    pushl $10 
    pushl $42 
    jmp isr_wrapper
isr43: 
    cli 
    pushl $11 
    pushl $43 
    jmp isr_wrapper
isr44: 
    cli 
    pushl $12 
    pushl $44 
    jmp isr_wrapper
isr45: 
    cli 
    pushl $13 
    pushl $45 
    jmp isr_wrapper
isr46: 
    cli 
    pushl $14 
    pushl $46 
    jmp isr_wrapper
isr47: 
    cli 
    pushl $15 
    pushl $47 
    jmp isr_wrapper
