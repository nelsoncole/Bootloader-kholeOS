section .text
global isr00,isr01,isr02,isr03,isr04,isr05,isr06,isr07,isr08,isr09
global isr10,isr11,isr12,isr13,isr14,isr15,isr16,isr17,isr18,isr19
global isr20,isr21,isr22,isr23,isr24,isr25,isr26,isr27,isr28,isr29
global isr30,isr31

extern fault_exception
isr_jmp:
	push eax
	push ecx
	push edx
	push ebx
	push ebp
	push esi
	push edi
	push ds
	push es
	push fs
	push gs
	mov eax,[esp+44]	; pega vetor
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax

	call  fault_exception
	pop eax
	
	pop gs
	pop fs
	pop es
	pop ds
	pop edi
	pop esi	
	pop ebp
	pop ebx
	pop edx
	pop ecx 
	pop eax	
	add esp,8
	iret

isr00:
	push BYTE 0
	push BYTE 0
	jmp isr_jmp
isr01:
	push BYTE 0
	push BYTE 1
	jmp isr_jmp
isr02:
	push BYTE 0
	push BYTE 2
	jmp isr_jmp
isr03:
	push BYTE 0
	push BYTE 3
	jmp isr_jmp
isr04:
	push BYTE 0
	push BYTE 4
	jmp isr_jmp
isr05:
	push BYTE 0
	push BYTE 5
	jmp isr_jmp
isr06:
	push BYTE 0
	push BYTE 6
	jmp isr_jmp
isr07:
	push BYTE 0
	push BYTE 7
	jmp isr_jmp
isr08:
	;error code
	push BYTE 8
	jmp isr_jmp
isr09:
	push BYTE 0
	push BYTE 9
	jmp isr_jmp
isr10:
	;error code
	push BYTE 10
	jmp isr_jmp
isr11:
	;error code
	push DWORD 11
	jmp isr_jmp
isr12:
	;error code
	push BYTE 12
	jmp isr_jmp
isr13:
	;error code
	push BYTE 13
	jmp isr_jmp
isr14:
	;error code
	push BYTE 14
	jmp isr_jmp
isr15:
	push BYTE 0
	push BYTE 15
	jmp isr_jmp
isr16:
	push BYTE 0
	push BYTE 16
	jmp isr_jmp
isr17:
	;error code
	push BYTE 17
	jmp isr_jmp
isr18:
	push BYTE 0
	push BYTE 18
	jmp isr_jmp
isr19:
	push BYTE 0
	push BYTE 19
	jmp isr_jmp
isr20:
	push BYTE 0
	push BYTE 20
	jmp isr_jmp
isr21:
	push BYTE 0
	push BYTE 21
	jmp isr_jmp
isr22:
	push BYTE 0
	push BYTE 22
	jmp isr_jmp
isr23:
	push BYTE 0
	push BYTE 23
	jmp isr_jmp
isr24:
	push BYTE 0
	push BYTE 24
	jmp isr_jmp
isr25:
	push BYTE 0
	push BYTE 25
	jmp isr_jmp
isr26:
	push BYTE 0
	push BYTE 26
	jmp isr_jmp
isr27:
	push BYTE 0
	push BYTE 27
	jmp isr_jmp
isr28:
	push BYTE 0
	push BYTE 28
	jmp isr_jmp
isr29:
	push BYTE 0
	push BYTE 29
	jmp isr_jmp
isr30:
	push BYTE 0
	push BYTE 30
	jmp isr_jmp
isr31:
	push BYTE 0
	push BYTE 31
	jmp isr_jmp

global irq00,irq01,irq02,irq03,irq04,irq05,irq06,irq07,irq08,irq09
global irq10,irq11,irq12,irq13,irq14,irq15


extern irq_interrupter
irq00:

	push BYTE 0
	push BYTE 32

irq_jmp:
	push eax
	push ecx
	push edx
	push ebx
	push ebp
	push esi
	push edi
	push ds
	push es
	push fs
	push gs
	mov eax,[esp+44]	; pega vetor
	push eax
	mov eax,0x10
	mov ds,ax
	mov es,ax
	mov fs,ax
	mov gs,ax


	

	

	call irq_interrupter
	pop eax

	pop gs
	pop fs
	pop es
	pop ds
	pop edi
	pop esi	
	pop ebp
	pop ebx
	pop edx
	pop ecx 
	pop eax	
	add esp,8
	iret


irq01:

	push BYTE 0
	push BYTE 33
	jmp irq_jmp

irq02:

	push BYTE 0
	push BYTE 34
	jmp irq_jmp

irq03:

	push BYTE 0
	push BYTE 35
	jmp irq_jmp

irq04:

	push BYTE 0
	push BYTE 36
	jmp irq_jmp

irq05:

	push BYTE 0
	push BYTE 37
	jmp irq_jmp

irq06:

	push BYTE 0
	push BYTE 38
	jmp irq_jmp

irq07:

	push BYTE 0
	push BYTE 39
	jmp irq_jmp

irq08:

	push BYTE 0
	push BYTE 40
	jmp irq_jmp

irq09:

	push BYTE 0
	push BYTE 41
	jmp irq_jmp

irq10:

	push BYTE 0
	push BYTE 42
	jmp irq_jmp

irq11:

	push BYTE 0
	push BYTE 43
	jmp irq_jmp

irq12:

	push BYTE 0
	push BYTE 44
	jmp irq_jmp

irq13:

	push BYTE 0
	push BYTE 45
	jmp irq_jmp

irq14:

	push BYTE 0
	push BYTE 46
	jmp irq_jmp

irq15:

	push BYTE 0
	push BYTE 47
	jmp irq_jmp
