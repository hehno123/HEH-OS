.extern isr_handler

.global isr_wrapper
.align 4

isr_wrapper:
	pushal
	cld
	call isr_handler
	popal
	iret
