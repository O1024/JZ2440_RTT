OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

MEMORY
{
    FLASH (rx)  : ORIGIN = 0x00000000, LENGTH = 64M
    RAM (rwx)   : ORIGIN = 0x30000000, LENGTH = 64M
}

SECTIONS
{
    .boot : 
    {
        *(.vectors)
        *(.init)
        build/kernel/libcpu/arm/s3c24x0/nand.o
        . = 0x2000;
    } > FLASH
    
    .text : 
    {
        _text_lma = LOADADDR(.text);
        _text_vma = .;

        *(.text)
        *(.gnu.linkonce.t*)
        
        /* section information for finsh shell */
        . = ALIGN(4);
        __fsymtab_start = .;
        KEEP(*(FSymTab))
        __fsymtab_end = .;
        . = ALIGN(4);
        __vsymtab_start = .;
        KEEP(*(VSymTab))
        __vsymtab_end = .;
        . = ALIGN(4);   

        . = ALIGN(4);
        __rt_init_start = .;
        KEEP(*(SORT(.rti_fn*)))
        __rt_init_end = .;
        . = ALIGN(4);

        /* section information for modules */
        . = ALIGN(4);
        __rtmsymtab_start = .;
        KEEP(*(RTMSymTab))
        __rtmsymtab_end = .;

        _etext = .;
    } > RAM AT > FLASH

    . = ALIGN(4);
    .rodata : 
    {
        rodata_lma = LOADADDR(.rodata);
        rodata_vma = .;
        *(.rodata) 
        *(.rodata.*) 
        *(.gnu.linkonce.r*) 
        *(.eh_frame)
        _erodata = .;
    } > RAM AT > FLASH

    . = ALIGN(4);
    .ctors :
    {
        PROVIDE(__ctors_start__ = .);
        KEEP(*(SORT(.ctors.*)))
        KEEP(*(.ctors))
        PROVIDE(__ctors_end__ = .);
    }

    .dtors :
    {
        PROVIDE(__dtors_start__ = .);
        KEEP(*(SORT(.dtors.*)))
        KEEP(*(.dtors))
        PROVIDE(__dtors_end__ = .);
    }

    . = ALIGN(4);
    .data :
    {
        data_lma = LOADADDR(.data);
        data_vma = .;
        *(.data)
        *(.data.*)
        *(.gnu.linkonce.d*)
        _edata = .;
    } > RAM AT > FLASH

    . = ALIGN(4);
    .bss : 
    {
        __bss_start = .;
        *(.bss)
        __bss_end = .;
    } > RAM

    /* stabs debugging sections. */
    .stab 0 : { *(.stab) }
    .stabstr 0 : { *(.stabstr) }
    .stab.excl 0 : { *(.stab.excl) }
    .stab.exclstr 0 : { *(.stab.exclstr) }
    .stab.index 0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 : { *(.comment) }
    .debug_abbrev 0 : { *(.debug_abbrev) }
    .debug_info 0 : { *(.debug_info) }
    .debug_line 0 : { *(.debug_line) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_aranges 0 : { *(.debug_aranges) }

    _end = .;
}
