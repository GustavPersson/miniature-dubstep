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
 * Subclass to munitions, it holds the pulse cannons shot  
 */

#include "Pulse.h"
#include "Events.h"
#include "../gfx/Particle.h"

using namespace logic;


Pulse::Pulse(int networkId, utilities::Position position,utilities::Direction direction, int efficiency, float damage) :
	Munitions(networkId, position, direction, efficiency, damage)
{
	mMunitionType = Munitions::MunitionType_Pulse;
	mScale = utilities::Vec3Float(6.0f, 4.0f, 6.0f);
	mpTransformObject = mpsDXModule->createObjectInstanced("Cube");
	mpTransformObject->setID(mId.convertToGfxId());
	mpTransformObject->setName("Pulse");
	mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
	mpTransformObject->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 1.0f));
	mpTransformObject->setFlagRender(false);
	dynamic_cast<gfx::InstanceObject*>(mpTransformObject)->setScale(mScale.convertToD3DXVECTOR3());
	
}

Pulse::~Pulse()
{
}

void Pulse::update(float deltaTime)
{	
	
	mPosition = mPosition + (ActorConstants::PULSE_SPEED * mDirection * deltaTime);
	if(mOwnerDead && mDestroyTimer.getTime(utilities::Timer::ReturnType_Seconds) > 10)
	{
		safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(mId)));
		DEBUG_MESSAGE(LEVEL_LOWEST, "pulseupdate");
		mOwnerDead = false;
	}
}

void Pulse::handleEventId(EventData const &eventMessage)
{
	Munitions::handleEventId(eventMessage);
}