#include "Macros.h"
#include "KeyHandler.h"

KeyHandler* KeyHandler::mpsInstances[MAX_PLAYERS] = {NULL};

KeyHandler::KeyHandler(int client)
{
	mClient = client;
	// Generate 1000 keys for each player
	for (int i = 0; i < MAX_PLAYERS * 1000; i++) {
		mFreeKeys[i % MAX_PLAYERS].push(i);
	}
}

KeyHandler::~KeyHandler()
{
	mpsInstances[mClient] = NULL;
}

KeyHandler* KeyHandler::getInstance(int client)
{
	if (mpsInstances[client] == NULL)
	{
		mpsInstances[client] = myNew KeyHandler(client);
	}
	return mpsInstances[client];
}

KeyType KeyHandler::allocateKey(int player)
{
	KeyType id = mFreeKeys[player].top();
	mFreeKeys[player].pop();
	return id;
}

void KeyHandler::freeKey(KeyType freeKey)
{
	mFreeKeys[freeKey % MAX_PLAYERS].push(freeKey);
}