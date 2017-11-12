#include <boot.h>


extern void isr00();	extern void isr10();	extern void isr20();	extern void isr30();		
extern void isr01();	extern void isr11();	extern void isr21();	extern void isr31();
extern void isr02();	extern void isr12();	extern void isr22();
extern void isr03();	extern void isr13();	extern void isr23();
extern void isr04();	extern void isr14();	extern void isr24();
extern void isr05();	extern void isr15();	extern void isr25();
extern void isr06();	extern void isr16();	extern void isr26();
extern void isr07();	extern void isr17();	extern void isr27();
extern void isr08();	extern void isr18();	extern void isr28();
extern void isr09();	extern void isr19();	extern void isr29();


char *exception_mensagem[32]={
	"Divide error\n",		// 0 - fault  -- no error 
	"Debug exception\n", 	// 1 - fault/trap -- no error 
	"NMI Interrupter\n",	//  2 - interrupter -- no error
	"Breakpoint\n",		// 3 - trap -- no error
	"Overflow\n",		// 4 - trap -- no error
	"BOUND Ranger exception\n", // 5 - trap -- no error
	"Invalide opcode (Undefined opcode)\n", // 6 - fault -- no error
	"Device not avaliable (Not Math coprocessor\n)", // 7 - fault -- no error
	"Double Fault (Erro de codigo)\n",		// 8 - about -- error code
	"Coprocessor segment overrun (reservado)\n",	// 9 - fault -- no error
	"Invalide TSS (Erro de codigo)\n", 	// 10 - fault -- error code
	"Segment not present (Erro de codigo)\n",		// 11 - fault -- error code
	"Stack segment fault (Erro de codigo)\n",	// 12 - fault -- error code
	"General protetion (Erro de codigo)\n",	// 13 - fault -- error code
	"Page fault (Erro de codigo)\n",		// 14 - fault -- error code
	"Intel reserved do not use 15\n",		// 15
	"x87 FPU Floating-Point error (Math fault)\n",	// 16 - fault -- no error
	"Alignment check (Erro de codigo)\n",		// 17 - fault -- error code
	"Machine check\n",	// 18 - about -- no error 
	"SIND Floating-Point exception\n", // 19 - fault -- no error
	"Virtualization exception\n",	// 20 - fault -- no error
	"Intel reserved do not use 21\n",	// 21
	"Intel reserved do not use 22\n",	// 22
	"Intel reserved do not use 23\n",	// 23
	"Intel reserved do not use 24\n",	// 24
	"Intel reserved do not use 25\n",	// 25
	"Intel reserved do not use 26\n",	// 26
	"Intel reserved do not use 27\n",	// 27
	"Intel reserved do not use 28\n",	// 28
	"Intel reserved do not use 29\n",    // 29
	"Intel reserved do not use 30\n",	// 30
	"Intel reserved do not use 31\n",	// 31

};



void fault_exception(DWORD n){
	int CR2;

	if(n == 8 || n == 18)set_color(0x4);
	else set_color(0xE);
	puts(exception_mensagem[n]);
	if(n == 14){

		__asm__ __volatile__("movl %%cr2,%%eax":"=a"(CR2):);
		printboot("\nCR2 = %x",CR2);

		}
	for (;;);	

}




void isr_install(){


	trap(0,(DWORD)isr00,0x8,0);
	trap(1,(DWORD)isr01,0x8,0);
    interrupter(2,(DWORD)isr02,0x8,0);
	trap(3,(DWORD)isr03,0x8,0);
	trap(4,(DWORD)isr04,0x8,0);
	trap(5,(DWORD)isr05,0x8,0);
	trap(6,(DWORD)isr06,0x8,0);
	trap(7,(DWORD)isr07,0x8,0);
	trap(8,(DWORD)isr08,0x8,0);
	trap(9,(DWORD)isr09,0x8,0);
	trap(10,(DWORD)isr10,0x8,0);
	trap(11,(DWORD)isr11,0x8,0);
	trap(12,(DWORD)isr12,0x8,0);
	trap(13,(DWORD)isr13,0x8,0);
	trap(14,(DWORD)isr14,0x8,0);
	trap(15,(DWORD)isr15,0x8,0);
	trap(16,(DWORD)isr16,0x8,0);
	trap(17,(DWORD)isr17,0x8,0);
	trap(18,(DWORD)isr18,0x8,0);
	trap(19,(DWORD)isr19,0x8,0);
	trap(20,(DWORD)isr20,0x8,0);
	trap(21,(DWORD)isr21,0x8,0);
	trap(22,(DWORD)isr22,0x8,0);
	trap(23,(DWORD)isr23,0x8,0);
	trap(24,(DWORD)isr24,0x8,0);
	trap(25,(DWORD)isr25,0x8,0);
	trap(26,(DWORD)isr26,0x8,0);
	trap(27,(DWORD)isr27,0x8,0);
	trap(28,(DWORD)isr28,0x8,0);
	trap(29,(DWORD)isr29,0x8,0);
	trap(30,(DWORD)isr30,0x8,0);
	trap(31,(DWORD)isr31,0x8,0);

}
