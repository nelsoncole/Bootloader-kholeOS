/* Arquivo "main.c"
	"Projecto KholeOS. Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com)
	
	Lubango, 20 de Julho de 2017
*/

#include <boot.h>
#include <ata.h>
#include <fs.h>
#include <typedef.h>


extern unsigned char BootDevice;
extern void flush();

unsigned int *buffer=(unsigned int *)0x100000;

void main(){


    idt_install();
    timer_install(1);
    keyboard_install();
    sti();
    printboot("[*] Operational System: kholeOS!\n");
    


    mount_disk(0x80);
    if((read("kernel/kernel.bin",buffer))!=0){
                    set_color(4);
                    printboot("[*] Erro ao ler arquivo \"kernel/kernel.bin\"");
                    set_color(0xf);
            }else puts("[*] Kernel.bin   ");
   

/*    int count;
    for(count =1;count >=0;--count){
      
        if(count > 9)printboot("\b\b%i",count);
        else  if(count == 9)printboot("\b\b%i",count);
        else printboot("\b%i",count);
        _100ns(1000000);
                
    }*/

            flush();
            
}
