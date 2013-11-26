/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
 * @author Matteus Magnusson <senth.wallace@gmail.com>
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
#include "EnemyWeapon.h"
#include "FireSpit.h"
#include "AcidSpit.h"
#include "PlasmaSpit.h"
#include "PlayerInfo.h"
#include <cassert>

// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
#include "../Ai/AiFactory.h"
#endif
// ------------------------------
// AI Section END
// ------------------------------

using namespace logic;

Enemy::Enemy(int networkId, float hp, float damage, Sizes size, float range, utilities::Position pos, utilities::Direction dir, const utilities::MapCoordinate& mapSize, ai::AiBehaviors aiBehavior) : Unit(pos, dir, hp, networkId, mapSize)
{
	mAiBehavior = aiBehavior;
	mActorType = ActorType_Enemy;
	mEnemyType = EnemyType_Invalid;

	mpEnemyWeapon = NULL;

	mDamage = damage;
	mSize = size;
	mRange = range;
	mCooldownCounter = 0.0f;
	mCooldownStartTime = 0.0f;
	mpGlobalTimer = logic::GlobalTimer::getInstance();
	mFire = false;


// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Add the enemy to the AiFactory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->addEnemy(this);
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}

Enemy::~Enemy()
{
// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Remove the enemy from the factory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->removeEnemy(getKey());
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}

void Enemy::update(float deltaTime)
{
	Unit::update(deltaTime);

	if (mFire)
	{
		attack();
	}
}

void Enemy::setFire(bool fire)
{
	mFire = fire;
}

void Enemy::handleEventId(EventData const &eventMessage)
{
	if(EvtData_To_EnemyUpdateHp::msEventType == eventMessage.getEventType())
	{
		const EvtData_To_EnemyUpdateHp& evtData = static_cast<const EvtData_To_EnemyUpdateHp&>(eventMessage);
		mHealthPoints -= evtData.mDamage;
	}
	else
		Actor::handleEventId(eventMessage);
}

Enemy::Sizes Enemy::getEnemySize()
{
	return mSize;
}

void Enemy::reduceHp(float damage)
{
	
	mHealthPoints -= damage;
	DEBUG_MESSAGE(LEVEL_MEDIUM, "HP: " << this->getName() << mHealthPoints);
	if(mHealthPoints <= 0 && !mDead && mId.isOwner())
	{
		DEBUG_MESSAGE(LEVEL_MEDIUM, "Lol i die ");
		safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(this->getKey())));
		mDead = true;
	}
}

float Enemy::getDamage()
{
	return mDamage;
}

Enemy::EnemyTypes Enemy::getEnemyType()
{
	return mEnemyType;
}

float Enemy::getRange()
{
	return mRange;
}

float Enemy::getHealthPoints()
{
	return mHealthPoints;
}

std::istream& operator>>(std::istream &in, logic::Enemy::EnemyTypes& enemyType)
{
	int intEnemyType;
	in >> intEnemyType;
	enemyType = static_cast<logic::Enemy::EnemyTypes>(intEnemyType);
	return in;
}

EnemyWeapon* Enemy::getEnemyWeapon()
{
	return mpEnemyWeapon;
}