;Wojtek tutaj czeka cie jedynie płacz

[BITS 16]
[ORG 0x7C00]

powitanie:
	mov ax, 0x3
	int 0x10

	xor cx, cx
	mov ds, cx
	mov es, cx
	mov bp, cx

	mov ah, 0x41
	mov bx, 0x55AA
	mov dl, 0x80
	int 0x13
	jc error
	inc bp

	mov ah, 0x42
	mov dl, 0x80
	mov si, os_packet
	int 0x13
	jc error
	
	cli
	lgdt [gdt_descriptor]

	mov 	eax, cr0
	or  	eax, 0b1
	mov 	cr0, eax

	jmp 0x08:reload

[BITS 32]
reload:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	;set up the stack
	mov ebp, 0x90000
	mov esp, ebp
	
	;enable the A20 line
	in al, 0x92
	or al, 0x02
	out 0x92, al

	;jump to kernel location
	call 0x08:0x10000

%define len 0x1E
[BITS 16]
error:
	;get correct error message
	imul 	bp, 0x1E
	add 	bp, error_list

	;insert error code to the beggining of string
	;in place of "00"
	shr 	ax, 0x4
	shr 	al, 0x4
	add 	WORD [bp + 0x0], ax

	;set video mode to text
	mov 	ax, 0x0003
	int 	0x10
	
	;print string
	mov 	ax, 0x1300
	mov 	bx, 0x000F
	mov 	cx, len
	xor 	dx, dx
	mov 	es, dx
	int 	0x10
	
	cli
.loop:
	hlt
	jmp 	.loop

error_list:
err_LBA  db    "00 LBA not supported          "	
err_PCK  db    "00 Ext read err occured       "	
err_APM  db    "00 APM not supported          "

gdt_start: 
    dd 0x0 
    dd 0x0

gdt_code: 
    dw 0xffff    
    dw 0x0      
    db 0x0       
    db 10011010b 
    db 11001111b 
    db 0x0       

gdt_data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10010010b
    db 11001111b
    db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

align 	16
os_packet	db	0x10
            	db	0x0
            	dw	0x15
            	dw      0x0000, 0x1000	
            	dq	0x1

times 510 - ($-$$) db 0
dw 0xAA55


