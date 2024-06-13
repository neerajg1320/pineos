#ifndef PRINT_H
#define PRINT_H

void terminal_initialize();
void print(const char* str);
void print_uint(const uint32_t val);
void print_pointer(const void* val);

#define VGA_WIDTH 80
#define VGA_HEIGHT 20

#endif /* PRINT_H */