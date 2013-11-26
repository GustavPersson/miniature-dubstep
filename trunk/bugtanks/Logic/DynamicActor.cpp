/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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

#include "DynamicActor.h"

using namespace logic;

DynamicActor::DynamicActor(utilities::Position pos, utilities::Direction dir, int networkId, const utilities::MapCoordinate& size) : Actor(pos, dir, networkId, size)
{
	mOriginalSpeed = 0.0f;
}

DynamicActor::~DynamicActor()
{
}

void DynamicActor::update(float deltaTime)
{
	if (mMoving || mPosition.y != 0.0f)
	{
		mPrePosition = mPosition;
		

		// Handle a special case when we have a direction in y (and isn't on the ground)
		if (mDirection.y != 0.0f || mPosition.y != 0.0f)
		{
			mDirection *= mSpeed;
			float ySpeed = mDirection.y;
			mDirection.y -= ActorConstants::GRAVITATION * deltaTime;
			float deltaYSpeed = ySpeed - mDirection.y;
			if (mDirection.y > 0.0f)
			{
				mSpeed -= deltaYSpeed;
			}
			else
			{
				mSpeed += deltaYSpeed;
			}
			mDirection.normalize();
		}

		mPosition = mPosition + (mSpeed * mDirection * deltaTime);

		// Fix if we happen to get underground
		if (mPosition.y < 0.0f)
		{
			mPosition.y = 0.0f;
			mDirection.y = 0.0f;
			mDirection.normalize();
			mSpeed = mOriginalSpeed;
		}
	}
}