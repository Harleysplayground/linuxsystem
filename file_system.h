#ifndef FILE_SYSTEM_H
#define FILE_SYSTEM_H

#include "lib.h"

#define ESP_MASK	   0xFFFFE000 // mask out 13 LSBs
#define TASK_ADDR        0x08048000

#define RESERVED1      24
#define RESERVED2      52
#define MAX_DENTRY     63
#define MAX_DBLOCK     1023
#define SIZE_BLOCK     4096
#define MAX_FILE_NAME  32
#define MAX_FILES      8

typedef struct directory_entry
{
	uint8_t file_name[MAX_FILE_NAME];
	uint32_t file_type;
	uint32_t inode_num;
	uint8_t reserved[RESERVED1];
} dentry_t;

typedef struct boot_block
{
	uint32_t num_dir_entry;
	uint32_t num_inode;
	uint32_t num_data_block;
	uint8_t reserved[RESERVED2];
	dentry_t dentry[MAX_DENTRY];
} bblock_t;

typedef struct inode
{
	uint32_t length;
	uint32_t dblock[MAX_DBLOCK];
} inode_t;

typedef struct file_op
{
	int32_t (*open) (const uint8_t *filename);
	int32_t (*read) (int32_t fd, void *buf, int32_t nbytes);
    int32_t (*write) (int32_t fd, const void *buf, int32_t nbytes);
	int32_t (*close) (int32_t fd);
} file_op_t;

typedef struct file_desc
{
    file_op_t *file_op;
    inode_t *inode;
    uint32_t file_pos;
    uint32_t flags;
} file_desc_t;

extern file_op_t terminal_op;
extern file_op_t rtc_op;
extern file_op_t file_op;

extern int32_t get_file_length(uint32_t inode);

extern void file_system_init(bblock_t *boot_block);

// read
extern int32_t read_dentry_by_name(const uint8_t *fname, dentry_t *dentry);

extern int32_t read_dentry_by_index(uint32_t index, dentry_t *dentry);

extern int32_t read_data(uint32_t inode, uint32_t offset, uint8_t *buf, uint32_t length);


// file
extern int32_t file_read(int32_t fd, void *buf, int32_t n_bytes);

extern int32_t file_write(int32_t fd, void *buf, int32_t n_bytes);

extern int32_t file_open(const uint8_t *filename);

extern int32_t file_close(int32_t fd);


// directory
extern int32_t directory_read(int32_t fd, void *buf, int32_t nbytes);

extern int32_t directory_write(int32_t fd, void *buf, int32_t n_bytes);

extern int32_t directory_open();

extern int32_t directory_close();

#endif //FILE_SYSTEM_H

