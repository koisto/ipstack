#include "ipstack.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

// this function writes to a log file
void log_write(char *format, ...)
{
	FILE * f;
	va_list args;

	f = fopen("log.txt", "a+");
	if (f != NULL)
	{
		va_start(args, format);
		vfprintf(f, format, args);
		va_end(args);

		fclose(f);
	}

}

// send_char function used by slip_send
// the demo uses a psuedo terminal and as such we write 
// data to stdout
void send_char(uint8_t c)
{
	fwrite(&c, 1, 1, stdout);
}

// recv_char function used by slip_recv
// the demo uses a psuedo terminal and as such we read 
// data from stdin
uint8_t recv_char(void)
{
	uint8_t c;
	fread(&c, 1, 1, stdin);
	return c;
}

int main (void)
{
	log_write("ipstack demo\n");
	while (1)
	{
		ipstack_poll();
	}

	return 0;
}
