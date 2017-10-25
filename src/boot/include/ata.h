/* 	Arquivo "ata.h"
   
	"Projecto KholeOS, Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com  +244-948-833-132)

  	Lubango, 15 de Setembro de 2017

*/

#ifndef __ATA_H__
#define __ATA_H__

#include <ata_info.h>

#define CMD_READ_SECTORS_EXT 0x24
#define CMD_IDENTIFY_DEVICE 0xEC
#define MASTER_DEV 0x40
#define SLAVE_DEV 0x50
#define DRQ 0x8
#define DRDY 0x40  
#define BSY 0x80
#define ERR 0x1
#define SRST 0x4



void select_device (int local_bus, unsigned char device);
unsigned char command_select(int local_bus,unsigned char command);
char define_bus(unsigned char device );  
void soft_reset(int local_bus);

void read_ata_pio(BYTE device,WORD sector_count,QWORD sector, void *buffer);

void identify_device(BYTE device,unsigned char command);


#endif
