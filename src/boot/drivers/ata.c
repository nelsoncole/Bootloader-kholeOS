

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




int BARS[5][8];











static unsigned ide_io [2][9]={
	{0x1F0,0x1F1,0x1F2,0x1F3,0x1F4,0x1F5,0x1F6,0x1F7,0x3F6}, // Barramento primário
	{0x170,0x171,0x172,0x173,0x174,0x175,0x176,0x177,0x376}  // Barramento secundário

};

static char *ata_msg[2]={
"(Primary Bus, Master Device)",
"(Secundary Bus, Slava Device)"

};



void read_sector_ata_pio(BYTE device,WORD sector_count, unsigned int bytes_per_sector,QWORD sector, void *buffer){



    int local_bus;
    unsigned int offset = bytes_per_sector;
    int count =0;
	local_bus = define_bus(device);

    

    bytes_per_sector = bytes_per_sector/2;

do{

    	cli();

    
	//soft_reset(local_bus);

	// Set sector LBA e sector count
	outb(ide_io[local_bus][2],0);	// Sector Count 15:8
	outb(ide_io[local_bus][3],sector >> 24);	// LBA 31-24   
	outb(ide_io[local_bus][4],sector >> 32);	// LBA 39-32
	outb(ide_io[local_bus][5],sector >> 40);	// LBA 47-40
	outb(ide_io[local_bus][2],1);	// Sector Count 7:0
	outb(ide_io[local_bus][3],sector);		// LBA 7-0   
	outb(ide_io[local_bus][4],sector >> 8);		// LBA 15-8
	outb(ide_io[local_bus][5],sector >> 16);	// LBA 23-16
	 select_device (local_bus,device);  //SELECT DEVICE
	
	if((command_select(local_bus,CMD_READ_SECTORS_EXT)) &DRQ)
	{

       
		__asm__ __volatile__("rep; insw"\
 	                 	     ::"D"(buffer+ (count *offset)),"d"(ide_io[local_bus][0]),"c"(bytes_per_sector*1));	
		}
        else {
		
		set_color(4);
		printboot("\n[Error. PIO (Programmed Input/Output) data transfer. %s ]",ata_msg[local_bus]);
		set_color(0xF);	
		soft_reset(local_bus);


		}

        count++;
        sector ++;

      	sti();

    }while(--sector_count);

  
	   

}





void soft_reset(int local_bus){

	outb(ide_io[local_bus][8],inb(ide_io[local_bus][8])| SRST);
	outb(ide_io[local_bus][8],inb(ide_io[local_bus][8]) &0xFB); //Precizamos redefinir o bit 2

}




void select_device (int local_bus, unsigned char device){

	/* 
	Device register:

	Bit  4   --->> Drive Select 0,1
	Bit  6   --->> =0 usa CHS, =1 usa LBA28 ou LBA48

*/   
	
	switch(local_bus)
		{ 
		case 0:outb(ide_io[local_bus][6],MASTER_DEV);
			break;
		case 1:outb(ide_io[local_bus][6],SLAVE_DEV);
			break;
		}
} 



unsigned char command_select(int local_bus,unsigned char command){



	unsigned char status_register;
	unsigned char busy, dev_ready, data_request,error;


	    outb(ide_io[local_bus][7],command);   // Command Read Sector
	    _100ns(4);  // Esperamos 400ns

	do{
          //Aqui devemos esperar 400ns    	
        status_register = inportb(ide_io[local_bus][7]);
    		
        busy = status_register &BSY;
   		dev_ready = status_register &DRDY;
		data_request = status_register &DRQ;
        error = status_register &ERR;

		if(error == ERR){

			set_color(4);
			printboot("\n[ Error write Command Register %x ]",CMD_READ_SECTORS_EXT);
			set_color(0xF);
			soft_reset(local_bus);
			return status_register;
		}

  	}while (busy || !dev_ready || !data_request);

	

	return status_register;

}


char define_bus(unsigned char device ){
	
	unsigned char bus;

	if(!(device & 1)) bus = 0;
	else bus = 1;

	return bus;

}








