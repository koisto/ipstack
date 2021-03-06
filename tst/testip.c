#include "unity.h"
#include "ip.h"

#include <string.h>

ip_addr_t g_host_addr = {{192,168,0,199}};

uint8_t g_buffer[500];
uint16_t g_packet_len;


void test_checksum_calc(void)
{
	uint8_t header[20] = {0x45,0x00,0x00,0x73,0x00,0x00,0x40,0x00,0x40,0x11,0x00,0x00,0xc0,0xa8,0x00,0x01,0xc0,0xa8,0x00,0xc7};

	TEST_ASSERT_EQUAL_UINT16_MESSAGE(0xb861, ip_checksum(header, 20), "Checksum calculation incorrect");
}

void test_checksum_calc_zero(void)
{
	uint8_t header[20] = {0x45,0x00,0x00,0x73,0x00,0x00,0x40,0x00,0x40,0x11,0xb8,0x61,0xc0,0xa8,0x00,0x01,0xc0,0xa8,0x00,0xc7};

	TEST_ASSERT_EQUAL_UINT16_MESSAGE(0x0000, ip_checksum(header, 20), "Checksum calculation incorrect");
}

void test_checksum_calc_fail(void)
{
	uint8_t header[20] = {0x45,0x00,0x00,0x73,0x00,0x00,0x40,0x00,0x40,0x11,0xb8,0x61,0xc0,0xa8,0x00,0x01,0xc0,0xa8,0x00,0xc9};

	TEST_ASSERT_NOT_EQUAL_MESSAGE(0x0000, ip_checksum(header, 20), "Checksum calculation incorrect");
}


void test_parse_invalid_version(void)
{
	uint8_t ip_header[20] = {
		0x05,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xd0,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc7
	};

	memcpy(g_buffer, ip_header, 20);
	g_packet_len = 20;

	TEST_ASSERT_EQUAL_UINT16_MESSAGE(IP_HEADER_ERROR, ip_parse_header(NULL, NULL, NULL), \
														"Parsing did not detect error in header version");
}

void test_parse_invalid_header_length(void)
{
	uint8_t ip_header[20] = {
		0x44,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xd0,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc7
	};

	memcpy(g_buffer, ip_header, 20);
	g_packet_len = 20;

	TEST_ASSERT_EQUAL_UINT16_MESSAGE(IP_HEADER_ERROR, ip_parse_header(NULL, NULL, NULL), \
														"Parsing did not detect error in header length field");
}

void test_parse_invalid_packet_length(void)
{
	uint8_t ip_header[20] = {
		0x45,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xd0,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc7
	};


	memcpy(g_buffer, ip_header, 20);
	g_packet_len = 10;


	TEST_ASSERT_EQUAL_UINT16_MESSAGE(IP_HEADER_ERROR, ip_parse_header(NULL, NULL, NULL), \
														"Parsing did not detect error in packet length - too short");
}

void test_parse_invalid_checksum(void)
{
	uint8_t ip_header[20] = {
		0x45,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0x00,	// checksum
		0x00,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc7
	};

	memcpy(g_buffer, ip_header, 20);
	g_packet_len = 20;

	TEST_ASSERT_EQUAL_UINT16_MESSAGE(IP_HEADER_ERROR, ip_parse_header(NULL, NULL, NULL), \
														"Parsing did not detect checksum error");
}

void test_parse_wrong_destination(void)
{
	uint8_t ip_header[20] = {
		0x45,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xd0,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc8
	};

	memcpy(g_buffer, ip_header, 20);
	g_packet_len = 20;

	TEST_ASSERT_EQUAL_UINT16_MESSAGE(IP_HEADER_ERROR, ip_parse_header(NULL, NULL, NULL) , \
														"Parsing did not detect incorrect destination");	
}

void test_parse_return_protocol(void)
{
	uint8_t ip_header[20] = {
		0x45,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xd0,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc7
	};

	uint8_t proto = 111;

	memcpy(g_buffer, ip_header, 20);
	g_packet_len = 20;

	ip_parse_header(NULL, &proto, NULL);
	TEST_ASSERT_EQUAL_MESSAGE(1, proto, "Parsing did not return protocol value");
}

void test_parse_return_sourceip(void)
{
	uint8_t ip_header[20] = {
		0x45,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xd0,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc7
	};

	ip_addr_t source;

	memcpy(g_buffer, ip_header, 20);
	g_packet_len = 20;

	ip_parse_header(NULL, NULL, &source);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(&ip_header[12], source.bytes, sizeof(ip_addr_t));
}

void test_parse_return_upper_packet(void)
{
	uint8_t ip_header[25] = {
		0x45,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x14,
		0x00,	// identification
		0x00,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xd0,
		0xc0,	// source IP
		0xa8,
		0x00,
		0x01,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0xc7,
		0x01,
		0x02,
		0x03,
		0x04,
		0x05
	};

	uint8_t proto = 111;
	ip_addr_t source;
	uint16_t payload_idx;

	memcpy(g_buffer, ip_header, 25);
	g_packet_len = 25;

	ip_parse_header(&payload_idx, &proto, &source);
	TEST_ASSERT_EQUAL_UINT16_MESSAGE(20, payload_idx, "Upper packet index not set");
}

void test_create_header(void)
{
	uint8_t expected_ip_header[20] = {
		0x45,	// version and header length
		0x00,	// dscp and ecn
		0x00,	// length	
		0x1E,
		0x00,	// identification
		0x01,
		0x00,	// flags and fragment offset
		0x00,	// fragment offset
		0x40,	// time to live
		0x01,	// protocol
		0xf8,	// checksum
		0xc5,
		0xc0,	// source IP
		0xa8,
		0x00,
		0xc7,
		0xc0,	// destination IP
		0xa8,
		0x00,
		0x01
	};


	ip_addr_t dest = {{192,168,0,1}};

	ip_create_header(10, 1, &dest);
	TEST_ASSERT_EQUAL_UINT8_ARRAY(expected_ip_header, g_buffer, 20);
}

int main (void)
{
	UNITY_BEGIN();
	RUN_TEST(test_checksum_calc);
	RUN_TEST(test_checksum_calc_zero);
	RUN_TEST(test_checksum_calc_fail);

	RUN_TEST(test_parse_invalid_version);
	RUN_TEST(test_parse_invalid_header_length);
	RUN_TEST(test_parse_invalid_packet_length);
	RUN_TEST(test_parse_invalid_checksum);
	RUN_TEST(test_parse_wrong_destination);
	RUN_TEST(test_parse_return_protocol);
	RUN_TEST(test_parse_return_sourceip);
	RUN_TEST(test_parse_return_upper_packet);

	RUN_TEST(test_create_header);

	return UNITY_END();
}
