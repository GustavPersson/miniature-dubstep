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
*/

#include "Tank.h"

using namespace ai;

Tank::Tank(logic::Actor* pTank) : Player(pTank)
{
	// Do Nothing
}

Tank::~Tank()
{
	// Do nothing
}

float Tank::getPotentialValue(const utilities::MapCoordinate &mapPosition) const
{
	utilities::MapCoordinate indexInPlayer = mapPosition - getMapPosition();

	indexInPlayer += msCenterIndex;

	float potentialValue = NEUTRAL_POTENTIAL_VALUE;

	// Return the value if the index is valid, else return INVALID
	if (validIndex(indexInPlayer))
	{
		potentialValue = mpsPotentialField[indexInPlayer.y * msPotentialRange.x + indexInPlayer.x];

		// TODO - Fix the values if we're close to the enemy
	}

	return potentialValue;
}