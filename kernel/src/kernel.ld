ENTRY(_start)

KERNEL_AP_PMA = 0x0000000000008000;
KERNEL_PMA = 0x0000000000100000;
KERNEL_VMA = 0xffffffff80000000;

SECTIONS
{
	. = KERNEL_AP_PMA;

	.apinit BLOCK(4K) : ALIGN(4K)
	{
		*(.apinit)
	}

	. = KERNEL_PMA;

	.boot BLOCK(4K) : ALIGN(4K)
	{
		KEEP(*(.multiboot_header))
		*(.boot32.text)
		*(.boot32.rodata)
		*(.boot32.bss)
		*(.boot64.text)
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
