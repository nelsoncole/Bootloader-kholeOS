/*


	Arquivo "io.c"
	"Projecto KholeOS. Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com)
	
	Lubango, 11 de Setembro de 2017


*/




#include <boot.h>
#include <typedef.h>



unsigned char inb(unsigned short port)
{
	unsigned char ret_val;
	asm volatile("inb %w1,%b0":"=a"(ret_val):"dN"(port));
	return ret_val;
}

void outb(unsigned int port, unsigned int val)
{
	asm volatile("outb %b0, %w1"::"a"(val),"dN"(port));
}


void _100ns(int n){

    do{
         outanyb(0x80);  //  porta inválida do DMAC,  extra page register (temporary storage)
        }while(--n);


}
