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

#include "FireSpit.h"

using namespace logic;

FireSpit::FireSpit(float damage, float weaponCooldown, float range):
		EnemyWeapon(damage, weaponCooldown, range)
{
	
	for(int i = 0; i < 2;i++)
	{
		mBulletKeys[i] = INVALID_KEY;
	}
}

FireSpit::~FireSpit()
{
	for(int i = 0; i < 2;i++)
	{
		if(mBulletKeys[i] != INVALID_KEY)
		{
			dynamic_cast<Munitions*>(mpActorHandler->getActor(mBulletKeys[i]))->startDestroyTimer();
		}
		
	}
}

void FireSpit::fireWeapon(KeyType ownerId)
{
	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	//check if cooldownCounter is higher than weaponCooldown
    if(mCooldownCounter > EnemyWeapon::getCooldown())
    {
		safeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyWeaponFired(ownerId)));
		Actor *pActor;
		mBulletCounter++;
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		pActor = mpActorHandler->getActor(ownerId);
		utilities::Position position = pActor->getPosition();
		utilities::Direction direction = pActor->getDirection();
		utilities::Direction aimDir = mpActorHandler->getActor(mpPlayerInfo->getPlayerId())->getPosition() - position;
		aimDir.normalize();

		if(mBullets <= 1)
		{
			safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateMunition(Munitions::MunitionType_Fire, position, direction, ownerId)));
		}

		if(mBulletKeys[mBulletCounter] != INVALID_KEY)
		{
		   Actor *pActor = mpActorHandler->getActor(mBulletKeys[mBulletCounter]);
		   pActor->setDirection(aimDir.x,aimDir.y,aimDir.z);
		   pActor->setPosition(position.x,position.y,position.z);
		   dynamic_cast<Munitions*>(pActor)->setVisible(true);
		}

		if(mBulletCounter == 1)
		{
			mBulletCounter = -1;
		}
	}
}

void FireSpit::addBullet(KeyType bulletId)
{
	mBulletKeys[mBullets] = bulletId;
	mBullets++;
}
