/**
* @file
* @author Gustav Persson <ninjamuff@gmail.com>
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
* 
* @section DESCRIPTION
*
* Holds information for drivers, for example tank actor id, soldier actor id etc.
*/
#include "../Utilities/Macros.h"
#include "PlayerInfo.h"
#include "KeyHandler.h"

using namespace logic;

PlayerInfo* PlayerInfo::mpsInstance = NULL;

PlayerInfo::PlayerInfo()
{
	pthread_mutex_init(&mMutex, NULL);
	pthread_cond_init(&mCondVar, NULL);

	mNetworkId = -1;

	mDriverId = INVALID_KEY;
	mTankActorId = INVALID_KEY;
	mDriverActorId = INVALID_KEY;
}

PlayerInfo::~PlayerInfo()
{
	mpsInstance = NULL;
}

PlayerInfo* PlayerInfo::getInstance()
{
	if(mpsInstance == NULL)
	{
		mpsInstance = myNew PlayerInfo();
	}

	return mpsInstance;
}

void PlayerInfo::setDriverActorId(KeyType driverId)
{
	mDriverActorId = driverId;
}
void PlayerInfo::setSpectatorId(KeyType spectatorId)
{
	mSpectatorId = spectatorId;
}
void PlayerInfo::threadSafeSetDriverActorId(KeyType driverId)
{
	pthread_mutex_lock(&mMutex);
	mDriverActorId = driverId;
	pthread_mutex_unlock(&mMutex);
}

void PlayerInfo::setDriverId(KeyType driverId)
{
	mDriverId = driverId;
}

void PlayerInfo::threadSafeSetDriverId(KeyType driverId)
{
	pthread_mutex_lock(&mMutex);
	mDriverId = driverId;
	pthread_mutex_unlock(&mMutex);
}

void PlayerInfo::setTankActorId(KeyType tankActorId)
{
	mTankActorId = tankActorId;
}

void PlayerInfo::threadSafeSetTankActorId(KeyType tankActorId)
{
	pthread_mutex_lock(&mMutex);
	mTankActorId = tankActorId;
	pthread_mutex_unlock(&mMutex);
}

void PlayerInfo::setNetworkId(int networkId)
{
	mNetworkId = networkId;

// This is a little hax, but it's the easiest way to do this for now.
#ifdef USE_NETWORK
	initLogger(networkId);
#endif
}

void PlayerInfo::threadSafeSetNetworkId(int networkId)
{
	pthread_mutex_lock(&mMutex);
	setNetworkId(networkId);
	pthread_mutex_unlock(&mMutex);

}

KeyType PlayerInfo::getTankActorId()
{
	return mTankActorId;
}

KeyType PlayerInfo::getSpectatorId()
{
	return mSpectatorId;
}

KeyType PlayerInfo::threadSafeGetTankActorId()
{
	pthread_mutex_lock(&mMutex);
	KeyType temp = mTankActorId;
	pthread_mutex_unlock(&mMutex);
	return temp;
}

KeyType PlayerInfo::getDriverActorId()
{
	return mDriverActorId;
}

KeyType PlayerInfo::threadSafeGetDriverActorId()
{
	pthread_mutex_lock(&mMutex);
	KeyType temp = mDriverActorId;
	pthread_mutex_unlock(&mMutex);
	return temp;
}

KeyType PlayerInfo::getDriverId()
{
	return mDriverId;
}

KeyType PlayerInfo::threadSafeGetDriverId()
{
	pthread_mutex_lock(&mMutex);
	KeyType temp = mDriverId;
	pthread_mutex_unlock(&mMutex);
	return temp;
}

int PlayerInfo::getNetworkId()
{
	return mNetworkId;
}

int PlayerInfo::threadSafeGetNetworkId()
{
	pthread_mutex_lock(&mMutex);
	int temp = mNetworkId;
	pthread_mutex_unlock(&mMutex);
	return temp;
}

KeyType PlayerInfo::getPlayerId()
{
	if(mTankActorId == INVALID_KEY && mDriverActorId == INVALID_KEY)
		return mSpectatorId;
	else if(mTankActorId == INVALID_KEY)
		return mDriverActorId;
	else
		return mTankActorId;
}