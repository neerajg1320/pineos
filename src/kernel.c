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
#include "gdt/gdt.h"
#include "config.h"
#include "memory/memory.h"
#include "task/tss.h"
#include "task/task.h"
#include "task/process.h"
#include "status.h"

struct paging_4gb_chunk* kernel_chunk = 0;

void panic(const char* msg) {
    print(msg);
    while(1) {}
}

// struct tss tss;

// struct gdt gdt_real[PINEOS_TOTAL_GDT_SEGMENTS];
// struct gdt_structured gdt_structured[PINEOS_TOTAL_GDT_SEGMENTS] = {
// 	{.base = 0x00, 			 .limit = 0x00, 		.type = 0x00},		// Null Segment
// 	{.base = 0x00, 			 .limit = 0xFFFFFFFF,   .type = 0x9A},		// Kernel Code Segment
// 	{.base = 0x00, 			 .limit = 0xFFFFFFFF,   .type = 0x92},		// Kernel Data Segment
// 	{.base = 0x00, 			 .limit = 0xFFFFFFFF,   .type = 0xF8},		// User Code Segment
// 	{.base = 0x00, 			 .limit = 0xFFFFFFFF,   .type = 0xF2},		// User Data Segment
// 	{.base = (uint32_t)&tss, .limit = sizeof(tss),  .type = 0xE9}		// TSS Segment
// };


struct tss tss;
struct gdt gdt_real[PINEOS_TOTAL_GDT_SEGMENTS];
struct gdt_structured gdt_structured[PINEOS_TOTAL_GDT_SEGMENTS] = {
    {.base = 0x00, .limit = 0x00, .type = 0x00},                // NULL Segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x9a},           // Kernel code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0x92},            // Kernel data segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf8},              // User code segment
    {.base = 0x00, .limit = 0xffffffff, .type = 0xf2},             // User data segment
    {.base = (uint32_t)&tss, .limit=sizeof(tss), .type = 0xE9}      // TSS Segment
};
void kernel_main()
{
    terminal_initialize();
    print("Hello world!\ntest");

    memset(gdt_real, 0x00, sizeof(gdt_real));
    gdt_structured_to_gdt(gdt_real, gdt_structured, PINEOS_TOTAL_GDT_SEGMENTS);

    // Load the gdt
    gdt_load(gdt_real, sizeof(gdt_real));

    // Initialize the heap
    kheap_init();

    // Initialize filesystems
    fs_init();

    // Search and initialize the disks
    disk_search_and_init();

    // Initialize the interrupt descriptor table
    idt_init();

    // Setup the TSS
    memset(&tss, 0x00, sizeof(tss));
    tss.esp0 = 0x600000;
    tss.ss0 = KERNEL_DATA_SELECTOR;

    // Load the TSS
    tss_load(0x28);

    // Setup paging
    kernel_chunk = paging_new_4gb(PAGING_IS_WRITEABLE | PAGING_IS_PRESENT | PAGING_ACCESS_FROM_ALL);
    
    // Switch to kernel paging chunk
    paging_switch(paging_4gb_chunk_get_directory(kernel_chunk));

    // Enable paging
    enable_paging();

    struct process* process = 0;
    int res = process_load("0:/blank.bin", &process);
    if (res != PINEOS_ALL_OK)
    {
        panic("Failed to load blank.bin\n");
    }

    task_run_first_ever_task();

    while(1) {}
}