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

		VectorList<int> vec;

		vec.add(0);
		vec.removeFirst();
		vec.add(0);
		vec.removeFirst();
		vec.add(0);
		vec.removeFirst();
		vec.add(0);
		vec.removeFirst();

		for(int i = 0; i<10; i++)
		{
			vec.add(i);
		}

		for (int i = 0; i < 10; i++)
		{
			TS_ASSERT_EQUALS(vec.getFirst(), i);
			vec.removeFirst();
		}

		for (int i = 0; i < 25; i++)
		{
			vec.add(i);
		}

		for (int i = 0; i < 25; i++)
		{
			TS_ASSERT_EQUALS(vec[i], i);
		}

		for (int i = 0; i < 25; i++)
		{
			TS_ASSERT_EQUALS(vec.getFirst(), i);
			vec.removeFirst();
		}

		for(int i = 0; i<10; i++)
		{
			vec.add(i);
		}
		for(int i = 0; i<10; i++)
		{
			TS_ASSERT_EQUALS(vec[i], i);
		}
		
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

		// Test exception
		TS_ASSERT_THROWS(maxTest.add(0), VectorList<int>::FullException);
	}

	void testGetFirstAndLast()
	{
		VectorList<int> vecList(3);

		vecList.add(0);
		vecList.add(1);
		vecList.add(2);

		TS_ASSERT_EQUALS(vecList.getFirst(), 0);
		TS_ASSERT_EQUALS(vecList.getLast(), 2);
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

	void testExceptions()
	{
		VectorList<int> vecList(0, 1);

		// Tests full exception
		TS_ASSERT_THROWS_NOTHING(vecList.add(1));
		TS_ASSERT_THROWS(vecList.add(1), VectorList<int>::FullException);

		// Test out of bounds exception
		TS_ASSERT_THROWS(vecList[-1], VectorList<int>::IndexOutOfBoundsException);
		TS_ASSERT_THROWS_NOTHING(vecList[0]);
		TS_ASSERT_THROWS(vecList[1], VectorList<int>::IndexOutOfBoundsException);

		// Note, remove functionality tests for empty exception

		// Test for empty exception
		vecList.removeFirst();

		TS_ASSERT_THROWS(vecList.getFirst(), VectorList<int>::EmptyException);
		TS_ASSERT_THROWS(vecList.getLast(), VectorList<int>::EmptyException);
		TS_ASSERT_THROWS(vecList.removeFirst(), VectorList<int>::EmptyException);
		TS_ASSERT_THROWS(vecList.removeLast(), VectorList<int>::EmptyException);

	}

	void testRemove()
	{
		// You need to change some test if you change this value
		const int VEC_SIZE = 5;

		VectorList<int> vecList(VEC_SIZE);

		// Test for empty
		TS_ASSERT_THROWS(vecList.remove(0), VectorList<int>::EmptyException);
		TS_ASSERT_THROWS(vecList.remove(0, true), VectorList<int>::EmptyException);

		// Add full array
		for (int i = 0; i < VEC_SIZE; i++)
		{
			vecList.add(i);
		}

		// Test out of bounds exceptions
		TS_ASSERT_THROWS(vecList.remove(-1), VectorList<int>::IndexOutOfBoundsException);
		TS_ASSERT_THROWS(vecList.remove(-1, true), VectorList<int>::IndexOutOfBoundsException);
		TS_ASSERT_THROWS(vecList.remove(VEC_SIZE), VectorList<int>::IndexOutOfBoundsException);
		TS_ASSERT_THROWS(vecList.remove(VEC_SIZE, true), VectorList<int>::IndexOutOfBoundsException);

		// Test keep order
		vecList.remove(2, true);

		// Check the array
		TS_ASSERT_EQUALS(vecList[0], 0);
		TS_ASSERT_EQUALS(vecList[1], 1);
		TS_ASSERT_EQUALS(vecList[2], 3);
		TS_ASSERT_EQUALS(vecList[3], 4);
		TS_ASSERT_EQUALS(vecList.size(), 4);

		// Test out of bounds
		TS_ASSERT_THROWS(vecList.remove(4), VectorList<int>::IndexOutOfBoundsException);

		// Remove a middle object but don't keep the order
		vecList.remove(1);

		TS_ASSERT_EQUALS(vecList[0], 0);
		TS_ASSERT_EQUALS(vecList[1], 4);
		TS_ASSERT_EQUALS(vecList[2], 3);
		TS_ASSERT_EQUALS(vecList.size(), 3);

		// Test out of bounds
		TS_ASSERT_THROWS(vecList.remove(3), VectorList<int>::IndexOutOfBoundsException);

		// Remove the first
		vecList.remove(0);

		TS_ASSERT_EQUALS(vecList[0], 4);
		TS_ASSERT_EQUALS(vecList[1], 3);
		TS_ASSERT_EQUALS(vecList.size(), 2);

		// Remove the last
		vecList.remove(1);

		TS_ASSERT_EQUALS(vecList.size(), 1);
		TS_ASSERT_EQUALS(vecList[0], 4);

		// Test out of bounds
		TS_ASSERT_THROWS(vecList.remove(1), VectorList<int>::IndexOutOfBoundsException);

		// Remove the last
		vecList.remove(0);

		TS_ASSERT_EQUALS(vecList.size(), 0);
		TS_ASSERT_THROWS(vecList.remove(0), VectorList<int>::EmptyException);
		TS_ASSERT_THROWS(vecList.remove(0, true), VectorList<int>::EmptyException);
		TS_ASSERT_THROWS(vecList.remove(1), VectorList<int>::EmptyException);
		TS_ASSERT_THROWS(vecList.remove(1, true), VectorList<int>::EmptyException);
	}
};