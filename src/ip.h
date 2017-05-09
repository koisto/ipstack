#ifndef __IP_H__
#define __IP_H__

#include <stdint.h>

#define IP_PROTO_ICMP		1
#define IP_PROTO_UDP		17

#define IP_HEADER_ERROR		99

typedef struct ip_addr_s {
	uint8_t bytes[4];
} ip_addr_t;



uint16_t ip_checksum (uint8_t * header, uint16_t len);

uint16_t ip_parse_header(uint8_t * packet, uint16_t len, uint16_t * payload_idx, uint8_t * proto, ip_addr_t * src);

void ip_create_header(uint8_t * packet, uint16_t len, uint8_t proto, ip_addr_t * dest);

#endif