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
 * This is the class for the Black Widow
 */

#include "BlackWidow.h"
#include "AcidSpit.h"
#include <iostream>

using namespace std;
using namespace logic;


BlackWidow::BlackWidow(int networkId, utilities::Position pos, utilities::Direction dir) :
Enemy(networkId, ActorConstants::BLACKWIDOW_HP, ActorConstants::BLACKWIDOW_DAMAGE, Enemy::Size_Medium, ActorConstants::BLACKWIDOW_RANGE, pos, dir, utilities::MapCoordinate(3,2), ai::AiBehavior_RangedAttacker)
{
	mScale = utilities::Vec3Float(2.5f, 3.5f, 2.5f);
	mSpeed = ActorConstants::BLACKWIDOW_SPEED;
	mOriginalSpeed	= mSpeed;
	mEnemyType = EnemyType_BlackWidow;
	mpEnemyWeapon = myNew AcidSpit(ActorConstants::BLACKWIDOW_DAMAGE, 1.0f, ActorConstants::BLACKWIDOW_RANGE, getKey());

	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mEnemyType));
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("BlackWidow");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	mpTransformObject->setColor(D3DXCOLOR(0.1f, 0.1f, 0.1f, 1.0f));
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

BlackWidow::~BlackWidow()
{
	SAFE_DELETE(mpEnemyWeapon);
}

void BlackWidow::attack()
{
	mpEnemyWeapon->fireWeapon(mTarget);  
}

void BlackWidow::update(float deltaTime)
{
	Enemy::update(deltaTime);

	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());

}

void BlackWidow::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}