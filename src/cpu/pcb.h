#ifndef IDT_H
#define IDT_H

#define PROC_RUN 1
#define PROC_STOP 1

struct process_block {
	uint16_t pid;
	uint8_t state;
	uint32_t sp;
	uint8_t stack[8192];
};

#endif
