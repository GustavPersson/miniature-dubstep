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
 * Subclass to munitions, it holds all kinds of bullet type projectiles  
 */

#include "Bullet.h"
#include "../Utilities/Constants.h"
#include "Events.h"
#include "ActorHandler.h"

using namespace logic;

Bullet::Bullet(int networkId, utilities::Position position, utilities::Direction direction, int efficiency, float damage, Weapon::WeaponTypes ownerType) :
	Munitions(networkId, position, direction, efficiency, damage)
{
	mOwnerType = ownerType;
	mMunitionType = MunitionType_Bullet;
	if(mOwnerType == Weapon::WeaponTypes_Cannon)
	{
		mScale = utilities::Vec3Float(2.0f, 1.0f, 2.0f);
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mMunitionType));
		mpTransformObject->setTextureID(-1);
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("Bullet");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		mpTransformObject->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		//mpTransformObject->setRotationX(-utilities::math::PI_HALF);
		dynamic_cast<gfx::InstanceObject*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	}
	if(mOwnerType == Weapon::WeaponTypes_GaussCannon)
	{
		mScale = utilities::Vec3Float(1.0f, 1.0f, 1.0f);
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mMunitionType));
		mpTransformObject->setTextureID(-1);
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("Bullet");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		mpTransformObject->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		//mpTransformObject->setRotationX(-utilities::math::PI_HALF);
		dynamic_cast<gfx::InstanceObject*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	}
	if(mOwnerType == Weapon::WeaponTypes_Shotgun)
	{
		mScale = utilities::Vec3Float(0.7f, 0.7f, 0.7f);
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mMunitionType));
		mpTransformObject->setTextureID(-1);
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("Bullet");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		mpTransformObject->setColor(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
		//mpTransformObject->setRotationX(-utilities::math::PI_HALF);
		dynamic_cast<gfx::InstanceObject*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	}
}

Bullet::~Bullet()
{
}

void Bullet::update(float deltaTime)
{	
	if(mOwnerType != Weapon::WeaponTypes_GaussCannon)
		mPosition = mPosition + (ActorConstants::BULLET_SPEED * mDirection * deltaTime);
	else
		mPosition = mPosition + (ActorConstants::BULLET_SPEED*2 * mDirection * deltaTime);

	if(mOwnerDead && mDestroyTimer.getTime(utilities::Timer::ReturnType_Seconds) > 10)
	{
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(mId)));
		DEBUG_MESSAGE(LEVEL_LOWEST, "bulletupdate");
		mOwnerDead = false;
	}
}

void Bullet::handleEventId(EventData const &eventMessage)
{
	Munitions::handleEventId(eventMessage);
}

