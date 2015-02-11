#include "pcb.h"

/* 
 * DESCRIPTION: Gives current value of ESP and a pointer to the PCB
 * INPUT: none
 * OUTPUT: a pointer to the current pcb
 */
pcb_t *get_cur_pcb()
{
	uint32_t esp;
	asm volatile("movl %%esp, %0"
		: "=r"(esp)
	);
	return (pcb_t *)(esp & ESP_MASK);
}

