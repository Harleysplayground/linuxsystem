#include "paging.h"
#include "x86_desc.h"

#define _PRESENT    0x00000001
#define _READWRITE  0x00000002
#define _USER       0x00000004
#define _SUPERVISOR 0x00000000
#define _4MB        0x00000080
#define _4KB        0x00000000

// Set up of Page Directory
static uint32_t page_dir[SIZE_PD] __attribute__ ((aligned(FOUR_KB))); 
// Set up of the Page Table from 0 to 4MB
static uint32_t page_table_0[SIZE_PT] __attribute__ ((aligned(FOUR_KB)));

/* paging_init()
 * DESCRIPTION - initializes the page directory and the page table
 * INPUT - NONE
 * OUTPUT - NONE
 * SIDE EFFECTS - None
 */
void paging_init()
{
	uint32_t val;
	
	page_dir[0] = (uint32_t)page_table_0 | _PRESENT | _READWRITE | _USER | _4KB;
	page_dir[1] = KERNEL_PHYS | _PRESENT | _READWRITE | _SUPERVISOR | _4MB;
		
	page_table_0[VIDEO_PHYS/(SIZE_PT*sizeof(uint32_t))] =
		VIDEO_PHYS | _PRESENT | _READWRITE | _USER;

	// Set CR4
	val = 0x00000010;
	asm volatile("movl %%cr4, %%eax; orl %0, %%eax; movl %%eax, %%cr4"
		: 
		: "r"(val)
		: "eax", "cc" );	
	
	// Set CR3
	asm volatile("movl %0, %%cr3"
		: 
		: "r"(page_dir)
		: "cc" );
		
	// Set CR0
	val = 0x80000000;
	asm volatile("movl %%cr0, %%eax; orl %0, %%eax; movl %%eax, %%cr0"
		: 
		: "r"(val)
		: "eax", "cc" );
}

void set_task_page(int32_t phys_addr)
{
	page_dir[32] = (phys_addr & 0xFFC00000) 
		| _PRESENT | _READWRITE | _USER | _4MB;

	// Set CR3
	asm volatile("movl %0, %%cr3"
		: 
		: "r"(page_dir)
		: "cc" );
	
}

