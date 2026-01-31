/* Multiboot header variables */
.set ALIGN,    1<<0             
.set MEMINFO,  1<<1             
.set FLAGS,    ALIGN | MEMINFO  
.set MAGIC,    0x1BADB002       
.set CHECKSUM, -(MAGIC + FLAGS) 

.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

.section .bss
.align 16
stack_bottom:
.skip 16384 # 16 KiB
stack_top:

.section .text
.global _start
.type _start, @function
_start:
	/* setup stack at 0x90000 */
	mov $0x90000, %esp

	/* setup gdt */
	cli
	mov $gdt_descriptor, %eax
	sub $0xC0000000, %eax
	lgdt (%eax)

        # Far jump to flush CS:
        ljmp $0x08, $(next - 0xC0000000)
next:
    	mov $0x10, %ax
    	mov %ax, %ds
    	mov %ax, %es
    	mov %ax, %fs
    	mov %ax, %gs
    	mov %ax, %ss
	
	/* pass control to kernel */
	pushl %ebx
	call kernel_main
1:	
	jmp 1b

.size _start, . - _start

.section .data
.align 8

gdt_start:
    .quad 0x0000000000000000             # Null segment

    .word 0xFFFF        
    .word 0x0000         
    .byte 0x00          
    .byte 0x9A           
    .byte 0xCF           
    .byte 0x00          

    .word 0xFFFF
    .word 0x0000
    .byte 0x00
    .byte 0x92           
    .byte 0xCF
    .byte 0x00

gdt_end:

gdt_descriptor:
    .word gdt_end - gdt_start - 1 - 0xc0000000      
    .long gdt_start - 0xc0000000                    

