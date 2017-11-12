#ifndef __FAT_H__
#define __FAT_H__
#include <typedef.h>


unsigned char *mount_fat(BYTE dev);
unsigned fat_read_file(char *path,void *directory,void *physical_memory,BYTE dev,BYTE flags);








#endif
