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
#include "FireAnt.h"
#include "ActorHandler.h"
#include "FireSpit.h"

using namespace logic;


FireAnt::FireAnt(int networkId, utilities::Position pos, utilities::Direction dir) :
	Enemy(networkId, ActorConstants::FIREANT_HP,ActorConstants::FIREANT_DAMAGE, Enemy::Size_Small , ActorConstants::FIREANT_RANGE, pos, dir, utilities::MapCoordinate(2,1), ai::AiBehavior_RangedAttacker)
{
	mScale = utilities::Vec3Float(0.89f, 5.0f, 1.5f);
	mSpeed = ActorConstants::FIREANT_SPEED;
	mEnemyType = EnemyType_FireAnt;
	mFire = false;
	mpEnemyWeapon = myNew FireSpit(ActorConstants::FIREANT_DAMAGE, 1.0f, ActorConstants::FIREANT_RANGE);
	mpEnemyWeapon->setOwner(mId);

	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mEnemyType));
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("FireAnt");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	mpTransformObject->setColor(D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f));
	//mpTransformObject->setScale(D3DXVECTOR3(2.89f, 10.0f, 2.5f));
	mpTransformObject->setScale(D3DXVECTOR3(0.89f, 5.0f, 1.5f));
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

FireAnt::~FireAnt()
{
	SAFE_DELETE(mpEnemyWeapon);
}

void FireAnt::attack()
{
	//if enemy is in range of tank
	Actor* pActor = ActorHandler::getInstance()->getActor(mpsPlayerInfo->getPlayerId());
	if((pActor->getPosition() - mPosition).longerThan(Enemy::getRange(), false) == false && mId.isOwner() && pActor->getActorType() != Actor::ActorType_Spectator) 
	{
		mpEnemyWeapon->fireWeapon(mId);
	}
}

void FireAnt::update(float deltaTime)
{
	Enemy::update(deltaTime);
	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());
}


void FireAnt::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}