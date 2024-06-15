PRE := /home/neeraj/opt/cross/bin/
GCC := ${PRE}i686-elf-gcc

all:
	nasm -f elf ./blank.asm -o ./build/blank.o
	${GCC} -g -T linker.ld -o blank.elf -ffreestanding -O0 -nostdlib -fpic -g ./build/blank.o
 
clean:
	rm -rf ./build/blank.o ./blank.elf