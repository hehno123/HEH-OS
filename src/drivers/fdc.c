#include "ports.h"

/* https://wiki.osdev.org/CMOS - values return by CMOS */

uint32_t m_sec_p_track;
uint32_t s_sec_p_track;
uint32_t m_track_p_side;
uint32_t s_track_p_side;
uint32_t m_cylinders;
uint32_t s_cylinders;

uint8_t fdc_get_driver()
{
	outb(0x10, 0x70); /* access CMOS register 10 */
	io_wait();
	uint8_t ret_val = inb(0x71); /* get CMOS register val */
	return ret_val;
}

void set_flop_var(uint8_t flop_val, uint32_t* spt, uint32_t* tps, uint32_t* cyl)
{
	switch(flop_val) {
		case 0:
			*spt = 0;
			*tps = 0;
			*cyl = 0;
			break;
		case 1:
			*spt = 9;
			*tps = 40;
			*cyl = 40;
			break;
		case 2:
			*spt = 15;
			*tps = 80;
			*cyl = 80;
			break;
		case 3:
			*spt = 9;
		        *tps = 80;
			*cyl = 80;
			break;
		case 4:
			*spt = 18;
			*tps = 80;
			*cyl = 80;
			break;
		case 5:
			*spt = 36;
			*tps = 80;
			*cyl = 80;
			break;
	}
}	

void lba_2_chs(uint32_t lba, uint16_t* cyl, uint16_t* head, uint16_t* sector, uint8_t s_m)
{
	uint32_t sec_per_track = (s_m) ? m_sec_p_track : s_sec_p_track;

	*cyl = lba / (2 * sec_per_track);
	*head = ((lba % (2 * sec_per_track)) / sec_per_track);
	*sector = ((lba % (2 * sec_per_track)) % sec_per_track + 1);
}

void fdc_driver_init()
{
	uint8_t floppies = fdc_get_driver();
	uint8_t slave_nib = floppies & 0xF;
	uint8_t master_nib = (floppies >> 4) & 0xF;
	
	set_flop_var(slave_nib, &s_sec_p_track, &s_track_p_side, &s_cylinders);
	set_flop_var(master_nib, &m_sec_p_track, &m_sec_p_track, &m_cylinders);
}

	
