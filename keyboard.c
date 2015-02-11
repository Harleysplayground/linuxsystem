#include "keyboard.h"
#include "i8259.h"
#include "terminal.h"

static int32_t eflags;
static int ctrl_pressed;

/*
 * scan code table
 */
static int8_t scan_code_table[89] = 
{
'\0','\0','1','2','3','4','5','6','7','8',
'9','0','-','=','\0','\t','q','w','e','r',
't','y','u','i','o','p','[',']','\n','\0',
'a','s','d','f','g','h','j','k','l',';',
'\'','`','\0','\\','z','x','c','v','b','n',
'm',',','.','/','\0','\0','\0','\0','\0','\0',
'\0','\0','\0','\0','\0','\0','\0','\0','\0','\0',
'\0','7','8','9','-','4','5','6','+','1',
'2','3','0','.','\0','\0','\0','\0','\0'
};

/* print_key(uint8_t scan_code)
 * DESCRIPTION - prints the key on the screen
 * INPUT - scan_code: scan code of the key press
 * OUTPUT - NONE
 * SIDE EFFECTS - None
 */
static void print_key(uint8_t scan_code)
{
	if (scan_code != BACKSPACE)
	{
		int8_t c = scan_code_table[scan_code];
		putc(c);
		if (buf_pos < BUF_SIZE)
		{
			if (c == '\n')
			{
				buffer[buf_pos] = c;
				buf_pos++;
				read_buf = 0;
			}
			else{				
				buffer[buf_pos] = c;
				buf_pos++;
				if (buf_pos == BUF_SIZE)
					read_buf = 0;
			}
		}
	}
	else
	{
		backspace();
		if (buf_pos > 0)
			buf_pos--;
	}
	return;

}

/* keyboard_handler()
 * DESCRIPTION - a keyboard handler that handles the keyboard input
 * INPUT - NONE
 * OUTPUT - NONE
 * SIDE EFFECTS - None
 */
void keyboard_handler()
{
	uint8_t scan_code;
	
	cli_and_save(eflags); // disable interrupts

	scan_code = inb(KEYBOARD_ENCODER); // get the scan code received from the port
	if (scan_code == CTRL_PRESSED)
		ctrl_pressed = 1;
	if (scan_code == CTRL_RELEASED)
		ctrl_pressed = 0;
	if (ctrl_pressed == 0)
	{
		if (!((scan_code >= 0x81) && (scan_code <= 0xD8))) // not release
			print_key(scan_code); // map the scan code to key and print it on the screen
	}
	else
	{
		if (scan_code_table[scan_code]=='l')
			clear_screen();
	}
	send_eoi(KEYBOARD_IRQ);
	restore_flags(eflags); // restore from cli_and_save
}

/* keyboard_init()
 * DESCRIPTION - initializes the keyboard settings
 * INPUT - NONE
 * OUTPUT - NONE
 * SIDE EFFECTS - None
 */
void keyboard_init()
{
	cli_and_save(eflags); // disable interrupts
	
	ctrl_pressed = 0;
	enable_irq(KEYBOARD_IRQ);
	
	restore_flags(eflags); // restore from cli_and_save
}



