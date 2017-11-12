#include <boot.h>
#include <typedef.h>

#define FREQ 100

static void timer_wait(DWORD ticks);

DWORD timer_ticks =0 ;
DWORD  eticks;

void set_frequencia(DWORD freq){

	DWORD divisor = 1193180/freq;


	cli();
	outportb(0x43,0x36);	/* Control WORD register
				 * bits 7-6 = 0 - Set counter 0 (counter divisor),bit 5-4=11 LSB/MSB 16bits
				 * bit 3-1=x11 Square wave generator, bit 0 =0 Binary counter 16bits
				 */
	
	outportb(0x40,(BYTE)(divisor & 0xFF));		// 0x40 counter 0
	outportb(0x40,(BYTE)(divisor >> 8) & 0xFF);

	irq_enable(0);
	sti();
}


void timer_install(BYTE flag){

    set_frequencia(FREQ);

	if(flag == 1) irq_enable(0);
	else irq_disable(0);

}


void timer_irq(){

	++timer_ticks;
	if(eticks)--eticks;

}

void delay(DWORD milesegundo){

	timer_wait(FREQ * milesegundo/1000);

}



static void timer_wait(DWORD ticks){

	timer_install(FALSE);
	eticks = ticks;
	timer_install(TRUE);

	while(eticks){
		update_cursor();	
	}


	

}
