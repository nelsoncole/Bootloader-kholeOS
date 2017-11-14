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
        
        mov [edi],byte 'N'
        mov [edi+1],byte 0xa
        mov [edi+2],byte 'E'
        mov [edi+3],byte 0xa
        mov [edi+4],byte 'L'
        mov [edi+5],byte 0xa       
        mov [edi+6],byte 'S'
        mov [edi+7],byte 0xa  
        mov [edi+8],byte 'O'
        mov [edi+9],byte 0xa
        mov [edi+10],byte 'N'
        mov [edi+11],byte 0xa  
    	

        ;extern main
    	;call main
    
L1:	cli
	hlt
    	jmp L1

section .bss
global _pilha
    resb 8192 ; 8 KiB
_pilha:
