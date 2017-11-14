#include <boot.h>
#include <fs.h>
#include <ata.h>
#include <typedef.h>


BYTE index_dev = 0;
BYTE index_local = 0;

static unsigned copy_path (char *s1, char *s2,unsigned start);

unsigned char mount_disk(unsigned char dev){


    

    mount_fat(dev); 
   
    return 0; //FIXME


}



_Bool read(char *path,void *buffer){
    
        char tmp_path[256];
        int count =0;
        int i =2;
         

    do{
 
            count = copy_path (tmp_path,path,count);
            if((fat_read_file(tmp_path,buffer,index_local)) != 0)return 1;
            memset(tmp_path,0,count);        
            index_local++;
   
        }while(--i);
        return 0;

}




static unsigned copy_path (char *s1, char *s2,unsigned start)
{
        int i=0;

        int size =strlen (s2);
        
        for (;s2[start] != '/';start++)
        {
             if(start <= size)s1[i++] =s2[start];
             else return start;
        }

        return start + 1;

}
