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

#include "Building.h"
#include "AiConstants.h"

using namespace ai;

int Building::msBuildingPotentialRange = 0;
float Building::msStartValue = 0.0f;
float* Building::mpsPotentialValues = NULL;

Building::Building(logic::Actor *pBuilding) : StaticUnit(pBuilding, true)
{
	assert(mpsPotentialValues != NULL);

	mPotentialSize = getSize();
	mPotentialSize += msBuildingPotentialRange * 2;
}

Building::~Building()
{
}

void Building::initPotentialField(int maxDistance)
{
	msBuildingPotentialRange = maxDistance;
	mpsPotentialValues = myNew float [(msBuildingPotentialRange+1) * (msBuildingPotentialRange+1)];
	msStartValue = AiConstants::POTENTIAL_BUILDING_START_VALUE;

	for (int y = 0; y <= msBuildingPotentialRange; y++)
	{
		for (int x = 0; x <= msBuildingPotentialRange; x++)
		{
			float distance = sqrt(static_cast<float>( x*x + y*y ));
			int arrayOffset = y * (msBuildingPotentialRange + 1) + x;
			mpsPotentialValues[arrayOffset] = -(msStartValue - sqrtf(distance));
			float test = mpsPotentialValues[arrayOffset];
			test = 1.0f;
		}
	}
}

void Building::destroyPotentialField()
{
	SAFE_DELETE_ARRAY(mpsPotentialValues);
}

float Building::getPotentialValue(const utilities::MapCoordinate& mapPosition) const
{
	utilities::MapCoordinate centerIndex(mPotentialSize.x >> 1, mPotentialSize.y >> 1);
	utilities::MapCoordinate indexInBuilding = mapPosition - getMapPosition();

	indexInBuilding += centerIndex;

	float value = NEUTRAL_POTENTIAL_VALUE;
	if (validIndex(indexInBuilding))
	{
		// Start to check where the x position lies, left/in/right
		// Left
		if (indexInBuilding.x < msBuildingPotentialRange)
		{
			// Test over/in/under for y
			// Over - We're in the upper left corner
			if (indexInBuilding.y < msBuildingPotentialRange)
			{
				utilities::MapCoordinate offset(reverseIndex(indexInBuilding.x), reverseIndex(indexInBuilding.y));
				value = getPotentialValueFromEdge(offset);
			}
			// Inside - We're to the left of the building
			else if (indexInBuilding.y < msBuildingPotentialRange + getSize().y)
			{
				utilities::MapCoordinate offset(reverseIndex(indexInBuilding.x), 0);
				value = getPotentialValueFromEdge(offset);
			}
			// Under - We're in the lower left corner
			else
			{
				utilities::MapCoordinate offset(reverseIndex(indexInBuilding.x), indexInBuilding.y - msBuildingPotentialRange - getSize().y + 1);
				value = getPotentialValueFromEdge(offset);
			}
		}
		// Inside
		else if(indexInBuilding.x < msBuildingPotentialRange + getSize().x)
		{
			// Test over/in/under for y
			// Over - We're over the building
			if (indexInBuilding.y < msBuildingPotentialRange)
			{
				utilities::MapCoordinate offset(0, reverseIndex(indexInBuilding.y));
				value = getPotentialValueFromEdge(offset);
			}
			// Inside
			else if (indexInBuilding.y < msBuildingPotentialRange + getSize().y)
			{
				value = INVALID_POTENTIAL_VALUE;
			}
			// Under - We're under the building
			else
			{
				utilities::MapCoordinate offset(0, indexInBuilding.y - msBuildingPotentialRange - getSize().y + 1);
				value = getPotentialValueFromEdge(offset);
			}
		}
		// Right
		else
		{
			// Test over/in/under for y
			// Over - We're in the upper right corner
			int xOffset = indexInBuilding.x - msBuildingPotentialRange - getSize().x + 1;
			if (indexInBuilding.y < msBuildingPotentialRange)
			{
				utilities::MapCoordinate offset(xOffset, reverseIndex(indexInBuilding.y));
				value = getPotentialValueFromEdge(offset);
			}
			// Inside - We're to the right of the building
			else if (indexInBuilding.y < msBuildingPotentialRange + getSize().y)
			{
				utilities::MapCoordinate offset(xOffset, 0);
				value = getPotentialValueFromEdge(offset);
			}
			// Under - We're in the lower left corner
			else
			{
				utilities::MapCoordinate offset(xOffset, indexInBuilding.y - msBuildingPotentialRange - getSize().y + 1);
				value = getPotentialValueFromEdge(offset);
			}
		}
	}

	return value;
}

void Building::update()
{
	StaticUnit::update();

	// Check so that the size is unaffected (buildings can be scaled)
	if ( (isRotated() && ( getSize().x != mpActor->getSize().y || getSize().y != mpActor->getSize().x ) ||
		(getSize().x != mpActor->getSize().x || getSize().y != mpActor->getSize().y)) )
	{
		updateOffsetAndUnitSize();

		mPotentialSize = getSize();
		mPotentialSize += msBuildingPotentialRange * 2;

#ifdef AI_DRAW_UNIT_SIZE_BLOCKS
		// Remove the old blocks

		for (int i = 0; i < mcPositionBlock; i++)
		{
			mpsDx10Module->removeRenderObject(mpPositionBlocks[i]);
		}

		SAFE_DELETE_ARRAY(mpPositionBlocks);

		mcPositionBlock = getSize().x * getSize().y;
		mpPositionBlocks = myNew gfx::TransformObject* [mcPositionBlock];

		for (int i = 0; i < mcPositionBlock; i++)
		{
			mpPositionBlocks[i] = mpsDx10Module->createObjectInstanced(1);
			utilities::MapCoordinate worldPos = mpActor->getPosition();
			worldPos.y = AiConstants::DRAW_HEIGHT_UNIT_SIZE;
			mpPositionBlocks[i]->setPosition(worldPos.convertToD3DXVECTOR3());
			mpPositionBlocks[i]->setScale(D3DXVECTOR3(utilities::MAP_IN_WORLD_COORDS_HALF, 0.1f, utilities::MAP_IN_WORLD_COORDS_HALF));
			mpPositionBlocks[i]->setColor(D3DXCOLOR(1.0f, 0.2f, 1.0f, 1.0f));
			mpPositionBlocks[i]->setID(logic::KEY_GROUND.convertToGfxId());
		}
#endif
	}

}

float Building::getPotentialValueFromEdge(const utilities::MapCoordinate& positionOffset)
{
	if (positionOffset.x < 0 || positionOffset.x > msBuildingPotentialRange ||
		positionOffset.y < 0 || positionOffset.y > msBuildingPotentialRange)
	{
		ERROR_MESSAGE("Position offset is out of range!");
		return INVALID_POTENTIAL_VALUE;
	}

	int arrayOffset = positionOffset.y * (msBuildingPotentialRange + 1) + positionOffset.x;
	return mpsPotentialValues[arrayOffset];
}