/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
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
#include "../Utilities/VectorList.h"
#include "IEventManager.h"
#include "Events.h"
#include "Actor.h"
#include <iostream>
#include <map>
#include "KeyHandler.h"

#ifndef __ACTORHANDLER_H__
#define __ACTORHANDLER_H__


namespace logic
{

/**
 * This class will have a map of all the game objects in the game and it will be used to update all objects smoothly
 */


class ActorHandler : public EventListener 
{

public:
/**
 *Deconstructor
 */
	virtual ~ActorHandler();

/**
* Gets a pointer to the instance of the class. if
* an iunstance is not already created, it does so first.
* @return pointer to an instance of ActorHandler
*/
	static ActorHandler* getInstance();

/**
 * Whenever an object is created, it should also be inserted into the map 
 * @param pActor the actor to be inserted
 */
	void insertActor(Actor *pActor);

/**
 * Whenever an object is destroyed it should also be removed from the map 
 * @param pActor the actor to be removed
 */
	void removeActor(Actor *pActor);

/**
 * Whenever and object is destroyed it should also be removed from the map
 * @param id the ID of the actor to be removed
 */
	void removeActor(KeyType id);

/**
 * updates actors 
 * @param deltaTime the time between calls, used to calculate movement
 */
	void update(float deltaTime);
/**
 * searches for the relevant actor and passes the event their way
 * @param eventMessage the event to be managed and passed on
 */
	bool handleEvent(EventData const &eventMessage);
/**
 * returns an actor based on the provided ID
 * @param actorId the id of the actor you want to get
 */
	Actor * getActor(KeyType actorId);
/**
 * updates a specific actor 
 */
	void updateActor(Actor *pActor);

	char const * getName(void) {return "ActorHandler";}

	void setDXModule();

private:
/**
 * Constructor
 */
	explicit ActorHandler();

	static ActorHandler* mpsInstance;
	gfx::DX10Module *mpDXModule;

	std::map<KeyType,Actor *> mActorMap;
	

	utilities::VectorList<KeyType> mDestroyedActors;


};
}

#endif