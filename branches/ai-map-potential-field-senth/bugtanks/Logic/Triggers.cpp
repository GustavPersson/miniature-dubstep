/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
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

#include "Triggers.h"

using namespace logic;

/****************************************************************
 * TriggerEndMap
 ****************************************************************/

TriggerEndMap::TriggerEndMap() : TriggerBase()
{
	setTriggerType(TriggerBase::TriggerTypes_EndMap);
}

TriggerEndMap::TriggerEndMap(const TriggerEndMap &trigger) : TriggerBase(trigger)
{

}

TriggerEndMap::~TriggerEndMap()
{

}

const TriggerEndMap &TriggerEndMap::operator=(const TriggerEndMap &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
	}
	return *this;
}

TriggerBase *TriggerEndMap::clone()
{
	return myNew TriggerEndMap(*this);
}

bool TriggerEndMap::handleEvent(EventData const &eventData)
{
	return TriggerBase::handleEvent(eventData);
}

void TriggerEndMap::activate()
{
	TriggerBase::activate();
}

/****************************************************************
 * TriggerCreateUnit
 ****************************************************************/

TriggerCreateUnit::TriggerCreateUnit() : TriggerBase()
{
	setTriggerType(TriggerBase::TriggerTypes_CreateUnit);
	mCreateCount = 1;
}

TriggerCreateUnit::TriggerCreateUnit(const TriggerCreateUnit &trigger) : TriggerBase(trigger)
{
	mCreateCount = trigger.mCreateCount;
}

TriggerCreateUnit::~TriggerCreateUnit()
{

}

const TriggerCreateUnit &TriggerCreateUnit::operator=(const TriggerCreateUnit &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
		mCreateCount = trigger.mCreateCount;
	}
	return *this;
}

TriggerBase *TriggerCreateUnit::clone()
{
	return myNew TriggerCreateUnit(*this);
}

bool TriggerCreateUnit::handleEvent(EventData const &eventData)
{
	return TriggerBase::handleEvent(eventData);
}

void TriggerCreateUnit::activate()
{
	TriggerBase::activate();
	D3DXVECTOR3 destinationPosition = getDestinationBV()->getPosition();
	destinationPosition.y = 0.0f;

	utilities::Position position(0.0f, 0.0f, 0.0f);
	position.x = destinationPosition.x;
	position.y = destinationPosition.y;
	position.z = destinationPosition.z;
	utilities::Direction direction(0.0f, 0.0f, 0.0f);
	utilities::Vec3Float scale(1.0f, 2.0f, 1.0f);

	int sizeX = (int)(getDestinationBV()->getScaleX() * 2.0f);
	int sizeZ = (int)(getDestinationBV()->getScaleZ() * 2.0f);

	switch(getValue())
	{
		case Actor::ActorType_Enemy:
			for(int n = 0; n < mCreateCount; ++n)
			{
				position.x = destinationPosition.x + (float)((rand() % sizeX) - sizeX * 0.5);
				position.z = destinationPosition.z + (float)((rand() % sizeZ) - sizeZ * 0.5);
				safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy((Enemy::EnemyTypes)getSubValue(), position, direction)));
			}
			break;

		case Actor::ActorType_StaticEnvironment:
			for(int n = 0; n < mCreateCount; ++n)
			{
				position.x = destinationPosition.x + (float)((rand() % sizeX) - sizeX * 0.5);
				position.z = destinationPosition.z + (float)((rand() % sizeZ) - sizeZ * 0.5);
				safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment((StaticEnvironment::StaticEnvironmentTypes)getSubValue(), position, direction, scale)));
			}
			break;

		default:
			break;
	}
}

/****************************************************************
 * TriggerDestroyUnit
 ****************************************************************/

TriggerDestroyUnit::TriggerDestroyUnit() : TriggerBase()
{
	setTriggerType(TriggerBase::TriggerTypes_DestroyUnit);
}

TriggerDestroyUnit::TriggerDestroyUnit(const TriggerDestroyUnit &trigger) : TriggerBase(trigger)
{

}

TriggerDestroyUnit::~TriggerDestroyUnit()
{

}

const TriggerDestroyUnit &TriggerDestroyUnit::operator=(const TriggerDestroyUnit &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
	}
	return *this;
}

TriggerBase *TriggerDestroyUnit::clone()
{
	return myNew TriggerDestroyUnit(*this);
}

bool TriggerDestroyUnit::handleEvent(EventData const &eventData)
{
	return TriggerBase::handleEvent(eventData);
}

void TriggerDestroyUnit::activate()
{
	TriggerBase::activate();
}