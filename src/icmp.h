#ifndef __ICMP_H__
#define __ICMP_H__


#include "ipstack.h"
#include "ip.h"

#define ICMP_ECHO_REPLY 		0
#define ICMP_DEST_UNREACHABLE	3
#define ICMP_ECHO_REQUEST		8

uint16_t icmp_receive(ip_addr_t * src, uint8_t * packet, uint16_t len);

uint16_t icmp_send(ip_addr_t * dest, uint8_t type, uint8_t code, uint8_t * rest_of_header, uint8_t * data, uint16_t data_len);

#endif