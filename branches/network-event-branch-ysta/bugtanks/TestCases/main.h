// Using this class to add a stub test to the build to prevent it from failing!
#include <cxxtest/TestSuite.h>

class StubTestCase : public CxxTest::TestSuite
{
public:
	void testStub()
	{
		// Is 1337 == 1337?
		TS_ASSERT_EQUALS(1337, 1337);
	}
};