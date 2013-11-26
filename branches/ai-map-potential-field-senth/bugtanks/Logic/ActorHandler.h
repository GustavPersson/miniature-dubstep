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

#ifndef __ACTORHANDLER_H__
#define __ACTORHANDLER_H__

#include "../Utilities/Macros.h"
#include "../Utilities/VectorList.h"
#include "IEventManager.h"
#include "Events.h"
#include "Actor.h"
#include <iostream>
#include <map>
#include "KeyHandler.h"
#include "Tank.h"
#include "Enemy.h"
#include "Munitions.h"
#include "StaticEnvironment.h"
#include "../GFX/GUIHandlerTest.h"
#include "../GFX/DayNightHandler.h"

namespace sound {
	class SoundSystem;
}

namespace logic
{
	class LogicQuadTree;

	struct DestroyActorInfo
	{
		KeyType mId;
		int mNetworkId;
	};


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
 * Destroys the actorhandler and the containing actors
 */
	static void destroy();

/**
 * Whenever an object is created, it should also be inserted into the map 
 * @param pActor the actor to be inserted
 */
	void insertActor(Actor *pActor);

/**
 * Whenever an object is destroyed it should also be removed from the map 
 * @param pActor the actor to be removed
 * @param networkId the networkId that the actor belongs to
 */
	void removeActor(Actor *pActor, int networkId);

/**
 * Whenever and object is destroyed it should also be removed from the map
 * @param id the ID of the actor to be removed
 * @param networkId the networkId that the actor belongs to
 */
	void removeActor(KeyType id, int networkId);
	
/**
 * updates a specific actor
 * @param pActor The actor to be destroyed
 */
	void updateActor(Actor *pActor);

/**
 * update all actors
 * @pre setDXModuleInstance must have been called
 * @pre setSoundSystemInstance must have been called
 * @pre setLogicQuadeTreeInstance must have have been called
 * @param deltaTime the time between calls, used to calculate movement
 */
	void updateAll(float deltaTime);

/**
* Update GFX for all actors with all new positions and directions.
*/
	void updateAllGFX();

/**
 * updates all dynamic actors
 * @pre setDXModuleInstance must have been called
 * @pre setSoundSystemInstance must have been called
 * @pre setLogicQuadeTreeInstance must have have been called
 * @param deltaTime the time between calls, used to calculate movement
 */
	void update(float deltaTime);
/**
* Update GFX for dynamic actorswith all new positions and directions.
*/
	void updateGFX();
/**
 * searches for the relevant actor and passes the event their way
 * @param eventData the event to be managed and passed on
 */
	bool handleEvent(EventData const &eventData);
/**
 * returns an actor based on the provided ID
 * @param actorId the id of the actor you want to get
 */
	Actor * getActor(KeyType actorId);

/**
* Returns an dynamic actor based on the provided ID
* @param dynamicActorId the id of the actor you want to get
*/
	DynamicActor * getDynamicActor(KeyType dynamicActorId);

/**
 * returns the last added actor.
 * @return a pointer to the last added actor
 */
	Actor * getLastAddedActor();

/**
* set the pointer to the DXModule singelton.
*/
	void setDXModuleInstance(gfx::DX10Module *pInstance);

/**
* set the pointer to the SoundSystem singelton.
*/
	void setSoundSystemInstance(sound::SoundSystem *pInstance);
/**
* set the pointer to the LogigQuadTree singelton.
*/
	void setLogicQuadTreeInstance(LogicQuadTree *pInstance);

	char const * getName(void) {return "ActorHandler";}

/**
 * Returns a const reference to the actor map.
 * @return Returns mActorMap as a reference.
 */
	inline const std::map<KeyType, Actor *> &getActorMap() const {return mActorMap;}

/**
 * Returns a const reference to the dynamic actor map.
 * @return Returns mDynamicActorMap as a reference.
 */
	inline const std::map<KeyType, Actor *> &getDynamicActorMap() const {return mDynamicActorMap;}

/**
* Send destroy events for all dynamic actors of my actors
*/
	void destroyDynamicActors();

/**
* Send destroy events for somebody else's dynamic actors
*/
	void destroyDynamicActors(int playerID);

/**
* Send destroy events for all my actors
*/
	void destroyAllActors();

	void playerDied(int networkId, Actor::PlayerMovingInfo playerMovingInfo);

	void handleMovingInfo(EventData const &eventMessage);

private:
/**
 * Constructor
 */
	explicit ActorHandler();

	static ActorHandler *mpsInstance;
	gfx::DX10Module		*mpDXModule;
	KeyHandler			*mpKeyHandler;
	PlayerInfo			*mpPlayerInfo;
#ifdef USE_SOUND
	sound::SoundSystem	*mpSoundSystem;
#endif // USE_SOUND
	LogicQuadTree		*mpLogicQuadTree;
	gfx::Light			*mpLight;


	std::map<KeyType,Actor *> mActorMap;
	std::map<KeyType,Actor *> mDynamicActorMap;

	Actor* mpLastAddedActor;
	int mSyncOffset;
	bool mPlayersDead[4];
	Actor::PlayerMovingInfo mPlayersMovingInfo[4];

	utilities::VectorList<DestroyActorInfo> mDestroyedActors;
	utilities::VectorList<CreateActorInfo*> mCreateActorQueue;

	//LOGIC FPS
	int mNrOfUpdates;
	float mTime;

	float mDayTime;

	gfx::GUIHandlerTest *mpFPS;

	gfx::DayNightHandler * mpDayNightHandler;
};
}

#endif