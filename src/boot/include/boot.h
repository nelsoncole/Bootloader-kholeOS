/* Arquivo "boot.h"
	"Projecto KholeOS. Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com)
	
	Lubango, 20 de Julho de 2017
*/



#ifndef __BOOT_H__
#define __BOOT_H__

#define inportb(p)({\
	unsigned char val;\
	__asm__ __volatile__(\
		"inb %%dx,%%al":"=a"(val):"d"(p));\
	val; })

#define outportb(p,val)__asm__ __volatile__(\
		"outb %%al,%%dx"::"a"(val),"d"(p))



typedef unsigned char BYTE;
typedef	unsigned short WORD;
typedef	unsigned long DWORD;
typedef	unsigned long long QWORD;

//INLINE IO.C

unsigned char inb(unsigned short port);
void outb(unsigned int port, unsigned int val);





//BOOT.C
void update_cursor();
void set_cursor(int x, int y);
void set_color(const char cor);
void putch (char c);
void puts(char *string);
void printboot(const char *args, ...);









#endif
