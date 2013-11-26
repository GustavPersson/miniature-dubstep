#include <cxxtest/TestSuite.h>
#include "../Logic/KeyHandler.h"

using namespace logic;

class KeyHandlerTest : public CxxTest::TestSuite
{
public:
	void setUp()
	{
		mpKeyHandler = logic::KeyHandler::getInstance();
	}

	void tearDown()
	{
		SAFE_DELETE(mpKeyHandler);
	}

	void testFunctionality()
	{

		TS_ASSERT_EQUALS(1, 1 % 4);
		KeyType firstId[MAX_PLAYERS];
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			firstId[i] = i * MAX_KEYS_PER_PLAYER;
		}

		// test first keys we remove
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			TS_ASSERT_EQUALS(firstId[i], mpKeyHandler->allocateKey(i));
		}

		// Remove 4 more keys
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			mpKeyHandler->allocateKey(i);
		}

		// Free the keys
		for (int i = 0; i < MAX_PLAYERS; i++) {
			mpKeyHandler->freeKey(firstId[i], i);
		}

		// Test the keys again it should be the same as first time
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			TS_ASSERT_EQUALS(firstId[i], mpKeyHandler->allocateKey(i));
		}
	}

private:
	KeyHandler*		mpKeyHandler;
};