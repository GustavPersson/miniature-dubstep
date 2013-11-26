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
 * This is a subclass to weapon, flamethrower is the closecombat tanks second weapon
 */

#include "Flamethrower.h"
#include "ActorHandler.h"

using namespace logic;


Flamethrower::Flamethrower() :
Weapon(ActorConstants::FLAMETHROWER_DAMAGE, ActorConstants::FLAMETHROWER_COOLDOWN, 1)
{
	mWeaponType = Weapon::WeaponTypes_Flamethrower;
	mBulletKey = INVALID_KEY;

}


Flamethrower::~Flamethrower()
{
	if(mBulletKey != INVALID_KEY && mBulletKey.isOwner())
	{
		Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKey));
		if(pMunition != NULL)
		{
			pMunition->startDestroyTimer();
			pMunition->setVisible(false);
		}
	}		
}


void Flamethrower::fireWeapon(KeyType ownerId)
{
  
	 mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	//check if cooldownCounter is higher than weaponCooldown
    if(mCooldownCounter > Weapon::getCooldown())
    {
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFired(ownerId)));
		mBulletCounter++;	
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		utilities::Position position = mpActorHandler->getActor(ownerId)->getPosition();
		utilities::Direction direction = mpActorHandler->getActor(ownerId)->getSubsetDirection();

	   //do not create more than specified objects	
	   if(mBullets <= 0)
	   {
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Flame, position, direction, ownerId)));
	   }	 

	   if(mBulletKey != INVALID_KEY)
	   {
		   Actor *pActor = mpActorHandler->getActor(mBulletKey);
		   if(pActor != NULL)
		   {
			   pActor->setDirection(direction.x,direction.y,direction.z);
			   pActor->setPosition(position.x + direction.x * 35.0f,position.y + 5.0f,position.z + direction.z * 35.0f);
			   dynamic_cast<Munitions*>(pActor)->setVisible(true);
		   }
	   }
	   if(mBulletCounter == 0)
	   {
		   mBulletCounter = -1;
	   }
	}
  
}

void Flamethrower::addBullet(KeyType bulletId)
{
	mBulletKey = bulletId;
	mBullets++;
}
