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
 * This is a subclass to weapon, the pulse cannon is the closecombat tanks third weapon
 */

#include "PulseCannon.h"
#include "ActorHandler.h"
#include "../gfx/Particle.h"

using namespace logic;



PulseCannon::PulseCannon() :
Weapon(ActorConstants::PULSECANNON_DAMAGE, ActorConstants::PULSECANNON_COOLDOWN, 1)
{
	mWeaponType = Weapon::WeaponTypes_PulseCannon;
	for(int i = 0; i < 2;i++)
	{
		mBulletKeys[i] = INVALID_KEY;
	}
}


PulseCannon::~PulseCannon()
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

void PulseCannon::fireWeapon(KeyType ownerId)
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
	   if(mBullets <= 1)
	   {
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Pulse, position, direction, ownerId)));
	   }	 

	   if(mBulletKeys[mBulletCounter] != INVALID_KEY)
	   {
		   
		   Actor *pActor = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKeys[mBulletCounter]));
		   pActor->setDirection(direction.x,direction.y,direction.z);
		   pActor->setPosition(position.x + direction.x * 35.0f,position.y + 5.0f,position.z + direction.z * 35.0f);
		   mpDX10Module->getParticles("Pulse")->addEmitter(D3DXVECTOR3(position.x + direction.x * 35.0f, position.y + 5.0f,position.z + direction.z * 35.0f),direction.convertToD3DXVECTOR3());
		   dynamic_cast<Munitions*>(pActor)->setVisible(true);
	   }
	   if(mBulletCounter == 1)
	   {
		   mBulletCounter = -1;
	   }
	}		
}

void PulseCannon::addBullet(KeyType bulletId)
{
	mBulletKeys[mBullets] = bulletId;
	mBullets++;
}