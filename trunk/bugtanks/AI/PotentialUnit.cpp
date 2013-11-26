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

#include "PotentialUnit.h"
#include "PathFinder.h"
#include "AiConstants.h"
#include "UnitHandler.h"
#include "Map.h"

using namespace ai;
using logic::Actor;
using utilities::Timer;

PathFinder* PotentialUnit::mpsPathFinder = NULL;
logic::GlobalTimer* PotentialUnit::mpsGlobalTimer = NULL;
UnitHandler* PotentialUnit::mpsUnitHandler = NULL;
Map* PotentialUnit::mpsMap = NULL;

#if defined(AI_DRAW_MAP_POTENTIAL_FIELD) || \
	defined(AI_DRAW_PLAYER_POTENTIAL_FIELD) || \
	defined(AI_DRAW_AGENT_GOALS) || \
	defined(AI_DRAW_UNIT_SIZE_BLOCKS) || \
	defined(AI_DRAW_UNIT_ORIGIN_LINES)
	gfx::DX10Module* PotentialUnit::mpsDx10Module = NULL;
#endif

PotentialUnit::PotentialUnit(Actor* pActor, bool applyOnMapPotentialField)
	: mpActor(pActor), mApplyOnMapPotentialField(applyOnMapPotentialField)
{
	assert(mpActor != NULL);
	assert(mpsPathFinder != NULL);

	mActive = true;

	mRotated = checkRotated();
	updateOffsetAndUnitSize();

	mCurrentPos = getMapPosition();
	mPreviousPos = mCurrentPos;

	mLastTime = mpsGlobalTimer->getTime(Timer::ReturnType_Seconds);

#if defined(AI_DRAW_MAP_POTENTIAL_FIELD) || \
	defined(AI_DRAW_PLAYER_POTENTIAL_FIELD) || \
	defined(AI_DRAW_AGENT_GOALS) || \
	defined(AI_DRAW_UNIT_SIZE_BLOCKS) || \
	defined(AI_DRAW_UNIT_ORIGIN_LINES)
	assert(mpsDx10Module != NULL);
#endif

#ifdef AI_DRAW_UNIT_ORIGIN_LINES
	mpOriginLineX = NULL;
	mpOriginLineZ = NULL;

	// Use the height of the unit
	mpOriginLineX = mpsDx10Module->createObjectInstanced(1);
	utilities::Vec3Float worldPos = mpActor->getPosition();
	worldPos.y = AiConstants::DRAW_HEIGHT_ORIGIN;
	mpOriginLineX->setPosition(worldPos.convertToD3DXVECTOR3());
	mpOriginLineX->setScale(D3DXVECTOR3(1000.0f, 0.1f, 0.1f));
	mpOriginLineX->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.4f));
	mpOriginLineX->setID(logic::KEY_GROUND.convertToGfxId());

	mpOriginLineZ = mpsDx10Module->createObjectInstanced(1);
	mpOriginLineZ->setPosition(worldPos.convertToD3DXVECTOR3());
	mpOriginLineZ->setScale(D3DXVECTOR3(0.1f, 0.1f, 1000.0f));
	mpOriginLineZ->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.4f));
	mpOriginLineZ->setID(logic::KEY_GROUND.convertToGfxId());

	mpOffsetOriginLineX = NULL;
	mpOffsetOriginLineZ = NULL;
	mpOffsetOriginLineX = mpsDx10Module->createObjectInstanced(1);
	worldPos = getPositionWithOffset();
	worldPos.y = AiConstants::DRAW_HEIGHT_ORIGIN;
	mpOffsetOriginLineX->setPosition(worldPos.convertToD3DXVECTOR3());
	mpOffsetOriginLineX->setScale(D3DXVECTOR3(1000.0f, 0.1f, 0.1f));
	mpOffsetOriginLineX->setColor(D3DXCOLOR(0.0f, 1.0f, 0.0f, 0.4f));
	mpOffsetOriginLineX->setID(logic::KEY_GROUND.convertToGfxId());

	mpOffsetOriginLineZ = mpsDx10Module->createObjectInstanced(1);
	mpOffsetOriginLineZ->setPosition(worldPos.convertToD3DXVECTOR3());
	mpOffsetOriginLineZ->setScale(D3DXVECTOR3(0.1f, 0.1f, 1000.0f));
	mpOffsetOriginLineZ->setColor(D3DXCOLOR(0.0f, 0.0f, 1.0f, 0.4f));
	mpOffsetOriginLineZ->setID(logic::KEY_GROUND.convertToGfxId());
#endif

#ifdef AI_DRAW_UNIT_SIZE_BLOCKS
	mcPositionBlock = getSize().x * getSize().y;
	mpPositionBlocks = myNew gfx::TransformObject* [mcPositionBlock];

	// Update position blocks
	utilities::MapCoordinate offset = getMapPosition();
	utilities::MapCoordinate unitSize = getSize();
	offset.x -= unitSize.x >> 1;
	offset.y -= unitSize.y >> 1;
	utilities::MapCoordinate currentPos;
	for (int y = 0; y < unitSize.y; y++)
	{
		currentPos.y = offset.y + y;
		for (int x = 0; x < unitSize.x; x++)
		{
			currentPos.x = offset.x + x;

			utilities::Vec3Float worldPos = currentPos.convertToWorldCoordinates();
			worldPos.y = AiConstants::DRAW_HEIGHT_UNIT_SIZE;

			int arrayOffset = y * unitSize.x + x;
			mpPositionBlocks[arrayOffset] = mpsDx10Module->createObjectInstanced(1);
			mpPositionBlocks[arrayOffset]->setScale(D3DXVECTOR3(utilities::MAP_IN_WORLD_COORDS_HALF, 0.1f, utilities::MAP_IN_WORLD_COORDS_HALF));
			mpPositionBlocks[arrayOffset]->setID(logic::KEY_GROUND.convertToGfxId());
			mpPositionBlocks[arrayOffset]->setPosition(worldPos.convertToD3DXVECTOR3());

			// Set a slightly different color for the center position
			if (getMapPosition() == currentPos)
			{
				mpPositionBlocks[arrayOffset]->setColor(D3DXCOLOR(0.3f, 0.0f, 0.3f, 0.45f));
			}
			else
			{
				mpPositionBlocks[arrayOffset]->setColor(D3DXCOLOR(1.0f, 0.2f, 1.0f, 0.45f));
			}
		}
	}
	
#endif
}

PotentialUnit::~PotentialUnit()
{

#ifdef AI_DRAW_UNIT_SIZE_BLOCKS
	for (int i = 0; i < mcPositionBlock; i++)
	{
		mpsDx10Module->removeRenderObject(mpPositionBlocks[i]);
	}

	SAFE_DELETE_ARRAY(mpPositionBlocks);
#endif

#ifdef AI_DRAW_UNIT_ORIGIN_LINES
	mpsDx10Module->removeRenderObject(mpOffsetOriginLineX);
	mpsDx10Module->removeRenderObject(mpOffsetOriginLineZ);
	mpsDx10Module->removeRenderObject(mpOriginLineX);
	mpsDx10Module->removeRenderObject(mpOriginLineZ);
#endif
}

void PotentialUnit::init()
{
	mpsPathFinder = PathFinder::getInstance();
	mpsGlobalTimer = logic::GlobalTimer::getInstance();
	mpsUnitHandler = UnitHandler::getInstance();
	mpsMap = Map::getInstance();

#if defined(AI_DRAW_MAP_POTENTIAL_FIELD) || \
	defined(AI_DRAW_PLAYER_POTENTIAL_FIELD) || \
	defined(AI_DRAW_AGENT_GOALS) || \
	defined(AI_DRAW_UNIT_SIZE_BLOCKS) || \
	defined(AI_DRAW_UNIT_ORIGIN_LINES)
	mpsDx10Module = gfx::DX10Module::getInstance();
#endif
}

void PotentialUnit::update()
{
	// Update delta time
	float currentTime = mpsGlobalTimer->getTime(Timer::ReturnType_Seconds);
	mDeltaTime = currentTime - mLastTime;
	mLastTime = currentTime;

	// Update the rotation
	handleRotation();

	// Update the last position if we have moved a map coordinate and set moved to true
	utilities::MapCoordinate currentPos = getMapPosition();
	if (mCurrentPos != currentPos)
	{
		mPreviousPos = mCurrentPos;
		mCurrentPos = currentPos;
		DEBUG_MESSAGE(LEVEL_LOWEST, "PotentialUnit::update() | " << getName() << " | Map position: " << getMapPosition().toString() << ", World position: " << getPosition().toString());
		unitMoved();

#ifdef AI_DRAW_UNIT_SIZE_BLOCKS
		// Update position blocks
		utilities::MapCoordinate offset = getMapPosition();
		utilities::MapCoordinate unitSize = getSize();
		offset.x -= unitSize.x >> 1;
		offset.y -= unitSize.y >> 1;
		utilities::MapCoordinate currentPos;
		for (int y = 0; y < unitSize.y; y++)
		{
			currentPos.y = offset.y + y;
			for (int x = 0; x < unitSize.x; x++)
			{
				currentPos.x = offset.x + x;

				utilities::Vec3Float worldPos = currentPos.convertToWorldCoordinates();
				worldPos.y = AiConstants::DRAW_HEIGHT_UNIT_SIZE;

				int arrayOffset = y * unitSize.x + x;
				mpPositionBlocks[arrayOffset]->setPosition(worldPos.convertToD3DXVECTOR3());

				// Set a slightly different color for the center position
				if (getMapPosition() == currentPos)
				{
					mpPositionBlocks[arrayOffset]->setColor(D3DXCOLOR(0.3f, 0.0f, 0.3f, 0.45f));
				}
				else
				{
					mpPositionBlocks[arrayOffset]->setColor(D3DXCOLOR(1.0f, 0.2f, 1.0f, 0.45f));
				}
			}
		}

#endif
	}

#ifdef AI_DRAW_UNIT_ORIGIN_LINES
	// Update the origin lines of the unit
	utilities::Vec3Float unitPos = mpActor->getPosition();
	unitPos.y = AiConstants::DRAW_HEIGHT_ORIGIN;
	mpOriginLineX->setPosition(unitPos.convertToD3DXVECTOR3());
	mpOriginLineZ->setPosition(unitPos.convertToD3DXVECTOR3());
	utilities::Vec3Float offsetPos = getPositionWithOffset();
	offsetPos.y = AiConstants::DRAW_HEIGHT_ORIGIN;
	mpOffsetOriginLineX->setPosition(offsetPos.convertToD3DXVECTOR3());
	mpOffsetOriginLineZ->setPosition(offsetPos.convertToD3DXVECTOR3());
#endif
}

void PotentialUnit::calculatePotentialField(float *potentialField, const utilities::MapCoordinate& potentialRange, bool negative)
{
	assert(potentialRange.x > 0);
	assert(potentialRange.y > 0);

	if (potentialRange.x % 2 != 1)
	{
		ERROR_MESSAGE("potentialRange.x is not an odd value, please correct this!");
	}
	if (potentialRange.y % 2 != 1)
	{
		ERROR_MESSAGE("potentialRange.y is not an odd value, please correct this!");
	}

	utilities::MapCoordinate centerPos(potentialRange.x >> 1, potentialRange.y >> 1);

	// Calculate the max radius
	float maxRadius = sqrtf(static_cast<float>((centerPos.x+1)*(centerPos.x+1) + (centerPos.y+1)*(centerPos.y+1)));
	DEBUG_MESSAGE(LEVEL_LOWEST, "PotentialUnit::calculatePotentialField() | maxRadius: " << maxRadius);

	// Calculate the max value the unit will have
	float maxValue = sqrtf(maxRadius);
	DEBUG_MESSAGE(LEVEL_LOWEST, "PotentialUnit::calculatePotentialField() | maxValue: " << maxValue);

	// Calculate the potential field's values
	for (int y = 0; y < potentialRange.y; y++)
	{
		int yRange = abs(centerPos.y - y);

		for (int x = 0; x < potentialRange.x; x++)
		{
			int xRange = abs(centerPos.x - x);
			float distance = sqrtf(static_cast<float>(xRange*xRange + yRange*yRange));
			potentialField[y*potentialRange.x + x] = maxValue - sqrtf(distance);
		}
	}

	// Negate if we should do that
	if (negative)
	{
		for (int y = 0; y < potentialRange.y; y++)
		{
			int offset = y * potentialRange.x;
			for (int x = 0; x < potentialRange.x; x++)
			{
				potentialField[offset + x] = -potentialField[offset + x];
			}
		}
	}
}

void PotentialUnit::unitMoved()
{
	// Update the position in the map if we should be applied to it's potential field
	if (applyOnMapPotentialField())
	{
		mpsMap->updatePositionForUnit(getKey());
	}
}

void PotentialUnit::updateOffsetAndUnitSize()
{
	if (mRotated)
	{
		mCurrentUnitSize.x = mpActor->getSize().y;
		mCurrentUnitSize.y = mpActor->getSize().x;
	}
	else
	{
		mCurrentUnitSize = mpActor->getSize();
	}

	// If the unit has an equal number of x or y size then the origin will be exactly between to
	// positions, to fix this we "move" the center position on the positive angle
	// I.e. if the unit has the size of 3x2 the origin will be at 1,1 (0,0 is the start).
	// Thus move the origin MAP_IN_WORLD_COORD_HALF away from the tank
	mOriginOffset.x = (mCurrentUnitSize.x % 2 == 0) ? utilities::MAP_IN_WORLD_COORDS_HALF : 0.0f;
	mOriginOffset.z = (mCurrentUnitSize.y % 2 == 0) ? utilities::MAP_IN_WORLD_COORDS_HALF : 0.0f;
}

bool PotentialUnit::checkRotated()
{
	float angle = getFocalDirection().getXZAngleCounterclockwise();

	// Use a threshold so that we don't switch back and forth all the time
	float threshold = mRotated ? -AiConstants::ROTATED_THRESHOLD : AiConstants::ROTATED_THRESHOLD;	
	if ((angle > (utilities::math::PI_QUARTER + threshold)
		&& angle < (utilities::math::PI - utilities::math::PI_QUARTER - threshold))
		||
		(angle > (utilities::math::PI + utilities::math::PI_QUARTER + threshold)
		&& angle < utilities::math::PI_TWO - utilities::math::PI_QUARTER - threshold))
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PotentialUnit::handleRotation()
{
	bool currentlyRotated = checkRotated();
	if (currentlyRotated != mRotated)
	{
		beforeRotateChange();
		mRotated = currentlyRotated;
		updateOffsetAndUnitSize();
		afterRotateChange();
	}
}


std::istream& operator>>(std::istream& in, ai::PotentialSquare& potentialSquare)
{
	in >> potentialSquare.position >> potentialSquare.value;
	return in;
}

std::ostream& operator<<(std::ostream& out, const ai::PotentialSquare& potentialSquare)
{
	out << potentialSquare.position << " " << potentialSquare.value << " ";
	return out;
}
