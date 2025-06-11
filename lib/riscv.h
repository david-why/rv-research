#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int printf(const char *fmt, ...);
int puts(const char *s);
int putchar(int c);

uint64_t getcycles(void);

#define RAM_START 0x00000000
#define ROM_START 0x00800000

#define IO_BASE   0x01000000
#define IO_UART0 (IO_BASE + 0x1000)
#define IO_EXIT  (IO_BASE + 0x2000)

#define IO_OUT(port, value) (*(volatile uint32_t *)(port)) = (value)
#define IO_IN(port) (*(volatile uint32_t *)(port))

#ifdef __cplusplus
}
#endif
