#include <stdint.h>
#include <stddef.h>
#include "print.h"

uint16_t* video_mem = 0;
uint16_t terminal_row = 0;
uint16_t terminal_col = 0;


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
		terminal_writechar(str[i], 15);
	}
}

void print_uint(const uint32_t val) {
    const char int_array[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

    for (int i=0; i < sizeof(int_array); i++) {
        char c = int_array[i];
        terminal_writechar(c, 15);
    }

    // uint32_t num = val;
    // uint8_t size = sizeof(uint32_t) / sizeof(char);
}
