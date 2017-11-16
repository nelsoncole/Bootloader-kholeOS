BITS 32
section .text

GLOBAL lidt_install
EXTERN idt
lidt_install:

    
    lidt [idt_ptr]
    ret

GLOBAL exet_kernel_i386
exet_kernel_i386:
   	

    cli
	push ebx
	jmp 0x8:0x100000


section .data

IDT_LIMIT equ $ - 256

idt_ptr:
	dw IDT_LIMIT - 1
	dd idt
