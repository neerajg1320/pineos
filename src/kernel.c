#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "terminal/print.h"
#include "idt/idt.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"
#include "disk/disk.h"
#include "fs/pparser.h"
#include "disk/streamer.h"
#include "string/string.h"
#include "fs/filesystem.h"

struct paging_4gb_chunk* kernel_chunk = 0;

void kernel_main() {
	terminal_initialize();

	// Initialize the heap
	kheap_init();

	// Initialize file system
	fs_init();

	// Search and Initialize disk
	disk_search_and_init();

	// Initialize the Interrupt Descriptor Table
	idt_init();

	// Setup paging
	kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);

	// Switch to kernel paging chunk
	paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

	char* ptr = kzalloc(4096); 

	// Our virtual address 0x1000 is mapped to ptr. 
	// Need to work and resolve this.
    paging_set(paging_4gb_chunk_get_directory(kernel_chunk), (void*)0x1000, (uint32_t)ptr | PAGING_ACCESS_FROM_ALL | PAGING_IS_PRESENT | PAGING_IS_WRITEABLE);

	// Enable paging to make the following print work properly
	enable_paging();

	char buf[512];
	disk_read_sector(0, 1, buf);

	// Info:
	// Below this part is for testing and it can be removed

    char* ptr2 = (char*) 0x1000;
    ptr2[0] = 'C';
    ptr2[1] = 'D';
    print(ptr2);

	// Enable paging to make the following print work properly
	enable_paging();

    print(ptr);

	// Enable interrupts after IDT has been initialized
	enable_interrupts();


	// Test Code:
	// Kept here to test the working of the code.

	
	print_char('\n');
	print("Test Results:\n");

	char* filename = "0:/hello.txt";
    int fd = fopen(filename, "r");
    if (fd) {
        printf("We opened '%s'\n", filename);

		char fbuf1[16];
		fread(fbuf1, 16, 1, fd);
		print(fbuf1);
    } else {
		printf("We could not open '%s'\n", filename);
	}
	
	// Loop to stop further execution
	while(1) {}

	struct disk_stream* stream = diskstreamer_new(0);
	diskstreamer_seek(stream, 0x201);
	unsigned char c = 0;
	diskstreamer_read(stream, &c, 1);
	print_uint((uint32_t)c);
	print_char('\n');

	char buffer[20];
	strcpy(buffer, "hello");
	print(buffer);
	print_char('\n');



	struct path_root* root_path = pathparser_parse("0:/bin/shell.exe", NULL);
	if (root_path) {

	}

	void* ptr11 = kmalloc(50);
	void* ptr12 = kmalloc(5000);
	void* ptr13 = kmalloc(5600);
	
	// free the pointer. Note it is dangling if we don't make ptr1 null.
	kfree(ptr11);

	void* ptr14 = kmalloc(100);

	if (ptr11 || ptr12 || ptr13 || ptr14) {
		print("Pointers allocated!\n");
	}

	print_uint(2009);
	print("\n");
	print_pointer((void*)2009);
	print("\n");

	printf("%s. %c and %f Hello %%%d and %s, %m ptr1=%p\n", "Kernel", 'K', 205.7, 42, "Code", ptr11);

	printf("ptr11=%p, ptr12=%p, ptr13=%p ptr14=%p", ptr11, ptr12, ptr13, ptr14);
 }