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
 */

#include "RangedAi.h"
#include "../logic/ActorConstants.h"
#include "../logic/ActorHandler.h"

using namespace ai;


RangedAi::RangedAi(logic::Enemy *pEnemy) :AgentAi(pEnemy)
{

}

RangedAi::~RangedAi()
{

}

float RangedAi::getPotentialValue(const utilities::MapCoordinate &mapPosition) const
{
	if((mpActor->getPosition() - mapPosition.convertToWorldCoordinates()).shorterThan(mpEnemy->getRange()-utilities::MAP_IN_WORLD_COORDS))
	{

		return INVALID_POTENTIAL_VALUE;
	}
	else
	{
		return AgentAi::getPotentialValue(mapPosition);
	}
}

