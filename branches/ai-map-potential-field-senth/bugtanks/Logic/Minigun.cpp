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
 * This is a subclass to weapon, minigun is the closecombat tanks basic weapon
 */

#include "Minigun.h"
#include "Bullet.h"
#include "ActorHandler.h"

using namespace logic;


Minigun::Minigun() :
Weapon(ActorConstants::MINIGUN_DAMAGE, ActorConstants::MINIGUN_COOLDOWN, 1)
{
	mWeaponType = Weapon::WeaponTypes_Minigun;
	mBulletKey = INVALID_KEY;
}

Minigun::~Minigun()
{
	if(mBulletKey != INVALID_KEY)
	{
		Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKey));
		if(pMunition != NULL)
		{
			pMunition->startDestroyTimer();
		}
	}
}


void Minigun::fireWeapon(KeyType ownerId)
{
	//check if cooldownCounter is higher than weaponCooldown
	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	if(mCooldownCounter > Weapon::getCooldown())
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFired(ownerId)));
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		
		utilities::Position position = mpActorHandler->getActor(mpPlayerInfo->getPlayerId())->getPosition();
		utilities::Direction direction = mpActorHandler->getActor(mpPlayerInfo->getPlayerId())->getSubsetDirection();
		
		//do not create more than specified objects	
		if(mBulletKey == INVALID_KEY)
		{  
			safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Bullet, position, direction, ownerId)));
		}

		D3DXVECTOR3 origin = position.convertToD3DXVECTOR3();
		D3DXVECTOR3 dir = direction.convertToD3DXVECTOR3();

		if(mBulletKey != INVALID_KEY)
		{
			Actor *pActor = mpActorHandler->getActor(mBulletKey);
			mpLogicQuadTree->collisionRay(origin, dir, PlayerInfo::getInstance()->getPlayerId(), pActor->getKey(), false);
		}		
	}
}


void Minigun::addBullet(KeyType bulletId)
{
	mBulletKey = bulletId;
}