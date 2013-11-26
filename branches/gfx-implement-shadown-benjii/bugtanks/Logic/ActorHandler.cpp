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
 * 
 * @section DESCRIPTION
 *
 * This class will have a map of all the game objects in the game and it will be used to update all objects smoothly
 */

#include "ActorHandler.h"
#include <typeinfo>
#include "Enemy.h"
#include "Driver.h"
#include "Tank.h"
#include "Munitions.h"
#include <cassert>
#include <cmath>
#include "../Sound/SoundSystem.h"

using namespace logic;

ActorHandler* ActorHandler::mpsInstance = NULL;

ActorHandler::ActorHandler()
{
	mpDXModule = NULL;
}

ActorHandler::~ActorHandler()
{	
	std::map<KeyType,Actor *>::iterator actorIt;
	mpsInstance = NULL;
	SAFE_DELETE(mpsInstance);
	for(actorIt = mActorMap.begin(); actorIt != mActorMap.end(); ++actorIt)
	{
		SAFE_DELETE(actorIt->second);
	}
}

ActorHandler* ActorHandler::getInstance()
{
	if (mpsInstance == NULL)
		mpsInstance = myNew ActorHandler();

	return mpsInstance;
}

void ActorHandler::insertActor(Actor * pActor)
{ 
	mActorMap[pActor->getKey()] = pActor;
	safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorCreated(pActor->getKey(), GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))));
}

void ActorHandler::removeActor(Actor * pActor)
{
	std::map<KeyType,Actor *>::iterator actorIt;
	actorIt = mActorMap.find(pActor->getKey());
	// TODO freekey must be here aswell
	KeyHandler::getInstance()->freeKey(pActor->getKey(),1);
	SAFE_DELETE(actorIt->second);
	mActorMap.erase(actorIt);	
}

void ActorHandler::removeActor(KeyType id)
{	
	std::map<KeyType,Actor *>::iterator actorIt;
	actorIt = mActorMap.find(id);
	// TODO freekey must be here aswell
	KeyHandler::getInstance()->freeKey(id,1);
	SAFE_DELETE(actorIt->second);
	mActorMap.erase(actorIt);	
}

void ActorHandler::updateActor(Actor *pActor)
{
	std::map<KeyType,Actor *>::iterator actorIt;
	actorIt = mActorMap.find(pActor->getKey());
	actorIt->second->update(0.1f);
}

void ActorHandler::update(float deltaTime)
{
 mpDXModule->lock(0);
 if(!mDestroyedActors.empty())
 {
	 while(!mDestroyedActors.empty())
	 {
		 mpDXModule->removeRenderObject(mDestroyedActors.getFirst());
		 mDestroyedActors.removeFirst();
	 }
 }
 std::map<KeyType,Actor *>::iterator actorIt;
 for(actorIt = mActorMap.begin() ; actorIt != mActorMap.end(); ++actorIt)
 {
	 (*actorIt).second->update(deltaTime);

	 sound::ActorInfo actorInfo;
	 actorInfo.mActorID = (*actorIt).second->getKey();
	 actorInfo.mMoving = (*actorIt).second->getMoving();
	 actorInfo.mPosition = (*actorIt).second->getPosition();
	 actorInfo.mRotating = (*actorIt).second->getRotating();
	 sound::SoundSystem::getInstance()->updateChannel(actorInfo);	
 }
 mpDXModule->unlock(0);

}

Actor* ActorHandler::getActor(KeyType actorId)
{
	std::map<KeyType,Actor *>::iterator actorIt = mActorMap.find(actorId);
	if (actorIt != mActorMap.end())
	{
		return actorIt->second;
	}
	else
	{
		return NULL;
	}
}

void ActorHandler::setDXModule()
{
	mpDXModule = gfx::DX10Module::getInstance();
}

bool ActorHandler::handleEvent(EventData const &eventMessage)
{   
	std::map<KeyType,Actor *>::iterator actorIt;
	if(EvtData_From_ActorCollision::mEventType == eventMessage.getEventType())
	{
		const EvtData_From_ActorCollision& evtData = static_cast<const EvtData_From_ActorCollision&>(eventMessage);
		std::map<KeyType, Actor*>::iterator tmpActor1 = mActorMap.find(evtData.mActorId1);
		std::map<KeyType, Actor*>::iterator tmpActor2 = mActorMap.find(evtData.mActorId2);
		std::map<KeyType, Actor*>::iterator tmpActor3 = mActorMap.find(evtData.mActorId3);
		assert(tmpActor1 != mActorMap.end());
		assert(tmpActor2 != mActorMap.end());
		assert(tmpActor3 != mActorMap.end());
		Enemy*		pEnemy1 = dynamic_cast<Enemy*>(tmpActor1->second);
		Enemy*		pEnemy2 = dynamic_cast<Enemy*>(tmpActor2->second);
		Enemy*		pEnemy3 = dynamic_cast<Enemy*>(tmpActor3->second);
		//Driver*		pDriver1 = dynamic_cast<Driver*>(tmpActor1->second);
		//Driver*		pDriver2 = dynamic_cast<Driver*>(tmpActor2->second);
		Tank*		pTank1 = dynamic_cast<Tank*>(tmpActor1->second);
		Tank*		pTank2 = dynamic_cast<Tank*>(tmpActor2->second);
		Munitions*	pMunitions1 = dynamic_cast<Munitions*>(tmpActor1->second);
		Munitions*	pMunitions2 = dynamic_cast<Munitions*>(tmpActor2->second);
		//If an enemy is hit by munition
		if(pEnemy1 != NULL && pMunitions2 != NULL && pEnemy3 == NULL)
		{
			pEnemy1->reduceHp(Weapon::calculateDamage(pMunitions2->getEfficiency(), pEnemy1->getSize(), pMunitions2->getDamage()));
		}
		//If an enemy is hit by munition
		else if(pEnemy2 != NULL && pMunitions1 != NULL && pEnemy3 == NULL)
		{
			pEnemy2->reduceHp(Weapon::calculateDamage(pMunitions1->getEfficiency(), pEnemy2->getSize(), pMunitions1->getDamage()));
		}
		//If an tank is hit by munition
		if(pTank1 != NULL && pMunitions2 != NULL && pEnemy3 != NULL)
		{
			pTank1->reduceHp(pEnemy3->getDamage());
		}
		//If an tank is hit by munition
		else if(pTank2 != NULL && pMunitions1 != NULL && pEnemy3 != NULL)
		{
			pTank2->reduceHp(pEnemy3->getDamage());
		}
	}
	if(EvtData_To_EnemyUpdateHp::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_EnemyUpdateHp& evtData = static_cast<const EvtData_To_EnemyUpdateHp&>(eventMessage);
		actorIt = mActorMap.find(evtData.mId);
		actorIt->second->handleEventId(eventMessage);
	} 
	
	if(EvtData_To_TankStartFire::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_TankStartFire& evtData = static_cast<const EvtData_To_TankStartFire&>(eventMessage);
		actorIt = mActorMap.find(evtData.mTankId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);
	}

	if(EvtData_To_TankStopFire::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_TankStopFire& evtData = static_cast<const EvtData_To_TankStopFire&>(eventMessage);
		actorIt = mActorMap.find(evtData.mTankId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);
	}

	if(EvtData_To_DriverStartFire::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_DriverStartFire& evtData = static_cast<const EvtData_To_DriverStartFire&>(eventMessage);
		actorIt = mActorMap.find(evtData.mDriverId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);
	}

	if(EvtData_To_DriverStopFire::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_DriverStopFire& evtData = static_cast<const EvtData_To_DriverStopFire&>(eventMessage);
		actorIt = mActorMap.find(evtData.mDriverId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);
	}

	if(EvtData_From_ActorDestroyed::mEventType == eventMessage.getEventType())
	{
    	const EvtData_From_ActorDestroyed& evtData = static_cast<const EvtData_From_ActorDestroyed&>(eventMessage);
		std::map<KeyType, Actor*>::iterator actorIt = mActorMap.find(evtData.mId);
		assert(actorIt != mActorMap.end());
		mDestroyedActors.add((*actorIt).second->getKey());
		removeActor((*actorIt).second);
	}
	if(EvtData_To_ActorStartMove::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventMessage);
		std::map<KeyType, Actor*>::iterator actorIt = mActorMap.find(evtData.mId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);	
	}
	if(EvtData_To_ActorStopMove::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStopMove& evtData = static_cast<const EvtData_To_ActorStopMove&>(eventMessage);
		std::map<KeyType, Actor*>::iterator actorIt = mActorMap.find(evtData.mId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);	
	}
	if(EvtData_To_ActorStartRotate::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartRotate& evtData = static_cast<const EvtData_To_ActorStartRotate&>(eventMessage);
		actorIt = mActorMap.find(evtData.mId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);
	}
	if(EvtData_To_ActorStopRotate::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStopRotate& evtData = static_cast<const EvtData_To_ActorStopRotate&>(eventMessage);
		actorIt = mActorMap.find(evtData.mId);
		assert(actorIt != mActorMap.end());
		actorIt->second->handleEventId(eventMessage);
	}
		
	return false;
}