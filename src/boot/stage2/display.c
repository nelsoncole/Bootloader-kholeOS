/* Arquivo "boot.c"
   
	"Projecto KholeOS, Bootloader!"
	
	Nelson Sapalo da Silva Cole (nelsoncole72@gmail.com  +244-948-833-132)

  	Lubango, 18 de Julho de 2017

*/


#include <boot.h>
#include <stdarg.h>

#define MEMORIA_START 0xB8000
#define MEMORIA_END 0xC0000

#define	INT_MAX	2147483647	// max value of an "int"

	WORD *video=(WORD*)0xb8000;
	BYTE color = 0x0F;
	BYTE cursor_x = 0;
	BYTE cursor_y = 0;
	BYTE coluna = 80;
static void scroll();
static void k_i2hex(DWORD val, char* dest, int len);
static void k_itoa(int value, char* valuestring);

// Drive de video cursor. Nota que iremos programar o driver sem gerar interrupções
void update_cursor(){

	WORD posicao_cursor = cursor_y *coluna + cursor_x;
	
	outb(0x3D4,0xE);	// Comando cursor 15:8
	outb(0x3D5,(BYTE)(posicao_cursor >> 8) &0xFF);
	outb(0x3D4,0xF);	// Comando cursor 7:0
	outb(0x3D5,(BYTE)(posicao_cursor) &0xFF);
}

// Esta função posiciona o cursor na tela

void set_cursor(int x, int y){
	
	if(cursor_x <= 80)cursor_x = x;
	if(cursor_y <= 25)cursor_y = y;
	
	}



// Esta função define a cor do texto
void set_color(const char cor){
	 color = (cor & 0x0F);
 }



// Esta função imprime um caracter na tela
void putch (char c){

	WORD *posicao = video + (cursor_y *coluna + cursor_x);
	WORD atributo = color << 8;
	
	if(MEMORIA_START == MEMORIA_END){
		
		set_cursor(0,0);
		}
	
	if(c == '\b'&&cursor_x){
		cursor_x--;
		putch(' ');
		cursor_x--;
	 }
	 
	else if(c == '\t'){
		cursor_x = (cursor_x + 8) &~(8-1);
		}
		
	else if(c == '\n'){
		cursor_x = 0;
		cursor_y++;
		update_cursor();
	
		}
		
	else if(c >= ' '){
	
		*posicao = c | atributo;
    		cursor_x++;
	}
	
	else if(cursor_x >= 80){
		cursor_x =0;
		cursor_y++;
		}
		
	if(cursor_y >= 25){
	
		scroll();
		
		}
		
	update_cursor();
}


// Esta função Imprime uma String na tela
void puts(char *string){


	int i;

	for(i=0;string[i]!= '\0';i++)putch(string[i]);

}



// Esta função imprime caracteres na tela tendo enconta o format %i, %c, %x, em outras palavras é um print formato

void printboot(const char *args, ...){
	char* ap;
	va_start (ap,args);
	int index = 0;
	unsigned char u;	
	int d;
	char c, *s;
	char buffer[256];

	while (args[index])
	{
		switch (args[index])
		{
		case '%':
			++index;
			switch (args[index])
			{
			
     case 'C':
			case 'c':
				c = (char) va_arg (ap, int);
				putch(c);
				break;
     
     case 'S':
			case 's':
				s = va_arg (ap, char*);
				puts(s);
				break;

			case 'd':
			case 'i':
				d = va_arg (ap, int);
				k_itoa(d, buffer);
				puts(buffer);
				break;

			case 'U':
			case 'u':
				u = va_arg (ap, DWORD);
				k_itoa(u, buffer);
				puts(buffer);
				break;

			case 'X':
			case 'x':
				d = va_arg (ap, int);
				k_i2hex(d, buffer,8);
				puts(buffer);
				break;
			
			default:
				putch('%');
				putch('%');
				break;
				
				
			}
			break;

		default:
			putch(args[index]);
			break;
		}
		++index;
	}

}

static void scroll()
{
    char attributeByte = (0 /*black*/ << 4) | (15 /*white*/ & 0x0F);
    short blank = 0x20 /* space */ | (attributeByte << 8);
    if(cursor_y >= 25)
    {
        int i;
        for (i = 0*80; i < 24*80; i++)
        {
            video[i] = video[i+80];
        }
        for (i = 24*80; i < 25*80; i++)
        {
            video[i] = blank;
        }
       
        cursor_y = 24;
    }
}

static void k_i2hex(DWORD val, char* dest, int len)
{
	char* cp;
	char x;
	DWORD n;
	n = val;
	cp = &dest[len];
	while (cp > dest)
	{
		x = n & 0xF;
		n >>= 4;
		*--cp = x + ((x > 9) ? 'A' - 10 : '0');
	}
    dest[len+1]='\0';
}



static void k_itoa(int value, char* valuestring)
{
  int min_flag;
  char swap, *p;
  min_flag = 0;

  if (0 > value)
  {
    *valuestring++ = '-';
    value = -INT_MAX> value ? min_flag = INT_MAX : -value;
  }

  p = valuestring;

  do
  {
    *p++ = (char)(value % 10) + '0';
    value /= 10;
  } while (value);

  if (min_flag != 0)
  {
    ++*valuestring;
  }
  *p-- = '\0';

  while (p > valuestring)
  {
    swap = *valuestring;
    *valuestring++ = *p;
    *p-- = swap;
  }
}
