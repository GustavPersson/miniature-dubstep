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
#include <vector>
#include "FireAnt.h" 
#include "Ant.h"
#include "KamikazeAnt.h"
#include "Tarantula.h"
#include "BlackWidow.h"
#include "Jumper.h"
#include "Scarab.h"
#include "PlasmaBeetle.h"
#include "Fire.h"
#include "EnemyWeapon.h"
#include "../Sound/SoundSystem.h"
#include "LogicQuadTree.h"
#include "Bullet.h"
#include "Beam.h"
#include "Flame.h"
#include "Pulse.h"
#include "../gfx/Particle.h"
#include <sstream>
#include "Spectator.h"
#include "Application.h"
#include "TriggerHandler.h"

using namespace logic;

ActorHandler* ActorHandler::mpsInstance = NULL;

const int ROTATE_LEFT = 1;
const int ROTATE_RIGHT = -1;
const int MOVE_FORWARD = 1;
const int MOVE_BACKWARDS = -1;

const int FRAMES_PER_SYNC = 50;
const int FRAMES_PER_SYNC_EDITOR = 10;

ActorHandler::ActorHandler()
{
	mpLastAddedActor = NULL;
	mpDXModule = NULL;
	mpKeyHandler = NULL;
	mpPlayerInfo = NULL;
#ifdef USE_SOUND
	mpSoundSystem = NULL;
#endif // USE_SOUND
	mpLogicQuadTree = NULL;
	mSyncOffset = 0;
	for(int i = 0; i < 4; ++i)
	{
		mPlayersDead[i] = false;
		mPlayersMovingInfo[i] = Actor::PlayerMovingInfo();
	}
	

	mpKeyHandler = KeyHandler::getInstance();
	mpPlayerInfo = PlayerInfo::getInstance();
	safeTriggerEvent(EvtData_System_ActorHandlerStarted());

	//Logic FPS
	mNrOfUpdates = 0;
	mTime = 0.0f;
	mDayTime = 0.0f;

	mMapCompleteTimer = 0.0f;
	mMapCompleteMaxTime = 3.0f;
	mFlagMapCompleteTimerStart = false;
	mFlagMapCompleted = false;
	mFlagMapVictory = false;

	mMapDefeatTimer = 0.0f;
	mMapDefeatMaxTimer = 3.0f;
	mFlagMapDefeatTimerStart = false;
	mFlagMapDefeated = false;
	mFlagMapLose = false;

	mpDayNightHandler = myNew gfx::DayNightHandler(0.0f);
}

ActorHandler::~ActorHandler()
{	
	destroy();
	SAFE_DELETE(mpDayNightHandler);
}

ActorHandler* ActorHandler::getInstance()
{
	if (mpsInstance == NULL)
		mpsInstance = myNew ActorHandler();

	return mpsInstance;
}

void ActorHandler::destroy()
{
	if (mpsInstance != NULL)
	{
		
		std::map<KeyType,Actor *>::iterator actorIt;
		for(actorIt = mpsInstance->mActorMap.begin(); actorIt != mpsInstance->mActorMap.end(); ++actorIt)
		{
			SAFE_DELETE(actorIt->second);
		}

		mpsInstance = NULL;
	}
}

void ActorHandler::insertActor(Actor * pActor)
{
	mpLastAddedActor = pActor;
	mActorMap[pActor->getKey()] = pActor;
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorCreated(pActor->getKey())));
	if(dynamic_cast<DynamicActor*>(pActor) != NULL)
	{
		mDynamicActorMap[pActor->getKey()] = pActor;
	}
}

void ActorHandler::removeActor(Actor * pActor, int networkId)
{
	removeActor(pActor->getKey(), networkId);
}

void ActorHandler::removeActor(KeyType id, int networkId)
{	
	if(id != INVALID_KEY && id.getOwnerId() == networkId)
	{
		std::map<KeyType,Actor *>::iterator actorIt;
		actorIt = mActorMap.find(id);
		assert(actorIt != mActorMap.end());
		mpKeyHandler->freeKey(id,networkId);
 		if(dynamic_cast<DynamicActor*>(actorIt->second) != NULL)
		{
 			mDynamicActorMap.erase(id);
		}
		SAFE_DELETE(actorIt->second);
		mActorMap.erase(actorIt);
	}
}

void ActorHandler::removeActorGlobal(KeyType id, int networkId)
{	
	if(id != INVALID_KEY)
	{
		std::map<KeyType,Actor *>::iterator actorIt;
		actorIt = mActorMap.find(id);
		if(actorIt != mActorMap.end())
		{
 			if(dynamic_cast<DynamicActor*>(actorIt->second) != NULL)
			{
 				mDynamicActorMap.erase(id);
			}
			SAFE_DELETE(actorIt->second);
			mActorMap.erase(actorIt);
		}
	}
}

void ActorHandler::updateActor(Actor *pActor)
{
	std::map<KeyType,Actor *>::iterator actorIt;
	actorIt = mActorMap.find(pActor->getKey());
	actorIt->second->update(0.1f);
}

void ActorHandler::updateAll(float deltaTime)
{
	assert(mpLogicQuadTree != NULL);
#ifdef USE_SOUND
	assert(mpSoundSystem != NULL);
#endif // USE_SOUND

	mSyncOffset++;
	
	Actor::ActorSyncInfo syncInfo;
	std::vector<Actor::ActorSyncInfo> syncInfos;

	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mActorMap.begin() ; actorIt != mActorMap.end(); ++actorIt)
	{
		(*actorIt).second->update(deltaTime);
		
		sound::ActorInfo actorInfo;
		actorInfo.mActorID = (*actorIt).second->getKey();
		actorInfo.mMoving = (*actorIt).second->getMoving();
		actorInfo.mPosition = (*actorIt).second->getPosition();
		actorInfo.mRotating = (*actorIt).second->getRotating();
		actorInfo.mSubsetRotating = false;
		actorInfo.mFire = false;
		if((*actorIt).second->getActorType() == Actor::ActorType_Tank)
		{
			actorInfo.mSubsetRotating = (*actorIt).second->getSubsetRotating();
			actorInfo.mFire = dynamic_cast<Tank*>((*actorIt).second)->getFiring();
		}
#ifdef USE_SOUND
		mpSoundSystem->updateChannel(actorInfo);
#endif // USE_SOUND
		if(mSyncOffset > FRAMES_PER_SYNC_EDITOR && actorInfo.mActorID.isOwner() && (*actorIt).second->getSync())
		{
			(*actorIt).second->setSync(false);
			syncInfo.mId		= actorInfo.mActorID;
			syncInfo.mDirection = (*actorIt).second->getDirection();
			syncInfo.mPosition	= actorInfo.mPosition;
			syncInfo.mScale		= (*actorIt).second->getScale();
			if ((*actorIt).second->getActorType() == Actor::ActorType_Tank)
			{
				syncInfo.mSubsetDirection = (*actorIt).second->getSubsetDirection();
			}
			syncInfos.push_back(syncInfo);
		}
	}
	if(mSyncOffset > FRAMES_PER_SYNC_EDITOR)
	{
		logic::safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorHandlerSyncActors(syncInfos)));
		mSyncOffset = 0;
	}
}

void ActorHandler::updateAllGFX()
{
	removeActors();
	addActors();

	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mActorMap.begin() ; actorIt != mActorMap.end(); ++actorIt)
	{
		Actor *pActor = (*actorIt).second;
		gfx::TransformObject *pObject = pActor->getTransformObject();
		if(pObject != NULL)
		{
			utilities::Position pos = pActor->getPosition();
			utilities::Direction dir = pActor->getDirection();

			pObject->setPosition(D3DXVECTOR3(pos.x, pos.y, pos.z));

			float angle = dir.getZXAngleClockwise();
			pObject->setRotationY(angle);

			pObject->setScale(pActor->getScale().convertToD3DXVECTOR3());

			if(mpPlayerInfo->getPlayerId() == pActor->getKey())
			{
				if(pActor->getActorType() == Actor::ActorType_Tank)
				{
					utilities::Direction subDir = pActor->getSubsetDirection();
					angle = subDir.getZXAngleClockwise();
					dynamic_cast<gfx::InstanceMesh*>(pObject)->getSubsetByIndex(1)->setRotationY(angle);
				}
				if(pActor->getKey().isOwner())
				{
					gfx::Camera *pCamera = mpDXModule->getCamera();
					pCamera->setPosition(pos.x - 100.0f, 500.0f, pos.z - 100.0f);
					pCamera->setTarget(pos.x, pos.y, pos.z);

					mpLight->setLightDir(mpDayNightHandler->getLightDirection());
					mpLight->setAmbientLight(mpDayNightHandler->getAmbientColor());

					//mpLight->setLightPos(pCamera->getPosition() + D3DXVECTOR3(200.0f, 200.0f, 200.0f));
					mpLight->setLightPos(pos.convertToD3DXVECTOR3() + mpDayNightHandler->getLightPosition());
				}				
			}	
		}
	}
}

void ActorHandler::update(float deltaTime)
{
	mpDayNightHandler->setTime(deltaTime);

	assert(mpLogicQuadTree != NULL);
#ifdef USE_SOUND
	assert(mpSoundSystem != NULL);
#endif // USE_SOUND

	
	mSyncOffset++;
	bool update = true;
	
	Actor::ActorSyncInfo syncInfo;
	std::vector<Actor::ActorSyncInfo> syncInfos;
	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mDynamicActorMap.begin() ; actorIt != mDynamicActorMap.end(); ++actorIt)
	{
		/*Munitions *munition = dynamic_cast<Munitions*>((*actorIt).second);
		if(munition != NULL)
		{
			if(!munition->getVisible())
			{
				update = false;
			}
		}*/

		if(update)
		{

			(*actorIt).second->update(deltaTime);			
			
			sound::ActorInfo actorInfo;
			actorInfo.mActorID = (*actorIt).second->getKey();
			actorInfo.mMoving = (*actorIt).second->getMoving();
			actorInfo.mPosition = (*actorIt).second->getPosition();
			actorInfo.mRotating = (*actorIt).second->getRotating();
			actorInfo.mSubsetRotating = false;
			actorInfo.mFire = false;
			if((*actorIt).second->getActorType() == Actor::ActorType_Tank)
			{
				actorInfo.mSubsetRotating = (*actorIt).second->getSubsetRotating();
				actorInfo.mFire = dynamic_cast<Tank*>((*actorIt).second)->getFiring();
			}	
#ifdef USE_SOUND
			mpSoundSystem->updateChannel(actorInfo);
#endif // USE_SOUND
			
			if((getActor(actorInfo.mActorID)->getActorType() == Actor::ActorType_Tank 
												|| getActor(actorInfo.mActorID)->getActorType() == Actor::ActorType_Driver))
			{				
					mpLogicQuadTree->collisionObject(actorInfo.mActorID, deltaTime);
			}
			else if(getActor(actorInfo.mActorID)->getActorType() == Actor::ActorType_Munition && getActor(actorInfo.mActorID)->getVisible() /*&& getActor(actorInfo.mActorID)->getKey().isOwner()*/)
			{
				Munitions *pMunition = dynamic_cast<Munitions*>(getActor(actorInfo.mActorID));
				if(pMunition->getOwnerType() == Actor::ActorType_Tank)
				{
					if(pMunition->getWeaponType() == Weapon::WeaponTypes_Cannon || pMunition->getWeaponType() == Weapon::WeaponTypes_GaussCannon || pMunition->getWeaponType() == Weapon::WeaponTypes_Shotgun)
					{	
						mpLogicQuadTree->collisionMunition(actorInfo.mActorID);				
					}
					else if(pMunition->getWeaponType() == Weapon::WeaponTypes_Flamethrower || pMunition->getWeaponType() == Weapon::WeaponTypes_PulseCannon)
					{
						if(pMunition->getKey().isOwner())
						{
							mpLogicQuadTree->collisionCubeMunition(actorInfo.mActorID);
							safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFired(pMunition->getOwner())));
						}
					}
				}
				else if(pMunition->getOwnerType() == Actor::ActorType_Enemy)
				{
					if(pMunition != NULL && (pMunition->getActorSubType() == Munitions::MunitionType_Fire || pMunition->getActorSubType() == Munitions::MunitionType_Acid))
					{	
						mpLogicQuadTree->collisionMunition(actorInfo.mActorID);
					}
					else
					{
						if(pMunition->getKey().isOwner())
							mpLogicQuadTree->collisionCubeMunition(actorInfo.mActorID);
					}
				}
				else
				{
					mpLogicQuadTree->collisionMunition(actorInfo.mActorID);
				}
			
			}		
			if(mSyncOffset > FRAMES_PER_SYNC && actorInfo.mActorID.isOwner() && ((*actorIt).second->getActorType() == Actor::ActorType_Tank || (*actorIt).second->getActorType() == Actor::ActorType_Driver))
			{
				syncInfo.mId		= actorInfo.mActorID;
				syncInfo.mDirection = (*actorIt).second->getDirection();
				syncInfo.mPosition	= actorInfo.mPosition;
				syncInfo.mScale = (*actorIt).second->getScale();
				if ((*actorIt).second->getActorType() == Actor::ActorType_Tank)
				{
					syncInfo.mSubsetDirection = (*actorIt).second->getSubsetDirection();
				}
				syncInfos.push_back(syncInfo);
			}
		}
	}
	if(mSyncOffset > FRAMES_PER_SYNC)
	{
		logic::safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorHandlerSyncActors(syncInfos)));
		mSyncOffset = 0;
	}

	detectMapCompletion(deltaTime);
}

void ActorHandler::updateGFX()
{
	addActors();

	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mDynamicActorMap.begin() ; actorIt != mDynamicActorMap.end(); ++actorIt)
	{
		Actor *pActor = (*actorIt).second;
		gfx::TransformObject *pObject = pActor->getTransformObject();
		utilities::Position pos = pActor->getPosition();

		if(mpPlayerInfo->getPlayerId() == pActor->getKey())
		{
			if(pActor->getKey().isOwner())
			{
				gfx::Camera *pCamera = mpDXModule->getCamera();
				pCamera->setPosition(pos.x - 100.0f, 500.0f, pos.z - 100.0f);
				pCamera->setTarget(pos.x, pos.y, pos.z);

				mpLight->setLightDir(mpDayNightHandler->getLightDirection());
				mpLight->setAmbientLight(mpDayNightHandler->getAmbientColor());

				//mpLight->setLightPos(pCamera->getPosition() + D3DXVECTOR3(200.0f, 200.0f, 200.0f));
				mpLight->setLightPos(pos.convertToD3DXVECTOR3() + mpDayNightHandler->getLightPosition());
			}				
		}

		if(pObject != NULL)
		{
			if(pActor->getActorType() == Actor::ActorType_Munition && pActor->getActorSubType() != Munitions::MunitionType_Flame && pActor->getActorSubType() != Munitions::MunitionType_Pulse && pActor->getActorSubType() != Munitions::MunitionType_Plasma && dynamic_cast<Munitions*>(pActor)->getVisible())
			{
				pActor->getTransformObject()->setFlagRender(true);

			}
			else if(pActor->getActorType() == Actor::ActorType_Munition && !dynamic_cast<Munitions*>(pActor)->getVisible())
			{	
				pActor->getTransformObject()->setFlagRender(false);
			}
			utilities::Direction dir;
			if(pActor->getActorType() != Actor::ActorType_Enemy)
				dir = pActor->getDirection();
			else
				dir = dynamic_cast<Unit*>(pActor)->getFocalDirection();

			pObject->setPosition(D3DXVECTOR3(pos.x, pos.y, pos.z));
			pObject->setScale(pActor->getScale().convertToD3DXVECTOR3());

			float angle = dir.getZXAngleClockwise();
			pObject->setRotationY(angle);

			if(pActor->getActorType() == Actor::ActorType_Tank)
			{
				utilities::Direction subDir = pActor->getSubsetDirection();
				angle = subDir.getZXAngleClockwise();
				dynamic_cast<gfx::InstanceMesh*>(pObject)->getSubsetByIndex(1)->setRotationY(angle);
			}			
		}
	}

	removeActors();
}

bool ActorHandler::handleEvent(EventData const &eventData)
{   
	std::map<KeyType,Actor *>::iterator actorIt;
	if(EvtData_System_MapVictory::msEventType == eventData.getEventType())
	{
		mFlagMapCompleted = true;
		mFlagMapVictory = true;
	}
	if(EvtData_System_MapDefeat::msEventType == eventData.getEventType())
	{
		mFlagMapDefeated = true;
		mFlagMapLose = true;
	}
	else if(EvtData_From_ActorCollisionObject::msEventType == eventData.getEventType())
	{	
		const EvtData_From_ActorCollisionObject& evtData = static_cast<const EvtData_From_ActorCollisionObject&>(eventData);
		Actor *pFirstActor = getActor(evtData.mFirstActorId);
		Actor *pSecondActor = getActor(evtData.mSecondActorId);

		if((pFirstActor != NULL && pSecondActor != NULL))
		{
			Actor::ActorTypes firstActorType = pFirstActor->getActorType();
			Actor::ActorTypes secondActorType = pSecondActor->getActorType();
	
			//if something collides with a building, tank, enemy or a driver
			if(secondActorType == Actor::ActorType_StaticEnvironment || secondActorType == Actor::ActorType_Driver || secondActorType == Actor::ActorType_Enemy || secondActorType == Actor::ActorType_Tank)
			{
				if(firstActorType == Actor::ActorType_Munition)
				{
					if(pFirstActor->getKey().isOwner())
					{
						pFirstActor->handleEventId(eventData);
					}
				}
				else
				{
					pFirstActor->handleEventId(eventData);
				}				
			}			
		}
	}
	else if(EvtData_From_ActorCollisionRay::msEventType == eventData.getEventType())
	{
		const EvtData_From_ActorCollisionRay& evtData = static_cast<const EvtData_From_ActorCollisionRay&>(eventData);
		Munitions *pMunition = dynamic_cast<Munitions*>(getActor(evtData.mBulletActorId));
		Actor *pShooter = getActor(evtData.mShooterActorId);
		Enemy *pTarget = dynamic_cast<Enemy*>(getActor(evtData.mTargetActorId));
		
		if(pMunition != NULL && pShooter != NULL && pTarget != NULL)
		{	
			int damageMultiplier = 1;
			if(pMunition->getKey().isOwner())
			{
				
				if(pMunition->getActorSubType() == Munitions::MunitionType_Beam && pTarget->getEnemySize() == Enemy::Size_Large)
				{
					damageMultiplier = 2;
				}
				if(pTarget->getKey().isOwner())
				{
					pTarget->reduceHp(Weapon::calculateDamage(pMunition->getEfficiency(), pTarget->getEnemySize(), pMunition->getDamage())*damageMultiplier, evtData.getNetworkId(), evtData.mShooterActorId);
				}
				else
				{
					safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorReduceHp(pTarget->getKey(), Weapon::calculateDamage(pMunition->getEfficiency(), pTarget->getEnemySize(), pMunition->getDamage())*damageMultiplier, evtData.mShooterActorId)));
					//safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyHit(evtData.mShooterActorId, evtData.mTargetActorId)));
				}	
			}
			if(pMunition->getWeaponType() == Weapon::WeaponTypes_Minigun)
			{
				pTarget->reduceHp(Weapon::calculateDamage(pMunition->getEfficiency(), pTarget->getEnemySize(), pMunition->getDamage())*damageMultiplier, eventData.getNetworkId(), evtData.mShooterActorId);
				mpDXModule->getParticles("SmallExplosion")->addEmitter(evtData.mPosition);
				safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_EnemySmallExplosion(utilities::Position(evtData.mPosition.x, evtData.mPosition.y, evtData.mPosition.z))));
			}
		}		
	}
	
	else if(EvtData_To_EnemyUpdateHp::msEventType == eventData.getEventType())
	{
		const EvtData_To_EnemyUpdateHp& evtData = static_cast<const EvtData_To_EnemyUpdateHp&>(eventData);
		actorIt = mActorMap.find(evtData.mId);
		actorIt->second->handleEventId(eventData);
	} 
	
	else if(EvtData_To_TankStartFire::msEventType == eventData.getEventType())
	{
		const EvtData_To_TankStartFire& evtData = static_cast<const EvtData_To_TankStartFire&>(eventData);
		actorIt = mActorMap.find(evtData.mTankId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);
		}
	}

	else if(EvtData_To_TankStopFire::msEventType == eventData.getEventType())
	{
		const EvtData_To_TankStopFire& evtData = static_cast<const EvtData_To_TankStopFire&>(eventData);
		actorIt = mActorMap.find(evtData.mTankId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);
		}
	}
	
	else if(EvtData_To_TankStartFireSecondary::msEventType == eventData.getEventType())
	{
		const EvtData_To_TankStartFireSecondary& evtData = static_cast<const EvtData_To_TankStartFireSecondary&>(eventData);
		actorIt = mActorMap.find(evtData.mTankId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);
		}
	}	

	else if(EvtData_To_TankStopFireSecondary::msEventType == eventData.getEventType())
	{
		const EvtData_To_TankStopFireSecondary& evtData = static_cast<const EvtData_To_TankStopFireSecondary&>(eventData);
		actorIt = mActorMap.find(evtData.mTankId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);
		}
	}

	else if(EvtData_To_DriverStartFire::msEventType == eventData.getEventType())
	{
		const EvtData_To_DriverStartFire& evtData = static_cast<const EvtData_To_DriverStartFire&>(eventData);
		actorIt = mActorMap.find(evtData.mDriverId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);
		}
	}

	else if(EvtData_To_DriverStopFire::msEventType == eventData.getEventType())
	{
		const EvtData_To_DriverStopFire& evtData = static_cast<const EvtData_To_DriverStopFire&>(eventData);
		actorIt = mActorMap.find(evtData.mDriverId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);
		}
	}

	else if(EvtData_From_ActorDestroyed::msEventType == eventData.getEventType())
	{
    	const EvtData_From_ActorDestroyed& evtData = static_cast<const EvtData_From_ActorDestroyed&>(eventData);
		std::map<KeyType, Actor*>::iterator actorIt = mActorMap.find(evtData.mId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			DestroyActorInfo info;
			info.mId = evtData.mId;
			info.mNetworkId = evtData.getNetworkId();
			mDestroyedActors.add(info);
		}
		
		/*if (!actorIt->second->getKey().isOwner())
		{
			//If it's not your actor, you're not supposed to send the event to the network, so the actorhandler simply eats it.
			//You can be sure this does not affect any other listeners, since actorhandler is registered as the second-to-last, just before network.
			return true;
		}*/		
	}
	else if(EvtData_To_ActorStartMove::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventData);
		std::map<KeyType, Actor*>::iterator actorIt = mActorMap.find(evtData.mId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			if(mPlayersDead[evtData.getNetworkId()])
			{
				Actor *pActor = getActor(evtData.mId);
				if(pActor != NULL && (pActor->getActorType() == Actor::ActorType_Driver || pActor->getActorType() == Actor::ActorType_Tank || pActor->getActorType() == Actor::ActorType_Spectator))
				{
					handleMovingInfo(eventData);
				}
			}
			else
			{
				actorIt->second->handleEventId(eventData);
			}			
		}
	}
	else if(EvtData_To_ActorStopMove::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorStopMove& evtData = static_cast<const EvtData_To_ActorStopMove&>(eventData);
		std::map<KeyType, Actor*>::iterator actorIt = mActorMap.find(evtData.mId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			if(mPlayersDead[evtData.getNetworkId()])
			{
				Actor *pActor = getActor(evtData.mId);
				if(pActor != NULL && (pActor->getActorType() == Actor::ActorType_Driver || pActor->getActorType() == Actor::ActorType_Tank || pActor->getActorType() == Actor::ActorType_Spectator))
				{
					handleMovingInfo(eventData);
				}
			}
			else
			{
				actorIt->second->handleEventId(eventData);
			}
		}
	}
	else if(EvtData_To_ActorStartRotate::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorStartRotate& evtData = static_cast<const EvtData_To_ActorStartRotate&>(eventData);
		actorIt = mActorMap.find(evtData.mId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			if(mPlayersDead[evtData.getNetworkId()])
			{
				Actor *pActor = getActor(evtData.mId);
				if(pActor != NULL && (pActor->getActorType() == Actor::ActorType_Driver || pActor->getActorType() == Actor::ActorType_Tank || pActor->getActorType() == Actor::ActorType_Spectator))
				{
					handleMovingInfo(eventData);
				}
			}
			else
			{
				actorIt->second->handleEventId(eventData);
			}
		}
	}
	else if(EvtData_To_ActorStopRotate::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorStopRotate& evtData = static_cast<const EvtData_To_ActorStopRotate&>(eventData);
		actorIt = mActorMap.find(evtData.mId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			if(mPlayersDead[evtData.getNetworkId()])
			{
				Actor *pActor = getActor(evtData.mId);
				if(pActor != NULL && (pActor->getActorType() == Actor::ActorType_Driver || pActor->getActorType() == Actor::ActorType_Tank || pActor->getActorType() == Actor::ActorType_Spectator))
				{
					handleMovingInfo(eventData);
				}
			}
			else
			{
				actorIt->second->handleEventId(eventData);
			}
		}
	}
	else if(EvtData_To_DriverStrafe::msEventType == eventData.getEventType())
	{
		const EvtData_To_DriverStrafe& evtData = static_cast<const EvtData_To_DriverStrafe&>(eventData);
		std::map<KeyType, Actor*>::iterator actorIt = mActorMap.find(evtData.mId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);	
		}
	}
	else if(EvtData_From_DriverEnterTank::msEventType == eventData.getEventType())
	{
		const EvtData_From_DriverEnterTank& evtData = static_cast<const EvtData_From_DriverEnterTank&>(eventData);
		actorIt = mActorMap.find(evtData.mActorId);
		if(actorIt != mActorMap.end())
		{
			assert(actorIt != mActorMap.end());
			actorIt->second->handleEventId(eventData);
		}
	}
	else if(EvtData_To_ActorCreateTank::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateTank& evtData = static_cast<const EvtData_To_ActorCreateTank&>(eventData);
		CreateTankInfo *pInfo = myNew CreateTankInfo();
		pInfo->mTankType = evtData.mTankType;
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mPlayerMovingInfo = evtData.mPlayerMovingInfo;
		pInfo->mNetworkId = evtData.getNetworkId();
		mCreateActorQueue.add(pInfo);
	}
	else if(EvtData_To_ActorCreateEnemy::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateEnemy& evtData = static_cast<const EvtData_To_ActorCreateEnemy&>(eventData);
		CreateEnemyInfo *pInfo = myNew CreateEnemyInfo();
		pInfo->mEnemyType = evtData.mEnemyType;
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mNetworkId = evtData.getNetworkId();
		mCreateActorQueue.add(pInfo);
	}
	else if(EvtData_To_ActorCreateEnemyID::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateEnemyID& evtData = static_cast<const EvtData_To_ActorCreateEnemyID&>(eventData);
		CreateEnemyInfo *pInfo = myNew CreateEnemyInfo();
		pInfo->mEnemyType = evtData.mEnemyType;
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mNetworkId = evtData.mCreatorID;
		mCreateActorQueue.add(pInfo);
		mpKeyHandler->resetKeyOffset(evtData.mCreatorID);
	}
	else if(EvtData_To_ActorCreateMunition::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateMunition& evtData = static_cast<const EvtData_To_ActorCreateMunition&>(eventData);
		CreateMunitionInfo *pInfo = myNew CreateMunitionInfo();
		pInfo->mMunitionType = evtData.mMunitionType;
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mCreatorId = evtData.mCreatorId;
		pInfo->mNetworkId = evtData.getNetworkId();
		mCreateActorQueue.add(pInfo);
	}
	else if(EvtData_To_ActorCreateDriver::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateDriver& evtData = static_cast<const EvtData_To_ActorCreateDriver&>(eventData);
		CreateDriverInfo *pInfo = myNew CreateDriverInfo();
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mPlayerMovingInfo = evtData.mPlayerMovingInfo;
		pInfo->mNetworkId = evtData.getNetworkId();
		mCreateActorQueue.add(pInfo);
	}
	else if(EvtData_To_ActorCreateSpectator::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateSpectator& evtData = static_cast<const EvtData_To_ActorCreateSpectator&>(eventData);
		CreateSpectatorInfo *pInfo = myNew CreateSpectatorInfo();
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mPlayerMovingInfo = evtData.mPlayerMovingInfo;
		pInfo->mNetworkId = evtData.getNetworkId();
		mCreateActorQueue.add(pInfo);
	}
	else if(EvtData_To_ActorCreateStaticEnvironment::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateStaticEnvironment& evtData = static_cast<const EvtData_To_ActorCreateStaticEnvironment&>(eventData);
		CreateStaticEnvironmentInfo *pInfo = myNew CreateStaticEnvironmentInfo();
		pInfo->mStaticEnvironmentType = evtData.mStaticEnvironmentType;
		pInfo->mScale = evtData.mScale;
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mNetworkId = evtData.getNetworkId();
		mCreateActorQueue.add(pInfo);
	}
	else if(EvtData_To_ActorCreateStaticEnvironmentID::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorCreateStaticEnvironmentID& evtData = static_cast<const EvtData_To_ActorCreateStaticEnvironmentID&>(eventData);
		CreateStaticEnvironmentInfo *pInfo = myNew CreateStaticEnvironmentInfo();
		pInfo->mStaticEnvironmentType = evtData.mStaticEnvironmentType;
		pInfo->mScale = evtData.mScale;
		pInfo->mPos = evtData.mPos;
		pInfo->mDir = evtData.mDir;
		pInfo->mNetworkId = evtData.mCreatorId;
		mCreateActorQueue.add(pInfo);
	}
	else if(EvtData_To_TankChangeWeapon::msEventType == eventData.getEventType())
	{
		const EvtData_To_TankChangeWeapon& evtData = static_cast<const EvtData_To_TankChangeWeapon&>(eventData);
		dynamic_cast<Tank*>(getActor(evtData.mId))->setWeapon(evtData.mWeaponType);
	}
	else if(EvtData_From_ActorHandlerSyncActors::msEventType == eventData.getEventType() && eventData.getNetworkId() != mpPlayerInfo->getNetworkId())
	{
		const EvtData_From_ActorHandlerSyncActors& evtData = static_cast<const EvtData_From_ActorHandlerSyncActors&>(eventData);

		Actor::ActorSyncInfo syncInfo;
		Actor *pActor = NULL;

		for (unsigned int i = 0; i < evtData.mStructSize; i++)
		{
			syncInfo = evtData.mSyncInfo[i];
			pActor = getActor(syncInfo.mId);
			if (pActor != NULL)
			{
				if(pActor->getActorType() == Actor::ActorType_Tank)
				{
					/*utilities::Position pos;
					pos = syncInfo.mPosition + (syncInfo.mDirection * pActor->getSpeed() * 0.5f);
					utilities::Direction dir;
					dir = pos - pActor->getPosition();
					dir.normalize();*/
					pActor->setDirection(syncInfo.mDirection);
					pActor->setPosition(syncInfo.mPosition);
					pActor->setSubsetDirection(syncInfo.mSubsetDirection);
					//pActor->setDestPosition(syncInfo.mPosition);
				}
				else if(pActor->getActorType() == Actor::ActorType_Driver)
				{
					/*utilities::Position pos;
					pos = syncInfo.mPosition + (syncInfo.mDirection * pActor->getSpeed() * 0.5f);
					utilities::Direction dir;
					dir = pos - pActor->getPosition();
					dir.normalize();*/
					pActor->setDirection(syncInfo.mDirection);
					pActor->setPosition(syncInfo.mPosition);
				}
				else
				{
					pActor->setDirection(syncInfo.mDirection);
					pActor->setPosition(syncInfo.mPosition);
					pActor->setScale(syncInfo.mScale);
				}
				/*else if(pActor->getActorType() == Actor::ActorType_Enemy)
				{
					Enemy* pEnemy = dynamic_cast<Enemy*>(pActor);
					Enemy::EnemyTypes enemyType = pEnemy->getEnemyType();
					if(enemyType == Enemy::EnemyType_Ant || enemyType == Enemy::EnemyType_FireAnt || enemyType == Enemy::EnemyType_KamikazeAnt)
					{ 
						utilities::Position pos;
						pos = syncInfo.mPosition + (syncInfo.mDirection * ActorConstants::ANT_MOVE_SPEED * 0.5f);
						utilities::Direction dir;
						dir = pos - pActor->getPosition();
						dir.normalize();
						pActor->setDirection(dir);
						pActor->setDestPosition(syncInfo.mPosition);
					}
					else if(enemyType == Enemy::EnemyType_PlasmaBeetle || enemyType == Enemy::EnemyType_PlasmaBeetle || enemyType == Enemy::EnemyType_Scarab)
					{
						utilities::Position pos;
						pos = syncInfo.mPosition + (syncInfo.mDirection * ActorConstants::BEETLE_MOVE_SPEED * 0.5f);
						utilities::Direction dir;
						dir = pos - pActor->getPosition();
						dir.normalize();
						pActor->setDirection(dir);
						pActor->setDestPosition(syncInfo.mPosition);
					}
					else if(enemyType == Enemy::EnemyType_BlackWidow || enemyType == Enemy::EnemyType_Jumper || enemyType == Enemy::EnemyType_Tarantula)
					{
						utilities::Position pos;
						pos = syncInfo.mPosition + (syncInfo.mDirection * ActorConstants::SPIDER_MOVE_SPEED * 0.5f);
						utilities::Direction dir;
						dir = pos - pActor->getPosition();
						dir.normalize();
						pActor->setDirection(dir);
						pActor->setDestPosition(syncInfo.mPosition);
					}
				}*/
			}
		}
	}
	else if(EvtData_To_ActorReduceHp::msEventType == eventData.getEventType())
	{
		const EvtData_To_ActorReduceHp& evtData = static_cast<const EvtData_To_ActorReduceHp&>(eventData);
		Actor *pActor = getActor(evtData.mId);
		if(pActor != NULL && pActor->getKey().isOwner())
		{
			if(pActor->getActorType() == Actor::ActorType_Enemy)
			{
				dynamic_cast<Enemy*>(pActor)->reduceHp(evtData.mDamage, evtData.getNetworkId(), evtData.mShooter);
			}
			if(pActor->getActorType() == Actor::ActorType_Driver)
			{
				dynamic_cast<Driver*>(pActor)->reduceHp(evtData.mDamage);
			}
			if(pActor->getActorType() == Actor::ActorType_Tank)
			{
				dynamic_cast<Tank*>(pActor)->reduceHp(evtData.mDamage);
			}
			
		}		
		if (evtData.getNetworkId() != mpPlayerInfo->getNetworkId())
		{
			//If it's not your actor, you're not supposed to send the event to the network, so the actorhandler simply eats it.
			//You can be sure this does not affect any other listeners, since actorhandler is registered as the second-to-last, just before network.
			return true;
		}
	}
	else if(EvtData_To_ActorDestroyAll::msEventType == eventData.getEventType())
	{
		destroyAllActors();
	}
	else if(EvtData_To_ActorDestroyAllNonPlayer::msEventType == eventData.getEventType())
	{
		destroyAllActorsNonPlayer();
	}
	else if(EvtData_From_WeaponFireMeshMunition::msEventType == eventData.getEventType())
	{
		const EvtData_From_WeaponFireMeshMunition& evtData = static_cast<const EvtData_From_WeaponFireMeshMunition&>(eventData);
		Munitions *pMunition = dynamic_cast<Munitions*>(getActor(evtData.mId));
		if(pMunition != NULL && !pMunition->getKey().isOwner())
		{
			pMunition->setVisible(true);
			pMunition->setPosition(evtData.mPos);
			pMunition->setDirection(evtData.mDir);
		}		
		if (evtData.getNetworkId() != mpPlayerInfo->getNetworkId())
		{
			//If it's not your actor, you're not supposed to send the event to the network, so the actorhandler simply eats it.
			//You can be sure this does not affect any other listeners, since actorhandler is registered as the second-to-last, just before network.
			return true;
		}
	}
	else if(EvtData_From_WeaponFireRayMunition::msEventType == eventData.getEventType())
	{
		const EvtData_From_WeaponFireRayMunition& evtData = static_cast<const EvtData_From_WeaponFireRayMunition&>(eventData);
		Munitions *pMunition = dynamic_cast<Munitions*>(getActor(evtData.mBulletId));
		Actor *pActor = getActor(evtData.mActorId);
		if(pMunition != NULL && pActor != NULL && !pMunition->getKey().isOwner())
		{
			if (pMunition->getWeaponType() == Weapon::WeaponTypes_Minigun)
			{
				mpDXModule->getParticles("GunSparks")->addEmitter(evtData.mPos.convertToD3DXVECTOR3(),evtData.mDir.convertToD3DXVECTOR3());
				//mpLogicQuadTree->collisionRay(evtData.mPos.convertToD3DXVECTOR3(), evtData.mDir.convertToD3DXVECTOR3(), evtData.mActorId, evtData.mBulletId, evtData.mBeam);
			}
			else if(pMunition->getWeaponType() == Weapon::WeaponTypes_BeamCannon)
			{
				mpDXModule->getParticles("Beam")->addEmitter(evtData.mPos.convertToD3DXVECTOR3(), evtData.mDir.convertToD3DXVECTOR3());
			}
		}		
		if (evtData.getNetworkId() != mpPlayerInfo->getNetworkId())
		{
			//If it's not your actor, you're not supposed to send the event to the network, so the actorhandler simply eats it.
			//You can be sure this does not affect any other listeners, since actorhandler is registered as the second-to-last, just before network.
			return true;
		}
	}
	else if(EvtData_To_PlayerToggleGodMode::msEventType == eventData.getEventType())
	{
		// Toggle god mode in the tank and driver
		logic::KeyType tankId = mpPlayerInfo->getTankActorId();
		if (tankId != INVALID_KEY)
		{
			Tank* pTank = dynamic_cast<Tank*>(getActor(tankId));
			if (pTank != NULL)
			{
				pTank->toggleGodMode();
			}
		}

		logic::KeyType driverId = mpPlayerInfo->getDriverId();
		if (driverId != INVALID_KEY)
		{
			Driver* pDriver = dynamic_cast<Driver*>(getActor(driverId));
			if (pDriver != NULL)
			{
				pDriver->toggleGodMode();
			}
		}
	}
	else if (EvtData_From_EnemySmallExplosion::msEventType == eventData.getEventType())
	{
		const EvtData_From_EnemySmallExplosion& evtData = static_cast<const EvtData_From_EnemySmallExplosion&>(eventData);
		mpDXModule->getParticles("SmallExplosion")->addEmitter(evtData.mPos.convertToD3DXVECTOR3());

		if (evtData.getNetworkId() != mpPlayerInfo->getNetworkId())
		{
			//If it's not your actor, you're not supposed to send the event to the network, so the actorhandler simply eats it.
			//You can be sure this does not affect any other listeners, since actorhandler is registered as the second-to-last, just before network.
			return true;
		}
	}
	return false;
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

DynamicActor* ActorHandler::getDynamicActor(KeyType dynamicActorId)
{
	std::map<KeyType, Actor*>::iterator actorIt = mDynamicActorMap.find(dynamicActorId);
	if (actorIt != mDynamicActorMap.end())
	{
		return dynamic_cast<DynamicActor*>(actorIt->second);
	}
	else
	{
		return NULL;
	}
}

Actor* ActorHandler::getLastAddedActor()
{
	return mpLastAddedActor;
}

void ActorHandler::setDXModuleInstance(gfx::DX10Module *pInstance)
{
	mpDXModule = pInstance;
	mpLight = mpDXModule->getLight();
}

void ActorHandler::setSoundSystemInstance(sound::SoundSystem *pInstance)
{
#ifdef USE_SOUND
	mpSoundSystem = pInstance;
#endif // USE_SOUND
}
void ActorHandler::setLogicQuadTreeInstance(LogicQuadTree * pInstance)
{
	mpLogicQuadTree = pInstance;
}

void ActorHandler::destroyDynamicActors()
{
	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mDynamicActorMap.begin() ; actorIt != mDynamicActorMap.end(); ++actorIt)
	{
		if(actorIt->second->getKey().isOwner())
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(actorIt->second->getKey(), -2)));
			DEBUG_MESSAGE(LEVEL_LOWEST, "destryDynamicActors");
		}
	}
}

void ActorHandler::destroyDynamicActors(int playerID)
{
	std::map<KeyType,Actor*>::iterator actorIt;
	for (actorIt = mDynamicActorMap.begin(); actorIt != mDynamicActorMap.end(); ++actorIt)
	{
		if (actorIt->second->getKey().getOwnerId() == playerID)
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(actorIt->second->getKey(), -2, playerID)));
			DEBUG_MESSAGE(LEVEL_LOWEST, "destryDynamicActors2");
		}
	}
}

void ActorHandler::destroyAllActors()
{
	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mActorMap.begin() ; actorIt != mActorMap.end(); ++actorIt)
	{
		if(actorIt->second->getKey().isOwner())
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(actorIt->second->getKey(), -2)));
			DEBUG_MESSAGE(LEVEL_LOWEST, "destroyallactors");
		}
	}
}

void ActorHandler::destroyAllActorsNonPlayer()
{
	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mActorMap.begin() ; actorIt != mActorMap.end(); ++actorIt)
	{
		if(actorIt->second->getKey().isOwner() && actorIt->second->getKey() != mpPlayerInfo->getPlayerId())
		{
			DEBUG_MESSAGE(LEVEL_LOWEST, "destroyallnonplayer");
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(actorIt->second->getKey(), -2)));
		}
	}
}

void ActorHandler::destroyAllActorsGlobal()
{
	std::map<KeyType,Actor *>::iterator actorIt;
	for(actorIt = mActorMap.begin() ; actorIt != mActorMap.end(); ++actorIt)
	{
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(actorIt->second->getKey(), -2)));
		DEBUG_MESSAGE(LEVEL_LOWEST, "destroyallactorsglobal");
	}
}

void ActorHandler::playerDied(int networkId, Actor::PlayerMovingInfo playerMovingInfo)
{
	mPlayersDead[networkId] = true;
	mPlayersMovingInfo[networkId] = playerMovingInfo;
}

void ActorHandler::handleMovingInfo(EventData const &eventMessage)
{
	int networkId = eventMessage.getNetworkId();
	if(EvtData_To_ActorStartMove::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventMessage);
		mPlayersMovingInfo[networkId].mMoving = true;
		if(evtData.mForward)
			mPlayersMovingInfo[networkId].mMovingDirection += MOVE_FORWARD;
		else
			mPlayersMovingInfo[networkId].mMovingDirection += MOVE_BACKWARDS;

		if(mPlayersMovingInfo[networkId].mMovingDirection == 0)
			mPlayersMovingInfo[networkId].mMoving = false;
	}
	else if(EvtData_To_ActorStopMove::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartMove& evtData = static_cast<const EvtData_To_ActorStartMove&>(eventMessage);
		if(evtData.mForward)
			mPlayersMovingInfo[networkId].mMovingDirection -= MOVE_FORWARD;
		else
			mPlayersMovingInfo[networkId].mMovingDirection -= MOVE_BACKWARDS;

		if(mPlayersMovingInfo[networkId].mMovingDirection == 0)
			mPlayersMovingInfo[networkId].mMoving = false;
		else
			mPlayersMovingInfo[networkId].mMoving = true;
	}
	else if(EvtData_To_ActorStartRotate::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStartRotate& evtData = static_cast<const EvtData_To_ActorStartRotate&>(eventMessage);
		/*if(evtData.mRotateSubset)
		{
			mPlayersMovingInfo[networkId].mSubsetRotating = true;
			if(evtData.mRotateLeft)
				mPlayersMovingInfo[networkId].mSubsetRotateDirection += ROTATE_LEFT;
			else
				mPlayersMovingInfo[networkId].mSubsetRotateDirection += ROTATE_RIGHT;

			if(mPlayersMovingInfo[networkId].mSubsetRotateDirection == 0)
				mPlayersMovingInfo[networkId].mSubsetRotating = false;
		}
		else*/
		if(!evtData.mRotateSubset)
		{
			mPlayersMovingInfo[networkId].mRotating = true;
			if(evtData.mRotateLeft)
				mPlayersMovingInfo[networkId].mRotatingDirection += ROTATE_LEFT;
			else
				mPlayersMovingInfo[networkId].mRotatingDirection += ROTATE_RIGHT;

			if(mPlayersMovingInfo[networkId].mRotatingDirection == 0)
				mPlayersMovingInfo[networkId].mRotating = false;
		}
	}
	else if(EvtData_To_ActorStopRotate::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_ActorStopRotate& evtData = static_cast<const EvtData_To_ActorStopRotate&>(eventMessage);
		/*if(evtData.mRotateSubset)
		{
			if(evtData.mRotateLeft)
				mPlayersMovingInfo[networkId].mSubsetRotateDirection -= ROTATE_LEFT;
			else
				mPlayersMovingInfo[networkId].mSubsetRotateDirection -= ROTATE_RIGHT;

			if(mPlayersMovingInfo[networkId].mSubsetRotateDirection == 0)
				mPlayersMovingInfo[networkId].mSubsetRotating = false;
			else
				mPlayersMovingInfo[networkId].mSubsetRotating = true;
		}
		else*/
		if(!evtData.mRotateSubset)
		{
			if(evtData.mRotateLeft)
				mPlayersMovingInfo[networkId].mRotatingDirection -= ROTATE_LEFT;
			else
				mPlayersMovingInfo[networkId].mRotatingDirection -= ROTATE_RIGHT;

			if(mPlayersMovingInfo[networkId].mRotatingDirection == 0)
				mPlayersMovingInfo[networkId].mRotating = false;
			else
				mPlayersMovingInfo[networkId].mRotating = true;
		}
	}
}

void ActorHandler::addActors()
{
	std::map<KeyType,Actor *>::iterator actorIt;
	while(!mCreateActorQueue.empty())
	{
		CreateActorInfo *pTest;
		pTest = mCreateActorQueue.getFirst();

		if(dynamic_cast<CreateTankInfo*>(mCreateActorQueue.getFirst()) != NULL)
		{
			CreateTankInfo *pInfo = dynamic_cast<CreateTankInfo*>(mCreateActorQueue.getFirst());
			mPlayersDead[pInfo->mNetworkId] = false;
			//Create CloseCombatTank
			if(pInfo->mTankType == Tank::TankType_CloseCombat)
			{
				Tank *pTank = myNew Tank(pInfo->mNetworkId, Tank::TankType_CloseCombat, pInfo->mPos, pInfo->mDir, mPlayersMovingInfo[pInfo->mNetworkId]);
				insertActor(pTank);
				if(pTank->getKey().isOwner())
				{
					mpPlayerInfo->setTankActorId(pTank->getKey());
					mpPlayerInfo->setDriverActorId(INVALID_KEY);
				}
			}
			//Create LongRangeTank
			if(pInfo->mTankType == Tank::TankType_LongRange)
			{
				Tank *pTank = myNew Tank(pInfo->mNetworkId, Tank::TankType_LongRange, pInfo->mPos, pInfo->mDir, mPlayersMovingInfo[pInfo->mNetworkId]);
				insertActor(pTank);
				if(pTank->getKey().isOwner())
				{
					mpPlayerInfo->setTankActorId(pTank->getKey());
					mpPlayerInfo->setDriverActorId(INVALID_KEY);
				}
			}
			SAFE_DELETE(pInfo);
		}
		else if(dynamic_cast<CreateDriverInfo*>(mCreateActorQueue.getFirst()) != NULL)
		{
			CreateDriverInfo *pInfo = dynamic_cast<CreateDriverInfo*>(mCreateActorQueue.getFirst());

			//Create Driver
			mPlayersDead[pInfo->mNetworkId] = false;
			Driver *pDriver = myNew Driver(pInfo->mNetworkId,pInfo->mPos, pInfo->mDir, mPlayersMovingInfo[pInfo->mNetworkId]);
			insertActor(pDriver);
			if(pDriver->getKey().isOwner())
			{
				mpPlayerInfo->setDriverActorId(pDriver->getKey());
				mpPlayerInfo->setTankActorId(INVALID_KEY);
			}
			SAFE_DELETE(pInfo);

		}
		else if(dynamic_cast<CreateSpectatorInfo*>(mCreateActorQueue.getFirst()) != NULL)
		{
			CreateSpectatorInfo *pInfo = dynamic_cast<CreateSpectatorInfo*>(mCreateActorQueue.getFirst());

			//Create Spectator
			mPlayersDead[pInfo->mNetworkId] = false;
			Spectator *pSpectator = myNew Spectator(pInfo->mNetworkId,pInfo->mPos,pInfo->mDir,mPlayersMovingInfo[pInfo->mNetworkId]);
			insertActor(pSpectator);
			if(pSpectator->getKey().isOwner())
			{
				mpPlayerInfo->setSpectatorId(pSpectator->getKey());
				mpPlayerInfo->setTankActorId(INVALID_KEY);
				mpPlayerInfo->setDriverActorId(INVALID_KEY);
			}
			SAFE_DELETE(pInfo);
		}
		else if(dynamic_cast<CreateEnemyInfo*>(mCreateActorQueue.getFirst()) != NULL)
		{
			CreateEnemyInfo *pInfo = dynamic_cast<CreateEnemyInfo*>(mCreateActorQueue.getFirst());

			//Create Ant
			if(pInfo->mEnemyType == Enemy::EnemyType_Ant)
			{
				Enemy *pEnemy = myNew Ant(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}	

			//Create KamikazeaAnt
			if(pInfo->mEnemyType == Enemy::EnemyType_KamikazeAnt)
			{
				Enemy *pEnemy = myNew KamikazeAnt(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}
			//Create FireAnt
			if(pInfo->mEnemyType == Enemy::EnemyType_FireAnt)
			{
				Enemy *pEnemy = myNew FireAnt(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}
			//Create Tarantula
			if(pInfo->mEnemyType == Enemy::EnemyType_Tarantula)
			{
				Enemy *pEnemy = myNew Tarantula(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}
			//Create BlackWidow
			if(pInfo->mEnemyType == Enemy::EnemyType_BlackWidow)
			{
				Enemy *pEnemy = myNew BlackWidow(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}
			//Create Jumper
			if(pInfo->mEnemyType == Enemy::EnemyType_Jumper)
			{
				Enemy *pEnemy = myNew Jumper(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}
			//Create Scarab
			if(pInfo->mEnemyType == Enemy::EnemyType_Scarab)
			{
				Enemy *pEnemy = myNew Scarab(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}
			//Create PlasmaBeetle
			if(pInfo->mEnemyType == Enemy::EnemyType_PlasmaBeetle)
			{
				Enemy *pEnemy = myNew PlasmaBeetle(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir);
				insertActor(pEnemy);
			}
			SAFE_DELETE(pInfo)

		}
		else if(dynamic_cast<CreateMunitionInfo*>(mCreateActorQueue.getFirst()) != NULL)
		{
			CreateMunitionInfo *pInfo = dynamic_cast<CreateMunitionInfo*>(mCreateActorQueue.getFirst());

			//Create Acid
			if(pInfo->mMunitionType == Munitions::MunitionType_Acid)
			{
				Actor *pActor = getActor(pInfo->mCreatorId);
				actorIt = mActorMap.find(pInfo->mCreatorId);
				if(pActor != NULL && actorIt != mActorMap.end() && pActor->getActorSubType() == Enemy::EnemyType_BlackWidow)
				{
					EnemyWeapon *pEnemyWeapon = dynamic_cast<Enemy*>(pActor)->getEnemyWeapon();
					Munitions *pMunition = myNew Acid(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir, 0, pEnemyWeapon->getDamage());
					pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

					/*utilities::Position pos = pActor->getPosition();
					utilities::Direction dir = getActor(mpPlayerInfo->getPlayerId())->getPosition() - pos;
					dir.normalize();

					pMunition->setPosition(pos.x, pos.y, pos.z);
					pMunition->setDirection(dir.x, dir.y, dir.z);*/

					pEnemyWeapon->addBullet(pMunition->getKey());
					insertActor(pMunition);
				}
			}
			//Create Beam
			if(pInfo->mMunitionType == Munitions::MunitionType_Beam)
			{
				Actor *pActor = getActor(pInfo->mCreatorId);
				actorIt = mActorMap.find(pInfo->mCreatorId);
				if(pActor != NULL && actorIt != mActorMap.end())
				{
					Weapon *pWeapon = dynamic_cast<Tank*>(pActor)->getWeapon();
					Munitions *pMunition = myNew Beam(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir, pWeapon->getEfficiency(), pWeapon->getDamage());
					pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

					utilities::Position pos = pActor->getPosition();
					utilities::Direction dir = pActor->getSubsetDirection();

					pMunition->setPosition(pos.x, pos.y, pos.z);
					pMunition->setDirection(dir.x, dir.y, dir.z);

					pMunition->setWeaponType(Weapon::WeaponTypes_BeamCannon);

					mpDXModule->getParticles("Beam")->addEmitter(D3DXVECTOR3(pos.x + dir.x * 35.0f, pos.y + 5.0f,pos.z + dir.z * 35.0f),dir.convertToD3DXVECTOR3());
					mpLogicQuadTree->collisionRay(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(dir.x, dir.y, dir.z), pInfo->mCreatorId, pMunition->getKey(), true);

					pWeapon->addBullet(pMunition->getKey());
					insertActor(pMunition);
				}
			}
			//Create Bullet
			if(pInfo->mMunitionType == Munitions::MunitionType_Bullet)
			{
				Actor *pActor = getActor(pInfo->mCreatorId);
				actorIt = mActorMap.find(pInfo->mCreatorId);
				if(pActor != NULL && actorIt != mActorMap.end())
				{
					if(pActor->getActorType() == Actor::ActorType_Tank)
					{
						Weapon *pWeapon = dynamic_cast<Tank*>(pActor)->getWeapon();
						Munitions *pMunition = myNew Bullet(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir, pWeapon->getEfficiency(), pWeapon->getDamage(), pWeapon->getWeaponType());
						pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

						utilities::Position pos = pActor->getPosition();
						utilities::Direction dir = pActor->getSubsetDirection();

						pMunition->setPosition(pos.x + dir.x * 35.0f,pos.y + 5.0f,pos.z + dir.z * 35.0f);
						pMunition->setDirection(dir.x, dir.y, dir.z);


						pMunition->setWeaponType(pWeapon->getWeaponType());
						if(pWeapon->getWeaponType() == Weapon::WeaponTypes_Minigun)
						{
							mpLogicQuadTree->collisionRay(D3DXVECTOR3(pos.x, pos.y, pos.z), D3DXVECTOR3(dir.x, dir.y, dir.z), pInfo->mCreatorId, pMunition->getKey(), false); 
						}
						pWeapon->addBullet(pMunition->getKey()); 
						insertActor(pMunition);
					}
					else
					{
						Weapon *pWeapon = dynamic_cast<Driver*>(pActor)->getWeapon();
						Munitions *pMunition = myNew Bullet(pInfo->mNetworkId,pInfo->mPos, pInfo->mDir, pWeapon->getEfficiency(), pWeapon->getDamage(), pWeapon->getWeaponType());
						pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

						utilities::Position pos = pActor->getPosition();
						utilities::Direction dir = pActor->getDirection();

						pMunition->setPosition(pos.x + dir.x * 5.0f,pos.y + 5.0f,pos.z + dir.z * 5.0f);
						pMunition->setDirection(dir.x, dir.y, dir.z);

						pMunition->setWeaponType(Weapon::WeaponTypes_Shotgun);

						pWeapon->addBullet(pMunition->getKey());
						insertActor(pMunition);
					}
				}
			}
			//Create Fire
			if(pInfo->mMunitionType == Munitions::MunitionType_Fire)
			{
				Actor *pActor = getActor(pInfo->mCreatorId);
				actorIt = mActorMap.find(pInfo->mCreatorId);
				if(pActor != NULL && actorIt != mActorMap.end() && pActor->getActorSubType() == Enemy::EnemyType_FireAnt)
				{
					EnemyWeapon *pEnemyWeapon = dynamic_cast<Enemy*>(pActor)->getEnemyWeapon();
					Munitions *pMunition = myNew Fire(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir, 0, pEnemyWeapon->getDamage());
					pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

					/*utilities::Position pos = pActor->getPosition();
					utilities::Direction dir = getActor(mpPlayerInfo->getPlayerId())->getPosition() - pos;
					dir.normalize();

					pMunition->setPosition(pos.x, pos.y, pos.z);
					pMunition->setDirection(dir.x, dir.y, dir.z);*/

					pEnemyWeapon->addBullet(pMunition->getKey());
					insertActor(pMunition);
				}
			}
			//Create Flame
			if(pInfo->mMunitionType == Munitions::MunitionType_Flame)
			{
				Actor *pActor = getActor(pInfo->mCreatorId);
				actorIt = mActorMap.find(pInfo->mCreatorId);
				if(pActor != NULL && actorIt != mActorMap.end())
				{
					Weapon *pWeapon = dynamic_cast<Tank*>(pActor)->getWeapon();
					Munitions *pMunition = myNew Flame(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir, pWeapon->getEfficiency(), pWeapon->getDamage());
					pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

					utilities::Position pos = pActor->getPosition();
					utilities::Direction dir = pActor->getSubsetDirection();

					pMunition->setPosition(pos.x, pos.y, pos.z);
					pMunition->setDirection(dir.x, dir.y, dir.z);

					pMunition->setWeaponType(Weapon::WeaponTypes_Flamethrower);

					pWeapon->addBullet(pMunition->getKey());
					insertActor(pMunition);
				}
			}
			//Create Plasma
			if(pInfo->mMunitionType == Munitions::MunitionType_Plasma)
			{
				Actor *pActor = getActor(pInfo->mCreatorId);
				actorIt = mActorMap.find(pInfo->mCreatorId);
				if(pActor != NULL && actorIt != mActorMap.end() && pActor->getActorSubType() == Enemy::EnemyType_PlasmaBeetle)
				{
					EnemyWeapon *pEnemyWeapon = dynamic_cast<Enemy*>(pActor)->getEnemyWeapon();
					Munitions *pMunition = myNew Plasma(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir, 0, pEnemyWeapon->getDamage());
					pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

					/*utilities::Position pos = pActor->getPosition();
					utilities::Direction dir = getActor(mpPlayerInfo->getPlayerId())->getPosition() - pos;
					dir.normalize();

					pMunition->setPosition(pos.x, pos.y, pos.z);
					pMunition->setDirection(dir.x, dir.y, dir.z);*/

					pEnemyWeapon->addBullet(pMunition->getKey());
					insertActor(pMunition);
				}
			}
			//Create Pulse
			if(pInfo->mMunitionType == Munitions::MunitionType_Pulse)
			{
				Actor *pActor = getActor(pInfo->mCreatorId);
				actorIt = mActorMap.find(pInfo->mCreatorId);
				if(pActor != NULL && actorIt != mActorMap.end())
				{
					Weapon *pWeapon = dynamic_cast<Tank*>(pActor)->getWeapon();
					Munitions *pMunition = myNew Pulse(pInfo->mNetworkId, pInfo->mPos, pInfo->mDir, pWeapon->getEfficiency(), pWeapon->getDamage());
					pMunition->setOwner(pInfo->mCreatorId, pActor->getActorType());

					utilities::Position pos = pActor->getPosition();
					utilities::Direction dir = pActor->getSubsetDirection();

					pMunition->setPosition(pos.x + dir.x * 35.0f,pos.y + 5.0f,pos.z + dir.z * 35.0f);
					pMunition->setDirection(dir.x, dir.y, dir.z);
					mpDXModule->getParticles("Pulse")->addEmitter(D3DXVECTOR3(pos.x + dir.x * 35.0f, pos.y + 5.0f,pos.z + dir.z * 35.0f),dir.convertToD3DXVECTOR3());
					pMunition->setWeaponType(Weapon::WeaponTypes_PulseCannon);

					pWeapon->addBullet(pMunition->getKey());
					insertActor(pMunition);
				}
			}
			SAFE_DELETE(pInfo);
		}
		else if(dynamic_cast<CreateStaticEnvironmentInfo*>(mCreateActorQueue.getFirst()) != NULL)
		{
			CreateStaticEnvironmentInfo *pInfo = dynamic_cast<CreateStaticEnvironmentInfo*>(mCreateActorQueue.getFirst());

			StaticEnvironment *pStaticEnvironment = myNew StaticEnvironment(pInfo->mNetworkId, pInfo->mStaticEnvironmentType, pInfo->mPos, pInfo->mDir, pInfo->mScale);
			insertActor(pStaticEnvironment);
			SAFE_DELETE(pInfo);
		}
		else
		{
			ERROR_MESSAGE("Could not cast CreateActorInfo to a derived class!");
		}
		mCreateActorQueue.removeFirst();
	}
}

void ActorHandler::removeActors()
{
	while(!mDestroyedActors.empty())
	{
		mpDXModule->removeRenderObject(mDestroyedActors.getFirst().mId.convertToGfxId());
		removeActor(mDestroyedActors.getFirst().mId, mDestroyedActors.getFirst().mNetworkId);

		mDestroyedActors.removeFirst();
	}
}

void ActorHandler::removeActorsGlobal()
{
	while(!mDestroyedActors.empty())
	{
		mpDXModule->removeRenderObject(mDestroyedActors.getFirst().mId.convertToGfxId());
		removeActorGlobal(mDestroyedActors.getFirst().mId, mDestroyedActors.getFirst().mNetworkId);

		mDestroyedActors.removeFirst();
	}
}

void ActorHandler::createEnemy(Enemy::EnemyTypes type, const utilities::Vec3Float &position, const utilities::Vec3Float &direction)
{
	int creatorID = mpKeyHandler->getNetworkIDWithMostFreeKeys();
	if(creatorID != -1)
	{
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemyID(type, position, direction, creatorID)));
	}
}

void ActorHandler::createEnemyNonID(Enemy::EnemyTypes type, const utilities::Vec3Float &position, const utilities::Vec3Float &direction)
{
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(type, position, direction)));
}

void ActorHandler::createStaticEnvironment(StaticEnvironment::StaticEnvironmentTypes type, const utilities::Vec3Float &position, const utilities::Vec3Float &direction, const utilities::Vec3Float &scale)
{
	int creatorID = mpKeyHandler->getNetworkIDWithMostFreeKeys();
	if(creatorID != -1)
	{
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironmentID(type, position, direction, scale, creatorID)));
	}
}

void ActorHandler::createStaticEnvironmentNonID(StaticEnvironment::StaticEnvironmentTypes type, const utilities::Vec3Float &position, const utilities::Vec3Float &direction, const utilities::Vec3Float &scale)
{
	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(type, position, direction, scale)));
}

int ActorHandler::getActiveEnemyCount()
{
	int enemyCount = 0;
	for(std::map<KeyType, Actor *>::iterator mapIter = mDynamicActorMap.begin();
		mapIter != mDynamicActorMap.end(); ++mapIter)
	{
		//Check for enemies.
		if((*mapIter).second->getActorType() == Actor::ActorType_Enemy)
		{
			++enemyCount;
		}
	}
	return enemyCount;
}

int ActorHandler::getHiddenEnemyCount()
{
	return TriggerHandler::getInstance()->getHiddenEnemyCount();
}

int ActorHandler::getTotalEnemyCount()
{
	int totalEnemyCount = 0;
	totalEnemyCount += getActiveEnemyCount();
	totalEnemyCount += getHiddenEnemyCount();
	return totalEnemyCount;
}

int ActorHandler::getTotalSpectatorCount()
{
	int spectatorCount = 0;
	for(std::map<KeyType, Actor *>::iterator actorIt = mDynamicActorMap.begin() ; actorIt != mDynamicActorMap.end(); ++actorIt)
	{
		if((*actorIt).second->getActorType() == Actor::ActorType_Spectator)
		{
			spectatorCount++;
		}
	}
	return spectatorCount;	
}

void ActorHandler::detectMapCompletion(float deltaTime)
{
	//Ignore if in editor mode.
	if(Application::getInstance()->mpGameOptions->mEditorMode || !Application::getInstance()->mpGameOptions->mIsServer)
	{
		return;
	}

	int nrOfPlayers = Application::getInstance()->mpGameOptions->mNumberOfPlayers;
	int nrOfSpectators = getTotalSpectatorCount();
	if(nrOfPlayers < nrOfSpectators)
	{
		if(!mFlagMapDefeatTimerStart)
		{
			mFlagMapDefeatTimerStart = true;
			mMapDefeatTimer = 0.0f;
		}
	}
	else if(mFlagMapDefeatTimerStart)
	{
		mFlagMapDefeatTimerStart = false;
	}

	if(mFlagMapDefeatTimerStart && !mFlagMapDefeated)
	{
		mMapDefeatTimer += deltaTime;
		if(mMapDefeatTimer > mMapDefeatMaxTimer)
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_System_MapDefeat()));
		}
	}
	
	int enemyCount = getTotalEnemyCount();

	//Check if all enemies are destroyed.
	if(enemyCount == 0)
	{
		//Start the map completion timer.
		if(!mFlagMapCompleteTimerStart)
		{
			mFlagMapCompleteTimerStart = true;
			mMapCompleteTimer = 0.0f;
		}
	}
	//New enemies have been created - stop the completion timer.
	else if(mFlagMapCompleteTimerStart)
	{
		mFlagMapCompleteTimerStart = false;
	}

	//Update the timer.
	if(mFlagMapCompleteTimerStart && !mFlagMapCompleted)
	{
		mMapCompleteTimer += deltaTime;
		if(mMapCompleteTimer > mMapCompleteMaxTime)
		{
			//End the map.
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_System_MapVictory()));
		}
	}
}

void ActorHandler::resetActorHandler()
{
	destroyAllActorsGlobal();
	mFlagMapCompleted = false;
	mFlagMapVictory = false;

	mFlagMapDefeated = false;
	mFlagMapLose = false;

	mMapDefeatTimer = 0.0f;
	mMapCompleteTimer = 0.0f;
}