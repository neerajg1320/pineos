#include "kernel.h"
#include <stdint.h>
#include <stddef.h>

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

void kernel_main() {
	terminal_initialize();

	terminal_writechar('A', 15);
	terminal_writechar('B', 15);
	// video_mem[0] = terminal_make_char('B', 15);
	// video_mem[1] = terminal_make_char('C', 15);
 }