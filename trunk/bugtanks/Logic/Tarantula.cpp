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
 * This is the class for the Tarantula
 */

#include "Tarantula.h"
#include "ActorHandler.h"

using namespace logic;

Tarantula::Tarantula(int networkId,  utilities::Position pos, utilities::Direction dir) :
Enemy(networkId, ActorConstants::TARANTULA_HP, ActorConstants::TARANTULA_DAMAGE, Enemy::Size_Medium, ActorConstants::TARANTULA_RANGE, pos, dir, utilities::MapCoordinate(3,2), ai::AiBehavior_MeleeAttacker)
{
	mScale = utilities::Vec3Float(2.5f, 3.5f, 2.5f);
	mSpeed = ActorConstants::TARANTULA_SPEED;
	mOriginalSpeed	= mSpeed;
	mEnemyType = EnemyType_Tarantula;
	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mEnemyType));
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Tarantula");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	mpTransformObject->setColor(D3DXCOLOR(0.25f, 1.0f, 0.25f, 1.0f));
	mpTransformObject->setActor(this);
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimation("Spider_Walk");
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimationSpeedFactor(2.0f);

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

Tarantula::~Tarantula()
{

}

void Tarantula::attack()
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
}

void Tarantula::update(float deltaTime)
{   
	Enemy::update(deltaTime);
	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());

}


void Tarantula::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}