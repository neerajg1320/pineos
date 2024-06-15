FILES = ./build/kernel.asm.o ./build/kernel.o ./build/terminal/print.o ./build/idt/idt.asm.o ./build/idt/idt.o ./build/memory/memory.o ./build/io/io.asm.o ./build/memory/heap/heap.o ./build/memory/heap/kheap.o ./build/memory/paging/paging.asm.o ./build/memory/paging/paging.o ./build/disk/disk.o ./build/string/string.o ./build/fs/pparser.o ./build/disk/streamer.o ./build/fs/filesystem.o ./build/fs/fat/fat16.o ./build/gdt/gdt.o ./build/gdt/gdt.asm.o ./build/task/tss.asm.o ./build/task/task.o ./build/task/process.o ./build/task/task.asm.o
INCLUDES = -I./src
FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unsed-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

PRE := /home/neeraj/opt/cross/bin/
LD := ${PRE}i686-elf-ld
GCC := ${PRE}i686-elf-gcc
GDB := ${PRE}i686-elf-gdb

all: ./bin/boot.bin ./bin/kernel.bin 
	rm -rf ./bin/os.bin 
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1048576 count=16 >> ./bin/os.bin 
	sudo mount -t vfat ./bin/os.bin /mnt/d
	# Copy a file
	sudo cp ./assets/hello.txt /mnt/d
	sudo umount /mnt/d
	
./bin/kernel.bin: $(FILES)
	${LD} -g -relocatable $(FILES) -o ./build/kernelfull.o
	${GCC} ${FLAGS} -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/kernel.c -o ./build/kernel.o

./build/terminal/print.o: src/terminal/print.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/terminal/print.c -o ./build/terminal/print.o

./build/idt/idt.asm.o: ./src/idt/idt.asm
	nasm -f elf -g ./src/idt/idt.asm -o ./build/idt/idt.asm.o

./build/idt/idt.o: ./src/idt/idt.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/idt/idt.c -o ./build/idt/idt.o

./build/memory/memory.o: ./src/memory/memory.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/io/io.asm.o: ./src/io/io.asm
	nasm -f elf -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/memory/heap/heap.o: ./src/memory/heap/heap.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/memory/heap/heap.c -o ./build/memory/heap/heap.o

./build/memory/heap/kheap.o: ./src/memory/heap/kheap.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/memory/heap/kheap.c -o ./build/memory/heap/kheap.o

./build/memory/paging/paging.asm.o: ./src/memory/paging/paging.asm
	nasm -f elf -g ./src/memory/paging/paging.asm -o ./build/memory/paging/paging.asm.o

./build/memory/paging/paging.o: ./src/memory/paging/paging.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/memory/paging/paging.c -o ./build/memory/paging/paging.o

./build/disk/disk.o: ./src/disk/disk.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/disk/disk.c -o ./build/disk/disk.o

./build/string/string.o: ./src/string/string.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/string/string.c -o ./build/string/string.o

./build/fs/pparser.o: ./src/fs/pparser.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/fs/pparser.c -o ./build/fs/pparser.o

./build/disk/streamer.o: ./src/disk/streamer.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/disk/streamer.c -o ./build/disk/streamer.o

./build/fs/filesystem.o: ./src/fs/filesystem.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/fs/filesystem.c -o ./build/fs/filesystem.o

./build/fs/fat/fat16.o: ./src/fs/fat/fat16.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/fs/fat/fat16.c -o ./build/fs/fat/fat16.o

./build/gdt/gdt.asm.o: ./src/gdt/gdt.asm
	nasm -f elf -g ./src/gdt/gdt.asm -o ./build/gdt/gdt.asm.o

./build/gdt/gdt.o: ./src/gdt/gdt.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/gdt/gdt.c -o ./build/gdt/gdt.o

./build/task/tss.asm.o: ./src/task/tss.asm
	nasm -f elf -g ./src/task/tss.asm -o ./build/task/tss.asm.o

./build/task/task.asm.o: ./src/task/task.asm
	nasm -f elf -g ./src/task/task.asm -o ./build/task/task.asm.o

./build/task/task.o: ./src/task/task.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/task/task.asm -o ./build/task/task.o

./build/task/process.o: ./src/task/process.c
	${GCC} ${INCLUDES} ${FLAGS} -std=gnu99 -c ./src/task/process.c -o ./build/task/process.o

clean:
	rm -rf ./bin/boot.bin
	rm -rf ./bin/kernel.bin
	rm -rf ./bin/os.bin
	rm -rf ${FILES}
	rm -rf kernelfull.o

show:
	@echo "PRE=${PRE}"
	@echo "TARGET=${TARGET}"
	@echo "PATH=${PATH}"
	@which i686-elf-ld
	@which i686-elf-gcc
	@which i686-elf-gdb

dump:
	bless ./bin/os.bin

gdb:
	${GDB}
	# target remote | qemu-system-i386 -drive file=./bin/os.bin,format=raw -S -gdb stdio

run:
	qemu-system-i386 -drive file=./bin/os.bin,format=raw

run-legacy:
	qemu-system-x86_64 -drive file=./bin/os.bin,format=raw
