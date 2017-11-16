#ifndef __FAT_H__
#define __FAT_H__
#include <typedef.h>


unsigned char * mount_fat(BYTE dev_t);
_Bool fat_read_file(char *path,void *physical_memory,BYTE flags);

unsigned char mount_disk(unsigned char dev);


_Bool read(char *path,void *buffer);



#endif
