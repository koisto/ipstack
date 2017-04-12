//
#include "ip.h"
#include "debug.h"

#include <stddef.h>

uint16_t ip_checksum (uint8_t * header, uint16_t len)
{
	uint16_t sum = 0, temp;

	while (len > 0)
	{
		temp = (header[len - 2] << 8) + header[len - 1];
		sum += temp;
		// check for overflow and add carry
		if (sum < temp)
			sum++;	  

		len -= 2;	 
	}

	return ~sum;
}

uint16_t ip_parse_header(uint8_t * packet, uint16_t len, uint8_t * payload, uint16_t * payload_len, uint8_t * proto, ip_addr_t * src)
{
	uint16_t hdr_len;

	// check the version in the header is valid	
	if ((packet[0] >> 4) != 4)
		return IP_HEADER_ERROR;

	hdr_len = (packet[0] & 0x0f) << 2;

	// check the length in the header is valid - at least 20 bytes 	
	if (hdr_len < 20)
		return IP_HEADER_ERROR;
	
	// check that the packet length is greater than the header size 	
	if (hdr_len > len)
		return IP_HEADER_ERROR;
	
	// calculate the header checksum 	
	if (ip_checksum(packet,hdr_len))
		return IP_HEADER_ERROR;
	
	// store the protocol value from the header
	if (proto != NULL)
		*proto = packet[9];

	return 0;
}