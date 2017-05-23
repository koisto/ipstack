#include "ipstack.h"
#include "slip.h"
#include "ip.h"

#include <stdint.h>

uint8_t g_buffer[500];
uint16_t g_packet_len;

uint16_t g_rx_packets = 0;

extern void log_write(char *format, ...);

void ipstack_poll(void)
{
	uint16_t	rc, payload_idx;
	uint8_t		proto;
	ip_addr_t 	src;

	g_packet_len = slip_recv_packet(g_buffer, 500);

	rc =  ip_parse_header(&payload_idx, &proto, &src);
	if (rc == 0)
	{
		++g_rx_packets; 
		log_write("%4d - Packet received: length = %3d, protocol = %2d, source = %u,%u,%u,%u\n", g_rx_packets, g_packet_len, proto, \
																				src.bytes[0], src.bytes[1], src.bytes[2], src.bytes[3]);

		
	}
	else
	{
		log_write("Error! %d %d\n", length, rc);
	}

}