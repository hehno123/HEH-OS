#include <stddef.h>
#include "stdint.h"
#include <stdarg.h>

#ifndef TTY_H
#define TTY_H

void terminal_set_color(uint8_t);
void terminal_clear();
void terminal_init();
void terminal_put_at(uint8_t, uint8_t, size_t, size_t);
void terminal_scroll();
void terminal_putchar(uint8_t );
void terminal_printstring(char* data);
void terminal_writedata(char* data, size_t size);
void kprintf(char*, ...);

#endif
