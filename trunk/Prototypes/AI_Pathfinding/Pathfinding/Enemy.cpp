/**
* @file
* @author Matteus Magnusson <senth.wallace@gmail.com>
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
* The enemy
*/
#include "Macros.h"
#include "Enemy.h"

Enemy::Enemy(int width,
			 int height,
			 const FixedCoordinate& position,
			 float range
			 )
{
	mWidth = width;
	mHeight = height;
	mPosition = position;
	mRange = range;
}

int Enemy::getWidth() const
{
	return mWidth;
}

int Enemy::getHeight() const
{
	return mHeight;
}

FixedCoordinate Enemy::getPosition() const
{
	return mPosition;
}

void Enemy::setPosition(const FixedCoordinate &newPosition)
{
	mPosition = newPosition;
}

float Enemy::getRange() const
{
	return mRange;
}

float Enemy::calculatePotentialValue(float distance)
{
	if (distance > 0)
	{
		float potentialValue = ENEMY_ATTRACT_FORCE - sqrt(distance);
		if (potentialValue > 0)
		{
			return potentialValue;
		}
	}

	return 0.0f;
}