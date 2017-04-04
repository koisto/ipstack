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

char rcvd_buffer [BUFFER_SIZE];
int rcvd_idx = 0;

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
	char c = 0;

	TEST_ASSERT_MESSAGE(rcvd_idx < BUFFER_SIZE, "Receive buffer overflow");
	if (rcvd_idx < BUFFER_SIZE)
		c = rcvd_buffer[rcvd_idx++];

	return c;
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


void test_recv_packet_hello(void)
{
	char expected[5] = {'h','e','l','l','o'};
	char packet[10];
	int bc;

	// set up the receive buffer by hand
	rcvd_buffer[0] = END;
	rcvd_buffer[1] = 'h';
	rcvd_buffer[2] = 'e';
	rcvd_buffer[3] = 'l';
	rcvd_buffer[4] = 'l';
	rcvd_buffer[5] = 'o';
	rcvd_buffer[6] = END;

	RESET_IDX(rcvd_idx);
	bc = slip_recv_packet(packet,10);

	TEST_ASSERT_EQUAL_INT_MESSAGE(5, bc, "Received byte count incorrect");
	TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, packet, 5, "Receive buffer contents incorrect");
}

void test_recv_packet_hello_with_end(void)
{
	char expected[6] = {'h','e',END,'l','l','o'};
	char packet[10];
	int bc;

	// set up the receive buffer by hand
	rcvd_buffer[0] = END;
	rcvd_buffer[1] = 'h';
	rcvd_buffer[2] = 'e';
	rcvd_buffer[3] = ESC;
	rcvd_buffer[4] = ESC_END;
	rcvd_buffer[5] = 'l';
	rcvd_buffer[6] = 'l';
	rcvd_buffer[7] = 'o';
	rcvd_buffer[8] = END;

	RESET_IDX(rcvd_idx);
	bc = slip_recv_packet(packet,10);

	TEST_ASSERT_EQUAL_INT_MESSAGE(6, bc, "Received byte count incorrect");
	TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, packet, 6, "Receive buffer contents incorrect");
}


void test_recv_packet_hello_with_esc(void)
{
	char expected[6] = {'h','e',ESC,'l','l','o'};
	char packet[10];
	int bc;

	// set up the receive buffer by hand
	rcvd_buffer[0] = END;
	rcvd_buffer[1] = 'h';
	rcvd_buffer[2] = 'e';
	rcvd_buffer[3] = ESC;
	rcvd_buffer[4] = ESC_ESC;
	rcvd_buffer[5] = 'l';
	rcvd_buffer[6] = 'l';
	rcvd_buffer[7] = 'o';
	rcvd_buffer[8] = END;

	RESET_IDX(rcvd_idx);
	bc = slip_recv_packet(packet,10);

	TEST_ASSERT_EQUAL_INT_MESSAGE(6, bc, "Received byte count incorrect");
	TEST_ASSERT_EQUAL_HEX8_ARRAY_MESSAGE(expected, packet, 6, "Receive buffer contents incorrect");
}

int main (void)
{
	UNITY_BEGIN();
	RUN_TEST(test_send_packet_hello);
	RUN_TEST(test_send_packet_hello_with_end);
	RUN_TEST(test_send_packet_hello_with_esc);
	RUN_TEST(test_recv_packet_hello);
	RUN_TEST(test_recv_packet_hello_with_end);
	RUN_TEST(test_recv_packet_hello_with_esc);

	return UNITY_END();
}