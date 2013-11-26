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

#include "AcidSpit.h"

using namespace logic;

AcidSpit::AcidSpit(float damage, float weaponCooldown, float range, const KeyType& ownerId):
		EnemyWeapon(damage, weaponCooldown, range, ownerId)
{
	
	for(int i = 0; i < 2;i++)
	{
		mBulletKeys[i] = INVALID_KEY;
	}
}

AcidSpit::~AcidSpit()
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

void AcidSpit::fireWeapon(const KeyType& targetId)
{
	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	Actor* pOwner = mpActorHandler->getDynamicActor(mOwner);
	Actor* pTarget = mpActorHandler->getDynamicActor(targetId);
	
	//check if cooldownCounter is higher than weaponCooldown
    if(pOwner != NULL && pTarget != NULL && mCooldownCounter > EnemyWeapon::getCooldown())
    {
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyWeaponFired(mOwner)));
		mBulletCounter++;
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		utilities::Position position = pOwner->getPosition();
		utilities::Direction aimDir = pTarget->getPosition() - position;
		aimDir.normalize();

	   if(mBullets <= 1)
	   {     
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Acid, position, aimDir, mOwner)));		
	   }
	
		if(mBulletKeys[mBulletCounter] != INVALID_KEY)
		{  
		   Actor *pBullet = mpActorHandler->getActor(mBulletKeys[mBulletCounter]);
		   pBullet->setDirection(aimDir.x,aimDir.y,aimDir.z);
		   pBullet->setPosition(position.x,position.y,position.z);
		   dynamic_cast<Munitions*>(pBullet)->setVisible(true);
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_WeaponFireMeshMunition(mBulletKeys[mBulletCounter], position, aimDir)));
		}

		if(mBulletCounter == 1)
			mBulletCounter = -1;
	}

}

void AcidSpit::addBullet(KeyType bulletId)
{
	mBulletKeys[mBullets] = bulletId;
	mBullets++;
}