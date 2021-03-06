#ifndef INT_HANDLER_H
#define INT_HANDLER_H

#include "lib.h"
#include "keyboard.h"
#include "rtc.h"

#define IRQ_KEYBOARD 1
#define IRQ_RTC 8

extern void int_handler(int index);

extern void irq0_handler();
extern void irq1_handler();
extern void irq2_handler();
extern void irq3_handler();
extern void irq4_handler();
extern void irq5_handler();
extern void irq6_handler();
extern void irq7_handler();
extern void irq8_handler();
extern void irq9_handler();
extern void irq10_handler();
extern void irq11_handler();
extern void irq12_handler();
extern void irq13_handler();
extern void irq14_handler();
extern void irq15_handler();

#endif //INT_HANDLER_H

