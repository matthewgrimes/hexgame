#include "UnitTest++/UnitTest++.h"
#include "Lighting.h"

TEST(SanityCheck) { CHECK_EQUAL(1,1); }

int main(int, const char*[])
{
	return UnitTest::RunAllTests();
}
