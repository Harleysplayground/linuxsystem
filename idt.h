#ifndef IDT_H
#define IDT_H

#include "lib.h"

#define USER_MODE 3
#define KERNEL_MODE 0

#define SYSCALL_ENTRY 0x80

extern void idt_init();

#endif //IDT_H
