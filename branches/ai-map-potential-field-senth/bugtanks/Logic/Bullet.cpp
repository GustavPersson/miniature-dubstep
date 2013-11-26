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

using namespace logic;

Bullet::Bullet(int networkId, utilities::Position position, utilities::Direction direction, int efficiency, float damage, Weapon::WeaponTypes ownerType) :
	Munitions(networkId, position, direction, efficiency, damage)
{
	mScale = utilities::Vec3Float(1.0f, 1.0f, 1.0f);
	mMunitionType = MunitionType_Bullet;
	if(ownerType != Weapon::WeaponTypes_Minigun)
	{
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
	mPosition = mPosition + (ActorConstants::BULLET_SPEED * mDirection * deltaTime);
	if(mOwnerDead && mDestroyTimer.getTime(utilities::Timer::ReturnType_Seconds) > 10)
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(mId)));
		mOwnerDead = false;
	}
}

