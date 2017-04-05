//
// from https://tools.ietf.org/html/rfc1055

#ifndef __SLIP_H__
#define __SLIP_H__

#include <stdint.h>

void slip_send_packet(uint8_t * p, uint16_t len);
uint16_t slip_recv_packet(uint8_t * p, uint16_t len);

#endif
