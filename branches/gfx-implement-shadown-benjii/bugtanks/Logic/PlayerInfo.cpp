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

KeyType PlayerInfo::getTankActorId()
{
	return mTankActorId;
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

