#include <boot.h>
#include <fat.h>
#include <ata.h>


#define EOF 0x0FFFFFFF


struct bpb_s bpb;
struct data_s *data;



void fat_init(){

    unsigned char copy_bpb[512];
    unsigned char *phys_root_dir = (unsigned char *) 0x01000000;
    unsigned char *phys_fat = (unsigned char *) 0x01100000;
    DWORD offset = 0;

    DWORD table_value = 0;

    unsigned char fat_table[512]; //4KB a cada leitura
    
    
    

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
        
        read_sector_ata_pio(bpb.specific.fat12_or_fat16.BS_DrvNum,sector_count,bpb.BPB_BytsPerSec,lba_start,phys_fat);

        // Lendo o Root Directory



        data->first_root_directory_sector_num = data->first_data_sector - data->root_directory_sectors;

        sector_count = data->root_directory_sectors;

        lba_start = data->first_root_directory_sector_num &0xffffffff;

        

 
       read_sector_ata_pio(bpb.specific.fat12_or_fat16.BS_DrvNum,sector_count,bpb.BPB_BytsPerSec,lba_start,phys_root_dir);

        

        
          
    }
    else if (data->fat_type == FAT32)
    {


    
        // FIXME 2
        // Lendo a File Allocation Table 32
        
        sector_count = data->fat_total_sectors;

        lba_start = bpb.BPB_RsvdSecCnt &0xffffffff;
        
        read_sector_ata_pio(bpb.specific.fat32.BS_DrvNum,sector_count,bpb.BPB_BytsPerSec,lba_start,phys_fat);



        // Lendo o first Directory ou Root Directory


        N = data->root_cluster;
        sector_count = bpb.BPB_SecPerClus;


    do{
            data->first_sector_of_cluster = ((  N -2)*bpb.BPB_SecPerClus) + data->first_data_sector;

            lba_start = data->first_sector_of_cluster;

            read_sector_ata_pio(bpb.specific.fat32.BS_DrvNum,sector_count,512,lba_start,phys_root_dir + offset);


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




    //Aqui vamos ler os arquivos a partir dos dados do directório raíz

       offset = 0;


GOTO_1:    if(phys_root_dir[1+ offset]==0) goto GOTO_9; // Terminar
     
GOTO_2:    if(phys_root_dir[1+ offset]==0xE5) goto GOTO_8; //Entrada não será utilizada

GOTO_3:    if(phys_root_dir[11 + offset]==0xF) goto GOTO_4; //Se igual a 0xF, entrada de LFN 
           else goto GOTO_5; // Entrada normal

GOTO_4:    // Ler a parte do nome do arquivo longo em um buffer temporário, GOTO_8
            




GOTO_5:    // Analizar a entrada, e extrair os dados para mais tarde poder utilizar. GOTO_6

            




GOTO_6:    // Existe um nome de arquivo longo no buffer temporário? Sim, GOTO_7.Não, GOTO_8

GOTO_7:    // Aplique o nome de arquivo longo para entrada que voce acabou de ler e limpe o buffer temporário. GOTO_8 

GOTO_8:    offset = offset + 32;
           goto GOTO_1;

GOTO_9: return;
        


}
























