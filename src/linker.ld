/* linker.ld */

OUTPUT_FORMAT(binary)    
ENTRY(_start)                   

SECTIONS {
    /* Place the kernel at physical address 0x10000 */
    . = 0x110000;

    /* Text section (code) */
    .text : AT(ADDR(.text) - 0x100000) {
        _start = .;            /* Mark the start of the kernel */
        *(.text)               /* Place all .text sections here */
    }

    /* Data section (initialized data) */
    .data : AT(ADDR(.data) - 0x100000) {
        _data = .;             /* Mark the start of the .data section */
        *(.data)               /* Place all .data sections here */
    }

    /* BSS section (uninitialized data) */
    .bss : AT(ADDR(.bss) - 0x100000) {
        _bss_start = .;        /* Mark the start of the .bss section */
        *(.bss)                /* Place all .bss sections here */
        _bss_end = .;          
    }
    
    . = ALIGN(0x1000);
    _end = .;
}

