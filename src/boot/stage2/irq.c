#include <boot.h>
#include <typedef.h>


extern void irq00();	extern void irq08();
extern void irq01();	extern void irq09();
extern void irq02();	extern void irq10();
extern void irq03();	extern void irq11();
extern void irq04();	extern void irq12();
extern void irq05();	extern void irq13();
extern void irq06();	extern void irq14();
extern void irq07();	extern void irq15();




void irq_install(){

	pic_install();
	interrupter(32,(DWORD)irq00,0x8,0);
	interrupter(33,(DWORD)irq01,0x8,0);
	interrupter(34,(DWORD)irq02,0x8,0);
	interrupter(35,(DWORD)irq03,0x8,0);
	interrupter(36,(DWORD)irq04,0x8,0);
	interrupter(37,(DWORD)irq05,0x8,0);
	interrupter(38,(DWORD)irq06,0x8,0);
	interrupter(39,(DWORD)irq07,0x8,0);
	interrupter(40,(DWORD)irq08,0x8,0);
	interrupter(41,(DWORD)irq09,0x8,0);
	interrupter(42,(DWORD)irq10,0x8,0);
	interrupter(43,(DWORD)irq11,0x8,0);
	interrupter(44,(DWORD)irq12,0x8,0);
	interrupter(45,(DWORD)irq13,0x8,0);
	interrupter(46,(DWORD)irq14,0x8,0);
	interrupter(47,(DWORD)irq15,0x8,0);

}


void irq_enable(int irq){
	
	WORD OCW1 = 0xFFFB;
	
	OCW1 &= ~(1 << irq);
	
	if(irq < 8 ) outportb(0x21, inportb(0x21) & OCW1);
	else {
	irq -= 8;
	
	outportb(0xA1,inportb(0xA1) & (OCW1 >> 8));}

}

void irq_disable(int irq){

	WORD OCW1 = 0x0000;
	
	OCW1  |= (1 << irq);
	
	if(irq < 8 )outportb(0x21, inb(0x21) | OCW1);
	else outportb(0xA1,inportb(0xA1) | ( OCW1 >> 8));

}


void irq_interrupter(DWORD irq){

	irq -= 32;

switch (irq){ 

	case 0:
		timer_irq();
		break;
	case 1:
		//keyboard_irq();
		break;
	case 2:
		break;

		case 3: 

		break;

		case 4: 

		break;

		case 5: 

		break;

		case 6: 

		break;

		case 7: 

		break;

	case 8: 
		break;

		case 9:

		break;

		case 10: 
		break;

		case 11:
		break;

	case 12:
		break;

		case 13: 
		break;

		case 14:
		break;

		case 15:

		break;
		
		default:

		printboot("\nFalha com IRQ%d",irq);

	}
        
        if(irq >=8 )outportb(0xA0,0x20);
	    outportb(0x20,0x20);
}
