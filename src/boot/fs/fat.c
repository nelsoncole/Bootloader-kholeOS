#include <boot.h>
#include <fs.h>
#include <ata.h>
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




 

unsigned char *root_directory = (unsigned char *) 0x01000000; //Limit 2Mb
unsigned char *directory = (unsigned char *) 0x01200000; //Limit 2Mb
unsigned char *fat_table = (unsigned char *) 0x2000000;


WORD sector_count;
QWORD lba_start =0;

unsigned int EOF;

struct directory_s st_directory;
struct bpb_s bpb;
struct data_s *data;



unsigned char dev;


// FUNCOES A FAT

unsigned char *mount_fat(BYTE dev_t){

    DWORD offset = 0;
    DWORD N;
    DWORD table_value;
     

    unsigned char copy_bpb[512];
    

    //init FAT

    dev = dev_t;

    read_sector_ata_pio(dev,1,512,0,copy_bpb); //FIXME Aqui devo ler o primeiro sector da partição (Boot Record da FAT) e não o do disco

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
        return 0;

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


    
    /*   FIXME Read FAT*/

    data->first_data_sector = bpb.BPB_RsvdSecCnt + (bpb.BPB_NumFATs * data->fat_size) + data->root_directory_sectors;
    data->root_cluster = bpb.specific.fat32.BPB_RootClus;


        // Lendo a File Allocation Table 12, 16 ou 32
        
        sector_count = data->fat_total_sectors;



        lba_start = bpb.BPB_RsvdSecCnt;
        read_sector_ata_pio(dev,data->fat_size,bpb.BPB_BytsPerSec,lba_start,fat_table);

// FIXME Lendo o Root Directory

    if (data->fat_type == FAT12 || data->fat_type == FAT16)
    {        



        data->first_root_directory_sector_num = data->first_data_sector - data->root_directory_sectors;

        sector_count = data->root_directory_sectors;

        lba_start = data->first_root_directory_sector_num &0xffffffff;

         read_sector_ata_pio(dev,sector_count,bpb.BPB_BytsPerSec,lba_start,root_directory);
        
          
    }
    else if (data->fat_type == FAT32)
    {


        EOF = 0x0fffffff;

        // Lendo o first Directory ou Root Directory


        N = data->root_cluster;
        sector_count = bpb.BPB_SecPerClus;


    do{
            data->first_sector_of_cluster = ((  N -2)*bpb.BPB_SecPerClus) + data->first_data_sector;

            lba_start = data->first_sector_of_cluster;

            read_sector_ata_pio(dev,sector_count,bpb.BPB_BytsPerSec,lba_start,root_directory + offset);


            table_value = fat_table [N * 4 + 0];
            table_value = table_value | (fat_table [N * 4 + 1] << 8); 
            table_value = table_value | (fat_table [N * 4 + 2] << 16);
            table_value = table_value | ((fat_table [N * 4 + 3] << 24) &0x0f); 
         
            if ( table_value != EOF){ 
                N = table_value;
                offset =  offset + (bpb.BPB_BytsPerSec * bpb.BPB_SecPerClus);
                }else offset = 0;  

            

        }while(table_value !=EOF);


    }
            
       
       return root_directory;


}




//FUNCAO FAT READ DIRECTORY/FILE

_Bool fat_read_file(char *path,void *physical_memory,BYTE flags){


    int i,c;
    char buffer_tmp [32];
    char name[32]; 
    BYTE *search;
    int offset_dir = 0;
    DWORD offset = 0;
    DWORD table_value;
    DWORD N;
    //DWORD lba_start = 0;

    
    memset(name,0,32);

// FIXME Aqui vamos ler os arquivos a partir dos dados do directório raíz

    
            
        if(flags == 0) search = root_directory;
        else if(flags == 1)search = directory; 
        offset_dir = 0;




goto_1:    if(search[1 + offset_dir]==0) return 1; // FIXME errro
     
           if(search[1+ offset_dir]==0xE5) goto goto_2; //Entrada não será utilizada

           if(search[11 + offset_dir]==FAT_ATTR_LONG_NAME) {//Se igual a 0xF, entrada de LFN


                    // Ler a parte do nome do arquivo longo em um buffer temporário, goto_8 

                    strncpy( buffer_tmp ,search + offset_dir,32);
                    goto goto_2;

            }
           else{ // Entrada padrão 8.3


                    // Analizar a entrada, e extrair os dados para mais tarde poder utilizar.

                    strncpy( &st_directory,search + offset_dir,32);


             }    

            
            // Existe um nome de arquivo longo no buffer temporário?


            if(buffer_tmp[11] == 0xF){

            //compara nome de arquivo LFN

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

            }else{

            // Nome de arquivo padrao 8.3

             strncpy(name,&st_directory,11);

        }


       //FIXME aqui vamos ler o sub directorio ou o arquivo seguindo cadei de cluster


       if((strcmpb(name,path,strlen (path))) == 0){
            

              // Lendo o first cluster


       if(data->fat_type == FAT12 || data->fat_type == FAT16)

        {

                    N = st_directory.DIR_FstClusLO &0xffff;   
             
        }
       else if (data->fat_type == FAT32)
        {

                    N = st_directory.DIR_FstClusLO;
                    N = N | (st_directory.DIR_FstClusHI << 16);

                  

            } else return 3; //FIXME Volume deve ser ExFAT, erro


    do{


            data->first_sector_of_cluster = ((  N - 2)*bpb.BPB_SecPerClus) + data->first_data_sector;

            lba_start = data->first_sector_of_cluster &0xFFFFFFFF;


            //FIXME analiza se entrada é arquivo ou directório


        if(st_directory.DIR_Attr == FAT_ATTR_DIRECTORY) 
                read_sector_ata_pio(dev,bpb.BPB_SecPerClus,bpb.BPB_BytsPerSec,lba_start,directory + offset);
        else if(st_directory.DIR_Attr == FAT_ATTR_ARCHIVE ) 
                read_sector_ata_pio(dev,bpb.BPB_SecPerClus,bpb.BPB_BytsPerSec,lba_start,physical_memory + offset);

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
      


               EOF = 0xfff;
        
        }

        else if (data->fat_type == FAT16)
        {

            table_value = fat_table [N * 2 + 0];
            table_value = table_value | (fat_table [N * 2 + 1] << 8);

            EOF = 0xffff;

        }


        else if (data->fat_type == FAT32)
        {
            
            table_value = fat_table [N * 4 + 0];
            table_value = table_value | (fat_table [N * 4 + 1] << 8);
            table_value = table_value | (fat_table [N * 4 + 2] << 16);
            table_value = table_value | ((fat_table [N * 4 + 3] << 24) &0x0f);

            EOF = 0xfffffff;

        }
            
            if ( table_value != EOF){ 
                N = table_value;
                offset =  offset + (bpb.BPB_BytsPerSec * bpb.BPB_SecPerClus);
                }else offset = 0;

	 }while(table_value != EOF); 




                    return 0;  //FIXME sucesso
     } else
            {


                // Entrada ainda não encontrada, incrementa ponteiro no goto_8
               memset(name,0,strlen (path));
                } //final do bloco if((strcmpb(name,path,strlen (path))) != 0)

  

goto_2:   

           // incrementa ponteiro
           offset_dir = offset_dir + 32;
           goto goto_1;


}



















