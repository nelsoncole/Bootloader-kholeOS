/*
	Este scripter é o mesmo usado no tutorial do projecto ErdOS, um tutorial super legal.


	Gostei dele e uso em quase todos os meus projectos open source.
 	Ele alinha todo codigo em 4 KiB envitando erros de leitura de sector e de paginação */
	


OUTPUT_FORMAT("binary")
OUTPUT_ARCH(i386)
ENTRY(start)

phys = 0x00008000;

SECTIONS
{
  .text phys : AT(phys) {
    code = .;
    *(.text)
    *(.rodata)
    . = ALIGN(4096);
  }
  .data : AT(phys + (data - code))
  {
    data = .;
    *(.data)
    . = ALIGN(4096);
  }
  .bss : AT(phys + (bss - code))
  {
    bss = .;
    *(.bss)
    . = ALIGN(4096);
   
  }
  end = .;
}
