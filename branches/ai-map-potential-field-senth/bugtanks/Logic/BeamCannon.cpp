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
 * @section DESCRIPTION
 *
 * This is a subclass to weapon, Beam Cannon is the longrange tanks third weapon
 */

#include "BeamCannon.h"
#include "ActorHandler.h"

using namespace logic;


BeamCannon::BeamCannon() :
Weapon(ActorConstants::BEAMCANNON_DAMAGE, ActorConstants::BEAMCANNON_COOLDOWN, 3)
{
	mWeaponType = Weapon::WeaponTypes_BeamCannon;
	mpBeam = NULL;
	for(int i = 0; i < 2;i++)
	{
		mBulletKeys[i] = INVALID_KEY;
	}
}


BeamCannon::~BeamCannon()
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

void BeamCannon::fireWeapon(KeyType ownerId)
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
		   safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Beam, position, direction, ownerId)));
	   }	 

	   D3DXVECTOR3 origin;
	   origin.x = position.x;
	   origin.y = position.y;
	   origin.z = position.z;
	   D3DXVECTOR3 dir;
	   dir.x = direction.x;
       dir.y = direction.y;
	   dir.z = direction.z;

	   if(mBulletKeys[mBulletCounter] != INVALID_KEY)
	   {
		   mpBeam = dynamic_cast<Beam*>(mpActorHandler->getActor(mBulletKeys[mBulletCounter]));
		   mpBeam->setDirection(direction.x,direction.y,direction.z);
		   mpBeam->setPosition(position.x + direction.x * 35.0f,position.y + 5.0f,position.z + direction.z * 35.0f);
		   mpLogicQuadTree->collisionRay(origin, dir, PlayerInfo::getInstance()->getPlayerId(), mpBeam->getKey(), true); 
	   }
	   if(mBulletCounter == 1)
	   {
		   mBulletCounter = -1;
	   }
	}
}

void BeamCannon::addBullet(KeyType bulletId)
{
	mBulletKeys[mBullets] = bulletId;
	mBullets++;
}