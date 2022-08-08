ENTRY(_start)

KERNEL_PMA = 0x00100000;
KERNEL_VMA = 0xc0000000;

SECTIONS
{
	. = KERNEL_PMA;

	.boot BLOCK(4K) : ALIGN(4K)
	{
		KEEP(*(.multiboot_header))
		*(.boot.text)
	}

	. += KERNEL_VMA;

	.text ALIGN(4K) : AT (ADDR (.text) - KERNEL_VMA)
	{
		*(.text)
	}

	.rodata ALIGN(4K) : AT (ADDR (.rodata) - KERNEL_VMA)
	{
		*(.rodata)
	}

	.data ALIGN(4K) : AT (ADDR (.data) - KERNEL_VMA)
	{
		*(.data)
	}

	.bss ALIGN(4K) : AT (ADDR (.bss) - KERNEL_VMA)
	{
		*(COMMON)
		*(.bss)
	}

	/DISCARD/ :
	{
		*(.comment)
		*(.note.*)
	}
}
