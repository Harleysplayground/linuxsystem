#ifndef TERMINAL_H
#define TERMINAL_H

#include "lib.h"

#define BUF_SIZE 128

volatile int32_t read_buf;
int32_t buf_pos;
int8_t buffer[BUF_SIZE];

extern int32_t terminal_open(const uint8_t *filename);
extern int32_t terminal_read(int32_t fd, void *buf, int32_t n_bytes);
extern int32_t terminal_write(int32_t fd, void *buf, int32_t n_bytes);
extern int32_t terminal_close(int32_t fd);

#endif //TERMINAL_H
