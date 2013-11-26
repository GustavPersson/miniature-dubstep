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
#include "ActorHandler.h"

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
	setMisc(1);
}

TriggerCreateUnit::TriggerCreateUnit(const TriggerCreateUnit &trigger) : TriggerBase(trigger)
{

}

TriggerCreateUnit::~TriggerCreateUnit()
{

}

const TriggerCreateUnit &TriggerCreateUnit::operator=(const TriggerCreateUnit &trigger)
{
	if(this != &trigger)
	{
		TriggerBase::operator=(trigger);
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

	float angle = getDestinationBV()->getRotationY();
	D3DXMATRIX MRotY;
	D3DXMatrixRotationY(&MRotY, angle);

	D3DXVECTOR3 transformedPosition;

	utilities::Position position(0.0f, 0.0f, 0.0f);
	utilities::Direction direction(0.0f, 0.0f, 0.0f);
	utilities::Vec3Float scale(1.0f, 2.0f, 1.0f);

	int sizeX = (int)(getDestinationBV()->getScaleX() * 2.0f);
	int sizeZ = (int)(getDestinationBV()->getScaleZ() * 2.0f);

	int createCount = getMisc();

	switch(getValue())
	{
		case Actor::ActorType_Enemy:
			for(int n = 0; n < createCount; ++n)
			{
				transformedPosition.x = (float)((rand() % sizeX) - sizeX * 0.5);
				transformedPosition.y = 0.0f;
				transformedPosition.z = (float)((rand() % sizeZ) - sizeZ * 0.5);

				//Rotate the position.
				D3DXVec3TransformCoord(&transformedPosition, &transformedPosition, &MRotY);

				//Translate the position.
				position.x = transformedPosition.x + destinationPosition.x;
				position.z = transformedPosition.z + destinationPosition.z;

				ActorHandler::getInstance()->createEnemy((Enemy::EnemyTypes)getSubValue(), position, direction);
			}
			break;

		case Actor::ActorType_StaticEnvironment:
			for(int n = 0; n < createCount; ++n)
			{
				transformedPosition.x = (float)((rand() % sizeX) - sizeX * 0.5);
				transformedPosition.y = 0.0f;
				transformedPosition.z = (float)((rand() % sizeZ) - sizeZ * 0.5);

				//Rotate the position.
				D3DXVec3TransformCoord(&transformedPosition, &transformedPosition, &MRotY);

				//Translate the position.
				position.x = transformedPosition.x + destinationPosition.x;
				position.z = transformedPosition.z + destinationPosition.z;

				ActorHandler::getInstance()->createStaticEnvironment((StaticEnvironment::StaticEnvironmentTypes)getSubValue(), position, direction, scale);
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