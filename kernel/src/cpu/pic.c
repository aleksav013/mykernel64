#include <io.h>
#include <pic.h>

void remap_pic(void)
{
	/* starts the initialization sequence (in cascade mode) */
	outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	outb(PIC1_DATA, 0xe0); /* ICW2: Master PIC vector offset */
	outb(PIC2_DATA, 0xe8); /* ICW2: Slave PIC vector offset */
	outb(PIC1_DATA,
	     4); /* ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100) */
	outb(PIC2_DATA,
	     2); /* ICW3: tell Slave PIC its cascade identity (0000 0010) */

	outb(PIC1_DATA, ICW4_8086);
	outb(PIC2_DATA, ICW4_8086);

	/* mask all interrupts */
	outb(PIC1_DATA, 0xff);
	outb(PIC2_DATA, 0xff);
}
