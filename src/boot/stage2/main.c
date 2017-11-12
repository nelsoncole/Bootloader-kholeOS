/* Arquivo "main.c"
	"Projecto KholeOS. Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com)
	
	Lubango, 20 de Julho de 2017
*/

#include <boot.h>
#include <ata.h>
#include <fs.h>


extern unsigned char BootDevice;

unsigned char *dir=(unsigned char *)0x100000;

void main(){


    idt_install();
    timer_install(1);
    printboot("\t\t\tOperational System: kholeOS!\n\n");
    sti();

    printboot("\t\tTesting, the pit and exception (divide error) \n\n\t\t\t\t      ");

    int count;
    for(count =29;count !=0;--count){
        if(count > 9)printboot("\b\b%i",count);
        else  if(count == 9)printboot("\b\b%i",count);
        else printboot("\b%i",count);
        delay(1000);
        
        

        
    }
       puts("\n\n\t\t\t      ");
    __asm__ __volatile__("div %%edx"::);


   
    

    int i=0;
    for(;i<(80*25)/2;i++)printboot("%c",dir[i]);
            
	}
