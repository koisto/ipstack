#include "icmp.h"

#include "ipstack.h"
#include "ip.h"
#include "slip.h"


uint16_t icmp_receive(ip_addr_t * src, uint8_t * packet, uint16_t len)
{
	uint8_t type, code;

	// calculate the header checksum 	
	if (ip_checksum(packet,len))
		return IP_HEADER_ERROR;

	type = packet[0];
	code = packet[1];

	switch (type)
	{ 
		case ICMP_ECHO_REPLY :
			break;

		case ICMP_DEST_UNREACHABLE :
			break;

		case ICMP_ECHO_REQUEST :
			icmp_send(src , ICMP_ECHO_REPLY, 0, &packet[4], &packet[8], len-8);
			break;

		default :
			break;		
	}



	return 0;
}

uint16_t icmp_send(ip_addr_t * dest, uint8_t type, uint8_t code, uint8_t * rest_of_header, uint8_t * data, uint16_t data_len)
{
	uint8_t i;
	uint16_t checksum;

	g_buffer[20] = type;
	g_buffer[21] = code;
	g_buffer[22] = 0;
	g_buffer[23] = 0;

	// copy rest of header into buffer
	for (i = 0; i < 4; i++)
		if (rest_of_header != NULL)
			g_buffer[24+i] = rest_of_header[i];
		else
			g_buffer[24+i] = 0;

	// copy data into buffer
	for (i = 0; i < data_len; i++)
		if (data != NULL)	
			g_buffer[28+i] = data[i];
		else
			g_buffer[28+i] = 0;	

	// caluculate the checksum
	checksum = ip_checksum(&g_buffer[20],data_len + 8);
	g_buffer[22] 	= (uint8_t) (checksum >> 8);	
	g_buffer[23] 	= (uint8_t) (checksum & 0xff);

	ip_create_header(data_len + 8, IP_PROTO_ICMP, dest);
	slip_send_packet(g_buffer, data_len + 8 + 20);
	return 0;
}