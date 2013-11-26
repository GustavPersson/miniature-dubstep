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

#ifndef __KEY_HANDLER_H__
#define __KEY_HANDLER_H__

#include "../Utilities/Macros.h"
#include <stack>
#include <cassert>
#include "KeyType.h"
// only include map if debug is on
#ifdef _DEBUG
#include <map>
#endif

namespace logic {

//typedef int KeyType;	/**< Using KeyId as a typedef if we want to change it later. */
//const KeyType INVALID_KEY = -1;

/**
 * Generates different keys for each player. Uses a stack for the key pool.
 */
class KeyHandler
{
public:
	/**
	* Sets the static pointer to null, DO NOT CALL DELETE EXCEPT IN MAIN or equivalent function.
	*/
	~KeyHandler();

	/**
	* Returns the instance of KeyHandler.
	* @return instance of KeyHandler
	*/
	static KeyHandler* getInstance();

	/**
	* Allocates the next free key. If there are no more free keys an error message is displayed and -1 is returned
	* @param driverId the id of the driver. should be 0-3.
	* @return the next free keyId.
	*/
	KeyType allocateKey(int driverId);

	/**
	* Frees a key that has been used.
	* @param keyId the key to be freed.
	* @param driverId the driver id that used the key.
	*/
	void freeKey(KeyType keyId, int driverId);

	int getNetworkIDWithMostFreeKeys();

	void resetKeyOffset(int playerID);

	void resetKeyHandler();

private:
	/**
	* Creates a new instance of keyhandler
	*/
	KeyHandler();

	static KeyHandler*		mpsInstance;

	std::stack<KeyType>		mFreeKeys[MAX_PLAYERS];
	int						mKeyOffset[MAX_PLAYERS];

#ifdef _DEBUG
	std::map<KeyType, bool>	mFreeKeysChecker[MAX_PLAYERS];
#endif
};
}

#endif