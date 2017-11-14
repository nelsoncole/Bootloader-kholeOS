#include <boot.h>
#include <typedef.h>


void kbdc_wait(BYTE type){
        
    
	if(type==0){

                  while(!inb(0x64)&1)outanyb(0x80);
            }

	else {

                 while(inb(0x64)&2)outanyb(0x80);

      }

}


void keyboard_install(){
	
	//reseta o teclado

	kbdc_wait(1);
	outb(0x60,0xFF);

	irq_enable(1);
}


void keyboard_irq(){


    inb(0x60);


}
