; "start.asm"
;
; "Projecto KholeOS. Bootloader" 
; Nelson Sapalo da Silva Cole  (nelsoncole72@gmail.com  +244-948-833-132)
; Lubango 18 de Julho de 2017



section .text

bits 16
global start
start:
	jmp short _start



; Essas informações são exteriores do link

	EXTERN code,data,bss,end
	dd code
	dd data
	dd bss
	dd end
	dd start


	EXTERN puts16
	EXTERN gateA20
	EXTERN gdtr

global BootDevice,BootSector
BootDevice dw 0


_start:
	cli	
	xor ax,ax
	mov ds,ax
	mov es,ax
	mov ss,ax
	mov sp,0x2000  ; 8 KiB de pilha
	sti


; Salvar o valor de disco 
	mov WORD [BootDevice],dx
	
; Habilita Gate A20
	call gateA20

; Testa a Gate A20 


; Mascara interrupções e desabilita NMI
	
	cli 	    ;

	in al,0x70
	or al,0x80
	out 0x70,al

; Instala a GDT
	db 0x66		; Execute GDT 32-Bits
	lgdt [gdtr]

; Habilita o PE 
	mov eax,cr0
	or eax,1
	mov cr0,eax

; Pula para modo protegido
	jmp 0x8:ModoProtegido

;	Mensagens de Erro
ErroA20 db "Erro com a Gate A20",0

; Retornos de Erro
booterro:
	call puts16
	pop si

; Espera uma interrupção do teclado
	xor ax,ax
	int 0x16
	int 0x18



bits 32

EXTERN main
ModoProtegido:
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax
	mov ss,ax
	mov esp,_stack	; Nossa pilha
	
	call main

	
	

; Põe o processador para dormir
L1:	cli
	hlt
	jmp L1
	
section .data


section .bss
	resb 8192	; 8 KiB pilha
_stack:
