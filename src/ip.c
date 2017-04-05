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