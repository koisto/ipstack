//
#include "ip.h"

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

uint16_t ip_parse_header(uint16_t * payload_idx, uint8_t * proto, ip_addr_t * src)
{
	uint16_t hdr_len;

	// check the version in the header is valid	
	if ((g_buffer[0] >> 4) != 4)
		return IP_HEADER_ERROR;

	hdr_len = (g_buffer[0] & 0x0f) << 2;

	// check the length in the header is valid - at least 20 bytes 	
	if (hdr_len < 20)
		return IP_HEADER_ERROR;
	
	// check that the g_buffer length is greater than the header size 	
	if (hdr_len > g_packet_len)
		return IP_HEADER_ERROR;
	
	// calculate the header checksum 	
	if (ip_checksum(g_buffer,hdr_len))
		return IP_HEADER_ERROR;
	
	// check that destination matches our host address
	if ((g_host_addr.bytes[0] != g_buffer[16]) ||
		(g_host_addr.bytes[1] != g_buffer[17]) ||
		(g_host_addr.bytes[2] != g_buffer[18]) ||
		(g_host_addr.bytes[3] != g_buffer[19]))
		return IP_HEADER_ERROR;		

	// store the protocol value from the header
	if (proto != NULL)
		*proto = g_buffer[9];

	// store the source IP address
	if (src != NULL){
		src->bytes[0] = g_buffer[12];
		src->bytes[1] = g_buffer[13];
		src->bytes[2] = g_buffer[14];
		src->bytes[3] = g_buffer[15];
	}	

	// set pointer to payload data
	if (payload_idx != NULL)
	{
		if (g_packet_len - hdr_len)
			*payload_idx = hdr_len;
		else
			*payload_idx = 0;
	}

	return 0;
}

void ip_create_header(uint16_t len, uint8_t proto, ip_addr_t * dest)
{
	uint16_t checksum;

	// add header size to len
	len += 20;

	g_buffer[0] 	= 0x45;		// version and header length
	g_buffer[1] 	= 0x00;		// dscp and ecn
	g_buffer[2] 	= (uint8_t) (len >> 8);	// length	
	g_buffer[3] 	= (uint8_t) (len & 0xff);
	g_buffer[4] 	= 0x00;		// identification
	g_buffer[5] 	= 0x01;
	g_buffer[6] 	= 0x00;		// flags and fragment offset
	g_buffer[7] 	= 0x00;		// fragment offset
	g_buffer[8] 	= 0x40;		// time to live
	g_buffer[9] 	= proto;	// protocol
	g_buffer[10] 	= 0x00;		// checksum
	g_buffer[11] 	= 0x00;
	g_buffer[12] 	= g_host_addr.bytes[0];	// source IP
	g_buffer[13] 	= g_host_addr.bytes[1];
	g_buffer[14] 	= g_host_addr.bytes[2];
	g_buffer[15] 	= g_host_addr.bytes[3];
	g_buffer[16] 	= dest->bytes[0];		// destination IP
	g_buffer[17] 	= dest->bytes[1];
	g_buffer[18] 	= dest->bytes[2];
	g_buffer[19] 	= dest->bytes[3];

	// calculate the header checksum 	
	checksum = ip_checksum(g_buffer,20);
	g_buffer[10] 	= (uint8_t) (checksum >> 8);	
	g_buffer[11] 	= (uint8_t) (checksum & 0xff);

}