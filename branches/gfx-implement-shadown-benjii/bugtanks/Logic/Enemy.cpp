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
 * This will be the baseclass of all enemies in the game
 */

#include "Enemy.h"
#include "GlobalTimer.h"

using namespace logic;



Enemy::Enemy(double hp,double damage,Sizes size, int aiStyle,float range)
{
	mpPlayerInfo = logic::PlayerInfo::getInstance();
	mpEnemyWeapon = NULL;
	mHealthPoints = hp;
	mDamage = damage;
	mSize = size;
	mAiStyle = aiStyle;
	mRange = range;
	mActorType = ActorType_Enemy;
}

Enemy::~Enemy()
{
}

void Enemy::handleEventId(EventData const &eventMessage)
{
	if(EvtData_To_EnemyUpdateHp::mEventType == eventMessage.getEventType())
	{
		const EvtData_To_EnemyUpdateHp& evtData = static_cast<const EvtData_To_EnemyUpdateHp&>(eventMessage);
		mHealthPoints -= evtData.mDamage;
	}
	else
		Actor::handleEventId(eventMessage);
}

Enemy::Sizes Enemy::getSize()
{
	return mSize;
}

void Enemy::reduceHp(double damage)
{
	
	mHealthPoints -= damage;
	DEBUG_MESSAGE("HP: " << mHealthPoints);
	if(mHealthPoints <= 0)
	{
		DEBUG_MESSAGE("Lol i die ");
		safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(this->getKey(), GlobalTimer::getInstance()->getTime(utilities::Timer::ReturnType_MilliSeconds))));
	}
}

double Enemy::getDamage()
{
	return mDamage;
}

float Enemy::getRange()
{
	return mRange;
}