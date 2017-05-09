#ifndef __ICMP_H__
#define __ICMP_H__

#include "ip.h"

#define ICMP_ECHO_REPLY 		0
#define ICMP_DEST_UNREACHABLE	3
#define ICMP_ECHO_REQUEST		8

uint16_t icmp_parse_header(uint8_t * packet, uint16_t len, uint8_t * type, uint8_t * code);

uint16_t icmp_send(uint8_t type, uint8_t code, uint8_t * rest_of_header, uint8_t * data, uint16_t len, uint8_t * packet);

#endif