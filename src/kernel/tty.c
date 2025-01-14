#include "../drivers/vga.h"
#include "tty.h"
#include <stddef.h>
#include "../libc/include/string.h"
#include "../drivers/keyboard.h"
#include <stdarg.h>

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
	vga_enable_cursor(0,0);
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

	vga_update_cursor(terminal_column, terminal_row + 1);
}

void terminal_printstring(char* data)
{
	terminal_writedata(data, strlen(data));
}

char get_character(char c)
{
	if(c <= 9)
		return (c + 48);
	
	if(c >= 10 && c <= 16)
		return (char)('A' + (char)(c - 10));

	return 0;
}

void format_str(int64_t num, char system, char sign, char* buf)
{
	int curr = 0;
	char num_sign = (num >= 0) ? '\0' : '-';
	uint64_t temp_val = (num < 0) ? (uint64_t)(~num + 1) : (uint64_t)(num);
	
	do
	{
		buf[curr] = get_character((char)(temp_val % system));
		curr++;
		temp_val /= system;
	} while(temp_val != 0);

	buf[curr] = num_sign;

	if(sign && num_sign != '\0')
		buf[curr + 1] = '\0';
	
	strrev(buf);
}

void kprintf(char* format, ...)
{
	va_list args;
	va_start(args, format);
	
	char* temp_format = format;
	int curr_len = 0;

	while(*format != '\0')
	{
		//check if %, to get format
		if(*format == '%')
		{
			format++;
			char buf[30];
			char is_num_format = 0;

			if(*format == 'd' || *format == 'i')
			{
				int32_t value = va_arg(args, int32_t);
				format_str((int64_t)(value), 10, 1, buf);
				is_num_format = 1;
			}
			
			else if(*format == 'X')
			{
				int32_t value = va_arg(args, int32_t);
				format_str((int64_t)(value), 16, 1, buf);
				is_num_format = 1;
			}

			else if (*format == 'l') 
			{
    				int64_t value = va_arg(args, int64_t);
				format_str(value, 16, 1, buf);
				is_num_format = 1;
			}
			
			else
			{
				curr_len += 1;
			}

			if(is_num_format)
			{
				terminal_writedata(temp_format, curr_len);
				terminal_printstring(buf);
				format++;
				temp_format = format;
				curr_len = 0;
			}

		}

		else
		{
			curr_len += 1;
			format++;
		}
	}
	
	terminal_writedata(temp_format, curr_len);
	va_end(args);
}
