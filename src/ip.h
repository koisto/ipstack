#ifndef __IP_H__
#define __IP_H__

#include <stdint.h>

uint16_t ip_checksum (uint8_t * header, uint16_t len);

#endif