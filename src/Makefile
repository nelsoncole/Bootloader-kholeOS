CFLAGS=-Wall -m32 -ffreestanding -nostdlib -nostdinc -I ./include
.PHONY: all


all:	

	dd if=/dev/zero of=kholeOS.img bs=1024 count=720

	./kholefs boot/stage1.bin kholeOS.img

	sudo mount kholeOS.img /mnt/

	sudo cp boot/stage2.bin /mnt/

	sudo umount /mnt/

	qemu-system-i386 -drive file=kholeOS.img,index=0,format=raw,media=disk
