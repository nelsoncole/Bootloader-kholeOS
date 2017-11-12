BITS 32
section .text

GLOBAL lidt_install
EXTERN idt
lidt_install:

    
    lidt [idt_ptr]
    ret



section .data

IDT_LIMIT equ $ - 256

idt_ptr:
	dw IDT_LIMIT - 1
	dd idt
