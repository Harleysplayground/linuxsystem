#ifndef PAGING_H
#define PAGING_H

#include "lib.h"

#define KERNEL_PHYS   0x00400000
#define VIDEO_PHYS    0x000B8000
#define PROGRAM_PHYS  0x00800000
#define SIZE_PD       1024 // size of page directory
#define SIZE_PT       1024 // size of page table
#define FOUR_KB       0x1000

extern void paging_init();
extern set_task_page(int32_t phys_addr);

#endif //PAGING_H
