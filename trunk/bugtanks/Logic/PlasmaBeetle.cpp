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
 * This is the class for the plasma beetle
 */

#include "PlasmaBeetle.h"
#include "PlasmaSpit.h"

using namespace logic;


PlasmaBeetle::PlasmaBeetle(int networkId, utilities::Position pos, utilities::Direction dir) :
Enemy(networkId, ActorConstants::PLASMABEETLE_HP, ActorConstants::PLASMABEETLE_DAMAGE, Enemy::Size_Large, ActorConstants::PLASMABEETLE_RANGE, pos, dir, utilities::MapCoordinate(3,3), ai::AiBehavior_RangedAttacker)
{
	mScale = utilities::Vec3Float(4.8f, 3.5f, 2.8f);
	mSpeed = ActorConstants::PLASMABEETLE_SPEED;
	mOriginalSpeed	= mSpeed;
	mEnemyType = EnemyType_PlasmaBeetle;
	mFire = false;
	mpEnemyWeapon = myNew PlasmaSpit(ActorConstants::PLASMABEETLE_DAMAGE, 1.0f, ActorConstants::PLASMABEETLE_RANGE, getKey());

	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mEnemyType));
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("PlasmaBeetle");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	mpTransformObject->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	mpTransformObject->setActor(this);
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimation("Beetle_Walk");
	((gfx::InstanceAnimationMesh *)mpTransformObject)->setAnimationSpeedFactor(1.0f);

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

PlasmaBeetle::~PlasmaBeetle()
{
	SAFE_DELETE(mpEnemyWeapon);
}

void PlasmaBeetle::attack()
{
	mpEnemyWeapon->fireWeapon(mId);
}

void PlasmaBeetle::update(float deltaTime)
{
	Enemy::update(deltaTime);

	mpPrePosCollisionObject->setPosition(mPosition.convertToD3DXVECTOR3());

}


void PlasmaBeetle::handleEventId(EventData const &eventMessage)
{
	Enemy::handleEventId(eventMessage);
}