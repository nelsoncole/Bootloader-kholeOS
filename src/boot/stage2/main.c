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
extern void   exet_kernel_i386();

unsigned int *buffer=(unsigned int *)0x100000;

void main(){


    idt_install();
    timer_install(1);
    keyboard_install();
    sti();
    printboot("[*] BootLoader: kholeOS!\n");
    printboot("[*] Testing ...\n");

    

    mount_disk(BootDevice);
    if((read("kernel/kernel.bin",buffer))!=0){
                    set_color(4);
                    printboot("[*] Erro ao ler arquivo \"kernel/kernel.bin\"");
		            for(;;);
                    set_color(0xf);
            }else puts("\n[*] Kernel.bin   ");
   

    int count;
    for(count =9;count >=0;--count){
      
        if(count > 9)printboot("\b\b%i",count);
        else  if(count == 9)printboot("\b\b%i",count);
        else printboot("\b%i",count);

        if(count ==0)break;
        else delay(1000);
                
    }


	    clear();
            exet_kernel_i386();
            
}
