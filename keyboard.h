#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "lib.h"

#define KEYBOARD_ENCODER 	0x60
#define KEYBOARD_CONTROLLER 0x64
#define CTRL_PRESSED		29
#define CTRL_RELEASED		157
#define BACKSPACE			14

extern void keyboard_handler();

extern void keyboard_init();

#endif //KEYBOARD_H


