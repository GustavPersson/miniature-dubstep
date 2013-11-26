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
	int highestIndex = msCenterIndex * msPotentialRange.x + msCenterIndex;
	float highestValue = mpsPotentialField[highestIndex];
	int lowestIndex = (msCenterIndex - positionOffset) * msPotentialRange.x + (msCenterIndex - positionOffset);
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
	
#endif
}

float Player::getPotentialValue(const utilities::MapCoordinate& mapPosition) const
{
	utilities::MapCoordinate indexInPlayer = mapPosition - getMapPosition();

	indexInPlayer += msCenterIndex;

	float potentialValue = NEUTRAL_POTENTIAL_VALUE;

	// Return the value if the index is valid, else return INVALID
	if (validIndex(indexInPlayer))
	{
		// Use the player size + 1 in all directions for the edges.
		// When the player is outside of this area we get the value from the potential field.
		// If the position is just outside the regular area we use the same values for all
		// If the position is inside the player we return an invalid position

		int outerLeftBorder = msCenterIndex - (getSize().x >> 1) - 1;
		int outerRightBorder = msCenterIndex + (getSize().x >> 1) + 1;
		int outerUpperBorder = msCenterIndex + (getSize().y >> 1) + 1;
		int outerLowerBorder = msCenterIndex - (getSize().y >> 1) - 1;
		
		// Left, right, over or under the player, get the value directly from the potential field
		if (indexInPlayer.x < outerLeftBorder || indexInPlayer.x > outerRightBorder ||
			indexInPlayer.y < outerUpperBorder || indexInPlayer.y > outerLowerBorder)
		{
			potentialValue = mpsPotentialField[indexInPlayer.y * msPotentialRange.x + indexInPlayer.x];
		}
		// Else if we're inside return invalid potential value
		else if (indexInPlayer.x > outerLeftBorder && indexInPlayer.x < outerRightBorder &&
			indexInPlayer.y > outerLowerBorder && indexInPlayer.y < outerUpperBorder)
		{
			return INVALID_POTENTIAL_VALUE;
		}
		// Else we're just on the border, calculate the value here and apply it on all goals
		else
		{
			// Get the distance to the highest value, then get the value from that distance
			// and apply it on all coordinates just around the player
			int highestValueDistance = getSize().x < getSize().y ? (getSize().x >> 1) : (getSize().y >> 1) ;

			return mpsPotentialField[(msCenterIndex + highestValueDistance) * msCenterIndex];
		}
	}

	return potentialValue;
}