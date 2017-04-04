/////
// from https://tools.ietf.org/html/rfc1055

#ifndef __SLIP_H__
#define __SLIP_H__

void slip_send_packet(char * p, int len);
int slip_recv_packet(char * p, int len);

#endif
