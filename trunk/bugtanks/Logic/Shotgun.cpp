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
 * This is a subclass to weapon, the shotgun is the last resort if you get thrown out of your tank
 */

#include "Shotgun.h"
#include "ActorHandler.h"
#include "Bullet.h"

using namespace logic;



Shotgun::Shotgun() :
Weapon(ActorConstants::SHOTGUN_DAMAGE, ActorConstants::SHOTGUN_COOLDOWN, 1)
{
	mWeaponType = Weapon::WeaponTypes_Shotgun;
	for(int i = 0; i < 2;i++)
	{
		mBulletKeys[i] = INVALID_KEY;
	}
}


Shotgun::~Shotgun()
{
	for(int i = 0; i < 2;i++)
	{
		if(mBulletKeys[i] != INVALID_KEY && mBulletKeys[i].isOwner())
		{
			Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKeys[i]));
			if(pMunition != NULL)
			{
				pMunition->startDestroyTimer();
			}
		}
		
	}
}

void Shotgun::fireWeapon(KeyType ownerId)
{
	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
  if(mCooldownCounter > Weapon::getCooldown())
  {
	  safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFired(ownerId)));
	  mBulletCounter++;	
	  mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
	  utilities::Position position = mpActorHandler->getActor(ownerId)->getPosition();
	  utilities::Direction direction = mpActorHandler->getActor(ownerId)->getDirection();
	  //do not create more than specified objects	
	  if(mBullets <= 1)
	  {      
		  safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Bullet, position, direction, ownerId)));
	  }	 
	

		if(mBulletKeys[mBulletCounter] != INVALID_KEY)
		{
			position = position + utilities::Position(direction.x * 5.0f, 5.0f, direction.z * 5.0f);
		   Actor *pActor = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKeys[mBulletCounter]));
		   pActor->setDirection(direction.x,direction.y,direction.z);
		   pActor->setPosition(position.x ,position.y ,position.z );
		   dynamic_cast<Munitions*>(pActor)->setVisible(true);
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFireMeshMunition(mBulletKeys[mBulletCounter], position, direction)));
		}
		if(mBulletCounter == 1)
			mBulletCounter = -1;
  }
}

void Shotgun::addBullet(KeyType bulletId)
{
	mBulletKeys[mBullets] = bulletId;
	mBullets++;
}