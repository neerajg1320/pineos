#include <stdint.h>
#include <stddef.h>
#include <stdarg.h> // For va_list
#include <stdbool.h>
#include "print.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;
uint16_t terminal_color = 15;

uint16_t terminal_make_char(char c, char colour) {
	return (colour << 8) | c;
}

void terminal_putchar(int x, int y, char c, char colour) {
	video_mem[(y * VGA_WIDTH) + x] = terminal_make_char(c , colour);
}

/* The following does not take a position from user */
void terminal_writechar(char c, char colour) {
	if (c == '\n') {
		terminal_row += 1;
		terminal_col = 0;
		return;
	}

	terminal_putchar(terminal_col, terminal_row, c, colour);
	terminal_col += 1;
	if (terminal_col >= VGA_WIDTH) {
		terminal_row += 1;
		terminal_col = 0;
	}
}

void terminal_initialize() {
	video_mem = (uint16_t*)(0xB8000);
	for (int y=0; y < VGA_HEIGHT; y++) {
		for (int x=0; x < VGA_WIDTH; x++) {
			terminal_putchar(x, y, ' ', 0);
		}
	}
}

size_t strlen(const char* str) {
	size_t len = 0;
	while(str[len]) {
		len++;
	}
	return len;
}


void print(const char* str) {
	size_t len = strlen(str);
	for (int i=0; i < len; i++) {
		terminal_writechar(str[i], terminal_color);
	}
}

void print_char(const char c) {
    terminal_writechar(c, terminal_color);
}

const char hex_array[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
#define MAX_DIGITS  32
#define MAX_BASE    16

static void print_base(const uint32_t val, char base, char size) {
    if (base > MAX_BASE) {
        print("base ");
        print_base(base, 10, 0);
        print(" not supported");
    }
    // We use stack as we have to write the digits in reverse order of their detection
    char stack[MAX_DIGITS];
    int sp = 0;

    uint32_t num = val;
    while (num > 0) {
        int index = (char)(num % base);
        num = num / base;
        stack[sp++] = hex_array[index];
    }

    if (size > 0) {
        while (sp < size) {
            stack[sp++] = hex_array[0];  // Prepend the digits with '0'
        }
    }

    // We have to write this in the reverse order
    for (int i = sp - 1; i >= 0; i--) {
        terminal_writechar(stack[i], terminal_color);
    }
}

void print_uint(const uint32_t val) {
    print_base(val, 10, 0);
}

void print_float(const float val) {
    print_base((uint32_t)val, 10, 0);
    print(".0");
}

void print_hex(const uint32_t val) {
    print_base(val, 16, 0);
}

void print_pointer(const void * ptr) {
    print("0x");
    print_base((uint32_t)ptr, 16, 8);
}

#define DEBUG 0

void printf(const char* format, ...) {
    // Initialize the list pointer
    va_list args;
    va_start(args, format);

    char token_count = 0;
    char no_token = 0;

    bool token_started = 0;

    print(format);
    print("\n");

    for (const char* p = format; *p != '\0'; p++) {
        if (!token_started && *p == '%') {
            token_started = 1;
            continue;
        }

        if (token_started) {
            switch(*p) {
                case 'd':
                token_count++;
                int i = va_arg(args, int);
                print_uint(i);
                break;

                case 's':
                token_count++;
                char* s = va_arg(args, char*);
                print(s);                 
                break;

                case 'c':
                token_count++;
                char c = (char)va_arg(args, int);
                print_char(c);  
                break;

                case 'f':
                token_count++;
                float f = (float)va_arg(args, double);
                print_float(f); 
                break;

                case '%':
                print_char(*p);
                no_token++;
                break;

                default:
                print_char('%');
                print_char(*p);
                print("(!error)");
            }  
            token_started = 0; 
            continue;        
        }

        print_char(*p);
    }

#if DEBUG
    print("\n");
    print("token_count:");
    print_uint(token_count);
    print("\n");
    print("no_token:");
    print_uint(no_token);
    print("\n");
#endif    
}