#include "unity.h"
#include "slip.h"

//
void send_char(char c)
{

}

char recv_char(void)
{
	return 0;
}


void test_send_packet(void)
{
TEST_ASSERT_EQUAL_HEX8(0,0);
}

int main (void)
{
	UNITY_BEGIN();
	RUN_TEST(test_send_packet);
	return UNITY_END();
}