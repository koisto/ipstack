//
// from https://tools.ietf.org/html/rfc1055

#ifndef __SLIP_H__
#define __SLIP_H__

#include <stdint.h>

/* @brief Function to send a packet using SLIP encoding
 * @param p Pointer to location of packet to send
 * @param len Size of the packet
 * @return None
 */
void slip_send_packet(uint8_t * p, uint16_t len);

/* @brief Function to receive SLIP formatted data
 * @param p Pointer to the buffer to store decoded data
 * @param len Size of the buffer
 * @return Amount of data received
 */
uint16_t slip_recv_packet(uint8_t * p, uint16_t len);

#endif
