//
#include <stddef.h>
#include "ip.h"
#include "ipstack_conf.h"

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

uint16_t ip_parse_header(uint8_t * packet, uint16_t len, uint16_t * payload_idx, uint8_t * proto, ip_addr_t * src)
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
	
	// check that destination matches our host address
	if ((g_host_addr.bytes[0] != packet[16]) ||
		(g_host_addr.bytes[1] != packet[17]) ||
		(g_host_addr.bytes[2] != packet[18]) ||
		(g_host_addr.bytes[3] != packet[19]))
		return IP_HEADER_ERROR;		

	// store the protocol value from the header
	if (proto != NULL)
		*proto = packet[9];

	// store the source IP address
	if (src != NULL){
		src->bytes[0] = packet[12];
		src->bytes[1] = packet[13];
		src->bytes[2] = packet[14];
		src->bytes[3] = packet[15];
	}	

	// set pointer to payload data
	if (payload_idx != NULL)
	{
		if (len - hdr_len)
			*payload_idx = hdr_len;
		else
			*payload_idx = 0;
	}

	return 0;
}

void ip_create_header(uint8_t * packet, uint16_t len, uint8_t proto, ip_addr_t * dest)
{
	uint16_t checksum;

	packet[0] 	= 0x45;		// version and header length
	packet[1] 	= 0x00;		// dscp and ecn
	packet[2] 	= (uint8_t) (len >> 8);	// length	
	packet[3] 	= (uint8_t) (len & 0xff);
	packet[4] 	= 0x00;		// identification
	packet[5] 	= 0x01;
	packet[6] 	= 0x00;		// flags and fragment offset
	packet[7] 	= 0x00;		// fragment offset
	packet[8] 	= 0x40;		// time to live
	packet[9] 	= proto;	// protocol
	packet[10] 	= 0x00;		// checksum
	packet[11] 	= 0x00;
	packet[12] 	= g_host_addr.bytes[0];	// source IP
	packet[13] 	= g_host_addr.bytes[1];
	packet[14] 	= g_host_addr.bytes[2];
	packet[15] 	= g_host_addr.bytes[3];
	packet[16] 	= dest->bytes[0];		// destination IP
	packet[17] 	= dest->bytes[1];
	packet[18] 	= dest->bytes[2];
	packet[19] 	= dest->bytes[3];

	// calculate the header checksum 	
	checksum = ip_checksum(packet,20);
	packet[10] 	= (uint8_t) (checksum >> 8);	
	packet[11] 	= (uint8_t) (checksum & 0xff);

}