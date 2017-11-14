bits 32
section .text

;global start

org 0x100000

start:
	mov ax,0x10
	mov ds,ax
	mov ss,ax
	mov esp, _pilha
	xor eax,eax
	mov es,ax
	mov fs,ax
	mov gs,ax
    	jmp MAIN

    	;EXTERN code, bss, end
    	;dd code
    	;dd bss
    	;dd end
    	;dd start

MAIN:
        
        mov edi,0xb8000
        mov esi,string_test
        mov ecx,len
        mov ah,0xe
.loop:  lodsb
        mov [edi],ax
        add edi,2
        loop .loop 
    	

        ;extern main
    	;call main
    
L1:	cli
	hlt
    	jmp L1

string_test db "Hello, Kernel",0
len equ $ - string_test
section .bss
global _pilha
    resb 8192 ; 8 KiB
_pilha:
