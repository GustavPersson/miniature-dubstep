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

#include "AgentAi.h"
#include "AiConstants.h"
#include "PathFinder.h"
#include "AiEvents.h"
#include "UnitHandler.h"
#include "Player.h"
#include "../logic/ActorHandler.h"

using namespace ai;
using utilities::MapCoordinate;
using logic::Enemy;

float* AgentAi::mpsPotentialField = NULL;
utilities::MapCoordinate AgentAi::msPotentialRange(0, 0);

#ifdef AI_DRAW_AGENT_GOALS
#include "../Logic/KeyType.h"
#endif

AgentAi::AgentAi(Enemy *pEnemy) : DynamicUnit(pEnemy, false), mpEnemy(pEnemy)
{
	assert(mpsPotentialField != NULL);

	mSortedGoals = NULL;
	mSortedNextGoals = NULL;
	mLastTargetTime = 0.0f;
	mUseGoals = true;
	
	mTarget = logic::INVALID_KEY;

	for (int i = PathFinder::GoalType_First; i < PathFinder::GoalType_Lim; i++)
	{
		mGoalInConflictMap[i] = false;
	}

	// Create the goal fields
	mSortedGoals = myNew PotentialSquare[GOAL_ARRAY_SIZE];
	mSortedNextGoals = myNew PotentialSquare[GOAL_ARRAY_SIZE];

	// Set the goals as invalid
	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		mSortedGoals[i].position = utilities::INVALID_MAP_COORDINATE;
		mSortedGoals[i].value = INVALID_POTENTIAL_VALUE;
	}

	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		mSortedNextGoals[i].position = utilities::INVALID_MAP_COORDINATE;
		mSortedNextGoals[i].value = INVALID_POTENTIAL_VALUE;
	}

#ifdef AI_DRAW_AGENT_GOALS
	utilities::MapCoordinate mapScaling(1,0);

	// Create blocks around the the agent for the possible goals
	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		mpGoalPositionBlocks[i] = mpsDx10Module->createObjectInstanced(1);
		mpGoalPositionBlocks[i]->setPosition(D3DXVECTOR3(0.0f, AiConstants::DRAW_HEIGHT_AGENT, 0.0f));
		mpGoalPositionBlocks[i]->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
		mpGoalPositionBlocks[i]->setScale(D3DXVECTOR3(utilities::MAP_IN_WORLD_COORDS_HALF, 0.1f, utilities::MAP_IN_WORLD_COORDS_HALF));
		mpGoalPositionBlocks[i]->setID(logic::KEY_GROUND.convertToGfxId());
	}
#endif
}

AgentAi::~AgentAi()
{
	// If we're owner of the AI
	if (getKey().isOwner())
	{
		// Removed the goals from the conflict maps if they're there
		if (getGoalInConflictMap(PathFinder::GoalType_Goal))
		{
			// Remove from conflict map
			mpsPathFinder->removeFromConflictMap(getBestGoalPosition(), getKey(), PathFinder::GoalType_Goal, getSize());
		}

		if (getGoalInConflictMap(PathFinder::GoalType_NextGoal))
		{
			// Remove from conflict map
			mpsPathFinder->removeFromConflictMap(getBestNextGoalPosition(), getKey(), PathFinder::GoalType_NextGoal, getSize());
		}
	}

	SAFE_DELETE_ARRAY(mSortedGoals);
	SAFE_DELETE_ARRAY(mSortedNextGoals);

#ifdef AI_DRAW_AGENT_GOALS
	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		mpsDx10Module->removeRenderObject(mpGoalPositionBlocks[i]);
	}
#endif
}

void AgentAi::init(int maxDistance)
{
	msPotentialRange = utilities::MapCoordinate(maxDistance, maxDistance);
	mpsPotentialField = myNew float [msPotentialRange.x * msPotentialRange.y];

	calculatePotentialField(mpsPotentialField, msPotentialRange);
}

void AgentAi::destroy()
{
	mpsUnitHandler = NULL;
	msPotentialRange = utilities::MapCoordinate(0, 0);
	SAFE_DELETE_ARRAY(mpsPotentialField);
}

void AgentAi::update()
{
	DynamicUnit::update();
	if(mUseGoals)
	{	
		// Stop if we don't have a valid goal.
		if (mSortedGoals[0].value == NEUTRAL_POTENTIAL_VALUE || mSortedGoals[0].value == INVALID_POTENTIAL_VALUE)
		{
			mpActor->setMoving(false);
		}
		// Set the direction of the agent if we haven't arrived at the goal
		else if (!arrivedAtGoal())
		{
			utilities::Vec3Float direction(getBestGoalPosition().convertToWorldCoordinates() - mpActor->getPosition());
			direction.y = 0.0f;
			direction.normalize();

			mpActor->setDirection(direction);
			mpActor->setMoving(true);

#ifdef AI_DRAW_AGENT_GOALS
			// Update position on the goal blocks
			for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
			{
				utilities::Vec3Float worldPosition = mSortedGoals[i].position.convertToWorldCoordinates();
				mpGoalPositionBlocks[i]->setPosition(D3DXVECTOR3(worldPosition.x, AiConstants::DRAW_HEIGHT_AGENT, worldPosition.z));
			}
#endif
		}
	}
}

void AgentAi::updateOnlyOwner()
{
	// Find or switch a target
	if (currentTime() - mLastTargetTime > AiConstants::AGENT_SEARCH_TARGET_TIME)
	{
		mLastTargetTime = currentTime();

		// Search for the closest target
		std::map<logic::KeyType, Player*>& players = mpsUnitHandler->getPlayers();
		std::map<logic::KeyType, Player*>::iterator playerIt;

		utilities::Vec2Float closestVector = (FLT_MAX, FLT_MAX);
		logic::KeyType closestKey = logic::INVALID_KEY;

		for (playerIt = players.begin(); playerIt != players.end(); ++playerIt)
		{
			const utilities::Vec2Float& diffVector = (getPosition() - playerIt->second->getPosition());
			if (diffVector.shorterThan(closestVector))
			{
				closestVector = diffVector;
				closestKey = playerIt->first;
			}
		}

		// If we have a valid key, set it as a target and focal point
		if (closestKey != logic::INVALID_KEY)
		{
			mTarget = closestKey;
			setFocalUnit(mTarget);
		}
	}
	if(mTarget != logic::INVALID_KEY)
	{
		logic::Actor *pTarget = logic::ActorHandler::getInstance()->getActor(mTarget);
		if(pTarget != NULL && (pTarget->getPosition() - mpActor->getPosition()).shorterThan(mpEnemy->getRange()))
		{
			mpEnemy->setFire(true);
		}
		else
		{
			mpEnemy->setFire(false);
		}
	}
	else
	{
		mpEnemy->setFire(false);
	}
	// Goals
	if(mUseGoals)
	{
		if (mSortedGoals[0].value == NEUTRAL_POTENTIAL_VALUE)
		{
			DEBUG_MESSAGE(LEVEL_LOWEST, "AgentAi::updateOnlyOwner() |" << mpActor->getKey() << " | Agent is out of bounds, calculating goal...");
			mpsPathFinder->calculateGoal(this, PathFinder::GoalType_Goal);
		}
		// If we have a goal and we arrived at the goal position set a new goal
		else if (mSortedGoals[0].value != INVALID_POTENTIAL_VALUE && arrivedAtGoal())
		{
			DEBUG_MESSAGE(LEVEL_LOWER, "AgentAi::updateOnlyOwner() | " << mpActor->getKey() << " | The agent arrived at the goal.");
			
			// TODO - Update goal
			// TODO - Update pheromones

			// Remove the goal positions from the conflict map
			if (getGoalInConflictMap(PathFinder::GoalType_Goal))
			{
				mpsPathFinder->removeFromConflictMap(getBestGoalPosition(), getKey(), PathFinder::GoalType_Goal, getSize());
			}

			// If the next goal isn't in the conflict map yet invalidate it
			if (!getGoalInConflictMap(PathFinder::GoalType_NextGoal))
			{
				invalidateGoals(PathFinder::GoalType_NextGoal);
			}

			swapGoals();
			invalidateGoals(PathFinder::GoalType_NextGoal);

			// Send events with the new goals
			logic::safeQueueEvent(logic::EventDataPtr(myNew EvtData_To_AgentAiGoals(getKey(), PathFinder::GoalType_Goal, getGoals())));
			logic::safeQueueEvent(logic::EventDataPtr(myNew EvtData_To_AgentAiGoals(getKey(), PathFinder::GoalType_NextGoal, getNextGoals())));

			// If we still don't got a goal calculate one
			if (mSortedGoals[0].value == INVALID_POTENTIAL_VALUE || mSortedGoals[0].value == NEUTRAL_POTENTIAL_VALUE)
			{
				DEBUG_MESSAGE(LEVEL_LOWEST, "AgentAi::updateOnlyOwner() | " << mpActor->getKey() << " | We didn't have a next goal, calculate a goal.");
				mpsPathFinder->calculateGoal(this, PathFinder::GoalType_Goal);
			}
			// Else we had a goal and thus add the new goal into the goal conflict map and remove it from the next goal conflict map.
			else
			{
				DEBUG_MESSAGE(LEVEL_LOWEST, "AgentAi::updateOnlyOwner() | " << mpActor->getKey() << " | We had a goal, remove the goal from the position");

				mpsPathFinder->addToConflictMap(getBestGoalPosition(), getKey(), PathFinder::GoalType_Goal, getSize());
				mpsPathFinder->removeFromConflictMap(getBestGoalPosition(), getKey(), PathFinder::GoalType_NextGoal, getSize());
			}

			DEBUG_MESSAGE(LEVEL_LOWEST, "AgentAi::updateOnlyOwner() | " << mpActor->getKey() << " | The next goal is at the position: " << getBestGoalPosition().toString());

#ifdef AI_DRAW_AGENT_GOALS
			// Update the colors
			int i = GOAL_ARRAY_SIZE-1;
			float minValue = INVALID_POTENTIAL_VALUE;
			while (i >= 0 && minValue == INVALID_POTENTIAL_VALUE)
			{
				minValue = mSortedGoals[i].value;
				i--;
			}
			float diff = mSortedGoals[0].value - minValue;

			for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
			{
				// When colorScale is 1 it should be red, 0 green
				float colorScale = 1.0f;
				// Only use calculation if diff isn't is 0.0f
				if (diff != 0.0f)
				{
					colorScale = (diff - (mSortedGoals[0].value - mSortedGoals[i].value)) / diff;
				}

				if (mSortedGoals[i].value == INVALID_POTENTIAL_VALUE)
				{
					mpGoalPositionBlocks[i]->setColor(D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f));
				}
				else
				{
					mpGoalPositionBlocks[i]->setColor(D3DXCOLOR(1.0f - colorScale, 1.0f, 1.0f - colorScale, 1.0f));
				}
			}
#endif

		}

		// If we don't have a next goal, calculate one
		if (mSortedNextGoals[0].value == INVALID_POTENTIAL_VALUE || mSortedNextGoals[0].value == NEUTRAL_POTENTIAL_VALUE)
		{
			DEBUG_MESSAGE(LEVEL_LOWEST, "AgentAi::updateOnlyOwner() | " << mpActor->getKey() << " | Calculate a next goal.");
			mpsPathFinder->calculateGoal(this, PathFinder::GoalType_NextGoal);
		}
	}
}

void AgentAi::setGoals(const PotentialSquare *newGoals)
{
	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		mSortedGoals[i] = newGoals[i];
	}
}

void AgentAi::setNextGoals(const PotentialSquare *newGoals)
{
	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		mSortedNextGoals[i] = newGoals[i];
	}
}

void AgentAi::setGoals(const PotentialSquare* newGoals, PathFinder::GoalTypes goalType)
{
	PotentialSquare* goalsToSet = NULL;
	switch (goalType)
	{
		case PathFinder::GoalType_Goal:
			goalsToSet = mSortedGoals;
			break;
			
		case PathFinder::GoalType_NextGoal:
			goalsToSet = mSortedNextGoals;
			break;

		default:
			ERROR_MESSAGE("Invalid goal type!");
			return;
	}
	
	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		goalsToSet[i] = newGoals[i];
	}
}

const PotentialSquare* AgentAi::getGoals(PathFinder::GoalTypes goalType) const
{
	switch (goalType)
	{
		case PathFinder::GoalType_Goal:
			return mSortedGoals;

		case PathFinder::GoalType_NextGoal:
			return mSortedNextGoals;

		default:
			return NULL;
	}
}

PotentialSquare* AgentAi::getGoals(PathFinder::GoalTypes goalType)
{
	switch (goalType)
	{
	case PathFinder::GoalType_Goal:
		return mSortedGoals;

	case PathFinder::GoalType_NextGoal:
		return mSortedNextGoals;

	default:
		return NULL;
	}
}

void AgentAi::invalidateGoals(PathFinder::GoalTypes goalType)
{
	PotentialSquare* goals = getGoals(goalType);

	if (goals == NULL)
	{
		return;
	}

	for (int i = 0; i < GOAL_ARRAY_SIZE; i++)
	{
		goals[i].position = utilities::INVALID_MAP_COORDINATE;
		goals[i].value = INVALID_POTENTIAL_VALUE;
	}
}

float AgentAi::getPotentialValue(const utilities::MapCoordinate& mapPosition) const
{
	utilities::MapCoordinate indexInPlayer(msPotentialRange.x >> 1, msPotentialRange.y >> 1);

	indexInPlayer -= mapPosition;

	// Return the value if the index is valid
	float value = INVALID_POTENTIAL_VALUE;
	if (validIndex(indexInPlayer))
	{
		float value = mpsPotentialField[indexInPlayer.y * msPotentialRange.x + indexInPlayer.x];

		// Iterate through all the pheromones
		for (int i = 0; i < mPheromones.size(); i++)
		{
			if (mPheromones[i].position == mapPosition)
			{
				value += mPheromones[i].value;
			}
		}
	}
	return value;
}

utilities::MapCoordinate AgentAi::getGoalOffset(PathFinder::GoalTypes goalType) const
{
	utilities::MapCoordinate offset;
	switch (goalType)
	{
		case PathFinder::GoalType_Goal:
			offset = getBestGoalPosition();
			break;
		case PathFinder::GoalType_NextGoal:
			offset = getBestNextGoalPosition();
			break;
	}

	offset.x -= getSize().x >> 1;
	offset.y -= getSize().y >> 1;

	return offset;
}

int AgentAi::nrOfValidGoals(ai::PathFinder::GoalTypes goalType) const
{
	PotentialSquare* goals = NULL;

	switch(goalType)
	{
		case PathFinder::GoalType_Goal:
			goals = mSortedGoals;
			break;

		case PathFinder::GoalType_NextGoal:
			goals = mSortedNextGoals;
			break;

		default:
			ERROR_MESSAGE("Invalid goalType!");
			return 0;
	}

	int validGoals = GOAL_ARRAY_SIZE;
	while (validGoals > 0 && goals[validGoals-1].value != INVALID_POTENTIAL_VALUE)
	{
		validGoals--;
	}

	return validGoals;
}

void AgentAi::pause()
{
	mpActor->setMoving(false);
	mpEnemy->setFire(false);

	removeGoals(PathFinder::GoalType_Goal);
	removeGoals(PathFinder::GoalType_NextGoal);
}

void AgentAi::resume()
{
	if (getKey().isOwner() && mUseGoals)
	{
		mpsPathFinder->calculateGoal(this, PathFinder::GoalType_Goal);
	}
}

void AgentAi::removeGoals(PathFinder::GoalTypes goalType)
{
	switch (goalType)
	{
		case PathFinder::GoalType_Goal:
			if (mGoalInConflictMap[goalType])
			{
				mpsPathFinder->removeFromConflictMap(getBestGoalPosition(), getKey(), goalType, getSize());
			}

			invalidateGoals(goalType);
			
			break;

		case PathFinder::GoalType_NextGoal:
			if (mGoalInConflictMap[goalType])
			{
				mpsPathFinder->removeFromConflictMap(getBestNextGoalPosition(), getKey(), goalType, getSize());
			}

			invalidateGoals(goalType);

			break;
	}
}

void AgentAi::beforeRotateChange()
{
	if (mUseGoals && getKey().isOwner())
	{
		DEBUG_MESSAGE(LEVEL_LOWER, "AgentAi::beforeRotateChange() | " << getKey() << " | Removing agent goals");

		// Remove the goals
		// Only remove the goals if we have a goal in the conflict map.
		if (getGoalInConflictMap(PathFinder::GoalType_Goal))
		{
			mpsPathFinder->removeFromConflictMap(getBestGoalPosition(), getKey(), PathFinder::GoalType_Goal, getSize());
		}

		if (getGoalInConflictMap(PathFinder::GoalType_NextGoal))
		{
			mpsPathFinder->removeFromConflictMap(getBestNextGoalPosition(), getKey(), PathFinder::GoalType_NextGoal, getSize());
		}
	}
}

void AgentAi::afterRotateChange()
{
	if (mUseGoals && getKey().isOwner())
	{
		DEBUG_MESSAGE(LEVEL_LOWER, "AgentAi::afterRotateChange() | " << getKey() << " | Adding new goals");
		// Calculate new goals
		mpsPathFinder->calculateGoal(this, PathFinder::GoalType_Goal);
		mpsPathFinder->calculateGoal(this, PathFinder::GoalType_NextGoal);
	}
}

bool AgentAi::arrivedAtGoal() const
{
	return mCurrentPos == getBestGoalPosition() || (getBestGoalPosition().convertToWorldCoordinates() - mpActor->getPosition()).shorterThan(AiConstants::GOAL_CLOSE_RANGE, false);
}

void AgentAi::sortGoals(PathFinder::GoalTypes goalType)
{
	PotentialSquare* goals = NULL;
	switch (goalType)
	{
		case PathFinder::GoalType_Goal:
			goals = mSortedGoals;
			break;
		case PathFinder::GoalType_NextGoal:
			goals = mSortedNextGoals;
			break;
	}

	// INEFFICIENT - Bubble sort
	for (int i = 0; i < GOAL_ARRAY_SIZE-1; i++)
	{
		for (int j = i+1; j < GOAL_ARRAY_SIZE; j++)
		{
			if (goals[j].value > goals[i].value)
			{
				std::swap(goals[j], goals[i]);
			}
		}
	}
}

void AgentAi::setUseGoals(bool use)
{
	mUseGoals = use;
	if(!mUseGoals)
	{
		if(getGoalInConflictMap(PathFinder::GoalType_Goal))
		{
			mpsPathFinder->removeFromConflictMap(getBestGoalPosition(), getKey(), PathFinder::GoalType_Goal, getSize());
			invalidateGoals(PathFinder::GoalType_Goal);
		}
		
		if(getGoalInConflictMap(PathFinder::GoalType_NextGoal))
		{
			mpsPathFinder->removeFromConflictMap(getBestNextGoalPosition(), getKey(), PathFinder::GoalType_NextGoal, getSize());
			invalidateGoals(PathFinder::GoalType_NextGoal);
		}
	}
	// Else - Calculate a new regular goal, only if we're the owner
	else if (getKey().isOwner())
	{
		mpsPathFinder->calculateGoal(this, PathFinder::GoalType_Goal);
	}

}