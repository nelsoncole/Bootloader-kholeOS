CFLAGS=-Wall -m32 -ffreestanding -nostdlib -nostdinc -I ./include
.PHONY: all


all:	
	nasm -f bin kernel_test/kernel.asm -o kernel.bin	
	dd of=kholeOS.img if=/dev/zero bs=512 count=1440
	./kholefs boot/stage1.bin kholeOS.img
	sudo mount kholeOS.img /mnt/
	sudo cp boot/stage2.bin /mnt/
	sudo mkdir /mnt/boot	
	sudo mkdir /mnt/kernel
	sudo cp kernel.bin /mnt/kernel/
	sudo umount /mnt/
	qemu-system-i386 -m 512 -drive file=kholeOS.img,index=0,format=raw,media=disk
	
