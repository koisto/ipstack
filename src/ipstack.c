#include "ipstack.h"

#include <stdint.h>

uint8_t g_packet[500];
uint16_t g_rx_packets = 0;

extern void log_write(char *format, ...);

void ipstack_poll(void)
{
	uint16_t	length, rc, payload_idx;
	uint8_t		proto;
	ip_addr_t 	src;

	length = slip_recv_packet(g_packet, 500);

	rc =  ip_parse_header(g_packet, length, &payload_idx, &proto, &src);
	if (rc == 0)
	{
		++g_rx_packets; 
		log_write("%4d - Packet received: length = %3d, protocol = %2d, source = %u,%u,%u,%u\n", g_rx_packets, length, proto, \
																				src.bytes[0], src.bytes[1], src.bytes[2], src.bytes[3]);

		
	}
	else
	{
		log_write("Error! %d %d\n", length, rc);
	}

}