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
 * This is the class for the Scarab
 */

#include "Scarab.h"
#include "ActorHandler.h"

using namespace logic;


Scarab::Scarab(int networkId, utilities::Position pos, utilities::Direction dir) :
Enemy(networkId, ActorConstants::SCARAB_HP, ActorConstants::SCARAB_DAMAGE, Enemy::Size_Large, ActorConstants::SCARAB_RANGE, pos, dir, utilities::MapCoordinate(3,3), ai::AiBehavior_MeleeAttacker)
{
	mScale = utilities::Vec3Float(4.8f, 3.5f, 2.8f);
	mSpeed = ActorConstants::SCARAB_SPEED;
	mEnemyType = EnemyType_Scarab;
	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mEnemyType));
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Scarab");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	mpTransformObject->setColor(D3DXCOLOR(0.1f, 0.45f, 0.4f, 1.0f));
	mpTransformObject->setActor(this);

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

Scarab::~Scarab()
{

}

void Scarab::attack()
{
	//if enemy is in range of tank
	Actor* pActor = ActorHandler::getInstance()->getActor(mpsPlayerInfo->getPlayerId());
	if(pActor != NULL && (pActor->getPosition() - mPosition).longerThan(Enemy::getRange(), false) == false && mId.isOwner())
	{
		mCooldownCounter = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds) - mCooldownStartTime;
		//check if cooldownCounter is higher than weaponCooldown
		if(mCooldownCounter > 1.0f)
		{
			Tank *pTank = dynamic_cast<Tank*>(pActor);
			Driver *pDriver = dynamic_cast<Driver*>(pActor);
			mCooldownStartTime = mpGlobalTimer->getTime(utilities::Timer::ReturnType_Seconds);

			if(pTank != NULL)
			{
				safeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyMeleeAttack(this->getKey())));
				pTank->reduceHp(45);	
			}

			else if(pDriver != NULL)
			{
				safeQueueEvent(EventDataPtr(myNew EvtData_From_EnemyMeleeAttack(this->getKey())));
				pDriver->reduceHp(45);
			}
		}

	}
}

void Scarab::update(float deltaTime)
{
	Enemy::update(deltaTime);
	attack();
	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());

}


void Scarab::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}