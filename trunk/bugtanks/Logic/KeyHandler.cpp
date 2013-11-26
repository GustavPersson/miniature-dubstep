/**
 * @file
 * @author Matteus Magnusson <senth.wallace@gmail.com>
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "../Utilities/Macros.h"
#include "KeyHandler.h"
#include "Application.h"

using namespace logic;

KeyHandler* KeyHandler::mpsInstance = NULL;

KeyHandler::~KeyHandler()
{
	// Set the static pointer to null.
	mpsInstance = NULL;
}

KeyHandler* KeyHandler::getInstance()
{
	// If the instance doesn't exist, create it
	if (mpsInstance == NULL)
	{
		mpsInstance = myNew KeyHandler();
	}
	// Return instance
	return mpsInstance;
}

KeyType KeyHandler::allocateKey(int playerId)
{
	assert(playerId >= 0);
	assert(playerId < MAX_PLAYERS);

	// Set an invalid key
	KeyType key = INVALID_KEY;
	// If there are any free keys
	if (!mFreeKeys[playerId].empty())
	{
		// Get the next free key
		key = mFreeKeys[playerId].top();
		mFreeKeys[playerId].pop();
		// DEBUG - remove the key from the map
#if _DEBUG
		mFreeKeysChecker[playerId].erase(key);
#endif
	}
	else
	{
		// Print error that no key is available
		ERROR_MESSAGE("No more free keys available!");
		assert(false);
	}
	// Return the key
	return key;
}

void KeyHandler::freeKey(KeyType keyId, int playerId)
{
	// assert - the right key with player
	assert(keyId.getOwnerId() == playerId);
#ifdef _DEBUG
	// DEBUG - assert - the key already exists
	assert(mFreeKeysChecker[playerId].find(keyId) == mFreeKeysChecker[playerId].end());
	// DEBUG - add key to the map
	mFreeKeysChecker[playerId][keyId] = true;
#endif
	// add the key to the stack
	mFreeKeys[playerId].push(keyId);
}

int KeyHandler::getNetworkIDWithMostFreeKeys()
{
	int ID = -1;
	int max = -INT_MAX;
	int playerCount = Application::getInstance()->mpGameOptions->mNumberOfPlayers + 1;

	for(int nPlayer = 0; nPlayer < playerCount; ++nPlayer)
	{
		if((int)mFreeKeys[nPlayer].size() - mKeyOffset[nPlayer] > max)
		{
			max = mFreeKeys[nPlayer].size() - mKeyOffset[nPlayer];
			ID = nPlayer;
		}
	}

	if(ID != -1)
	{
		++mKeyOffset[ID];
	}
	return ID;
}

void KeyHandler::resetKeyOffset(int playerID)
{
	mKeyOffset[playerID] = 0;
}

void KeyHandler::resetKeyHandler()
{
	// Initialize KeyType
	KeyType::init();

	for (int player = 0; player < MAX_PLAYERS; player++)
	{
		while(!mFreeKeys[player].empty())
		{
			mFreeKeys[player].pop();
		}
		mKeyOffset[player] = 0;
		int stopValue = MAX_KEYS_PER_PLAYER * player;
		int startValue = ( MAX_KEYS_PER_PLAYER * (player + 1)) - 1;
		// Create keys for each player
		for (int key = startValue; key >= stopValue; key--)
		{
			mFreeKeys[player].push(key);
#ifdef _DEBUG
			mFreeKeysChecker[player][key] = true;
#endif
		}
	}
}

KeyHandler::KeyHandler()
{
	// Initialize KeyType
	KeyType::init();

	// Create all the available keys
	// Inefficient, but it only happends at startup
	// Iterate through all the players
	DEBUG_MESSAGE(LEVEL_LOW, "KeyHandler::KeyHandler() | Creating free keys.");
	for (int player = 0; player < MAX_PLAYERS; player++)
	{
		mKeyOffset[player] = 0;
		int stopValue = MAX_KEYS_PER_PLAYER * player;
		int startValue = ( MAX_KEYS_PER_PLAYER * (player + 1)) - 1;
		// Create keys for each player
		for (int key = startValue; key >= stopValue; key--)
		{
			mFreeKeys[player].push(key);
#ifdef _DEBUG
			mFreeKeysChecker[player][key] = true;
#endif
		}
	}
	DEBUG_MESSAGE(LEVEL_LOW, "KeyHandler::KeyHandler() | Free keys created.");
}