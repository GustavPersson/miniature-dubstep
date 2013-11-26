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

#include "Map.h"
#include "PotentialUnit.h"
#include "AiConstants.h"

using namespace ai;

Map* Map::mpsInstance = NULL;

Map::Map()
{
	mpPotentialField = NULL;
	mArraySize = 0;

#ifdef AI_DRAW_MAP_POTENTIAL_FIELD
	mpDx10Module = gfx::DX10Module::getInstance();
	mppGfxPotentialField = NULL;
#endif
}

Map::~Map()
{
	SAFE_DELETE_ARRAY(mpPotentialField);

	mpsInstance = NULL;

#ifdef AI_DRAW_MAP_POTENTIAL_FIELD
	// Delete the potential field blocks from the gfx
	for (int i = 0; i < mArraySize; i++)
	{
		mpDx10Module->removeRenderObject(mppGfxPotentialField[i]);
	}
	SAFE_DELETE_ARRAY(mppGfxPotentialField);

	mpDx10Module = NULL;
#endif
}

Map* Map::getInstance()
{
	if (mpsInstance == NULL)
	{
		mpsInstance = myNew Map();
	}
	return mpsInstance;
}

void Map::clear()
{
	SAFE_DELETE_ARRAY(mpPotentialField);

	mPotentialUnits.clear();
}

void Map::setMapSize(const utilities::MapCoordinate& mapSize)
{
#ifdef AI_DRAW_MAP_POTENTIAL_FIELD
	// Delete the potential field blocks from the gfx
	for (int i = 0; i < mArraySize; i++)
	{
		mpDx10Module->removeRenderObject(mppGfxPotentialField[i]);
	}

	SAFE_DELETE_ARRAY(mppGfxPotentialField);
#endif

	SAFE_DELETE_ARRAY(mpPotentialField);

	mMapSize = mapSize;
	mArraySize = mMapSize.x * mMapSize.y;
	mpPotentialField = myNew float [mArraySize];

	mCenterIndex.x = mMapSize.x >> 1;
	mCenterIndex.y = mMapSize.y >> 1;

	// Set the potential field to neutral values
	for (int i = 0; i < mArraySize; i++)
	{
		mpPotentialField[i] = NEUTRAL_POTENTIAL_VALUE;
	}

	readdPotentialUnits();

#ifdef AI_DRAW_MAP_POTENTIAL_FIELD
	// Create the potential field blocks on the gfx and draw the color

	mppGfxPotentialField = myNew gfx::TransformObject* [mArraySize];

	for (int y = 0; y < mMapSize.y; y++)
	{
		utilities::MapCoordinate mapPos(0, y - mCenterIndex.y);
		int arrayOffsetY = y * mMapSize.x;

		for (int x = 0; x < mMapSize.x; x++)
		{
			mapPos.x = x - mCenterIndex.x;
			int arrayOffset = arrayOffsetY + x;

			utilities::Vec3Float worldPos = mapPos.convertToWorldCoordinates();
			worldPos.y = AiConstants::DRAW_HEIGHT_MAP;

			mppGfxPotentialField[arrayOffset] = mpDx10Module->createObjectInstanced(1);
			assert(mppGfxPotentialField[arrayOffset] != NULL);
			mppGfxPotentialField[arrayOffset]->setScale(D3DXVECTOR3(utilities::MAP_IN_WORLD_COORDS_HALF, 0.1f, utilities::MAP_IN_WORLD_COORDS_HALF));
			mppGfxPotentialField[arrayOffset]->setID(logic::KEY_GROUND.convertToGfxId());
			mppGfxPotentialField[arrayOffset]->setPosition(worldPos.convertToD3DXVECTOR3());
			mppGfxPotentialField[arrayOffset]->setColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.45f));
		}
	}
#endif
}

void Map::addPotentialUnit(const PotentialUnit* pPotentialUnit)
{
	// Because we use this function internally only add the potential unit if 
	// it doesn't exist in the potential unit
	if (mPotentialUnits.find(pPotentialUnit->getKey()) == mPotentialUnits.end())
	{
		mPotentialUnits[pPotentialUnit->getKey()] = pPotentialUnit;
	}
	

	// Get the mapOffset of the first coordinate in the potential unit
	utilities::MapCoordinate mapOffset = pPotentialUnit->getMapPosition();
	
	mapOffset -= pPotentialUnit->getPotentialSize() >> 1;

	// Get the index offset in the map's potential field
	utilities::MapCoordinate indexOffset = indexInFieldByPosition(mapOffset);

	// Iterate through the potential unit's field
	for (int y = 0; y < pPotentialUnit->getPotentialSize().y; y++)
	{
		int arrayOffsetY = (indexOffset.y + y) * mMapSize.x;
		utilities::MapCoordinate currentPos(0, mapOffset.y + y);

		for (int x = 0; x < pPotentialUnit->getPotentialSize().x; x++)
		{
			utilities::MapCoordinate currentIndex(indexOffset.x + x, indexOffset.y + y);
			if (validIndex(currentIndex))
			{
				int arrayOffset = arrayOffsetY + currentIndex.x;

				// Don't bother if the spot already is an invalid potential value
				if (mpPotentialField[arrayOffset] != INVALID_POTENTIAL_VALUE)
				{
					currentPos.x = mapOffset.x + x;
					float value = pPotentialUnit->getPotentialValue(currentPos);
					if (value != INVALID_POTENTIAL_VALUE)
					{
						mpPotentialField[arrayOffset] += value;
					}
					else
					{
						mpPotentialField[arrayOffset] = INVALID_POTENTIAL_VALUE;
					}
				}

#ifdef AI_DRAW_MAP_POTENTIAL_FIELD
				// Update the potential fields color
				D3DXCOLOR color(1.0f, 1.0f, 1.0f, 0.45f);

				// Make invalid positions red
				if (mpPotentialField[arrayOffset] == INVALID_POTENTIAL_VALUE)
				{
					color.r = 1.0f;
					color.g = 0.0f;
					color.b = 0.0f;
				}
				// Make negative values blueish
				else if (mpPotentialField[arrayOffset] < 0.0f)
				{
					float colorScale = 0.85f * mpPotentialField[arrayOffset] / AiConstants::MAP_POTENTIAL_MIN_DRAW_VALUE;
					color.r = 0.85f - colorScale;
					color.g = 0.85f - colorScale;
					color.b = 1.0f;
				}
				// Make positive values green
				else if (mpPotentialField[arrayOffset] > 0.0f)
				{
					float colorScale = 0.85f * mpPotentialField[arrayOffset] / AiConstants::MAP_POTENTIAL_MAX_DRAW_VALUE;
					color.r = 0.85f - colorScale;
					color.g = 0.85f - colorScale;
					color.b = 1.0f;
				}

				mppGfxPotentialField[arrayOffset]->setColor(color);
#endif

			}
		}
	}
}

void Map::removePotentialUnit(const logic::KeyType& keyId)
{
	std::map<logic::KeyType, const PotentialUnit*>::iterator it = mPotentialUnits.find(keyId);

	if (it != mPotentialUnits.end())
	{
		// The position we have to recalculate due to the fact that it was invalid.
		utilities::VectorList<utilities::MapCoordinate> recalculatePositions;

		const PotentialUnit* pPotentialUnit = it->second;

		// Get the mapOffset of the first coordinate in the potential unit
		utilities::MapCoordinate mapOffset = pPotentialUnit->getMapPosition();

		mapOffset -= pPotentialUnit->getPotentialSize() >> 1;

		// Get the index offset in the map's potential field
		const utilities::MapCoordinate& indexOffset = indexInFieldByPosition(mapOffset);

		// Remove the unit from the map's potential field
		for (int y = 0; y < pPotentialUnit->getPotentialSize().y; y++)
		{
			int arrayOffsetY = (indexOffset.y + y) * mMapSize.x;
			utilities::MapCoordinate currentPos(0, mapOffset.y + y);

			for (int x = 0; x < pPotentialUnit->getPotentialSize().x; x++)
			{
				currentPos.x = mapOffset.x + x;
				int arrayOffset = arrayOffsetY + x;

				float value = pPotentialUnit->getPotentialValue(currentPos);
				
				// If the value isn't invalid and the current map position isn't invalid
				// If the current value in the map's potential field is invalid that means
				// that another unit has placed the invalid value there and thus it should
				// still be invalid when we remove our unit
				if (value != INVALID_POTENTIAL_VALUE && mpPotentialField[arrayOffset] != INVALID_POTENTIAL_VALUE)
				{
					mpPotentialField[arrayOffset] -= value;
				}
				// Else if the value was invalid, set the map's value as neutral and add the
				// position to the recalculate vector list
				else if (value == INVALID_POTENTIAL_VALUE)
				{
					mpPotentialField[arrayOffset] = NEUTRAL_POTENTIAL_VALUE;

					recalculatePositions.add(currentPos);
				}
			}
		}

		// If we have positions to recalculate, recalculate them
		if (!recalculatePositions.empty())
		{
			recalculateValueOnPositions(recalculatePositions);
		}

		mPotentialUnits.erase(it);
	}
}

float Map::getPotentialValue(const utilities::MapCoordinate& mapPosition) const
{
	utilities::MapCoordinate indexInMap = mapPosition;
	indexInMap += mCenterIndex;

	// Return the value we found if we're in bounds
	if (validIndex(indexInMap))
	{
		return mpPotentialField[indexInMap.y * mMapSize.x + indexInMap.x];
	}
	// If we're out of bounds return invalid potential value
	else
	{
		return INVALID_POTENTIAL_VALUE;
	}
}

void Map::readdPotentialUnits()
{
	// Iterate through all the units and add the to the map's potential field
	std::map<logic::KeyType, const PotentialUnit*>::iterator it;
	for (it = mPotentialUnits.begin(); it != mPotentialUnits.end(); ++it)
	{
		addPotentialUnit(it->second);
	}
}

void Map::recalculateValueOnPositions(const utilities::VectorList<utilities::MapCoordinate> &positions)
{
	while (!positions.empty())
	{
		const utilities::MapCoordinate& currentPosition = positions.getFirst();
		const utilities::MapCoordinate& index = indexInFieldByPosition(currentPosition);
		int arrayOffset = index.y * mMapSize.x + index.x;

		// Iterate through all the potential units
		std::map<logic::KeyType, const PotentialUnit*>::iterator it;
		for (it = mPotentialUnits.begin(); it != mPotentialUnits.end(); ++it)
		{
			float value = it->second->getPotentialValue(currentPosition);

			// Add the value to the map's potential value
			if (value != INVALID_POTENTIAL_VALUE)
			{
				mpPotentialField[arrayOffset] += value;
			}
			// If the value is invalid we set the map's potential value to invalid
			// and skip the rest of iterations
			else
			{
				mpPotentialField[arrayOffset] = INVALID_POTENTIAL_VALUE;
				return;
			}
		}
	}
}