#include "idt.h"
#include "x86_desc.h" // for idt_desc_t idt & Segment selector
#include "excp_handler.h"
#include "int_handler.h"
#include "syscall_handler.h"

/*
 * DESCRIPTION - used to setup an individual entry in the idt table. 
 * INPUT -
 *	uint8_t index
 *	uint32_t handler
 * OUTPUT - NONE
 * SIDE EFFECTS - NONE
 */
// interrupt gate, offset = interrupt handler
static void set_intr_gate(uint8_t index, uint32_t handler)
{
	SET_IDT_ENTRY(idt[index], handler);

	//idt[index].offset_15_00
	idt[index].seg_selector  = KERNEL_CS;
	idt[index].reserved4 = 0x00;
	idt[index].reserved3 = 0;
	idt[index].reserved2 = 1;
	idt[index].reserved1 = 1;
	idt[index].size = 1;
	idt[index].reserved0 = 0;
	idt[index].dpl = KERNEL_MODE; 
	idt[index].present = 1; 
	//idt[index].offset_31_16	
}

/*
 * DESCRIPTION - used to setup an individual entry in the idt table. 
 * INPUT -
 *	uint8_t index
 *	uint32_t handler
 * OUTPUT - NONE
 * SIDE EFFECTS - NONE
 */
// interrupt gate, offset = exception handler
static void set_trap_gate(uint8_t index, uint32_t handler)
{
	SET_IDT_ENTRY(idt[index], handler);

	//idt[index].offset_15_00
	idt[index].seg_selector  = KERNEL_CS;
	idt[index].reserved4 = 0x00;
	idt[index].reserved3 = 1;
	idt[index].reserved2 = 1;
	idt[index].reserved1 = 1;
	idt[index].size = 1;
	idt[index].reserved0 = 0;
	idt[index].dpl = KERNEL_MODE; 
	idt[index].present = 1; 
	//idt[index].offset_31_16		
}

/*
 * DESCRIPTION - used to setup an individual entry in the idt table. 
 * INPUT -
 *	uint8_t index
 *	gdt_index index of gdt
 * OUTPUT - NONE
 * SIDE EFFECTS - NONE
 */
// task gate
/*
static void set_task_gate(uint8_t index, uint32_t gdt_index)
{
}
*/

/*
 * DESCRIPTION - initializes the idt, calls lidt to load the interrupt 
 *               descriptor table, populates the idt table.
 * INPUT - NONE
 * OUTPUT - NONE
 * SIDE EFFECTS - NONE
 */
void idt_init()
{
	memset(idt, 0, sizeof(idt_desc_t) * NUM_VEC); // clear all

	// exception
	set_trap_gate(0x00, (uint32_t)&excp0_handler);
	set_trap_gate(0x01, (uint32_t)&excp1_handler);
	set_intr_gate(0x02, (uint32_t)&excp2_handler);
	set_system_intr_gate(0x03, (uint32_t)&excp3_handler);
	set_system_gate(0x04, (uint32_t)&excp4_handler);
	set_system_gate(0x05, (uint32_t)&excp5_handler);
	set_trap_gate(0x06, (uint32_t)&excp6_handler);
	set_trap_gate(0x07, (uint32_t)&excp7_handler);
	set_trap_gate(0x08, (uint32_t)&excp7_handler); // should use set_task_gate for double fault
	set_trap_gate(0x09, (uint32_t)&excp9_handler);
	set_trap_gate(0x0A, (uint32_t)&excp10_handler);
	set_trap_gate(0x0B, (uint32_t)&excp11_handler);
	set_trap_gate(0x0C, (uint32_t)&excp12_handler);
	set_trap_gate(0x0D, (uint32_t)&excp13_handler);
	set_intr_gate(0x0E, (uint32_t)&excp14_handler);
	//iintel reserved: set_gate(0x0F, (uint32_t)&excp15_handler); 
	set_trap_gate(0x10, (uint32_t)&excp16_handler);
	set_trap_gate(0x11, (uint32_t)&excp17_handler);
	set_trap_gate(0x12, (uint32_t)&excp18_handler);
	set_trap_gate(0x13, (uint32_t)&excp19_handler);
	//iintel reserved: set_gate(0x14, (uint32_t)&excp20_handler);
	//iintel reserved: set_gate(0x15, (uint32_t)&excp21_handler);
	//iintel reserved: set_gate(0x16, (uint32_t)&excp22_handler);
	//iintel reserved: set_gate(0x17, (uint32_t)&excp23_handler);
	//iintel reserved: set_gate(0x18, (uint32_t)&excp24_handler);
	//iintel reserved: set_gate(0x19, (uint32_t)&excp25_handler);
	//iintel reserved: set_gate(0x1A, (uint32_t)&excp26_handler);
	//iintel reserved: set_gate(0x1B, (uint32_t)&excp27_handler);
	//iintel reserved: set_gate(0x1C, (uint32_t)&excp28_handler);
	//iintel reserved: set_gate(0x1D, (uint32_t)&excp29_handler);
	//iintel reserved: set_gate(0x1E, (uint32_t)&excp30_handler);
	//iintel reserved: set_gate(0x1F, (uint32_t)&excp31_handler);
	
	// interrupt
	set_intr_gate(0x20, (uint32_t)&irq0_handler);
	set_intr_gate(0x21, (uint32_t)&irq1_handler); // keyboard
	set_intr_gate(0x22, (uint32_t)&irq2_handler); // PIC cascading
	set_intr_gate(0x23, (uint32_t)&irq3_handler);
	set_intr_gate(0x24, (uint32_t)&irq4_handler);
	set_intr_gate(0x25, (uint32_t)&irq5_handler);
	set_intr_gate(0x26, (uint32_t)&irq6_handler);
	set_intr_gate(0x27, (uint32_t)&irq7_handler);
	set_intr_gate(0x28, (uint32_t)&irq8_handler); // rtc
	set_intr_gate(0x29, (uint32_t)&irq9_handler);
	set_intr_gate(0x2A, (uint32_t)&irq10_handler);
	set_intr_gate(0x2B, (uint32_t)&irq11_handler);
	set_intr_gate(0x2C, (uint32_t)&irq12_handler);
	set_intr_gate(0x2D, (uint32_t)&irq13_handler);
	set_intr_gate(0x2E, (uint32_t)&irq14_handler);
	set_intr_gate(0x2F, (uint32_t)&irq15_handler);
	
	// system call
	set_system_gate(SYSCALL_ENTRY, (uint32_t)&syscall_handler);
	
	// load the memory address in the idtr
	lidt(idt_desc_ptr); 
}
