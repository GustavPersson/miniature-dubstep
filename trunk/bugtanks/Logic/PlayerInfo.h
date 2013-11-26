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
*/

#ifndef __PlayerInfo_H__
#define __PlayerInfo_H__

#include <pthread.h>
#include "KeyHandler.h"

namespace logic
{

const int INVALID_NETWORK_ID = -1;

/**
* Singletonclass holding information for players.
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
	 * Sets the Spectator ID 
	 * @param spectatorId the id you want to set as spectator id
	 */
	void setSpectatorId(KeyType spectatorId);
	/**
	* Sets the network ID.
	*/
	void setNetworkId(int networkId);

	/**
	* Sets the network ID, thread safe.
	*/
	void threadSafeSetNetworkId(int networkId);

	/**
	* Returns the current player's Driver ID
	*/
	KeyType getDriverId();

	/**
	 * Gets the spectator id
	 */
	KeyType getSpectatorId();
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

	/**
	* Retunrs the current player's network ID.
	*/
	int getNetworkId();

	/**
	* Returns the current player's network ID, thread safely.
	*/
	int threadSafeGetNetworkId();

	/**
	* Returns the tank id if the tank is alive otherwise it returns the driver id.
	* @return The id of either the tank or the driver.
	*/
	KeyType getPlayerId();

private:
	/**
	* The constructor.
	*/
	PlayerInfo();

	static PlayerInfo* mpsInstance;
	
	KeyType mDriverId;
	KeyType mSpectatorId;
	KeyType mTankActorId;
	KeyType mDriverActorId;

	int mNetworkId;

	pthread_mutex_t mMutex;
	pthread_cond_t mCondVar;
};
}
#endif