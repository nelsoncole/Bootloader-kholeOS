#include <boot.h>
#include <typedef.h>





unsigned strlen ( const char* str ) {

	unsigned len=0;
	while (str[++len]);
	return len;
}

int strncpy(void* s1, void* s2,unsigned count){

 __asm__ __volatile__ ("cld; rep; movsb"::"c"(count),"D"(s1),"S"(s2));

    return count;
}


unsigned char strcmpb (void* s1, void* s2,unsigned count){

    unsigned char ret;
    __asm__ __volatile__ ("cld;\
                           rep;\
                           cmpsb;\
                           je 1f;\
                           movb $0,%%al; \
                           jmp 2f;\
                           1: movb $1,%%al;\
                           2:"\
                           :"=aN"(ret):"c"(count),"D"(s1),"S"(s2));

    return ret;
}


void* memset(void *s, char val, unsigned count)
{
	
        unsigned char *temp = (unsigned char *)s;
        for( ; count != 0; count--) *temp++ = val;
        return s;    	
}
