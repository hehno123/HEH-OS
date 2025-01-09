#ifndef __PORTS_H
#define __PORTS_H

#include <stdint.h>

static inline uint8_t inb(uint32_t port)
{
	uint8_t res;

	__asm__ volatile ("inb %w1, %b0" : "=a"(res) : "Nd"(port) : "memory");
	return res;
}

static inline uint16_t inw(uint32_t port)
{
	uint16_t res;

	__asm__ volatile("inw %w1, %b0" : "=a"(res) : "Nd"(port) : "memory");
	return res;
}

static inline uint32_t inl(uint32_t port)
{
	uint32_t res;

	__asm__ volatile("inl %w1, %b0" : "=a"(res) : "Nd"(port) : "memory");
	return res;
}

static inline uint8_t outb(uint8_t val, uint32_t port)
{
	__asm__ volatile("outb %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint16_t outw(uint16_t val, uint32_t port)
{
	__asm__ volatile("outw %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline uint32_t outl(uint32_t val, uint32_t port)
{
	__asm__ volatile("outl %b0, %w1" : : "a"(val), "Nd"(port) : "memory");
}

static inline void io_wait(void)
{
    outb(0x80, 0x80);
}

#endif
