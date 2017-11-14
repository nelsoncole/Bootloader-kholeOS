/* Arquivo "boot.h"
	"Projecto KholeOS. Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com)
	
	Lubango, 20 de Julho de 2017
*/



#ifndef __BOOT_H__
#define __BOOT_H__



#include <typedef.h>


#define outanyb(p) __asm__ __volatile__(\
		"outb %%al,%0"::"dN"((p)) :"eax"\
		)  /* Valeu Fred */


#define inportb(p)({\
	unsigned char val;\
	__asm__ __volatile__(\
		"inb %%dx,%%al":"=a"(val):"d"(p));\
	val; })

#define outportb(p,val)__asm__ __volatile__(\
		"outb %%al,%%dx"::"a"(val),"d"(p))

/*#define cli()__asm__ __volatile__ ("cli"::)
#define sti()__asm__ __volatile__ ("sti"::)*/

#define TRUE 1
#define FALSE 0
#define NULL 0


void cli();
void sti();

//INLINE IO.C

unsigned char inb(unsigned short port);
void outb(unsigned int port, unsigned int val);

void _100ns(int n);



//BOOT.C
void update_cursor();
void set_cursor(int x, int y);
void set_color(const char cor);
void putch (char c);
void puts(char *string);
void printboot(const char *args, ...);


// STRINGS

unsigned strlen ( const char* str );
int strncpy(void* s1, void* s2,unsigned count);
unsigned char strcmpb (void* s1, void* s2,unsigned count);
void* memset(void *s, char val, unsigned count);



//IDT

void idt_install();
void trap(int n,DWORD offset,WORD sel,BYTE dpl );
void interrupter(int n,DWORD offset,WORD sel,BYTE dpl );

// EXCEPTIONs and IRQs
void fault_exception(DWORD n);
void isr_install();

void irq_install();
void irq_interrupter(DWORD irq);
void irq_enable(int irq);
void irq_disable(int irq);


//PIC
void pic_install();


//TIMER ou PIT
void timer_install(BYTE flag);
void delay(DWORD milesegundo);
void timer_irq();


//KEYBOARD
void keyboard_irq();
void keyboard_install();




#endif
