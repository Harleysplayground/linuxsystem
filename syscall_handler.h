#ifndef SYSCALL_HANDLER_H
#define SYSCALL_HANDLER_H

#include "lib.h"

#define PCB_MASK	 0xFFFFE000
#define ENTRY_OFFSET 24
#define MAG_NUM_LEN  4
#define STDIN_DESC   0
#define STDOUT_DESC  1
#define MAX_TASK     6

#define MB(n)  ((n) * 0x100000)
#define KB(n) ((n) * 0x400)
#define GET_PCB(pid) (MB(8)-KB(8)*(pid+1))
#define GET_ESP(pid) (MB(8)-KB(8)*pid-4)
#define USER_ESP (MB(132)-4)

extern void syscall_handler();

extern int32_t halt(uint8_t status);
extern int32_t execute(const uint8_t *command);
extern int32_t read(int32_t fd, void *buf, int32_t nbytes);
extern int32_t write(int32_t fd, const void *buf, int32_t nbytes);
extern int32_t open(const uint8_t *filename);
extern int32_t close(int32_t fd);
extern int32_t getargs(uint8_t *buf, int32_t nbytes);
extern int32_t vidmap(uint8_t **screen_start);
extern int32_t set_handler(int32_t signum, void *handler_address);
extern int32_t sigreturn(void);

#endif //SYSCALL_HANDLER_H
