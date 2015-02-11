#ifndef EXCP_HANDLER_H
#define EXCP_HANDLER_H

#include "lib.h"

/* array of Exception message */
char *excp_signals[32] =
{
	// Non-maskable interrupts and exceptions
	"ERROR: Divide error", //0
	"ERROR: Debug",
	"ERROR: NMI",
	"ERROR: Breakpoint",
	"ERROR: Overflow",
	"ERROR: Bounds check", //5
	"ERROR: Invalid opcode",
	"ERROR: Device not available",
	"ERROR: Double fault",
	"ERROR: Coprocessor segment overrun",
	"ERROR: Invalid TSS", //10
	"ERROR: Segment not present",
	"ERROR: Stack segment fault",
	"ERROR: General protection",
	"ERROR: Page Fault",
	"ERROR: Intel-reserved", //15
	"ERROR: Floating-point error",
	"ERROR: Alignment check",
	"ERROR: Machine check",
	"ERROR: SIMD floating point",
	// Intel-reserved
	"ERROR: Intel-reserved", //20
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved", //25
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved",
	"ERROR: Intel-reserved", //30
	"ERROR: Intel-reserved",
};

/*
 * DESCRIPTION - prints the exception to the screen
 * INPUT - int index: index of the exception in the idt table
 * OUTPUT - NONE
 */
void excp_handler(int index)
{
	char *excp = excp_signals[index];
	puts(excp);
	while (1);
}

extern void excp0_handler();
extern void excp1_handler();
extern void excp2_handler();
extern void excp3_handler();
extern void excp4_handler();
extern void excp5_handler();
extern void excp6_handler();
extern void excp7_handler();
extern void excp8_handler();
extern void excp9_handler();
extern void excp10_handler();
extern void excp11_handler();
extern void excp12_handler();
extern void excp13_handler();
extern void excp14_handler();
extern void excp15_handler();
extern void excp16_handler();
extern void excp17_handler();
extern void excp18_handler();
extern void excp19_handler();
extern void excp20_handler();
extern void excp21_handler();
extern void excp22_handler();
extern void excp23_handler();
extern void excp24_handler();
extern void excp25_handler();
extern void excp26_handler();
extern void excp27_handler();
extern void excp28_handler();
extern void excp29_handler();
extern void excp30_handler();
extern void excp31_handler();

#endif //EXCP_HANDLER_H
