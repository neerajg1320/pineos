#ifndef KERNEL_H
#define KERNEL_H

#define PINEOS_MAX_PATH               108
#define PINEOS_SECTOR_SIZE            512
#define PINEOS_MAX_FILESYSTEMS         16
#define PINEOS_MAX_FILE_DESCRIPTORS     8

void kernel_main();


#endif /* KERNEL */