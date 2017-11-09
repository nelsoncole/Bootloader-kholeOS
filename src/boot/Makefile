

CFLAGS=-Wall -m32 -ffreestanding -nostdlib -nostdinc -I ./include
.PHONY: all


all:
	nasm -f bin stage1/stage1.asm -o stage1.bin
	nasm -f elf stage2/start.asm -o start.o
	nasm -f elf stage2/puts16.asm -o puts16.o
	nasm -f elf stage2/a20.asm -o a20.o
	nasm -f elf stage2/gdt.asm -o gdt.o
	nasm -f elf stage2/flush.asm -o flush.o

	gcc ${CFLAGS} -c stage2/*.c
	gcc ${CFLAGS} -c drivers/*.c
	gcc ${CFLAGS} -c fs/*.c


	ld -T stage2/boot.ld start.o puts16.o a20.o gdt.o idt.o flush.o main.o io.o display.o strings.o ata.o ata_info.o fat.o -o stage2.bin


	rm *.o
	
