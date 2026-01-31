/* linker.ld */
   
ENTRY(_start)                   

SECTIONS {
    /* Place the kernel at physical address 0x100000 */
    . = 0x00010000;
    . += 0xC0000000;

    /* Text section (code) */
    .text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000) {
	*(.multiboot)
        *(.text)               /* Place all .text sections here */
    }
    
    .rodata ALIGN (4K) : AT (ADDR (.rodata) - 0xC0000000) {
	    *(.rodata)
    }
		
    /* Data section (initialized data) */
    .data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000) {
        *(.data)               /* Place all .data sections here */
    }

    /* BSS section (uninitialized data) */
    .bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000) {
        *(COMMON)        /* Mark the start of the .bss section */
        *(.bss)                /* Place all .bss sections here*/        
    }
    
    . = ALIGN(0x1000);
    _end = .;
}

