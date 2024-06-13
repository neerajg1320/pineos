#include "kernel.h"
#include <stdint.h>
#include <stddef.h>
#include "terminal/print.h"
#include "idt/idt.h"
#include "memory/heap/kheap.h"
#include "memory/paging/paging.h"




struct paging_4gb_chunk* kernel_chunk = 0;

void kernel_main() {
	terminal_initialize();

	// for (int i=0; i < 250; i++) {
	// 	terminal_writechar('A', 15);
	// 	terminal_writechar('B', 15);
	// }
	// print("\n");
	// print("Hello World!\n");

	// video_mem[0] = terminal_make_char('G', 15);
	// video_mem[1] = terminal_make_char('H', 15);

	// Initialize the heap
	kheap_init();


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

	printf("%s. Hello %% and %s, \n", "Kernel");
	// printf("Hello %%, your friend %s has sent %d messages. %% and %f\n", "Kernel");
 }