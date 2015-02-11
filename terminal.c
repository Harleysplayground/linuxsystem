#include "terminal.h"

int32_t terminal_open(const uint8_t *filename)
{
	read_buf = 0;
	buf_pos = 0;
	return 0;
}

int32_t terminal_read(int32_t fd, void *buf, int32_t n_bytes)
{
	int i, buf_size;
	read_buf = 1;
	while (read_buf) {}
	for (i = 0; i < buf_pos && i < n_bytes; i++)
	{
		((int8_t *)buf)[i] = buffer[i];
	}
	buf_size = buf_pos;
	buf_pos = 0;
	return buf_size;
}

/* terminal_write(int32_t fd, void *buf, int32_t n_bytes)
 * This function prints the output
 * INPUTS:	None
 * OUTPUTS:	None
 * SIDE EFFECTS: NONE
 */
int32_t terminal_write(int32_t fd, void *buf, int32_t n_bytes)
{
	return puts((int8_t *)buf);
}

/* terminal_close()
 * This function does nothing
 * INPUTS:	None
 * OUTPUTS:	None
 * SIDE EFFECTS: NONE
 */
int32_t terminal_close(int32_t fd)
{
	return 0;
}
