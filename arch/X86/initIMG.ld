/* Used to link initIMG */
ENTRY(_start)

SECTIONS
{
    . = 1M;
    PROVIDE(__KERN_MEM_START = .);

    .boot :
    {
        *(.multiboot_header)
    }
    .text :
    {
        *(.text)
    }

    .rodata BLOCK(4K) : ALIGN(4K)
    {
        *(.rodata)
    }

    .data BLOCK(4K) : ALIGN(4K)
    {
        *(.data)
    }

    .bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}

    PROVIDE (__KERN_MEM_END = .);
    PROVIDE (__KERN_MEM_SIZE = __KERN_MEM_END - __KERN_MEM_START);
}