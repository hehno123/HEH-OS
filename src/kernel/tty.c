#include "../drivers/vga.h"
#include "tty.h"
#include <stddef.h>
#include "../libc/include/string.h"
#include "../drivers/keyboard.h"

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* cell_address;

void terminal_scroll()
{
	for(size_t i = 0; i < MAX_ROWS - 1; i++)
	{
		for(size_t j = 0; j < MAX_COL; j++)
		{	
			cell_address[i * MAX_COL + j] = cell_address[(i + 1) * MAX_COL + j];
			cell_address[(i + 1) * MAX_COL + j] = change_text_cell(' ', terminal_color);
		}
	}		
}

void terminal_set_color(uint8_t color)
{
	terminal_color = color;
}

void terminal_clear()
{
	for(size_t y = 0; y < MAX_ROWS; y++)
	{
		for(size_t x = 0; x < MAX_COL; x++)
		{
			cell_address[y * MAX_COL + x] = change_text_cell(' ', terminal_color);
		}
	}
}

void terminal_init()
{
	terminal_row = 0;
	terminal_column = 0;
	terminal_color = change_cell_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
	cell_address = (uint16_t*)(VGA_ADDRESS);

	terminal_clear();
}

void terminal_put_at(uint8_t uc, uint8_t color, size_t x, size_t y)
{
	cell_address[y * MAX_COL + x] = change_text_cell(uc, color);
}

void terminal_putchar(uint8_t c)
{
	if(terminal_column >= MAX_COL || c == '\n')
	{
		terminal_row++;

		if(terminal_row >= MAX_ROWS)
		{
			terminal_scroll();
			terminal_row = MAX_ROWS - 1;
		}

		terminal_column = 0;
	}
	
	if(c != '\n') {	
		terminal_put_at((unsigned char)(c), terminal_color, terminal_column, terminal_row);
		terminal_column++;
	}
}	

void terminal_writedata(char* data, size_t size)
{
	for(size_t i = 0; i < size; i++)
	{
		terminal_putchar(data[i]);
	}
}

void terminal_printstring(char* data)
{
	terminal_writedata(data, strlen(data));
}
