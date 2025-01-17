[BITS 32]
[extern kernel_main]

mov eax, kernel_main
sub eax, 0x100000

call eax
jmp $
