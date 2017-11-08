#include <boot.h>
#include <ata.h>
#include <ata_info.h>



static unsigned short IDE_IO [2][9]={
	{0x1F0,0x1F1,0x1F2,0x1F3,0x1F4,0x1F5,0x1F6,0x1F7,0x3F6}, // Barramento primário
	{0x170,0x171,0x172,0x173,0x174,0x175,0x176,0x177,0x376}  // Barramento secundário

};

static char *ata_msg[2]={
"(Primary Bus, Master Device)",
"(Secundary Bus, Slava Device)"

};

void identify_device(BYTE device,unsigned char command){

	unsigned char status_register;
	int local_bus;

	set_color(9);
	printboot("[ Detalhe de disco: ");

	
	local_bus = define_bus(device);

	//soft_reset(local_bus);

	select_device (local_bus,device);  //SELECT DEVICE 
	status_register = command_select(local_bus,CMD_IDENTIFY_DEVICE); // CMD
	
	if(status_register &DRQ)
	{
		__asm__ __volatile__("rep; insw"\
 	                 	     ::"D"(ata_info),"d"(IDE_IO[local_bus][0]),"c"(0x100));	
		}
        else {
		
		set_color(4);
		printboot("\n[Error. IDENTIFY DEVICE... %s ]",ata_msg[local_bus]);
		set_color(0xF);		
		soft_reset(local_bus);


		}
	
}
