#include "rtc.h"
#include "i8259.h" // for enable_irq()

#define RTC_CMD			0x70
#define RTC_DATA		0x71
#define RTC_REG_A		0x0A
#define RTC_REG_B		0x0B
#define RTC_REG_C		0x0C
#define RTC_MAX_FREQ	1024
#define RTC_MIN_FREQ	2
#define RTC_MAX_RATE	0x0F
#define RTC_MIN_RATE	6
#define RTC_MASK1       0x40
#define RTC_MASK2       0xF0

static int32_t eflags;
static volatile int32_t read_flag;

/* rtc_handler()
 * This function is the RTC handler and calls test_interrupts
 * INPUTS:	None
 * OUTPUTS:	None
 * SIDE EFFECTS: calls test_interrupts which changes characters on the screen
 */
void rtc_handler()
{	
	cli_and_save(eflags); // disable interrupts
	
	//test_interrupts();
	
	outb(RTC_REG_C, RTC_CMD); // select register C
	inb(RTC_DATA); // throw away contents

	send_eoi(RTC_IRQ);
	restore_flags(eflags); // restore from cli_and_save
	
	read_flag = 0;
}

/* rtc_init()
 * This function initializes the RTC handler
 * INPUTS:	None
 * OUTPUTS:	None
 * SIDE EFFECTS: None
 */
void rtc_init()
{
	int8_t prev;
	
	cli_and_save(eflags); // disable interrupts

	enable_irq(RTC_IRQ); // enable IRQ
	
	outb(RTC_REG_B , RTC_CMD); // select register B, and disable NMI
	prev = inb(RTC_DATA); // read the current value of register B
	outb(RTC_REG_B, RTC_CMD); // set the index again
	outb(prev | RTC_MASK1, RTC_DATA); // turns on bit 6 of register B
	
	restore_flags(eflags); // restore from cli_and_save
}

/* rtc_read()
 * This function waits for an interrupt to occur before returns
 * INPUTS:	None
 * OUTPUTS:	0
 * SIDE EFFECTS: waits for the interrupt before returns
 */
int32_t rtc_read(int32_t fd, void *buf, int32_t n_bytes)
{
	read_flag = 1;
	while (read_flag) {}
	return 0; // always returns 0
}

/* rtc_write()
 * This function changes interrupt frequency by changing the output divider
 * INPUTS:	buf - the desired interrupt frequency for the RTC
 * OUTPUTS:	0 if success, -1 otherwise
 * SIDE EFFECTS: NONE
 */
int32_t rtc_write(int32_t fd, void *buf, int32_t n_bytes)
{	
	uint32_t freq;
	uint32_t rate;
	int8_t prev;
	
	memcpy(&freq, buf, sizeof(int32_t));
	
	/*
	 * convert from freq to rate
	 * frequency =  32768 >> (rate-1 )= 2^(16-rate), range = [2, 1024]
	 * rate = 16 - log2(freq), range = [6, 15]
	 */
	if ((freq < RTC_MIN_FREQ) || (freq > RTC_MAX_FREQ)) // freq is out of range
		return -1;
	rate = 0;
	while (freq > 1)
	{
		if (freq % 2) // freq is not a power of 2
			return -1;
		freq /= 2;
		rate++;
	}
	rate = RTC_MAX_RATE + 1 - rate; // rate = 16 - rate
	
	cli_and_save(eflags); // disable interrupts
	
	//rate &= RTC_MAX_RATE; // make sure rate is not over 15
	outb(RTC_REG_A, RTC_CMD); // select register A, and disable NMI
	prev = inb(RTC_DATA); // get initial value of register A
	outb(RTC_REG_A, RTC_CMD);  // reset index to A
	outb((prev & RTC_MASK2) | rate, RTC_DATA); // write the rate to A; the rate is the bottom 4 bits
	
	restore_flags(eflags); // restore from cli_and_save
	return sizeof(uint32_t);
}

/* rtc_open()
 * This function set the frequency to be 2 Hz
 * INPUTS:	NONE
 * OUTPUTS:	0
 * SIDE EFFECTS: NONE
 */
int32_t rtc_open(const uint8_t *filename)
{
	uint32_t rate;
	int8_t prev;

	cli_and_save(eflags); // disable interrupts
	
	rate = RTC_MAX_RATE; // freq = 2 -> rate = 16 - log2(freq) = 15
	outb(RTC_REG_A, RTC_CMD); // select register A, and disable NMI
	prev = inb(RTC_DATA); // get initial value of register A
	outb(RTC_REG_A, RTC_CMD);  // reset index to A
	outb((prev & RTC_MASK2) | rate, RTC_DATA); // write the rate to A; the rate is the bottom 4 bits
	
	restore_flags(eflags); // restore from cli_and_save
	return 0;
}

/* rtc_close()
 * This function does nothing
 * INPUTS:	NONE
 * OUTPUTS:	NONE
 * SIDE EFFECTS: NONE
 */
int32_t rtc_close(int32_t fd)
{
	return 0;
}

