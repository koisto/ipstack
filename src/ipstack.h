#ifndef __IPSTACK_H__
#define __IPSTACK_H__

#include <stdint.h>
#include <stddef.h>

#include "ipstack_conf.h"

// types
typedef struct ip_addr_s {
	uint8_t bytes[4];
} ip_addr_t;





// globals
extern ip_addr_t g_host_addr;

extern uint8_t g_buffer[500];
extern uint16_t g_packet_len;
extern uint16_t g_rx_packets;

void ipstack_poll(void);

#endif