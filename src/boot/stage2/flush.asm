BITS 32
section .text

GLOBAL lidt_install
EXTERN idt
lidt_install:

    
    lidt [idt_ptr]
    ret

GLOBAL flush
flush:
   	

    cli
    mov ax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov eax,esp
	push 0x10
	push eax
	pushf
	push 0x8
	push 0x100000
	iretd


section .data

IDT_LIMIT equ $ - 256

idt_ptr:
	dw IDT_LIMIT - 1
	dd idt
