/*
Copyright (C) 2013-2017, Fujitsu Electronics Europe GmbH or a               
subsidiary of Fujitsu Electronics Europe GmbH.  All rights reserved.        
                                                                            
This software, including source code, documentation and related             
materials ("Software"), is owned by Fujitsu Electronics Europe GmbH or      
one of its subsidiaries ("Fujitsu").
                                                                            
If no EULA applies, Fujitsu hereby grants you a personal, non-exclusive,    
non-transferable license to copy, modify, and compile the                   
Software source code solely for use in connection with Fujitsu's            
integrated circuit products.  Any reproduction, modification, translation,  
compilation, or representation of this Software except as specified         
above is prohibited without the express written permission of Fujitsu.      
                                                                            
Disclaimer: THIS SOFTWARE IS PROVIDED AS-IS, WITH NO                        
WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING,                        
BUT NOT LIMITED TO, NONINFRINGEMENT, IMPLIED                                
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A                             
PARTICULAR PURPOSE. Fujitsu reserves the right to make                      
changes to the Software without notice. Fujitsu does not assume any         
liability arising out of the application or use of the Software or any      
product or circuit described in the Software. Fujitsu does not              
authorize its products for use in any products where a malfunction or       
failure of the Fujitsu product may reasonably be expected to result in      
significant property damage, injury or death ("High Risk Product"). By      
including Fujitsu's product in a High Risk Product, the manufacturer        
of such system or application assumes all risk of such use and in doing     
so agrees to indemnify Fujitsu against all liability.                       

*/

/*********************************************************************************/
/*  Linker script for  GNU toolchain                                            */
/*  Target-mcu          AMA3B1KK-KBR                                             */
/*********************************************************************************/

OUTPUT_FORMAT ("elf32-littlearm", "elf32-bigarm", "elf32-littlearm")
/*ENTRY(main)*/
SEARCH_DIR(.)
GROUP(-lgcc -lc)
MEMORY
{
   ROM   (rx)   : ORIGIN = 0x0000C000, LENGTH = 976K
   RAM   (rwx)  : ORIGIN = 0x10000000, LENGTH = 383K
}

/* These force the linker to search for particular symbols from
 * the start of the link process and thus ensure the user's
 * overrides are picked up
 */
EXTERN(__MCU_reset_cortex_m)
EXTERN(__MCU_interrupt_vector_cortex_m)
EXTERN(__MCU_start_c main __MCU_stack __MCU_stack_size __MCU_heap_size)

PROVIDE(__MCU_stack = __MCU_region_start_ram + __MCU_region_size_ram);
PROVIDE(__MCU_stack_start = __MCU_stack - __MCU_stack_size);
PROVIDE(__MCU_heap_start = __MCU_stack - __MCU_stack_size - __MCU_heap_size);

/* default values if not defined externally
 */
PROVIDE(__MCU_stack_size = 0x00000400);
PROVIDE(__MCU_heap_size  = 0x00000400);

SECTIONS
{
  .text :
  {
    CREATE_OBJECT_SYMBOLS
    __MCU_region_start_rom = .;
    *(.MCU.region-head.ROM)
    __MCU_interrupt_vector = __MCU_interrupt_vector_cortex_m;
    *(.MCU.interrupt_vector)
    /* Make sure we pulled in an interrupt vector.  */
    ASSERT (. != __MCU_interrupt_vector_cortex_m, "No interrupt vector");
    *(.ROM)
    *(.ROM.b)

    __MCU_reset = __MCU_reset_cortex_m;
    *(.MCU.reset)
    /* Make sure we pulled in some reset code.  */
    ASSERT (. != __MCU_reset, "No reset code");

    *(.text .text.* .gnu.linkonce.t.*)
    *(.plt)
    *(.gnu.warning)
    *(.glue_7t) *(.glue_7) *(.vfp11_veneer)

    *(.rodata .rodata.* .gnu.linkonce.r.*)

    *(.ARM.extab* .gnu.linkonce.armextab.*)
    *(.gcc_except_table)
    *(.eh_frame_hdr)
    *(.eh_frame)

    . = ALIGN(4);
    __MCU_regions = .;
    LONG (0)
    LONG (__MCU_region_init_ram)
    LONG (__MCU_region_start_ram)
    LONG (__MCU_region_init_size_ram)
    LONG (__MCU_region_zero_size_ram)
  } > ROM

  /* .ARM.exidx is sorted, so has to go in its own output section.  */
  __exidx_start = .;
  .ARM.exidx :
  {
    *(.ARM.exidx* .gnu.linkonce.armexidx.*)
  } >ROM
  __exidx_end = .;
  .text.align :
  {
    . = ALIGN(4);
    _etext = .;
  } >ROM
  __MCU_region_size_rom = LENGTH(ROM);
  __MCU_region_num = 1;

  .data :
  {
    _sdata = .;
    __MCU_region_start_ram = .;
    *(.MCU.region-head.RAM)
    KEEP(*(.jcr))
    *(.got.plt) *(.got)
    *(.shdata)
    *(.data .data.* .gnu.linkonce.d.*)
    *(.RAM)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT>ROM
  data_size = _edata - _sdata;
  .bss :
  {
    _sbss = .;
    *(.shbss)
    *(.bss .bss.* .gnu.linkonce.b.*)
    *(COMMON)
    *(.RAM.b)
    . = ALIGN(4);
    _ebss = .;
    end = .;
    _end = .;
    __end = .;
  } >RAM AT>ROM
  bss_size = _ebss - _sbss;
  .heap :
  {
    . = ALIGN(4);
    . = . + __MCU_heap_size;
  } >RAM
  .stack :
  {
    . = ALIGN(4);
    . = . + __MCU_stack_size;
  } >RAM
  __MCU_region_init_ram = LOADADDR (.data);
  __MCU_region_init_size_ram = _edata - __MCU_region_start_ram;
  __MCU_region_zero_size_ram = _end - _edata;
  __MCU_region_size_ram = LENGTH(RAM);
  __MCU_region_num = 1;

  .stab 0 (NOLOAD) : { *(.stab) }
  .stabstr 0 (NOLOAD) : { *(.stabstr) }
  /* DWARF debug sections.
   * Symbols in the DWARF debugging sections are relative to the beginning
   * of the section so we begin them at 0.  */
  /* DWARF 1 */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2 */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2 */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }

  .note.gnu.arm.ident 0 : { KEEP (*(.note.gnu.arm.ident)) }
  .ARM.attributes 0 : { KEEP (*(.ARM.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) }
}
