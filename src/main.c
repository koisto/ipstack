#include "ipstack.h"
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

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

void send_char(uint8_t c)
{
	fwrite(&c, 1, 1, stdout);
}

uint8_t recv_char(void)
{
	uint8_t c;
	fread(&c, 1, 1, stdin);
	return c;
}

int main (void)
{
	while (1)
	{
		ipstack_poll();
	}

	return 0;
}