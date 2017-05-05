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
	uint8_t 	packet[500];
	uint16_t	length, rc, payload_idx;
	uint8_t		proto;
	ip_addr_t 	src;

	int i = 0;

	while (1)
	{
		length = slip_recv_packet(packet, 500);

		rc =  ip_parse_header(packet, length, &payload_idx, &proto, &src);
		if (rc == 0)
		{
			log_write("%4d - Packet received: length = %3d, protocol = %2d, source = %u,%u,%u,%u\n", i++, length, proto, \
																				src.bytes[0], src.bytes[1], src.bytes[2], src.bytes[3]);
		}
		else
		{
			log_write("Error! %d %d\n", length, rc);
		}

	}

	return 0;
}