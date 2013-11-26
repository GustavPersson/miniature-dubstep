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
	mBulletKey = INVALID_KEY;
}


BeamCannon::~BeamCannon()
{
	for(int i = 0; i < 2;i++)
	{
		if(mBulletKey != INVALID_KEY && mBulletKey.isOwner())
		{
			Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKey));
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
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFired(ownerId)));	
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		
		utilities::Position position = mpActorHandler->getActor(ownerId)->getPosition();
		utilities::Direction direction = mpActorHandler->getActor(ownerId)->getSubsetDirection();

	   //do not create more than specified objects	
	   if(mBulletKey == INVALID_KEY)
	   {
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Beam, position, direction, ownerId)));
	   }	 

	   position.x = position.x + direction.x * 30.0f;
	   position.y = position.y;
	   position.z = position.z + direction.z * 30.0f;
	   D3DXVECTOR3 origin = position.convertToD3DXVECTOR3();
	   D3DXVECTOR3 dir = direction.convertToD3DXVECTOR3();

	   if(mBulletKey != INVALID_KEY)
	   {
		   Actor *pActor = mpActorHandler->getActor(mBulletKey);
		   mpDX10Module->getParticles("Beam")->addEmitter(D3DXVECTOR3(position.x + direction.x * 35.0f, position.y + 5.0f, position.z + direction.z *35.0f),direction.convertToD3DXVECTOR3());
		   mpLogicQuadTree->collisionRay(origin, dir, PlayerInfo::getInstance()->getPlayerId(), pActor->getKey(), true); 
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFireRayMunition(position, direction,PlayerInfo::getInstance()->getPlayerId(), pActor->getKey(), true)));
	   }
	}
}

void BeamCannon::addBullet(KeyType bulletId)
{
	mBulletKey = bulletId;
	mBullets++;
}