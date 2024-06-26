#ifndef PRINT_H
#define PRINT_H

#include <stdint.h>

void terminal_initialize();
void print(const char* str);
void print_char(const char str);
void print_uint(const uint32_t val);
void print_float(const float val);
void print_pointer(const void* val);
void printf(const char* str, ...);

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#endif /* PRINT_H */