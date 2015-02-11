#include "syscall_handler.h"
#include "x86_desc.h"
#include "terminal.h"
#include "rtc.h"
#include "paging.h"
#include "file_system.h"
#include "pcb.h"

static int8_t pid_bitmap[MAX_TASK];

static uint32_t min_pid()
{
	uint32_t i;
	for (i = 0; i < MAX_TASK; i++)
	{
		if (pid_bitmap[i] == 0)
			return i;
	}
	return -1;
}

int32_t halt(uint8_t status)
{
    printf("halt\n");
    uint32_t i;
	pcb_t *cur_pcb = get_cur_pcb();

	// close file descriptor
	for (i = 0; i < MAX_FILES; i++)
	{
		if (cur_pcb->file_arr[i].flags != 0)
			close(i);
	}

	// remove from bitmap
	pid_bitmap[cur_pcb->pid] = 0;

	// reset paging 
	set_task_page(cur_pcb->parent.phys_addr);

	// restore tss
	tss.esp0 = cur_pcb->esp0;

	// switch back
    asm volatile("              \n\
            cli                 \n\
                                \n\
            movw  %0, %%ax      \n\
            movw %%ax, %%ds     \n\
                                \n\
            pushf               \n\
            popl %%eax          \n\
            orl $0x200, %%eax   \n\
            pushl %%eax         \n\
                                \n\
            pushl %2            \n\
            pushl %3            \n\
            movl %4, %%ebp      \n\
            iret                \n\
            "
            : 
            : "i"(KERNEL_DS), "g"(cur_pcb->ret_esp), "i"(KERNEL_CS), "g"(cur_pcb->ret_eip), "g"(cur_pcb->ret_ebp)
            : "%eax"
            );

    return 0;
}

static void parse_command(const uint8_t *command, pcb_t *pcb)
{
	uint32_t i, j;

	// program name
	for (i = 0; i < BUF_SIZE; i++)
	{
		if ((command[i] == ' ') || (command[i] == '\0'))
		{
			// '\0' == 0, '0' == 48
			pcb->prog_name[i] = '\0';
			break;
		}
		else
		{
			pcb->prog_name[i] = command[i];
		}
	}
	// skip space
	for (; i < BUF_SIZE; i++)
	{
		if ((command[i] != ' ') || (command[i] != '\0'))
			break;
	}
	// arguments
	for (j = 0; i < BUF_SIZE; i++, j++)
	{
		pcb->arg[j] = command[i];
	}
}

int32_t execute(const uint8_t *command)
{
    printf("execute\n");
	uint8_t prog_name[BUF_SIZE];
	uint8_t mag_num[MAG_NUM_LEN];
	dentry_t dentry;
	pcb_t new_pcb;
	uint32_t entry_pt;
	uint32_t length;
	
	if (command == NULL)
		return -1; // command is a null pointer

	// parse the command to get the name of the program
	parse_command(command, &new_pcb);
	
	// get dentry
	if (read_dentry_by_name(new_pcb.prog_name, &dentry) == -1)
		return -1; //file does not exist

	// checking if the file is indeed an executable
	read_data(dentry.inode_num, 0, mag_num, MAG_NUM_LEN); 
	if ((mag_num[0] != 0x7f) || (mag_num[1] != 0x45) || 
	    (mag_num[2] != 0x4c) || (mag_num[3] != 0x46))
	{
		printf("not an exe\n");
		return -1;
	}
	printf("an exe\n");
	
	// get virtual address 
	read_data(dentry.inode_num, ENTRY_OFFSET, (uint8_t *)&entry_pt, 4); 
	printf("entry point: %x\n", entry_pt);
	
	// set up pcb
	// set first entry in file array to be stdin
	new_pcb.file_arr[STDIN_DESC].file_op = &terminal_op;
	new_pcb.file_arr[STDIN_DESC].inode = NULL;
	new_pcb.file_arr[STDIN_DESC].file_pos = 0;
	new_pcb.file_arr[STDIN_DESC].flags = 1;
	// set second entry in file array to be stdout
	new_pcb.file_arr[STDOUT_DESC].file_op = &terminal_op;
	new_pcb.file_arr[STDOUT_DESC].inode = NULL;
	new_pcb.file_arr[STDOUT_DESC].file_pos = 0;
	new_pcb.file_arr[STDOUT_DESC].flags = 1;
	// other informations
	if (new_pcb.pid = min_pid() == -1)
		return -1;	
	pid_bitmap[new_pcb.pid] = 1;
	new_pcb.phys_addr = MB(8)+MB(4)*new_pcb.pid;
	new_pcb.n_file_desc = 2;
	new_pcb.parent = NULL;
	new_pcb.sig_info = 0;
	//
	
	set_task_page(new_pcb.phys_addr);

	// load to virtual memory
	length = get_file_length(dentry.inode_num);
	printf("file size: %d KB\n", length);
	if (read_data(dentry.inode_num, 0, (uint8_t *)TASK_ADDR, length) == -1)
		return -1;
	printf("load to %x\n", TASK_ADDR);
	
	// save registers
	asm volatile("               \n\
			movl %%ebp, %0       \n\
			movl %%esp, %1       \n\
			movl $exe_return, %2 \n\
			"
		: "=a" (new_pcb.ret_ebp), "=a" (new_pcb.ret_esp), "=a" (new_pcb.ret_eip)
		:
		: "cc", "memory");

	// change tss
	new_pcb.esp0 = tss.esp0;
	tss.ss0 = KERNEL_DS;
	tss.esp0 = GET_ESP(new_pcb.pid);

	memcpy((void *)GET_PCB(new_pcb.pid), &new_pcb, sizeof(pcb_t));
	printf("set up pcb\n");

	// switch to user mode
	// stack prior to IRET: SS/ESP/EFLAGS/CS/EIP
	// IF: bit 9 of EFLAGS
	asm volatile("        	   \n\
			cli                \n\
			movw %1, %%ax      \n\
			movw %%ax, %%ds    \n\
							   \n\
			pushl %1           \n\
			pushl %3           \n\
							   \n\
			pushfl             \n\
			popl %%eax         \n\
			orl $0x200, %%eax  \n\
			pushl %%eax        \n\
							   \n\
			pushl %0           \n\
			pushl %2           \n\
			iret               \n\
			exe_return:        \n\
			"
		: 
		: "g" (USER_CS), "g" (USER_DS), "g" (entry_pt), "g" (USER_ESP)
		: "%eax", "memory");

    return 0;
}

int32_t read(int32_t fd, void *buf, int32_t nbytes)
{
    printf("read: %d\n", fd);
	if ((fd >= 0) && (fd < NUM_FILES))
	{
		pcb_t* cur_pcb = get_cur_pcb();
		return cur_pcb->file_arr[fd].file_op->read(fd, buf, nbytes);
	}
	else
	{
		return -1;
	}
}

int32_t write(int32_t fd, const void *buf, int32_t nbytes)
{
	uint32_t eesspp;
	asm volatile ("mov %%esp, %0": "=r"(eesspp));
	printf("esp: %x\n", eesspp);
    printf("write: %d\n", fd);
	if ((fd >= 0) && (fd < NUM_FILES))
	{
		pcb_t* cur_pcb = 0x007FE000;//get_cur_pcb();
		cur_pcb->file_arr[fd].file_op->write(fd, buf, nbytes);
		printf("write done: %d\n", fd);
		return 0;
	}
	else
	{
		return -1;
	}
}

static int open_pcb(dentry_t *dentry, pcb_t *pcb)
{
	if (pcb->n_file_desc == NUM_FILES)
		return -1;
	
	switch (dentry->file_type)
	{
	case 0:
		pcb->file_arr[pcb->n_file_desc].file_op = &rtc_op;
		pcb->file_arr[pcb->n_file_desc].inode = NULL;
		pcb->file_arr[pcb->n_file_desc].file_pos = 0;
		pcb->file_arr[pcb->n_file_desc].flags = 1;
	case 1:
		pcb->file_arr[pcb->n_file_desc].file_op = &file_op;
		pcb->file_arr[pcb->n_file_desc].inode = NULL;
		pcb->file_arr[pcb->n_file_desc].file_pos = 0;
		pcb->file_arr[pcb->n_file_desc].flags = 1;
	case 2:
		pcb->file_arr[pcb->n_file_desc].file_op = &file_op;
		pcb->file_arr[pcb->n_file_desc].inode = NULL;
		pcb->file_arr[pcb->n_file_desc].file_pos = 0;
		pcb->file_arr[pcb->n_file_desc].flags = 1;
	}
	pcb->n_file_desc++;
	
	return 0;
}

int32_t open(const uint8_t *filename)
{
    printf("open\n");
	dentry_t dentry;
	pcb_t *cur_pcb;
	
	if ((filename == NULL) ||
		(read_dentry_by_name(filename, &dentry) == -1))
		return -1;
	
	cur_pcb = get_cur_pcb();
	
	if (!open_pcb(&dentry, cur_pcb))
	{
		cur_pcb->file_arr[cur_pcb->n_file_desc-1].file_op->open(filename);
		cur_pcb->n_file_desc++;
		return 0;
	}
	else
	{
		return -1;
	}
}

int32_t close(int32_t fd)
{
    printf("close\n");
	pcb_t* cur_pcb = get_cur_pcb();
	
	if ((fd < NUM_FILES) && (cur_pcb->file_arr[fd].flags != 0))
	{
		(cur_pcb->file_arr[fd]).flags = 0;
		cur_pcb->file_arr[fd].file_op->close(fd);
		cur_pcb->n_file_desc--;
		return 0;
	}
	else
	{
		return -1;
	}
}

int32_t getargs(uint8_t *buf, int32_t nbytes)
{
    printf("getargs\n");
    return 0;
}

int32_t vidmap(uint8_t **screen_start)
{
    printf("vidmap\n");
    return 0;
}

int32_t set_handler(int32_t signum, void *handler_address)
{
    return -1;
}

int32_t sigreturn(void)
{
    return -1;
}
