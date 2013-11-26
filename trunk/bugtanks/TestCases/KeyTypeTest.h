#include <cxxtest/TestSuite.h>
#include "../Logic/KeyType.h"
#include "../Logic/PlayerInfo.h"

using namespace logic;

class KeyTypeTest : public CxxTest::TestSuite
{
private:
	PlayerInfo* mpPlayerInfo;

public:
	void setUp()
	{
		mpPlayerInfo = PlayerInfo::getInstance();
		KeyType::init();
	}

	void tearDown()
	{
		SAFE_DELETE(mpPlayerInfo);
	}

	void testOwner()
	{
		// Test invalid value first
		TS_ASSERT_EQUALS(INVALID_KEY.getOwnerId, INVALID_NETWORK_ID);
		TS_ASSERT_EQUALS(KEY_LIM.getOwnerId, INVALID_NETWORK_ID);

		// Test owners in range
		for (int i = 0; i < MAX_PLAYERS; i++)
		{
			int middleKeyValue = i * MAX_KEYS_PER_PLAYER;
			KeyType beforeKey(middleKeyValue - 1);
			KeyType middleKey(middleKeyValue);
			KeyType afterKey(middleKeyValue + 1);

			if (beforeKeyValue == -1)
			{
				TS_ASSERT_EQUALS(beforeKey, INVALID_KEY);
			}
			else
			{
				TS_ASSERT_EQUALS(beforeKey, i-1);
			}

			TS_ASSERT_EQUALS(middleKey, i);
			TS_ASSERT_EQUALS(afterKey, i);
		}
	}

};