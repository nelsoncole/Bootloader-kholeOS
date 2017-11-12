#include <boot.h>
#include <fs.h>
#include <ata.h>






unsigned char mount_disk(){






   return 0; 


}








unsigned copy_path (char *s1, char *s2,unsigned start)
{
    
        
        for (;s2[start] != '/';start++)
        {
             if(s2[start] != 0)s1[start] =s2[start];
             else return start;
        }

        return start;

}
