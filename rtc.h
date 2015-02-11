#ifndef RTC_H 
#define RTC_H

#include "lib.h"

extern void rtc_handler();

extern void rtc_init();

extern int32_t rtc_open(const uint8_t *filename);

extern int32_t rtc_read(int32_t fd, void *buf, int32_t n_bytes);

extern int32_t rtc_write(int32_t fd, void *buf, int32_t n_bytes);

extern int32_t rtc_close(int32_t fd);

#endif //RTC_H
