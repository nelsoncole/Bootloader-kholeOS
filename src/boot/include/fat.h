#ifndef __FAT_H__
#define __FAT_H__

#include <typedef.h>


#define FAT12 12
#define FAT16 16
#define FAT32 32
#define ExFAT 0


#define FAT_ATTR_READ_ONLY	0x01
#define FAT_ATTR_HIDDEN		0x02
#define FAT_ATTR_SYSTEM		0x04
#define FAT_ATTR_VOLUME_ID	0x08
#define FAT_ATTR_DIRECTORY	0x10
#define FAT_ATTR_ARCHIVE		0x20
#define FAT_ATTR_LONG_NAME	(FAT_ATTR_READ_ONLY | FAT_ATTR_HIDDEN\
 				| FAT_ATTR_SYSTEM| FAT_ATTR_VOLUME_ID)

#define FAT_DIR_ENTRY_SIZE	32


struct bpb_s 
{

	char BS_jmpBoot[3];
	char BS_OEMName[8];
	WORD BPB_BytsPerSec;
	BYTE BPB_SecPerClus;
	WORD BPB_RsvdSecCnt;
	BYTE BPB_NumFATs;
	WORD BPB_RootEntCnt;
	WORD BPB_TotSec16;
	BYTE BPB_Media;
	WORD BPB_FATSz16;
	WORD BPB_SecPertrk;
	WORD BPB_NumHeads;
	DWORD BPB_HiddSec;
	DWORD BPB_TotSec32;
	
union {
 	struct 
    {// Para FAT12/FAT16

    	BYTE BS_DrvNum;
    	BYTE BS_Reserved1;
    	BYTE BS_BootSig;
	    DWORD BS_VolID;
	    char BS_VolLab[11];
	    char BS_FilSysType[8];
		
	}__attribute__ ((packed)) fat12_or_fat16;

            struct 
            { // Para FAT32
	
	            DWORD BPB_FATSz32;
	            WORD BPB_ExtFlags;
	            WORD BPB_FSVer;
	            DWORD BPB_RootClus;
	            WORD BPB_FSInfo;
	            WORD BPB_BkBootSec;
	            char BPB_Reserved[12];
	            BYTE BS_DrvNum;
	            BYTE BS_Reserved1;
	            BYTE BS_BootSig;
	            DWORD BS_VolID;
	            char BS_VolLab[11];
	            char BS_FilSysType[8];

	        }__attribute__ ((packed)) fat32;
	}__attribute__ ((packed)) specific;
}__attribute__ ((packed));

struct directory_s
{
	
	char DIR_Name[8];
	char DIR_Name_Ext[3];
	BYTE DIR_Attr;
	BYTE DIR_NTRes;
	BYTE DIR_CrtTimeTenth;
	WORD DIR_CrtTime;
	WORD DIR_CrtDate;
	WORD DIR_LstAccDate;
	WORD DIR_FstClusHI;
	WORD DIR_WrtTime;
	WORD DIR_WrtDate;
	WORD DIR_FstClusLO;
	DWORD DIR_FileSize;
	
}__attribute__ ((packed));



struct data_s
{


    DWORD count_of_clusters;
    DWORD data_sectors;
    DWORD first_data_sector;
    DWORD first_fat_sector;
    DWORD fat_offset;
    DWORD first_root_directory_sector_num;
    DWORD first_sector_of_cluster;
    DWORD fat_size;
    DWORD fat_type;
    DWORD root_cluster;
    DWORD root_directory_sectors;
    DWORD volume_total_sectors;

    DWORD fat_total_sectors;
    DWORD fat_ent_offset;
  


}__attribute__ ((packed));




void fat_init();








#endif
