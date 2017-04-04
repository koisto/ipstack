#include "slip.h"


void send_char(char c)
{

}

char recv_char(void)
{
	return 0;
}

int main (void)
{
	char buffer[12];


	slip_send_packet(buffer, 12);
	return 0;
}