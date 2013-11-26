#include <cxxtest/TestSuite.h>
#include "../Utilities/Vector2D.h"

using utilities::Vector2D;

class Vector2DTest : public CxxTest::TestSuite
{
private:
	Vector2D<int>* mpVector;
	
public:
	void setUp()
	{
		mpVector = new Vector2D<int>(4, 3, -1);
	}

	void tearDown()
	{
		SAFE_DELETE(mpVector);
	}

	void testSet()
	{
		mpVector->get(0,0) = 1;

		TS_ASSERT_EQUALS(mpVector->get(0,0), 1);

		int& value = mpVector->get(0, 0);
		value = 2;

		TS_ASSERT_EQUALS(mpVector->get(0,0), 2);
	}

	void testShiftLeft()
	{
		int i = 0;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				mpVector->get(x, y) = i;
				i++;
			}
		}

		// Test shift left with wrap
		mpVector->shiftLeft(true);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), (y*4+1));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), (y*4));
		}

		// Test shift left without wrap
		mpVector->shiftLeft(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), (y*4+2));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), -1);
		}

		// Test all shift lefts without wrap
		mpVector->shiftLeft(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), (y*4+3));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), -1);
		}

		// 4th shift
		mpVector->shiftLeft(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), (y*4));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), -1);
		}

		// 5th shift
		mpVector->shiftLeft(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), -1);
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), -1);
		}
	}

	void testShiftRight()
	{
		int i = 0;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				mpVector->get(x, y) = i;
				i++;
			}
		}

		// Test shift left with wrap
		mpVector->shiftRight(true);

		TS_ASSERT_EQUALS(true, true);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), (y*4+2));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), (y*4+3));
		}

		// Test shift left without wrap
		mpVector->shiftRight(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), (y*4+1));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), -1);
		}

		// Test all shift lefts without wrap
		mpVector->shiftRight(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), (y*4));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), -1);
		}

		// 4th shift
		mpVector->shiftRight(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), (y*4+3));
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), -1);
		}

		// 5th shift
		mpVector->shiftRight(false);

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(3, y), -1);
		}

		for (int y = 0; y < 3; y++)
		{
			TS_ASSERT_EQUALS(mpVector->get(0, y), -1);
		}
	}

	void testShiftUp()
	{
		int i = 0;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				mpVector->get(x, y) = i;
				i++;
			}
		}

		// Test shift left with wrap
		mpVector->shiftUp(true);

		TS_ASSERT_EQUALS(true, true);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), x+4);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), x);
		}

		// Test shift left without wrap
		mpVector->shiftUp(false);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), x+8);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), -1);
		}

		// Test all shift lefts without wrap
		mpVector->shiftUp(false);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), x);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), -1);
		}

		// 4th shift
		mpVector->shiftUp(false);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), -1);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), -1);
		}
	}

	void testShiftDown()
	{
		int i = 0;
		for (int y = 0; y < 3; y++)
		{
			for (int x = 0; x < 4; x++)
			{
				mpVector->get(x, y) = i;
				i++;
			}
		}

		// Test shift left with wrap
		mpVector->shiftDown(true);

		TS_ASSERT_EQUALS(true, true);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), x+8);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), x+4);
		}

		// Test shift left without wrap
		mpVector->shiftDown(false);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), -1);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), x);
		}

		// Test all shift lefts without wrap
		mpVector->shiftDown(false);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), -1);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), x+8);
		}

		// 4th shift
		mpVector->shiftDown(false);

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 0), -1);
		}

		for (int x = 0; x < 4; x++)
		{
			TS_ASSERT_EQUALS(mpVector->get(x, 2), -1);
		}
	}

	void testExceptions()
	{
		// Index out of bounds
		TS_ASSERT_THROWS_NOTHING(mpVector->get(0,0));
		TS_ASSERT_THROWS(mpVector->get(-1, 0), Vector2D<int>::IndexOutOfBoundsException);
		TS_ASSERT_THROWS(mpVector->get(0, -1), Vector2D<int>::IndexOutOfBoundsException);
		TS_ASSERT_THROWS(mpVector->get(4, 0), Vector2D<int>::IndexOutOfBoundsException);
		TS_ASSERT_THROWS(mpVector->get(0, 3), Vector2D<int>::IndexOutOfBoundsException);
	}

};