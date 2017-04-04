#include "unity.h"
#include "slip.h"

#define BUFFER_SIZE		50
#define RESET_IDX(p) 	p=0

// SLIP special character codes 
#define END             0300    /* indicates end of packet */
#define ESC             0333    /* indicates byte stuffing */
#define ESC_END         0334    /* ESC ESC_END means END data byte */
#define ESC_ESC         0335    /* ESC ESC_ESC means ESC data byte */

char sent_buffer [BUFFER_SIZE];
int send_idx = 0;

//
void send_char(char c)
{
	sent_buffer[send_idx] = c;
	TEST_ASSERT_MESSAGE(send_idx < BUFFER_SIZE, "Sent buffer overflow");
	if (send_idx < BUFFER_SIZE)
		send_idx++;
}

char recv_char(void)
{
	return 0;
}


void test_send_packet_hello(void)
{
	char to_send[5] = {'h','e','l','l','o'};
	char expected[7] = {END,'h','e','l','l','o',END};


	RESET_IDX(send_idx);
	slip_send_packet(to_send,5);

	TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, sent_buffer, 7, "Sent buffer contents incorrect");
}

void test_send_packet_hello_with_end(void)
{
	char to_send[6] = {'h','e',END,'l','l','o'};
	char expected[9] = {END,'h','e',ESC,ESC_END,'l','l','o',END};

	RESET_IDX(send_idx);
	slip_send_packet(to_send,6);

	TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, sent_buffer, 9, "Sent buffer contents incorrect");
}

void test_send_packet_hello_with_esc(void)
{
	char to_send[6] = {'h','e',ESC,'l','l','o'};
	char expected[9] = {END,'h','e',ESC,ESC_ESC,'l','l','o',END};

	RESET_IDX(send_idx);
	slip_send_packet(to_send,6);

	TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, sent_buffer, 9, "Sent buffer contents incorrect");
}

int main (void)
{
	UNITY_BEGIN();
	RUN_TEST(test_send_packet_hello);
	RUN_TEST(test_send_packet_hello_with_end);
	RUN_TEST(test_send_packet_hello_with_esc);

	return UNITY_END();
}