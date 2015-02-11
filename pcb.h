#ifndef PCB_H
#define PCB_H

#include "lib.h"
#include "file_system.h"
#include "terminal.h" // for BUF_SIZE

typedef struct pcb pcb_t;
struct pcb // similar to the thread_info structure in process descriptor
{
	// INFO
	uint32_t pid;
	uint32_t phys_addr;

	// parsed from the command
	uint8_t prog_name[BUF_SIZE];
	uint8_t arg[BUF_SIZE];

	// file system
	file_desc_t file_arr[MAX_FILES];
	uint32_t n_file_desc;
	
	// parent
	pcb_t *parent;
	
	// return info
	uint32_t ret_esp;
	uint32_t ret_ebp;
	uint32_t ret_eip;

	// tss
	uint32_t ss0;
	uint32_t esp0;

	// no idea what it is for
	uint32_t sig_info; 
};

extern pcb_t *get_cur_pcb();

#endif //PCB_H
