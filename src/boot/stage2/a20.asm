; "a20.asm"
;
; Projecto "Projecto KholeOS. Bootloader" 
; Nelson Sapalo da Silva Cole  (nelsoncole72@gmail.com  +244-948-833-132)
; Lubango 26 de Setembro de 2017
;





BITS 16
GLOBAL  gateA20

; Gate A20

gateA20:
	pusha
	cli
; Desabilita teclado
	call a20_wait1
	mov al,0xAD
	out dx,al

;Ler o status gate a20
	call a20_wait1
	mov al,0xD0	
	out 0x64,al

	call a20_wait0
	in al,0x60	;ler o status lido
	mov cl,al

;Escreve status gate a20
	call a20_wait1
	mov al,0xD1	
	out 0x64,al

; Hablita a gate a20
	call a20_wait1
	mov al,cl
	or al,2		;liga o bit 2
	out 0x60,al	

; Habilita o teclado
	call a20_wait1
	mov al,0xAE
	out 0x64,al

	call a20_wait1	;Espera
	sti		;Habilita interrupcoes
	popa
	ret

a20_wait0:
	in al,0x64
	test al,1
	jz a20_wait0  
	ret




a20_wait1:
	in al,0x64
	test al,2
	jnz a20_wait1
	ret



