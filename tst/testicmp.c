#include "unity.h"
#include "icmp.h"

uint16_t g_packet_len;
uint8_t g_buffer[500];

uint16_t ip_checksum (uint8_t * header, uint16_t len)
{
    return 0;
}

void ip_create_header(uint16_t len, uint8_t proto, ip_addr_t * dest)
{

}

void slip_send_packet(uint8_t * p, uint16_t len)
{

}

void test_icmp (void)
{
	TEST_FAIL();
}

int main (void)
{
	UNITY_BEGIN();

	RUN_TEST(test_icmp);

	return UNITY_END();
}