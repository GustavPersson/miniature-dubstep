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

#ifndef __PlayerInfo_H__
#define __PlayerInfo_H__

#include <pthread.h>
#include "KeyHandler.h"

namespace logic
{
/**
* Singletonclass holding information for drivers.
* For example, Tank-id and Driver-ID.
* Thread safe.
*/
class PlayerInfo
{
public:
	/**
	* Destructor. Destroys the static instance.
	*/
	virtual ~PlayerInfo();

	/**
	* Gets a pointer to the instance of the class. If
	* an instance is not already created, it creates it first.
	* @return pointer to an instance of PlayerInfo
	*/
	static PlayerInfo* getInstance();

	/**
	* Sets the driver ID.
	* @param driverId
	*/
	void setDriverId(KeyType driverId);
	/**
	* Sets the driver ID. Thread Safe.
	* @param driverId
	*/
	void threadSafeSetDriverId(KeyType driverId);
	
	/**
	* Sets the Tank actor ID.
	*/
	void setTankActorId(KeyType tankActorId);
	/**
	* Sets the Tank actor ID. Thread Safe.
	*/
	void threadSafeSetTankActorId(KeyType tankActorId);

	/**
	* Sets the Driver actor ID.
	*/
	void setDriverActorId(KeyType driverId);
	/**
	* Sets the Driver actor ID. Thread Safe.
	*/
	void threadSafeSetDriverActorId(KeyType driverId);

	/**
	* Returns the current player's Driver ID
	*/
	KeyType getDriverId();
	/**
	* Returns the current player's Driver ID. Thread Safe.
	*/	
	KeyType threadSafeGetDriverId();

	/**
	* Returns the current player's Tank Actor ID
	*/
	KeyType getTankActorId();
	/**
	* Returns the current player's Tank Actor ID. Thread Safe
	*/
	KeyType threadSafeGetTankActorId();
	
	/**
	* Returns the current player's Driver Actor ID.
	*/
	KeyType getDriverActorId();
	/**
	* Returns the current player's Driver Actor ID. Thread Safe
	*/
	KeyType threadSafeGetDriverActorId();
private:
	PlayerInfo();
	static PlayerInfo* mpsInstance;
	
	KeyType mDriverId;
	KeyType mTankActorId;
	KeyType mDriverActorId;

	pthread_mutex_t mMutex;
	pthread_cond_t mCondVar;
};
}
#endif