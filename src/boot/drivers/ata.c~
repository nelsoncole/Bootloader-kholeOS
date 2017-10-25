

/* Arquivo "io.c"
   
	"Projecto KholeOS, Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com  +244-948-833-132)

  	Lubango, 18 de Julho de 2017

*/



/*




	Aqui iremos programar o driver de disco em modo PIO

	LBA48


*/
 

#include <boot.h>
#include <ata.h>


static unsigned short IDE_IO [2][9]={
	{0x1F0,0x1F1,0x1F2,0x1F3,0x1F4,0x1F5,0x1F6,0x1F7,0x3F6}, // Barramento primário
	{0x170,0x171,0x172,0x173,0x174,0x175,0x176,0x177,0x376}  // Barramento secundário

};


static char *ata_msg[2]={
"(Primary Bus, Master Device)",
"(Secundary Bus, Slava Device)"

};



void read_ata_pio(BYTE device,WORD sector_count,QWORD sector, void *buffer){


	printboot("\n\n[ Inicialize PIO (programmed input/output) data transfer ... ]");

	unsigned char status_register;
	int local_bus;



	local_bus = define_bus(device);
	//soft_reset(local_bus);



	// Set sector LBA e sector count
	outb(IDE_IO[local_bus][2],sector_count >> 8);	// Sector Count 15:8
	outb(IDE_IO[local_bus][3],sector >> 24);	// LBA 31-24   
	outb(IDE_IO[local_bus][4],sector >> 32);	// LBA 39-32
	outb(IDE_IO[local_bus][5],sector >> 40);	// LBA 47-40
	outb(IDE_IO[local_bus][2],sector_count);	// Sector Count 7:0
	outb(IDE_IO[local_bus][3],sector);		// LBA 7-0   
	outb(IDE_IO[local_bus][4],sector >> 8);		// LBA 15-8
	outb(IDE_IO[local_bus][5],sector >> 16);	// LBA 23-16



	select_device (local_bus,device);  //SELECT DEVICE
	status_register = command_select(local_bus,CMD_READ_SECTORS_EXT); // CMD
	
	if(status_register &DRQ)
	{
		__asm__ __volatile__("rep; insw"\
 	                 	     ::"D"(buffer),"d"(IDE_IO[local_bus][0]),"c"(0x100*sector_count));	
		}
        else {
		
		set_color(4);
		printboot("\n[Error. PIO (Programmed Input/Output) data transfer. %s ]",ata_msg[local_bus]);
		set_color(0xF);		
		soft_reset(local_bus);


		}


	printboot("\n[ Status register: %x ]",status_register);

}





void soft_reset(int local_bus){

	outb(IDE_IO[local_bus][8],inb(IDE_IO[local_bus][8]) | SRST);
	outb(IDE_IO[local_bus][8],inb(IDE_IO[local_bus][8]) &0xFB); //Precizamos redefinir o bit 2

}




void select_device (int local_bus, unsigned char device){


	unsigned char status, busy, dev_ready,data_request;
	int count=4;

	/* 
	Device register:

	Bit  4   --->> Drive Select 0,1
	Bit  6   --->> =0 usa CHS, =1 usa LBA28 ou LBA48

*/   
	
	switch(local_bus)
		{ 
		case 0:outb(IDE_IO[local_bus][6],MASTER_DEV);
			break;
		case 1:outb(IDE_IO[local_bus][6],SLAVE_DEV);
			break;
		}


		// Aqui esperamos 400ns, se BYS e DRQ for 1 arrobamos com um device reset ou soft reset

		do{

			status = inb(IDE_IO[local_bus][7]);
			busy = status &0x80;
			dev_ready = status &0x40;
			data_request = status &0x08;

		if(count == 0  && busy == 0x80){

			set_color(4);
			printboot("\n[ Not Select Device%i ]",local_bus);
			set_color(0xF);
			soft_reset(local_bus);
			return;

			}

		}while(count-- || busy || data_request || !dev_ready);
	 

} 



unsigned char command_select(int local_bus,unsigned char command){



	unsigned char status_register;
	unsigned char busy, dev_ready, data_request;
	int count=4;



	outb(IDE_IO[local_bus][7],command);   // Command Read Sector

	
	do{
    		status_register = inb(IDE_IO[local_bus][7]);
    		busy =status_register &0x80;
   		dev_ready =status_register &0x40;
		data_request = status_register &0x8;

		if(count == 0 && busy == 0x80){

			set_color(4);
			printboot("\n[ Error write Command Register %x ]",CMD_READ_SECTORS_EXT);
			set_color(0xF);
			soft_reset(local_bus);
			return status_register;
		}

  	}while (count-- || busy || !dev_ready || !data_request);

	

	return status_register;

}


char define_bus(unsigned char device ){
	
	unsigned char bus;

	if(!(device & 1)) bus = 0;
	else bus = 1;

	return bus;

}








