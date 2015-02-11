#include "file_system.h"
#include "terminal.h"
#include "rtc.h"

#define MAX_NAME_LEN 31
#define MIN(a, b) ((a) < (b))?(a):(b)

static int32_t cur_file;
static bblock_t *bblock;

file_op_t terminal_op;
file_op_t rtc_op;
file_op_t file_op;

/* get_file_length()
 * DESCRIPTION - gets the file length in current directory entry 
 * INPUT - int inode: index of the directory entry
 * OUTPUT - int length: the file length 
 */
int32_t get_file_length(uint32_t inode)
{
	inode_t* inode_ptr = (inode_t*) (bblock + inode + 1);
	return inode_ptr->length;
}

void file_system_init(bblock_t *boot_block)
{
	bblock = boot_block;
	
	terminal_op.open = (void *)terminal_open;
	terminal_op.read = (void *)terminal_read;
	terminal_op.write = (void *)terminal_write;
	
	rtc_op.open = (void *)rtc_open;
	rtc_op.read = (void *)rtc_read;
	rtc_op.write = (void *)rtc_write;
	
	file_op.open = (void *)file_open;
	file_op.read = (void *)file_read;
	file_op.write = (void *)file_write;
}

/* read_dentry_by_name(const uint8_t *fname, dentry_t *dentry)
 * DESCRIPTION - find the correct directory entry with name fname and copy it back to dentry_t* dentry
 * INPUT - const uint8_t *fname: the name to find in dentry
 * INPUT - dentry_t* dentry: copy the correct directory entry here
 * OUTPUT - 0 if successful, -1 if error
 */
int32_t read_dentry_by_name(const uint8_t *fname, dentry_t *dentry)
{
	uint32_t i;
	uint32_t len1, len2;
	uint8_t * filename;
	for (i = 0; i < MAX_DENTRY; i++)
	{	
		filename = bblock->dentry[i].file_name;
		len1 = 0;
		//Get the length of fnames, respectively	
		while(len1 < MAX_NAME_LEN+1 && filename[len1] != '\0')
			len1++;
		
		len2 = strlen((int8_t *)fname);
		if (len1 == MAX_NAME_LEN && filename[len1] == '\0')
		{
			if (len2 > MAX_NAME_LEN) len2 = MAX_NAME_LEN;
		}

		//Check if the file names and lengths match
		if ((len1 == len2) && (!strncmp((int8_t *)bblock->dentry[i].file_name, ((int8_t *)fname), len1)))
		{
			if (!memcpy((void *)dentry, (void *)(&(bblock->dentry[i])), sizeof(dentry_t)))
				return -1;
			return 0;
		}
	}

	return -1; // non-existent file
}

/* read_dentry_by_index(uint32_t index, dentry_t *dentry)
 * DESCRIPTION - find the correct directory entry with name fname and copy it back to dentry_t* dentry
 * INPUT - const uint8_t *fname: the name to find in dentry
 * INPUT - dentry_t* dentry: copy the correct directory entry here
 * OUTPUT - 0 if successful, -1 if error
 */
int32_t read_dentry_by_index(uint32_t index, dentry_t *dentry)
{
	if ((index < 0) || (index >= bblock->num_dir_entry)) 
		return -1; // invalid index
	
	if (!memcpy((void *)dentry, (void *)(&(bblock->dentry[index])), sizeof(dentry_t)))
		return -1;
	return 0;
}

int32_t read_data(uint32_t inode, uint32_t offset, uint8_t *buf, uint32_t length)
{
	void *dblock_ptr;
	volatile int end;
	
	if (inode >= bblock->num_inode)
		return -1; // invalid inode number
	
	inode_t *inode_ptr = (inode_t *)(bblock + inode + 1);
	
	uint32_t n_byte_copied = 0;
	uint32_t n_byte_left = length;
	uint32_t n_byte_avail = inode_ptr->length - offset;
	uint32_t n_byte_copying = 0;
	
	uint32_t dblock_num = offset / SIZE_BLOCK;
	uint32_t index_dblock = inode_ptr->dblock[dblock_num];
	uint32_t offset_dblock = offset % SIZE_BLOCK;
	
	dblock_ptr = (void *)(((uint32_t)(bblock + 1 + bblock->num_inode + index_dblock)) + offset_dblock);
	
	end = 0;
	while (!end)
	{
		uint8_t copy_case;
		// case 1: n_byte_copying = n_byte_left (n_byte left to copy)
		// case 2: n_byte_copying = n_byte_avail (n_byte left in file)
		// case 3: n_byte_copying = SIZE_BLOCK - offset_dblock (n_byte left in data block)
		uint32_t case1 = n_byte_left;
		uint32_t case2 = n_byte_avail;
		uint32_t case3 = SIZE_BLOCK - offset_dblock;
		if ((case1 <= case2) && (case1 <= case3))
		{
			n_byte_copying = case1;
			copy_case = 1;
		}
		if ((case2 <= case1) && (case2 <= case3))
		{
			n_byte_copying = case2;
			copy_case = 2;
		}
		if ((case3 <= case1) && (case3 <= case2))
		{
			n_byte_copying = case3;
			copy_case = 3;
		}
		
		memcpy((void *)(buf + n_byte_copied),
			dblock_ptr,
			n_byte_copying);
			
		n_byte_copied += n_byte_copying;
		n_byte_left   -= n_byte_copying;
		n_byte_avail  -= n_byte_copying;
		
		switch (copy_case)
		{
		case 1:
		{
			end = 1;
		}
		break;
		case 2:
		{
			end = 1;
		}
		break;
		case 3:
		{
			dblock_num++;
			index_dblock = inode_ptr->dblock[dblock_num];
			offset_dblock = 0;
			dblock_ptr = (void *)(((uint32_t)(bblock + 1 + bblock->num_inode + index_dblock)) + offset_dblock);
		}
		break;
		default:
		{
		}
		break;
		}
	}
	
	n_byte_copied = length;
	n_byte_avail = inode_ptr->length - offset;
	
	if (n_byte_copied >= n_byte_avail) return 0; // end of file has been reached
	else return n_byte_copied; // number of bytes read
}

int32_t file_read(int32_t fd, void *buf, int32_t nbytes)
{
	/*
	dentry_t dentry;
	int32_t bytes_read, i;

	uint32_t inode;
	pcb_t* cur_pcb = get_cur_pcb();

	file_desc_t* fd_ptr = &(cur_pcb->file_arr[fd]);

	// Check that file descriptor is "in-use"
	if ( fd_ptr->flags != 1)  {
		return -1;
	}
	
	// Check if it is a directory read
	if ( fd_ptr->inode == NULL ) {
		return directory_read(fd, buf, nbytes);
	}
	
	//Get the inode by (inode ptr - bblock ptr)/SIZE_BLOCK 
	inode = (((uint32_t)(fd_ptr -> inode)) - (uint32_t)bblock)/SIZE_BLOCK - 1;
	
	for(i = 0; i < MAX_DENTRY; i++)
	{

			// Check that directory entry matches
		if ( bblock->dentry[i].inode_num == inode )
		{
			//If file doesn't exist
			if(read_dentry_by_name(bblock->dentry[i].file_name, &dentry) != 0)
			{
				printf("%d", bytes_read );
				return 0;
			}

			bytes_read = read_data(dentry.inode_num, fd_ptr->file_pos, buf, nbytes);

			//Update file position
			if(nbytes != 0 && bytes_read ==0)
				fd_ptr->file_pos = 0;
			else
				fd_ptr->file_pos += bytes_read;

			return bytes_read;
		}

	}
	*/
	return 0;
}

int32_t file_write(int32_t fd, void *buf, int32_t n_bytes)
{
	return -1; // failure since the file system is read-only
}

int32_t file_open(const uint8_t *filename)
{
	return 0;
}

int32_t file_close(int32_t fd)
{
	return 0;
}

int32_t directory_read(int32_t fd, void *buf, int32_t nbytes)
{
	uint32_t num_dentry = bblock->num_dir_entry;
	if (cur_file >= num_dentry) // 
        return 0;
	
	uint8_t *fname = bblock->dentry[cur_file].file_name;
	strcpy((int8_t *)buf, (int8_t *)fname);
	cur_file++;
	return strlen((int8_t *)fname);
}

int32_t directory_write(int32_t fd, void *buf, int32_t nbytes)
{
	return -1; // failure since the file system is read-only
}

int32_t directory_open()
{
	cur_file = 0;
	return 0;
}

int32_t directory_close()
{
	return 0;
}

