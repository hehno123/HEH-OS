#include "../kernel/tty.h"

void isr_handler()
{
	terminal_printstring("%s INTERRUPT DETECTED!!!!!!!!!!!!!!!!!\n");
}
