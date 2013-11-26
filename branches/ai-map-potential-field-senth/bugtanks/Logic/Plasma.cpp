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
 * Subclass to munitions, it holds plasma spew projectile  
 */

#include "Plasma.h"
#include "Events.h"

using namespace logic;

Plasma::Plasma(int networkId, utilities::Position position, utilities::Direction direction,int efficiency, float damage) :
	Munitions(networkId, position,direction,efficiency,damage)
{
	mMunitionType = MunitionType_Plasma;
}

Plasma::~Plasma()
{
}

void Plasma::update(float deltaTime)
{
	//mPosition = mPosition + (ActorConstants::PLASMA_SPEED * mDirection * deltaTime);
	if(mOwnerDead && mDestroyTimer.getTime(utilities::Timer::ReturnType_Seconds) > 10)
	{
		safeQueueEvent(EventDataPtr(myNew EvtData_From_ActorDestroyed(mId)));
		mOwnerDead = false;
	}
}

void Plasma::handleEventId(EventData const &eventMessage)
{

}