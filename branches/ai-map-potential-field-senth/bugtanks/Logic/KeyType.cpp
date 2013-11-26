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

#include "KeyType.h"
#include "PlayerInfo.h"

std::istream& operator>>(std::istream& in, logic::KeyType& key)
{
	in >> key.mKey;
	return in;
}

std::ostream& operator<<(std::ostream& out, const logic::KeyType& key)
{
	out << key.mKey;
	return out;
}

using namespace logic;

PlayerInfo* KeyType::mpsPlayerInfo = NULL;

void KeyType::init()
{
	mpsPlayerInfo = PlayerInfo::getInstance();
}

bool KeyType::isOwner() const
{
	assert(mpsPlayerInfo != NULL);

	return getOwnerId() == mpsPlayerInfo->getNetworkId();
}

int KeyType::getOwnerId() const
{
	assert(mpsPlayerInfo != NULL);

	// INVALID_KEY automatically gets tested later
	if (mKey >= KEY_LIM.mKey)
	{
		return INVALID_NETWORK_ID;
	}

	int owner = INVALID_NETWORK_ID;
	int tempKey = mKey;
	while (tempKey >= 0)
	{
		owner++;
		tempKey -= MAX_KEYS_PER_PLAYER;
	}

	return owner;
}