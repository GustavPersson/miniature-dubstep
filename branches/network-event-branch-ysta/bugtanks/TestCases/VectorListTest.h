#include <cxxtest/TestSuite.h>
#include "../Utilities/VectorList.h"

using utilities::VectorList;

class VectorListTest : public CxxTest::TestSuite
{
private:
	VectorList<int> mVectorList;
public:

	void testAdd()
	{
		// Test size
		TS_ASSERT_EQUALS(mVectorList.size(), 0);

		for (int i = 0; i < 11; i++)
		{
			mVectorList.add(i);
		}
		
		// Test the equality
		for (int i = 0; i < 11; i++)
		{
			TS_ASSERT_EQUALS(mVectorList[i], i);
		}

		// Test size
		TS_ASSERT_EQUALS(mVectorList.size(), 11);
	}

	void testAddWithInitialSize()
	{
		VectorList<int> initialSize(1);

		// Test size
		TS_ASSERT_EQUALS(initialSize.size(), 0);

		initialSize.add(1);
		initialSize.add(2);

		TS_ASSERT_EQUALS(initialSize[0], 1);
		TS_ASSERT_EQUALS(initialSize[1], 2);

		// Test size
		TS_ASSERT_EQUALS(initialSize.size(), 2);
	}

	void testRemoveFirst()
	{
		VectorList<int> removeFirst;

		removeFirst.add(1);
		removeFirst.add(2);

		removeFirst.removeFirst();

		// Test size
		TS_ASSERT_EQUALS(removeFirst.size(), 1);

		// Test the equality
		TS_ASSERT_EQUALS(removeFirst[0], 2);
	}

	void testRemoveLast()
	{
		VectorList<int> removeLast;

		removeLast.add(1);
		removeLast.add(2);
		
		removeLast.removeLast();

		// Test size
		TS_ASSERT_EQUALS(removeLast.size(), 1);

		// Test equality
		TS_ASSERT_EQUALS(removeLast[0], 1);
	}

	void testChangeValue()
	{
		VectorList<int> testChange;

		testChange.add(1);
		testChange.add(2);

		testChange[0] = 5;
		testChange[1] = 10;

		TS_ASSERT_EQUALS(testChange[0], 5);
		TS_ASSERT_EQUALS(testChange[1], 10);
	}

	void testMax()
	{
		VectorList<int> maxTest(0, 5);

		for (int i = 0; i < 5; i++)
		{
			maxTest.add(i);
		}

		// Test size
		TS_ASSERT_EQUALS(maxTest.size(), 5);		
	}

	void testClear()
	{
		VectorList<int> vl;

		vl.add(1);
		vl.add(2);
		
		vl.clear();

		// Test size
		TS_ASSERT_EQUALS(vl.size(), 0);

		vl.add(3);
		vl.add(4);

		// Test size
		TS_ASSERT_EQUALS(vl.size(), 2);

		// Test equality
		TS_ASSERT_EQUALS(vl[0], 3);
		TS_ASSERT_EQUALS(vl[1], 4);
	}
};