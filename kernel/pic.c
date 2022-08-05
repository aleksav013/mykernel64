#include <io.h>
#include <pic.h>

void remap_pic(void)
{
	// save masks
//	uint8_t a1 = inb(PIC1_DATA);
//	uint8_t a2 = inb(PIC2_DATA);

	// starts the initialization sequence (in cascade mode)
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC1_DATA, 0x20);				// ICW2: Master PIC vector offset
	outb(PIC2_DATA, 0x28);				// ICW2: Slave PIC vector offset
	outb(PIC1_DATA, 4);				// ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	outb(PIC2_DATA, 2);				// ICW3: tell Slave PIC its cascade identity (0000 0010)

	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	// mask interrupts
	outb(PIC1_DATA, 0xfd);
	outb(PIC2_DATA, 0xff);
}
