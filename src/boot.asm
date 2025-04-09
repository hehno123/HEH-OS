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
	
	in al, 0x92
	or al, 0x02      ; Enable A20 line
	and al, 0xFE     ; Clear bit 0
	out 0x92, al
	in al, 0x92      ; Read back to verify
	test al, 0x02    ; Check if A20 is active
	jz error
	
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
	 
	mov eax, 0xE820 ;function call
	xor ebx, ebx ;must be zero
	xor bp, bp ;keep track of counts
	mov di, 0x8004 ;memory location to store
	mov edx, 0x534D4150 ;SMAP signature
	mov dword[es:di + 20], 1 ;get additional information
	mov ecx, 24 ;request 24 bytes of data
	int 0x15
	
	jc memory_err ;if carry flag, it indicates memory error
	mov edx, 0x534D4150
	cmp eax, edx
	jnz memory_err
	test ebx, ebx
	je memory_err
	jmp check_zero

get_memory:
	mov eax, 0xE820
	mov dword[es:di + 20], 1
	mov ecx, 24
	int 0x15
	jc finish_memory

	mov edx, 0x534D4150
check_zero:
	jcxz skip_entry ;if exc = 0, then skip entry
	cmp cl, 20
	jbe check_len; ecx > 20
	test byte[es:di + 20], 1
	je skip_entry

check_len:
	mov eax, [es:di + 8] ;get lower address
	mov ecx, [es:di + 12] ;get higher address
	or eax, ecx
	jz skip_entry
	
	inc bp
	add di, 24

skip_entry:
	test ebx, ebx
	jne get_memory

finish_memory:
	mov [es:0x8000], bp
	clc

gdt_setup:
	cli
	lgdt [gdt_descriptor]
	
	mov eax, cr0
	mov eax, 0b1
	mov cr0, eax

	jmp 0x08:reload

[BITS 32]
reload:
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	
	mov ebp, 0x90000
	mov esp, ebp

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

memory_err:
	mov bp, 3
	jmp error

error_list:
err_LBA  db    "00 LBA not supported          "	
err_PCK  db    "00 Ext read err occured       "	
err_APM  db    "00 APM not supported          "
err_MAP  db    "00 Memory map layout error    "

gdt_start: 
    dd 0x0 
    dd 0x0

gdt_code:  
     	dw 0xFFFF ;16 bits limit
     	dw 0x0 ;base - 16 bits
	db 0x0
	db 0x9A
	db 11001111b
	db 0x0
           

gdt_data:
    	dw 0xFFFF
    	dw 0x0
    	db 0x0
    	db 0x92
    	db 11001111b
    	db 0x0

gdt_end:

gdt_descriptor:
    dw gdt_end - gdt_start - 1
    dd gdt_start

align 	16
os_packet	db	0x10
            	db	0x0
            	dw	0x20
            	dw 	0x0, 0x1000
            	dq	0x1

times 510 - ($-$$) db 0
dw 0xAA55


