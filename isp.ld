/*Copyright (C) 2011 by Sagar G V, Thejasvi M V

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.*/

OUTPUT_FORMAT ("elf32-littlearm")
ENTRY(Reset_Handler)
SEARCH_DIR(.)

MEMORY
{
        /* stm32f4xx : 1024k ROM + 64k + 128K SRAM */
        /*------------------------------ */

	/* On-chip ROM is a readable (r), executable region (x) */
	/* On-chip SRAM is a readable (r), writable (w) and */
	/* executable region (x) */

	/* Main ROM region - 1024k for stm32f4xx */
	flash (rx) : ORIGIN = 0x08000000, LENGTH = 64k

    /* local static RAM - 64k for stm32f4xx */
    sram (rwx) : ORIGIN = 0x10000000, LENGTH = 63k

    /* IRAM - 128k for stm32f4xx - often used for USB,LAN */
    iram1(rwx) : ORIGIN = 0x20000000, LENGTH = 128k
}

/* SECTION command : Define mapping of input sections */
/* into output sections. */
sram_top = ORIGIN(sram) + LENGTH(sram) + 1k;
iram1_top = ORIGIN(iram1) + LENGTH(iram1);


SECTIONS
{
  .text :
  {
    KEEP(*(.isr_vector))
    /*. = 0x200;
    KEEP(*(.update_cfg))*/

    KEEP(*(.fault))
    KEEP(*(.reset))

    *(.text*)
    *(.rodata*)
/* add by zp 2013.01.14 为了增加CPP的构造函数初始化*/
		*(.gnu.linkonce.t.*)
		*(.gcc_except_table)
		*(.rodata .rodata*)
		*(.gnu.linkonce.r.*)
		. = ALIGN(4);
		KEEP(*(.init))

		. = ALIGN(4);
		__preinit_array_start = .;
		KEEP (*(.preinit_array))
		__preinit_array_end = .;

		. = ALIGN(4);
		__init_array_start = .;
		KEEP (*(SORT(.init_array.*)))
		KEEP (*(.init_array))
		__init_array_end = .;

		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.ctors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .ctors))
		KEEP (*(SORT(.ctors.*)))
		KEEP (*crtend.o(.ctors))

		. = ALIGN(4);
		KEEP(*(.fini))

		. = ALIGN(4);
		__fini_array_start = .;
		KEEP (*(.fini_array))
		KEEP (*(SORT(.fini_array.*)))
		__fini_array_end = .;


		. = ALIGN(0x4);
		KEEP (*crtbegin.o(.dtors))
		KEEP (*(EXCLUDE_FILE (*crtend.o) .dtors))
		KEEP (*(SORT(.dtors.*)))
		KEEP (*crtend.o(.dtors))

		/* End Of .text section */
  } > flash

  /*
   * More information about Special Section Indexes is available in the
   * free "ELF for the ARM Architecture" document from ARM Limited
   * http://simplemachines.it/doc/aaelf.pdf
   *
   */
  .ARM.extab : { *(.ARM.extab* .gnu.linkonce.armextab.*) } > flash
  __exidx_start = .;
  .ARM.exidx : { *(.ARM.exidx* .gnu.linkonce.armexidx.*) } > flash
  __exidx_end = .;

  _etext = __exidx_end;

  .data : AT ( _etext )
  {
    _data = .;
    *(vtable)
    *(.data*)
    _edata = .;
    _heap1_start = .;
  } > sram

  /* zero initialized data */
  .bss :
  {
    _bss = .;
    *(.bss*)
    *(COMMON)
    _ebss = .;
    _heap2_start = .;
  } > iram1

/*  .eram_bss :
  {
	 KEEP(*(.iram))
    _heap2_start = .;
  } > iram1 */

  end = .;

  stack_entry = sram_top - 16 - 32;
  _heap1_end = sram_top - 1024;
  _heap2_end = iram1_top - 1024;
}
