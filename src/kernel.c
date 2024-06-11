#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "idt/idt.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"

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

struct paging_4gb_chunk* kernel_directory = 0;

void kernel_main() {
	terminal_initialize();

	for (int i=0; i < 250; i++) {
		terminal_writechar('A', 15);
		terminal_writechar('B', 15);
	}
	print("\n");
	print("Hello World!\n");

	video_mem[0] = terminal_make_char('G', 15);
	video_mem[1] = terminal_make_char('H', 15);

	// Initialize the heap
	kheap_init();


	// Initialize the Interrupt Descriptor Table
	idt_init();

	// Setup paging
	kernel_directory = paging_new_4gb(PAGING_IS_WRITABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);


	// Enable interrupts after IDT has been initialized
	enable_interrupts();

	void* ptr1 = kmalloc(50);
	void* ptr2 = kmalloc(5000);
	void* ptr3 = kmalloc(5600);
	
	// free the pointer. Note it is dangling if we don't make ptr1 null.
	kfree(ptr1);

	void* ptr4 = kmalloc(100);

	if (ptr1 || ptr2 || ptr3 || ptr4) {
		print("Pointers allocated!\n");
	}


	
 }