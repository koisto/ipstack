#include "ipstack.h"
#include "slip.h"
#include "ip.h"
#include "icmp.h"

#include <stdint.h>

ip_addr_t g_host_addr = {HOST_ADDR};

uint8_t g_buffer[500];
uint16_t g_packet_len;

uint16_t g_rx_packets = 0;

extern void log_write(char *format, ...);		


void ipstack_poll(void)
{
	uint16_t	rc, payload_idx, proto_len;
	uint8_t		proto;
	ip_addr_t 	src;

	g_packet_len = slip_recv_packet(g_buffer, 500);

	rc =  ip_parse_header(&payload_idx, &proto, &src);
	if (rc == 0)
	{
		++g_rx_packets; 
		log_write("%4d - Packet received: length = %3d, protocol = %2d, source = %u,%u,%u,%u\n", g_rx_packets, g_packet_len, proto, \
																				src.bytes[0], src.bytes[1], src.bytes[2], src.bytes[3]);

		proto_len = g_packet_len - payload_idx;

		switch (proto)
		{
			case IP_PROTO_ICMP:
				icmp_receive(&src, &g_buffer[payload_idx], proto_len);
				break;
			

			default: 
				
				break;	
		}	

		
	}
	else
	{
		log_write("Error! %d %d\n", g_packet_len, rc);
	}

}