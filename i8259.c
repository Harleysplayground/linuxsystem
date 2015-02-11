/* iI8259.c - Functions to interact with the I8259 interrupt controller
 * vim:ts=4 noexpandtab
 */

#include "i8259.h"

#define INT_MASK 0xFF 
#define NUM_MASTER_IRQ 8

/* Interrupt masks to determine which interrupts
 * are enabled and disabled */
uint8_t master_mask; /* IRQs 0-7 */
uint8_t slave_mask; /* IRQs 8-15 */

int32_t eflags;

/* Initialize the I8259 PIC */
void
i8259_init(void)
{
	// usage: outb(data, port)
	//        inb(port)
	
	cli_and_save(eflags); // disable interrupts
	
	// mask all interrupts
	master_mask = INT_MASK;
	slave_mask = INT_MASK;
	outb(master_mask, I8259_MASTER_DATA);
	outb(slave_mask, I8259_SLAVE_DATA);
	
	/* ICW1: command port */
	outb(ICW1, I8259_MASTER_CMD); /* Master port A */
	outb(ICW1, I8259_SLAVE_CMD); /* Slave port A */

	/* ICW2: data port */
	outb(ICW2_MASTER, I8259_MASTER_DATA); /* Master offset of 0x20 in the IDT */
	outb(ICW2_SLAVE, I8259_SLAVE_DATA); /* Master offset of 0x28 in the IDT */

	/* ICW3: data port */
	outb(ICW3_MASTER, I8259_MASTER_DATA); /* Slaves attached to IR line 2 */
	outb(ICW3_SLAVE, I8259_SLAVE_DATA); /* This slave in IR line 2 of master */

	/* ICW4: data port */
	outb(ICW4, I8259_MASTER_DATA); /* Set as master */
	outb(ICW4, I8259_SLAVE_DATA); /* Set as slave */
	
	// mask all interrupts
	outb(master_mask, I8259_MASTER_DATA);
	outb(slave_mask, I8259_SLAVE_DATA);
	
	// enable slave PIC
	enable_irq(SLAVE_IRQ);	

	restore_flags(eflags); // restore from cli_and_save	
}

/* Enable (unmask) the specified IRQ */
void
enable_irq(uint32_t irq_num)
{
	// changes IMR (Interrupt Mask Register)
	if (irq_num < NUM_MASTER_IRQ)
	{
		master_mask = (uint8_t)(inb(I8259_MASTER_DATA) & ~(1 << irq_num));
		outb(master_mask, I8259_MASTER_DATA);
	}
	else
	{
		slave_mask = (uint8_t)(inb(I8259_SLAVE_DATA) & ~(1 << (irq_num-8)));
		outb(slave_mask, I8259_SLAVE_DATA);
	}
}

/* Disable (mask) the specified IRQ */
void
disable_irq(uint32_t irq_num)
{
	// changes IMR (Interrupt Mask Register)
	if (irq_num < NUM_MASTER_IRQ)
	{
		master_mask = (uint8_t)(inb(I8259_MASTER_DATA) | (1 << irq_num));
		outb(master_mask, I8259_MASTER_DATA);
	}
	else
	{
		slave_mask = (uint8_t)(inb(I8259_SLAVE_DATA) | (1 << (irq_num-8)));
		outb(slave_mask, I8259_SLAVE_DATA);
	}
}

/* Send end-of-interrupt signal for the specified IRQ */
void
send_eoi(uint32_t irq_num)
{
	if (irq_num < NUM_MASTER_IRQ)
	{
		master_mask = (uint8_t)(EOI | irq_num);
		outb(master_mask, I8259_MASTER_CMD);
	}
	else
	{
		slave_mask = (uint8_t)(EOI | (irq_num-8));
		outb(slave_mask, I8259_SLAVE_CMD);
		master_mask = (uint8_t)(EOI | SLAVE_IRQ);
		outb(master_mask, I8259_MASTER_CMD);
	}
}
