#include "ports.h"

uint8_t fdc_get_driver()
{
	outb(0x10, 0x70); /* access CMOS register 10 */
	io_wait();
	uint8_t ret_val = inb(0x71); /* get CMOS register val */
	return ret_val;
}

