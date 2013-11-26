#include <cxxtest/TestSuite.h>
#include "../Utilities/Vec2Int.h"

using utilities::Vec2Int;

class Vec2IntTest : public CxxTest::TestSuite
{
public:

	void testEquality()
	{
		Vec2Int vec1(1,1), vec2(1,1);

		TS_ASSERT_EQUALS(vec1, vec2);
	}

	void testDiffer()
	{
		Vec2Int vec1(1,1), vec2;

		TS_ASSERT_DIFFERS(vec1, vec2);
	}

	void testDifference()
	{
		Vec2Int left(5,5), right(2,2), answer(3,3);
		Vec2Int difference = left - right;

		TS_ASSERT_EQUALS(difference, answer);
	}

	void testLongerThan()
	{
		Vec2Int longVec(10,10), Vec2Int shorterVec(9,10), longerVec(10, -11);
		
		TS_ASSERT_EQUALS(longVec.longerThan(longVec), false);
		TS_ASSERT_EQUALS(longVec.longerThan(shorterVec), true);
		TS_ASSERT_EQUALS(longVec.longerThan(longerVec), false);
	}
};


