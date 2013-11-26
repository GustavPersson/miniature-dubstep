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
 * This is a subclass to weapon, gauss cannon is the longrange tanks second weapon
 */

#include "GaussCannon.h"
#include "Bullet.h"
#include "ActorHandler.h"

using namespace logic;


GaussCannon::GaussCannon() :
Weapon(ActorConstants::GAUSSCANNON_DAMAGE, ActorConstants::GAUSSCANNON_COOLDOWN, 3)
{
	mWeaponType = Weapon::WeaponTypes_GaussCannon;
	for(int i = 0; i < 2;i++)
	{
		mBulletKeys[i] = INVALID_KEY;
	}
}


GaussCannon::~GaussCannon()
{
	for(int i = 0; i < 2;i++)
	{
		if(mBulletKeys[i] != INVALID_KEY)
		{
			Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKeys[i]));
			if(pMunition != NULL)
			{
				pMunition->startDestroyTimer();
			}
		}
		
	}
}


void GaussCannon::fireWeapon(KeyType ownerId)
{

	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	//check if cooldownCounter is higher than weaponCooldown
    if(mCooldownCounter > Weapon::getCooldown())
    {
	     safeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFired(ownerId)));
		 mBulletCounter++;	
		 mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		 utilities::Position position = mpActorHandler->getActor(ownerId)->getPosition();
		 utilities::Direction direction = mpActorHandler->getActor(ownerId)->getSubsetDirection();
		//do not create more than specified objects
		if(mBullets <= 1)
		{
			safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Bullet, position, direction, ownerId)));
		}
	   if(mBulletKeys[mBulletCounter] != INVALID_KEY)
	   {
		   Actor *pActor = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKeys[mBulletCounter]));
		   pActor->setDirection(direction.x,direction.y,direction.z);
		   pActor->setPosition(position.x + direction.x * 35.0f,position.y + 5.0f,position.z + direction.z * 35.0f);
		   dynamic_cast<Munitions*>(pActor)->setVisible(true);
	   }
		if(mBulletCounter == 1)
	   {
		   mBulletCounter = -1;
	   }

		//LogicQuadTree::getInstance()->collisionRay(origin, dir, PlayerInfo::getInstance()->getPlayerId(), mpBullet->getKey()); 
	}

	 
}

void GaussCannon::addBullet(KeyType bulletId)
{
	mBulletKeys[mBullets] = bulletId;
	mBullets++;
}