BITS 32

GLOBAL idt_m
EXTERN idt
idt_m:


    lidt [idt]

    ret
