#include <boot.h>
#include <fat.h>
#include <ata.h>



static unsigned *cmp_path (char *s1, char *s2,unsigned start);


struct bpb_s bpb;
struct data_s *data;


BYTE DrvNum = 0x80;


void fat_init(char *path){


    unsigned int EOF;

    unsigned pathsz = strlen (path);

    int i,c;
    unsigned char copy_bpb[512];
    unsigned char *root_directory = (unsigned char *) 0x01000000;
    unsigned char *fat_table= (unsigned char *) 0x01100000;
    struct directory_s st_directory;

    char buffer_tmp [32];
    char name[26];
    char buffer_path[255];
    
    DWORD offset = 0;

    DWORD table_value = 0;

 
        
    WORD sector_count;
    QWORD lba_start;
    DWORD N;



    //int

    read_sector_ata_pio(0x80,1,512,0,copy_bpb);

  // __asm__ __volatile__ ("cld; rep; movsb"::"c"(90),"D"(&bpb),"S"(buffer));

    strncpy(&bpb,copy_bpb,90);
    


    /* Determina o tipo de FAT montado no volume, o tipo de fat é determinado pelo número total de clusters em partiçao  */

    data->root_directory_sectors =((bpb.BPB_RootEntCnt * 32)+(bpb.BPB_BytsPerSec - 1))/bpb.BPB_BytsPerSec; // para fat32 e sempre zero
    data->fat_size = (bpb.BPB_FATSz16 == 0)? bpb.specific.fat32.BPB_FATSz32 : bpb.BPB_FATSz16;
    data->volume_total_sectors = (bpb.BPB_TotSec16 == 0)? bpb.BPB_TotSec32 : bpb.BPB_TotSec16;
    data->data_sectors = data->volume_total_sectors - (bpb.BPB_RsvdSecCnt + (bpb.BPB_NumFATs * data->fat_size) +  data->root_directory_sectors); 
    data->count_of_clusters = data->data_sectors / bpb.BPB_SecPerClus;


 


 
    if (data->count_of_clusters < 4085 && bpb.BPB_FATSz16 != 0)
    {
        data->fat_type = FAT12;
    }
    else if (data->count_of_clusters < 65525 && bpb.BPB_FATSz16 != 0)
    {
        data->fat_type = FAT16;
    }
    else if (data->count_of_clusters < 268435445)
    {
        data->fat_type = FAT32;
    }
    else 
    {
        data->fat_type = ExFAT;
        printboot("Sem suporte, o volume deve ser ExFAT");
        return;

    }

    /*determine FAT offset, tamanho da FAT em bytes */


    switch (data->fat_type)
    {
        case 12:
            data->fat_offset = data->fat_size + (data->fat_size/2); // div 1.5
            break;
        case 16:
            data->fat_offset = data->fat_size * 2;
            break;
        case 32:
            data->fat_offset = data->fat_size * 4;
            break;
        default:
            // O volume eve ser ExFAT
            break;
    }

    


    /* Tamanho da FAT em sectors  */
    data->fat_total_sectors = bpb.BPB_RsvdSecCnt + (data->fat_offset / bpb.BPB_BytsPerSec);

    data->fat_ent_offset = data->fat_offset % bpb.BPB_BytsPerSec;


    
    /* Reading Directory */

    data->first_data_sector = bpb.BPB_RsvdSecCnt + (bpb.BPB_NumFATs * data->fat_size) + data->root_directory_sectors;
    data->root_cluster = bpb.specific.fat32.BPB_RootClus;


    if (data->fat_type == FAT12 || data->fat_type == FAT16)
    {        
        

        // FIXME 1
        // Lendo a File Allocation Table 12 e 16
        
        sector_count = data->fat_total_sectors;

        lba_start = bpb.BPB_RsvdSecCnt;
        
        read_sector_ata_pio(bpb.specific.fat12_or_fat16.BS_DrvNum,sector_count,bpb.BPB_BytsPerSec,lba_start,fat_table);

        // Lendo o Root Directory



        data->first_root_directory_sector_num = data->first_data_sector - data->root_directory_sectors;

        sector_count = data->root_directory_sectors;

        lba_start = data->first_root_directory_sector_num &0xffffffff;

        

 
       read_sector_ata_pio(bpb.specific.fat12_or_fat16.BS_DrvNum,sector_count,bpb.BPB_BytsPerSec,lba_start,root_directory);

        

        
          
    }
    else if (data->fat_type == FAT32)
    {


        EOF = 0x0fffffff;
    
        // FIXME 2
        // Lendo a File Allocation Table 32
        
        sector_count = data->fat_total_sectors;

        lba_start = bpb.BPB_RsvdSecCnt &0xffffffff;
        
        read_sector_ata_pio(bpb.specific.fat32.BS_DrvNum,sector_count,bpb.BPB_BytsPerSec,lba_start,fat_table);



        // Lendo o first Directory ou Root Directory


        N = data->root_cluster;
        sector_count = bpb.BPB_SecPerClus;


    do{
            data->first_sector_of_cluster = ((  N -2)*bpb.BPB_SecPerClus) + data->first_data_sector;

            lba_start = data->first_sector_of_cluster;

            read_sector_ata_pio(bpb.specific.fat32.BS_DrvNum,sector_count,512,lba_start,root_directory + offset);


            table_value = fat_table [N * 4 + 0];
            table_value = table_value | (fat_table [N * 4 + 1] << 8); 
            table_value = table_value | (fat_table [N * 4 + 2] << 16);
            table_value = table_value | (fat_table [N * 4 + 3] << 24);
            
            if ( table_value != EOF){ 
                N = table_value;
                offset =  offset + (bpb.BPB_BytsPerSec * bpb.BPB_SecPerClus);
                }else offset = 0;
        

            

        }while(table_value !=EOF);


    }


    //Aqui vamos ler os arquivos a partir dos dados do directório raízh


    // path  ./sys/kernel.bin


goto_0:

       offset = 0;
       int offset_dir =0;





GOTO_1:    if(root_directory[1 + offset_dir]==0) goto GOTO_9; // Terminar
     
GOTO_2:    if(root_directory[1+ offset_dir]==0xE5) goto GOTO_8; //Entrada não será utilizada

GOTO_3:    if(root_directory[11 + offset_dir]==0xF) goto GOTO_4; //Se igual a 0xF, entrada de LFN 
           else goto GOTO_5; // Entrada normal

GOTO_4:    // Ler a parte do nome do arquivo longo em um buffer temporário, GOTO_8
            
           strncpy( buffer_tmp ,root_directory + offset_dir,32);

            

            goto GOTO_8;


        
GOTO_5:    // Analizar a entrada, e extrair os dados para mais tarde poder utilizar. GOTO_6

            strncpy( &st_directory,root_directory + offset_dir,32);
            


GOTO_6:    // Existe um nome de arquivo longo no buffer temporário? Sim, GOTO_7.Não, GOTO_8


            if(buffer_tmp[11] == 0xF){

            //compara nome de arquivo lfn

            int index =1;
            c = 0;
            for(i=1;i < 11; i++)
            {

                if(buffer_tmp[i] != 0)
                {
                 name[c] = buffer_tmp[index];
                 c++;
                }
                index++;
            }

             index =14;
           for(i=14;i < 26; i++)
            {
                if(buffer_tmp[i] != 0)
                {
                 name[c] = buffer_tmp[index];
                 c++;
                }
                index++;
            }
               
                index =28;
             for(i=28;i < 32; i++)
            {
                if(buffer_tmp[i] != 0)
                 {
                 name[c] = buffer_tmp[index];
                 c++;
                }
                 index++;
            }
    
            memset(buffer_tmp,0,32);

             printboot("\n>>>  %s ",name);


            }else{

            // compara de aquivo padrao

             strncpy(name,&st_directory,11);
  

        }


       //FIXME aqui vamos ler o directorio ou o arquivo seguindo cadei de cluster


       c  = cmp_path (buffer_path,path,0); // funcao retorna o tamanho se char for '/' 

       if((strcmpb(name,buffer_path,c)) != 0){



              // Lendo o first cluster


       if(data->fat_type == FAT12 || data->fat_type == FAT16)

        {

            
                    N = st_directory.DIR_FstClusLO &0xffff;

                    DrvNum = bpb.specific.fat12_or_fat16.BS_DrvNum;   


             
        }
       else if (data->fat_type == FAT32)
        {

                    N = st_directory.DIR_FstClusLO;
                    N = N | (st_directory.DIR_FstClusHI << 16);

                    DrvNum = bpb.specific.fat32.BS_DrvNum;


            } else return;


    do{


            data->first_sector_of_cluster = ((  N - 2)*bpb.BPB_SecPerClus) + data->first_data_sector;

            lba_start = data->first_sector_of_cluster;

            read_sector_ata_pio(DrvNum,sector_count,512,lba_start,root_directory + offset);

        if (data->fat_type == FAT12)
        {

           if((N%2)==0){
                table_value = fat_table [N+(N/2)];
                table_value = table_value | (fat_table [N + (N/2) + 1 ] << 8);
                table_value = table_value &0xfff;


                }   
                
           else {

                table_value = fat_table [N+(N/2)];
                table_value = table_value | (fat_table [N + (N/2) + 1 ] << 8);

                table_value = table_value >> 4 &0xfff;
        }
 

            


               EOF = 0x00000fff;
        
        }

        else if (data->fat_type == FAT16)
        {

            table_value = fat_table [N * 2 + 0];
            table_value = table_value | (fat_table [N * 2 + 1] << 8);

            EOF = 0x0000ffff;

        }


        else if (data->fat_type == FAT32)
        {
            table_value = fat_table [N * 4 + 0];
            table_value = table_value | (fat_table [N * 4 + 1] << 8); 
            table_value = table_value | (fat_table [N * 4 + 2] << 16);
            table_value = table_value | (fat_table [N * 4 + 3] << 24);


            EOF = 0x0fffffff;

        }
            
            if ( table_value != EOF){ 
                N = table_value;
                offset =  offset + (bpb.BPB_BytsPerSec * bpb.BPB_SecPerClus);
                }else offset = 0;
        

            

        }while(table_value != EOF);

                     goto goto_0;

                 



            }


            else{


                memset(name,0,c);
                memset(buffer_path,0,c);
        
                goto GOTO_8;
                } 

   

        
    

GOTO_7:    // Aplique o nome de arquivo longo para entrada que voce acabou de ler e limpe o buffer temporário. GOTO_8

             

GOTO_8:   

           offset_dir = offset_dir + 32;
           goto GOTO_1;

GOTO_9: 

   

    
        return;
        


}




static unsigned *cmp_path (char *s1, char *s2,unsigned start)
{
    
        
        for (;s2[start] != '/';start++)
        {
             if(s2[start] != 0)s1[start] =s2[start];
             else return start;
        }

        return start;

}



















