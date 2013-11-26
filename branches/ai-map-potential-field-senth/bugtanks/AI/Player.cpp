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

#include "Player.h"
#include "PathFinder.h"
#include "../Utilities/Vectors.h"

#ifdef AI_DRAW_PLAYER_POTENTIAL_FIELD
#include "AiConstants.h"
#endif

using namespace ai;

float* Player::mpsPotentialField = NULL;
utilities::MapCoordinate Player::msPotentialRange(0, 0);
int Player::msCenterIndex = 0;

Player::Player(logic::Actor *pPlayer) : DynamicUnit(pPlayer, false)
{
	assert(mpsPotentialField != NULL);

#ifdef AI_DRAW_PLAYER_POTENTIAL_FIELD
	assert(mpsDx10Module != NULL);

	mpGoalPositionBlocks = new gfx::TransformObject* [POTENTIAL_BLOCK_ARRAY_SIZE];
	
	utilities::MapCoordinate playerPos = getMapPosition();

	int positionOffset = POTENTIAL_BLOCK_WIDTH >> 1;

	// Get the highest and lowest value to scale the color
	int highestIndex = msCenterIndex * msPotentialRange + msCenterIndex;
	float highestValue = mpsPotentialField[highestIndex];
	int lowestIndex = (msCenterIndex - positionOffset) * msPotentialRange + (msCenterIndex - positionOffset);
	float lowestValue = mpsPotentialField[lowestIndex];

	float diff = highestValue - lowestValue;

	for (int y = 0; y < POTENTIAL_BLOCK_WIDTH; y++)
	{
		utilities::MapCoordinate mapPos;
		mapPos.y = playerPos.y - positionOffset + y;
		int arrayOffsetY = y * POTENTIAL_BLOCK_WIDTH;

		for (int x = 0; x < POTENTIAL_BLOCK_WIDTH; x++)
		{
			int index = arrayOffsetY + x;
			mapPos.x = playerPos.x - positionOffset + x;
			utilities::Vec3Float worldPos = mapPos.convertToWorldCoordinates();

			mpGoalPositionBlocks[index] = mpsDx10Module->createObjectInstanced(1);
			mpGoalPositionBlocks[index]->setScale(D3DXVECTOR3(utilities::MAP_IN_WORLD_COORDS_HALF, 0.1f, utilities::MAP_IN_WORLD_COORDS_HALF));
			mpGoalPositionBlocks[index]->setID(logic::KEY_GROUND.convertToGfxId());
			mpGoalPositionBlocks[index]->setPosition(D3DXVECTOR3(worldPos.x, AiConstants::DRAW_HEIGHT_PLAYER, worldPos.z));

			float colorScale = 0.65f * (diff - (highestValue - getPotentialValue(mapPos))) / diff;

			mpGoalPositionBlocks[index]->setColor(D3DXCOLOR(1.0f, 0.65f-colorScale, 0.65f-colorScale, 0.5f));
		}
	}

#endif
}

Player::~Player()
{
#ifdef AI_DRAW_PLAYER_POTENTIAL_FIELD
	for (int i = 0; i < POTENTIAL_BLOCK_ARRAY_SIZE; i++)
	{
		mpsDx10Module->removeRenderObject(mpGoalPositionBlocks[i]);
	}

	// Remove the transformObject pointer array
	SAFE_DELETE_ARRAY(mpGoalPositionBlocks);
#endif
}

void Player::init(int maxDistance)
{
	msPotentialRange = utilities::MapCoordinate(maxDistance, maxDistance);
	mpsPotentialField = myNew float [msPotentialRange.x * msPotentialRange.y];
	msCenterIndex = msPotentialRange.x >> 1;

	calculatePotentialField(mpsPotentialField, msPotentialRange);
}

void Player::destroy()
{
	msCenterIndex = 0;
	msPotentialRange = utilities::MapCoordinate(0, 0);
	SAFE_DELETE_ARRAY(mpsPotentialField);
}

void Player::unitMoved()
{
	DynamicUnit::unitMoved();

	mpsPathFinder->invalidateNextGoals();

#ifdef AI_DRAW_PLAYER_POTENTIAL_FIELD
	// update potential block positions

	utilities::MapCoordinate playerPos = getMapPosition();

	int positionOffset = POTENTIAL_BLOCK_WIDTH >> 1;

	for (int y = 0; y < POTENTIAL_BLOCK_WIDTH; y++)
	{
		utilities::MapCoordinate mapPos;
		mapPos.y = playerPos.y - positionOffset + y;
		int arrayOffsetY = y * POTENTIAL_BLOCK_WIDTH;

		for (int x = 0; x < POTENTIAL_BLOCK_WIDTH; x++)
		{
			int index = arrayOffsetY + x;
			mapPos.x = playerPos.x - positionOffset + x;
			utilities::Vec3Float worldPos = mapPos.convertToWorldCoordinates();

			mpGoalPositionBlocks[index]->setPosition(D3DXVECTOR3(worldPos.x, AiConstants::DRAW_HEIGHT_PLAYER, worldPos.z));
		}
	}

	// TODO - Use this in the map instead Iterate through all the blocks and apply colors
	//for (int i = 0; i < POTENTIAL_BLOCK_ARRAY_SIZE; i++)
	//{
	//	utilities::Vec3Float color(1.0f, 1.0f, 1.0f);

	//	// Make invalid positions red
	//	if (values[i] == INVALID_POTENTIAL_VALUE)
	//	{
	//		color.x = 1.0f;
	//		color.y = 0.0f;
	//		color.z = 0.0f;
	//	}
	//	// If it's a positive value make it red
	//	else if (values[i] > 0.0f)
	//	{
	//		float colorScale = 0.85f * (maxValue - values[i]) / maxValue;
	//		color.x = colorScale;
	//		color.y = 1.0f;
	//		color.z = colorScale;
	//	}
	//	// negative should be blue
	//	else if (values[i] < 0.0f)
	//	{
	//		float colorScale = 0.85f * ((-minValue) + values[i]) / -minValue;
	//		color.x = colorScale;
	//		color.y = colorScale;
	//		color.z = 1.0f;
	//	}
	//	mpGoalPositionBlocks[i]->setColor(D3DXCOLOR(color.x, color.y, color.z, 0.5f));
	//}
	
#endif
}

float Player::getPotentialValue(const utilities::MapCoordinate& mapPosition) const
{
	utilities::MapCoordinate indexInPlayer = mapPosition - getMapPosition();

	indexInPlayer += msCenterIndex;

	// Return the value if the index is valid, else return INVALID
	if (validIndex(indexInPlayer))
	{
		return mpsPotentialField[indexInPlayer.y * msPotentialRange.x + indexInPlayer.x];
	}
	else
	{
		return NEUTRAL_POTENTIAL_VALUE;
	}
}