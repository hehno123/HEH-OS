[BITS 32]
[extern kernel_main]

mov eax, kernel_main
sub eax, 0xC0000000

call eax
hlt
