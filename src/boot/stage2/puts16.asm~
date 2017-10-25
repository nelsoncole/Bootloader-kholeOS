; "puts16.asm"
;
; Projecto "Projecto KholeOS. Bootloader" 
; Nelson Sapalo da Silva Cole  (nelsoncole72@gmail.com  +244-948-833-132)
; Lubango 18 de Julho de 2017
;


bits 16
global puts16


; Display
puts16:
	pusha	
.next:
	cld 		
	lodsb		
	cmp al,0	
	je  .end	
	mov ah,0x0e
	int 0x10
	jmp .next
.end:
	popa
	ret



section .data

