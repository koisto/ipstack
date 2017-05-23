#include "icmp.h"

#include "ipstack.h"
#include "ip.h"


uint16_t icmp_receive(uint8_t * packet, uint16_t len)
{
	uint8_t type, code;

	// calculate the header checksum 	
	//if (ip_checksum(packet,len))
	//	return IP_HEADER_ERROR;

	type = packet[0];
	code = packet[1];

	switch (type)
	{ 
		case ICMP_ECHO_REPLY :
			break;

		case ICMP_DEST_UNREACHABLE :
			break;

		case ICMP_ECHO_REQUEST :
			break;

		default :
			break;		
	}



	return 0;
}

uint16_t icmp_send(uint8_t type, uint8_t code, uint8_t * rest_of_header, uint8_t * data, uint16_t len, uint8_t * packet)
{
	return 0;
}