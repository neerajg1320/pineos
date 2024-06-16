#ifndef KERNEL_H
#define KERNEL_H

#define PINEOS_SECTOR_SIZE            512
#define PINEOS_MAX_FILESYSTEMS         16
#define PINEOS_MAX_FILE_DESCRIPTORS     8

#define ERROR(value) (void*)(value)
#define ERROR_I(value) (int)(value)
#define ISERR(value) ((int)value < 0)

void panic(const char* msg);
void kernel_main();
void kernel_page();
void kernel_registers();

#endif /* KERNEL */