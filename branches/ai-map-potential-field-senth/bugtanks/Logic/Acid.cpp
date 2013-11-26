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
 * Subclass to munitions, it holds acid spit projectiles 
 */

#include "Acid.h"
#include "Events.h"

using namespace logic;

Acid::Acid(int networkId, utilities::Position position, utilities::Direction direction, int efficiency, float damage) :
	Munitions(networkId, position, direction, efficiency,damage)
{
	mScale = utilities::Vec3Float(2.0f, 2.0f, 2.0f);
	mMunitionType = MunitionType_Acid; 
	mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mMunitionType));
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Acid");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	mpTransformObject->setColor(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	dynamic_cast<gfx::InstanceObject*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
}

Acid::~Acid()
{
}

void Acid::update(float deltaTime)
{
	mPosition = mPosition + (ActorConstants::ACID_SPEED * mDirection * deltaTime);
	if(mOwnerDead && mDestroyTimer.getTime(utilities::Timer::ReturnType_Seconds) > 10)
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(mId)));
		mOwnerDead = false;
	}
}

void Acid::handleEventId(EventData const &eventMessage)
{
	Munitions::handleEventId(eventMessage);
}