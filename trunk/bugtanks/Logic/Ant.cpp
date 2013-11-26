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
 * This is the class for the most basic of the games enemies
 */

#include "Ant.h"
#include "ActorHandler.h"

using namespace logic;

Ant::Ant(int networkId, utilities::Position pos, utilities::Direction dir) :
	Enemy(networkId, ActorConstants::ANT_HP, ActorConstants::ANT_DAMAGE, Enemy::Size_Small, ActorConstants::ANT_RANGE, pos, dir, utilities::MapCoordinate(2,1), ai::AiBehavior_MeleeAttacker)
{
	mScale = utilities::Vec3Float(0.89f, 5.0f, 1.5f);
	mSpeed = ActorConstants::ANT_SPEED;
	mOriginalSpeed	= mSpeed;
	mEnemyType = EnemyType_Ant;

	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mEnemyType));
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Ant");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	mpTransformObject->setColor(D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.0f));
	mpTransformObject->setActor(this);
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimation("Ant_Melee");
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimationSpeedFactor(4.0f);

	for(std::vector<gfx::InstanceSubset *>::const_iterator instanceIter = ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin();
		instanceIter != ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().end(); ++instanceIter)
	{
		(*instanceIter)->setLogicBoundingVolume((*instanceIter)->getBoundingVolume()->clone());
		if(instanceIter - ((gfx::InstanceMesh *)mpTransformObject)->getSubsetList().begin() == 0)
		{
			mpPrePosCollisionObject = (*instanceIter)->getLogicBoundingVolume();
		}
	}
}

Ant::~Ant()
{

}

void Ant::attack()
{
	Actor *pTarget = ActorHandler::getInstance()->getActor(mTarget);
	mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
	//check if cooldownCounter is higher than weaponCooldown
	if(mCooldownCounter > 1.0f)
	{
		Tank *pTank = dynamic_cast<Tank*>(pTarget);
		Driver *pDriver = dynamic_cast<Driver*>(pTarget);
		mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);
		
		if(pTank != NULL)
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyMeleeAttack(this->getKey())));
			pTank->reduceHp(mDamage);	
		}

		else if(pDriver != NULL)
		{
			safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyMeleeAttack(this->getKey())));
			pDriver->reduceHp(mDamage);
		}
	}
  //send event to have the enemy perform his attack animation
  //send event to collision checking for a hit
}

void Ant::update(float deltaTime)
{   
	Enemy::update(deltaTime);
	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());

}


void Ant::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}