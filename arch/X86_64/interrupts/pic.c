#include "interrupts.h"
#include <synos/arch/io.h>
#include <stddef.h>
#include "../cpuid.h"

#define PIC1		0x20		/* IO base address for master PIC */
#define PIC2		0xA0		/* IO base address for slave PIC */
#define PIC1_COMMAND	PIC1
#define PIC1_DATA	(PIC1+1)
#define PIC2_COMMAND	PIC2
#define PIC2_DATA	(PIC2+1)

#define ICW1_ICW4	0x01		/* ICW4 (not) needed */
#define ICW1_SINGLE	0x02		/* Single (cascade) mode */
#define ICW1_INTERVAL4	0x04		/* Call address interval 4 (8) */
#define ICW1_LEVEL	0x08		/* Level triggered (edge) mode */
#define ICW1_INIT	0x10		/* Initialization - required! */
 
#define ICW4_8086	0x01		/* 8086/88 (MCS-80/85) mode */
#define ICW4_AUTO	0x02		/* Auto (normal) EOI */
#define ICW4_BUF_SLAVE	0x08		/* Buffered mode/slave */
#define ICW4_BUF_MASTER	0x0C		/* Buffered mode/master */
#define ICW4_SFNM	0x10		/* Special fully nested (not) */

#ifdef APIC_ENABLE
extern void APIC_disable();
#endif

size_t irq_spurious = 0;

int PIC_Configure(uint8_t offset1, uint8_t offset2)
{
	#ifdef APIC_ENABLE
	if(x64ID.APIC) APIC_disable();
	#endif

    unsigned char a1, a2;
 
	a1 = inb(PIC1_DATA);                        // save masks
	a2 = inb(PIC2_DATA);

    outb(PIC1_COMMAND, ICW1_INIT | ICW1_ICW4);  // starts the initialization sequence (in cascade mode)
	io_wait();
	outb(PIC2_COMMAND, ICW1_INIT | ICW1_ICW4);
	io_wait();
	outb(PIC1_DATA, offset1);                 // ICW2: Master PIC vector offset
	io_wait();
	outb(PIC2_DATA, offset2);                 // ICW2: Slave PIC vector offset
	io_wait();
	outb(PIC1_DATA, 4);                       // ICW3: tell Master PIC that there is a slave PIC at IRQ2 (0000 0100)
	io_wait();
	outb(PIC2_DATA, 2);                       // ICW3: tell Slave PIC its cascade identity (0000 0010)
	io_wait();
 
	outb(PIC1_DATA, ICW4_8086);
	io_wait();
	outb(PIC2_DATA, ICW4_8086);
	io_wait();
 
	outb(PIC1_DATA, a1);   // restore saved masks.
	outb(PIC2_DATA, a2);

    return 0;
}

#define PIC_READ_IRR 0x0a
#define PIC_READ_ISR 0x0b
static uint16_t _pic_read(uint8_t ocw3)
{
	outb(PIC1_COMMAND, ocw3);
	outb(PIC2_COMMAND, ocw3);
	return (inb(PIC2_COMMAND) << 8) | inb(PIC1_COMMAND);
}
static uint16_t _pic_isr()
{
	return _pic_read(PIC_READ_ISR);
}

#define PIC_EOI_CODE 0x20

#define IRQ_RET asm ("iretq")
#define ISR_MASTER(isr) (isr & 0xFF00)
#define ISR_SLAVE(isr) (isr & 0xFF)
void PIC_SOI(uint8_t irq)
{
	if (irq != 7 && irq != 15) return;

	// Check if interrupt was spurious
	uint16_t isr = _pic_isr();
	if (irq == 7 && ISR_MASTER(isr) != 7)
	{
		// Interrupt 7 was spurious
		++irq_spurious;
		IRQ_RET;
	}
	if (irq == 15 && ISR_SLAVE(isr) != 15)
	{
		// Interrupt 15 was spurious
		++irq_spurious;
		outb(PIC1_COMMAND, PIC_EOI_CODE); // We still need to send master pic an EOI
		IRQ_RET;
	}
}
void PIC_EOI(uint8_t irq)
{
    if (irq >= 8)
        outb(PIC2_COMMAND, PIC_EOI_CODE);
    outb(PIC1_COMMAND, PIC_EOI_CODE);
}

uint8_t irq1, irq2;
void PIC_IRQ_save()
{
	irq1 = inb(PIC1_DATA);
	irq2 = inb(PIC2_DATA);
}
void PIC_IRQ_kill()
{
	outb(PIC1_DATA, 0xFF);
	outb(PIC2_DATA, 0xFF);
}
void PIC_IRQ_restore()
{
	outb(PIC1_DATA, irq1);
	outb(PIC2_DATA, irq2);
}

void PIC_disable()
{
	PIC_IRQ_kill();
}