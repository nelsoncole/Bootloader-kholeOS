#ifndef __ATA_INFO_H__
#define __ATA_INFO_H__

#include <typedef.h>


typedef struct ata_info_s {

	WORD resrved0;			//0
	WORD obsolete1; 		//1
	WORD specific_configuration; 	//2
	short reserved1[20]; 		//3-22
	char firmware_revision[8];	//23-26 Firmware revision (8 ASCII characters)
	char model_number[40];		//27-46 Model number (40 ASCII characters)
	short reserved2[2];		//47-48 
	WORD capabilities;		/*49 Capabilities
					11 --> 1 = IORDY supported, 0 = IORDY may be supported
					10 --> 1 = IORDY may be disabled
					9 --> 1 = LBA supported
					8 --> 1 = DMA supported.
					7-0 --> Retired */
	short reserved3[10];		//50-59
	DWORD total_number_of_user_addr_sector_lba28;	//60-61 Total number of user addressable sectors
	short reserved4[2];		//62-63
	WORD pio_modes_supported;	//64  15-8 --> Reserved, 7-0 --> PIO modes supported
	short reserved5[15];		//65-79
	WORD major_version_number;	//80
	short reserved6[12];		//81-92
	WORD devices_info;		//93
	short reserved7[6];		//94-99
	QWORD total_number_of_user_addr_sector_lba48; //100-103
	short reserved[152];		//104-255




}ata_info_s_t;

ata_info_s_t *ata_info;






#endif
