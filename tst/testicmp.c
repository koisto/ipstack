#include "unity.h"
#include "icmp.h"

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