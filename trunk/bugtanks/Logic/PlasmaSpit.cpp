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

#include "PlasmaSpit.h"

using namespace logic;

PlasmaSpit::PlasmaSpit(float damage, float weaponCooldown, float range, const KeyType& ownerId):
		EnemyWeapon(damage, weaponCooldown, range, ownerId)
{
		mBulletKey = INVALID_KEY;
}

PlasmaSpit::~PlasmaSpit()
{
	if(mBulletKey != INVALID_KEY && mBulletKey.isOwner())
	{
		Munitions *pMunition = dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKey));
		if(pMunition != NULL)
		{	
			pMunition->setVisible(false);
			pMunition->startDestroyTimer();
		}
	}
}

void PlasmaSpit::fireWeapon(const KeyType& targetId)
{
	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	Actor* pOwner = mpActorHandler->getDynamicActor(mOwner);
	Actor* pTarget = mpActorHandler->getDynamicActor(targetId);
	
	//check if cooldownCounter is higher than weaponCooldown
    if(mCooldownCounter > EnemyWeapon::getCooldown())
    {
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyWeaponFired(mOwner)));
		mBulletCounter++;
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		utilities::Position position = pOwner->getPosition();
		utilities::Direction aimDir = pTarget->getPosition() - position;
		aimDir.normalize();

	   if(mBullets <= 0)
	   {      
		   safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Plasma, position, aimDir, mOwner)));
	   }

	   if(mBulletKey != INVALID_KEY)
	   {
		   Actor *pBullet = mpActorHandler->getActor(mBulletKey);
		   pBullet->setDirection(aimDir.x,aimDir.y,aimDir.z);
		   pBullet->setPosition(position.x + aimDir.x * 35.0f,position.y + 5.0f,position.z + aimDir.z * 35.0f);
		   dynamic_cast<Munitions*>(pBullet)->setVisible(true);
	   }

		if(mBulletCounter == 0)
			mBulletCounter = -1;
	}

}

void PlasmaSpit::addBullet(KeyType bulletId)
{
	mBulletKey = bulletId;
	mBullets++;
}