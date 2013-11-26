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
 * This is a mid class between Actor and Enemies and Players(Tank,Driver)
 */

#include "Unit.h"
#include "Events.h"
#include "ActorHandler.h"

using namespace logic;

Unit::Unit(utilities::Position pos ,utilities::Direction dir, float hp, int networkId, const utilities::MapCoordinate& size) : DynamicActor(pos, dir, networkId, size)
{
	mFocalDirection = utilities::Direction(1.0f, 0.0f, 0.0f);
	mFocalPoint = utilities::INVALID_WORLD_POSITON;
	mFocalUnitId = INVALID_KEY;

	mHealthPoints = hp;
	mFire = false;
}

Unit::~Unit()
{

}

void Unit::handleEventId(const logic::EventData &eventMessage)
{

}

void Unit::update(float deltaTime)
{
	DynamicActor::update(deltaTime);

	// Set the focal direction to the focal point
	if (mFocalPoint != utilities::INVALID_WORLD_POSITON)
	{
		mFocalDirection = mFocalPoint - mPosition;
		mFocalDirection.normalize();
	}

	if (mFocalUnitId != INVALID_KEY)
	{
		// Get the actor
		DynamicActor* pActor = mpsActorHandler->getDynamicActor(mFocalUnitId);
		if (pActor != NULL)
		{
			mFocalDirection = pActor->getPosition() - mPosition;
			mFocalDirection.normalize();
		}
		// Else the unit has probably been destroyed, set the id as invalid
		else
		{
			mFocalUnitId = INVALID_KEY;
		}
	}
}