#include <boot.h>
#include <typedef.h>

extern void lidt_install();

struct idt_s 
{

    unsigned long long offset_15_0 :16; 
	unsigned long long sel : 16;
    unsigned long long unused :8;
	unsigned long long flags : 5;
	unsigned long long dpl : 2;
    unsigned long long p :1;
	unsigned long long offset_31_16 : 16;


}__attribute__ ((packed));



struct idt_s idt[256];


static void set_gate_idt(int n,DWORD offset, WORD sel,BYTE flags, BYTE dpl, BYTE p);


void idt_install(){

    memset(idt,0,sizeof(idt));
    
    isr_install();
    irq_install();
    lidt_install();


}


void trap(int n,DWORD offset,WORD sel,BYTE dpl ){
    set_gate_idt(n,offset,sel,0x8F,dpl,1);

}

void interrupter(int n,DWORD offset,WORD sel,BYTE dpl ){
	set_gate_idt(n,offset,sel,0x8E,dpl,1);

}



static void set_gate_idt(int n,DWORD offset, WORD sel,BYTE flags, BYTE dpl, BYTE p){

            idt[n].offset_15_0 = offset &0xFFFF;
            idt[n].sel = sel;
            idt[n].unused = 0;
            idt[n].flags = flags;
            idt[n].dpl = dpl;
            idt[n].p = p;
            idt[n].offset_31_16 = offset >> 16 &0xFFFF;



}




