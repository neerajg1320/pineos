FILES =./build/kernel.asm.o

all: ./bin/boot.bin ./bin/kernel.bin 
	rm -rf ./bin/os.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/kernel.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=100 >> ./bin/os.bin 
	
./bin/kernel.bin: $(FILES)
	i686-elf-ld -g -relocatable $(FILES) -o ./build/kernelfull.o
	i686-elf-gcc -T ./src/linker.ld -o ./bin/kernel.bin -ffreestanding -O0 -nostdlib ./build/kernelfull.o

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./build/kernel.asm.o: ./src/kernel.asm
	nasm -f elf -g ./src/kernel.asm -o ./build/kernel.asm.o

clean:
	rm -rf ./bin/boot.bin

show:
	@echo "PREFIX=${PREFIX}"
	@echo "TARGET=${TARGET}"
	@echo "PATH=${PATH}"
	@which i686-elf-ld
	@which i686-elf-gcc
	@which i686-elf-gdb

dump:
	bless ./bin/os.bin

gdb:
	i686-elf-gdb
run-legacy:
	qemu-system-x86_64 -hda ./boot.bin
