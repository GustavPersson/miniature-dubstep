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

#include "PathFinder.h"
#include "AgentAi.h"
#include "UnitHandler.h"
#include "Map.h"
#include "Player.h"
#include "../Utilities/Vector2D.h"
#include "AiEvents.h"
#include "Map.h"

using namespace ai;

PathFinder* PathFinder::mpsInstance = NULL;

const std::string DEFINED = "AgentAi";
const std::string FILE_PATH = "../Logs/AgentAi_";

PathFinder::PathFinder()
{
	mpUnitHandler = NULL;
	mpMap = NULL;

	mpUnitHandler = UnitHandler::getInstance();
	mpMap = Map::getInstance();
}

PathFinder* PathFinder::getInstance()
{
	if (mpsInstance == NULL)
	{
		mpsInstance = myNew PathFinder();
	}

	return mpsInstance;
}

PathFinder::~PathFinder()
{
	clear();
	mpsInstance = NULL;
}

void PathFinder::clear()
{
	mAgentGoalPositions.clear();
	mAgentNextGoalPositions.clear();

	// Clear the queues
	while (!mGoalConflictQueue.empty())
	{
		mGoalConflictQueue.pop();
	}

	while (!mNextGoalConflictQueue.empty())
	{
		mNextGoalConflictQueue.pop();
	}
}

void PathFinder::restart()
{
	clear();
}

void PathFinder::update(float deltaTime)
{
	resolveConflicts();
}

void PathFinder::addToConflictMap(const utilities::MapCoordinate& goalPosition, const logic::KeyType& keyId, GoalTypes goalType, const utilities::MapCoordinate& unitSize)
{
	// Create an event and forward the parameters to the internal function
	addToConflictMapFromEvent(goalPosition, keyId, goalType, unitSize);
	logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew EvtData_To_AiAddToConflictMap(goalPosition, keyId, goalType, unitSize)));
}

void PathFinder::removeFromConflictMap(const utilities::MapCoordinate& goalPosition, const logic::KeyType& keyId, GoalTypes goalType)
{
	// Create an event and forward the parameters to the internal function
	removeFromConflictMapFromEvent(goalPosition, keyId, goalType);
	logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew EvtData_To_AiRemoveFromConflictMap(goalPosition, keyId, goalType)));
}

void PathFinder::addToConflictMapFromEvent(const utilities::MapCoordinate& goalPosition, const logic::KeyType& keyId, GoalTypes goalType, const utilities::MapCoordinate& unitSize)
{
	// Get the agent
	AgentAi* pAgentAi = mpUnitHandler->getAgentAi(keyId);
	if (pAgentAi == NULL)
	{
		DEBUG_MESSAGE(LEVEL_LOW, "PathFinder::addToConflictMap() | Didn't find the specified actor in the UnitHandler when checking collisions, probably died along the way!");
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "PathFinder::addToConflictMap() | Didn't find the specified actor in the UnitHandler when checking collisions, probably died along the way!");
		return;
	}

	// Fetch conflict info
	const AgentAi::ConflictInfo& conflictInfo = pAgentAi->getConflictInfo(goalType);

	// check so that we don't got a goal already so we don't "add" two
	if (conflictInfo.inConflictMap)
	{
		DEBUG_MESSAGE(LEVEL_LOWER, "Pathfinder::addToConflictMap() | Already got a goal in the conflict map, skipping to add another one");
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "Pathfinder::addToConflictMap() | Already got a goal in the conflict map, skipping to add another one");
		return;
	}

	DEBUG_MESSAGE(LEVEL_LOWER, "PathFinder::addToConflictMap(" << goalPosition.toString() << ", " << keyId << ", " << goalType << ", " << unitSize.toString() << ")");
	DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "PathFinder::addToConflictMap(" << goalPosition.toString() << ", " << keyId << ", " << goalType << ", " << unitSize.toString() << ")");
	// Don't add invalid coordinate, print error
	if (goalPosition == utilities::INVALID_MAP_COORDINATE)
	{
		ERROR_MESSAGE("Tried to add an INVALID_MAP_COORDINATE to the conflict map! KeyId: " << keyId << ", GoalType: " << goalType);
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "---ERROR--- Tried to add an INVALID_MAP_COORDINATE to the conflict map! GoalType: " << goalType);
		return;
	}

	// If there is a conflict
	logic::KeyType conflictKey = checkForConflicts(goalPosition, unitSize, goalType);
	if (conflictKey != logic::INVALID_KEY)
	{
		DEBUG_MESSAGE(LEVEL_LOWEST, "PathFinder::addToConflictMap() | Found a conflict with: " << conflictKey);
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "PathFinder::addToConflictMap() | Found a conflict with: " << conflictKey);
		// If we own the lowest id add it to the conflict queue
		logic::KeyType lowestId = conflictKey < keyId ? conflictKey : keyId;
		if (lowestId.isOwner())
		{
			DEBUG_MESSAGE(LEVEL_LOWEST, "PathFinder::addToConflictMap() | We own the lowest id, add to conflict queue");
			DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "PathFinder::addToConflictMap() | We own the lowest id, add to conflict queue");

			// Get the agent
			//AgentAi* pFirstAgent = mpUnitHandler->getAgentAi(conflictKey);
			//if (pFirstAgent != NULL)
			//{
			//	resolveConflict(pFirstAgent, pAgentAi, goalType);
			//}
			switch (goalType)
			{
				case GoalType_Goal:
					mGoalConflictQueue.push(keyId);
					break;
				case GoalType_NextGoal:
					mNextGoalConflictQueue.push(keyId);
					break;
			}
		}
	}
	// Else - Add the agent to the conflict goal map
	else
	{
		pAgentAi->setConflictInfo(goalPosition, unitSize, goalType);

		std::map<utilities::MapCoordinate, logic::KeyType>& goalPositions = getConflictMap(goalType);

		utilities::MapCoordinate offset = goalPosition;
		offset -= unitSize >> 1;
		utilities::MapCoordinate currentPos;
		for (int y = 0; y < unitSize.y; y++)
		{
			currentPos.y = offset.y + y;
			for (int x = 0; x < unitSize.x; x++)
			{
				currentPos.x = offset.x + x;

				goalPositions[currentPos] = keyId;

				DEBUG_MESSAGE(LEVEL_LOWEST, "PathFinder::addToConflictMap(" << currentPos.toString() << ", " << keyId << ", " << goalType << ") | ADDING THIS to conflict map.");
				DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "PathFinder::addToConflictMap(" << currentPos.toString() << ", " << keyId << ", " << goalType << ") | ADDING THIS to conflict map.");
			}
		}
	}
}

void PathFinder::removeFromConflictMapFromEvent(const utilities::MapCoordinate &goalPosition, const logic::KeyType& keyId, GoalTypes goalType)
{
	DEBUG_MESSAGE(LEVEL_LOWER, "PathFinder::removeFromConflictMap(" << goalPosition.toString() << ", " << keyId << ", " << goalType << ")");
	DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "PathFinder::removeFromConflictMap(" << goalPosition.toString() << ", " << keyId << ", " << goalType << ")");

	AgentAi* pAgentAi = mpUnitHandler->getAgentAi(keyId);

	// Skip if the agent was deleted. Because then it was already removed from the conflict map
	if (pAgentAi == NULL)
	{
		return;
	}

	// Get conflict info
	const AgentAi::ConflictInfo& conflictInfo = pAgentAi->getConflictInfo(goalType);

	// Check to make sure that we don't got another goal position in the conflict map.
	// This could occur when we are using the network
	// If this is true, just skip removing the goal position.
	// Also skip if we actually haven't added the position to the conflict map
	if (conflictInfo.conflictPosition != goalPosition || conflictInfo.inConflictMap == false)
	{
		return;
	}

	pAgentAi->unsetInConflict(goalType);

	// Use the right map
	std::map<utilities::MapCoordinate, logic::KeyType>& goalPositions = getConflictMap(goalType);

	// Find all the goals and check if we it contains the right key
	std::map<utilities::MapCoordinate, logic::KeyType>::iterator it;

	utilities::MapCoordinate offset = goalPosition;
	offset -= conflictInfo.unitSize >> 1;

	utilities::MapCoordinate currentPos;
	for (int y = 0; y < conflictInfo.unitSize.y; y++)
	{
		currentPos.y = offset.y + y;

		for (int x = 0; x < conflictInfo.unitSize.x; x++)
		{
			currentPos.x = offset.x + x;

			it = goalPositions.find(currentPos);

			DEBUG_MESSAGE(LEVEL_LOWER, "PathFinder::removeFromConflictMap(" << currentPos.toString() << ", " << keyId << ", " << goalType << ") | REMOVING THIS from the conflict map");
			DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << keyId << ".log", "PathFinder::removeFromConflictMap(" << currentPos.toString() << ", " << keyId << ", " << goalType << ") | REMOVING THIS from the conflict map");


			// Remove the goal if we found it
			if (it != goalPositions.end())
			{
				if (it->second == keyId)
				{
					goalPositions.erase(it);
				}
				// Else it's not the right key, print error
				else
				{
					ERROR_MESSAGE("Couldn't remove the goal position from the conflict map, wrong owner of the goal position! Pos: " << currentPos.toString());
				}
			}
			// Else print an error
			else
			{
				ERROR_MESSAGE("Couldn't remove the goal position from the conflict map, didn't find the specified location! Pos: " << currentPos.toString());
			}
		}
	}
}

void PathFinder::calculateGoal(AgentAi *pAgentAi, GoalTypes goalType)
{
	// Calculate all the possible goals for the agent

	// Get all the possibly goals on and around the agent
	utilities::MapCoordinate agentPos;
	PotentialSquare* goalField = NULL;
	
	// Set appropriate agentPos depending on what we have for goal type
	switch (goalType)
	{
		case GoalType_Goal:
			agentPos = pAgentAi->getMapPosition();
			goalField = pAgentAi->getGoals();
			break;
		case GoalType_NextGoal:
			agentPos = pAgentAi->getBestGoalPosition();
			goalField = pAgentAi->getNextGoals();
			break;
	}

	/*************************************************************************************
							DESCRIPTION FOR GOAL CALCULATING
							--------------------------------

	The goal size is always 3x3 and the unit is always treated as a 1x1.
	Below is the unit size description and where our center position is.
	If we have a 3x2 unit size then the goal for position (0,0) is calculated using
	the (0,0) + the goals around them


		1x1				2x1					3x2										3x2
	| 0 0 0 |		| 0 0 0 0 |			| 0 0 0 0 0 |							| g g g 0 0 |
	| 0 # 0 |		| 0 x # 0 |			| 0 x x x 0 |  Goal Calc for (0,0) (G)	| g G g x 0 |
	| 0 0 0 |		| 0 0 0 0 |			| 0 x # x 0 |  ---------------------->	| 0 x # x 0 |
										| 0 0 0 0 0 |							| 0 0 0 0 0 |

	x = a square where the unit is located
	# = the middle position of the unit
	0 = unoccupied square
	G = the actual goal position
	g = the surrounding positions we take into account when calculating G.

	-----

	Algorithm for calculating goal with the surrounding goals, depending on the unit size:

	We have the goalPos (can be extracted from agentPos)
	Calculate the offset for top left goal. (goalPos.x -= unitSize.x >> 1;)
	Iterate through all the goal positions and sum the goals.

	>> 1 is equal to division by two on ints.

	**************************************************************************************/

	utilities::MapCoordinate unitSize = pAgentAi->getSize();
	utilities::MapCoordinate goalPos = agentPos;
	goalPos.x -= 1;
	goalPos.y -= 1;

	utilities::MapCoordinate currentMapPosition;

	// Set the right position
	for (int y = 0; y < AgentAi::GOAL_WIDTH; y++)
	{
		currentMapPosition.y = goalPos.y + y;
		int arrayOffset = y * AgentAi::GOAL_WIDTH;

		for (int x = 0; x < AgentAi::GOAL_WIDTH; x++)
		{
			currentMapPosition.x = goalPos.x + x;

			goalField[arrayOffset + x].position = currentMapPosition;
			goalField[arrayOffset + x].value = NEUTRAL_POTENTIAL_VALUE;
		}
	}

	// Add the values from the map
	addPotentialValueFromMapToField(pAgentAi, goalField);

	// Iterate through all the possible players positions
	std::map<logic::KeyType, Player*>& players = mpUnitHandler->getPlayers();
	std::map<logic::KeyType, Player*>::iterator playerIt;
	for (playerIt = players.begin(); playerIt != players.end(); ++playerIt)
	{
		addPotentialValueFromPlayerToField(playerIt->second, pAgentAi, goalField);
	}

	// Add pheromone values
	addPotentialValueFromPheromones(pAgentAi, goalType, goalField);

	pAgentAi->sortGoals(goalType);

#ifdef _DEBUG
	std::string goalString;
	// Only print the 3 best goals
	for (int i = 0; i < AiConstants::GOAL_WIDTH; i++)
	{
		goalString += goalField[i].toString() + ", ";
	}
	DEBUG_MESSAGE_FILE("AgentAi", FILE_PATH << pAgentAi->getKey() << ".log", "PathFinder::calculateGoal() | " << goalType << " | New goals: " << goalString);
#endif

	// Send goals through events
	logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew EvtData_To_AgentAiGoals(pAgentAi->getKey(), goalType, pAgentAi->getConflictInfo(goalType).conflictPosition, pAgentAi->getGoals(goalType))));

	// Only add goals to the conflict map if they aren't neutral and not invalid
	if (goalField[0].value != NEUTRAL_POTENTIAL_VALUE && goalField[0].value != INVALID_POTENTIAL_VALUE)
	{
		// Add to conflict map
		addToConflictMap(goalField[0].position, pAgentAi->getKey(), goalType, unitSize);
	}
}

void PathFinder::invalidateNextGoals()
{
	// Iterate through all the agents
	std::map<logic::KeyType, AgentAi*>& agentsAis = mpUnitHandler->getAgentAis();
	std::map<logic::KeyType, AgentAi*>::iterator it;

	for (it = agentsAis.begin(); it != agentsAis.end(); ++it)
	{
		if (it->first.isOwner())
		{
			it->second->removeGoals(GoalType_NextGoal);
		}
	}
}

void PathFinder::addPlayer(const Player* pPlayer, bool buildField)
{
	if (buildField)
	{
		invalidateNextGoals();
	}
}

void PathFinder::removePlayer(const logic::KeyType& keyId, bool recalculateGoals)
{
	if (recalculateGoals)
	{
		invalidateNextGoals();
	}
}

void PathFinder::calculateGoalOnPositionWithPlayer(
	const utilities::MapCoordinate& goalPosition,
	const AgentAi* pAgentAi,
	const Player* pPlayer,
	PotentialSquare* pPotentialSquare) const
{
	// Skip the whole calculation if we already found out that the
	// goal is invalid
	if (pPotentialSquare->value == INVALID_POTENTIAL_VALUE) {
		return;
	}

	utilities::MapCoordinate currentGetGoalValuePosition;
	const utilities::MapCoordinate unitSize = pAgentAi->getSize();

	// If the position is too close to the player set invert the goals values.
	// I.e. stronger the further away you get from the enemy.
	float tooCloseRange = pAgentAi->getTooCloseRange();
	const utilities::Vec3Float& diffVec = goalPosition.convertToWorldCoordinates() - pPlayer->getPositionWithOffset();
	float totalValue = NEUTRAL_POTENTIAL_VALUE;
	// Using a counter since it's more efficient than multiplying two integers.
	int counter = 0;

	if (tooCloseRange != 0.0f && diffVec.shorterThan(tooCloseRange))
	{
		//normVec = diffVec.normalize()
		//thresholdVec = normVec * tooCloseRange
		//distanceOut = (tresholdVec - goalPos).length
		//fetchGoalPos = tresholdVec + (normVec * distanceOut)
		utilities::Vec3Float normVec = diffVec;
		normVec.normalize();
		utilities::Vec3Float thresholdPos = pPlayer->getPositionWithOffset() + (normVec * tooCloseRange);
		float distanceOut = (thresholdPos - goalPosition.convertToWorldCoordinates()).length();
		utilities::MapCoordinate fetchGoalPosOffset = (thresholdPos + (normVec * distanceOut)).convertToMapCoordinates();

		for (int y = 0; y < unitSize.y; y++)
		{
			currentGetGoalValuePosition.y = fetchGoalPosOffset.y + y;
			for (int x = 0; x < unitSize.x; x++)
			{
				currentGetGoalValuePosition.x = fetchGoalPosOffset.x + x;

				// The player's own generated field
				float value = pPlayer->getPotentialValue(currentGetGoalValuePosition);
				if (value != INVALID_POTENTIAL_VALUE)
				{
					totalValue += value;
				}
				// If the position isn't valid then we set the whole goal as invalid
				else
				{
					pPotentialSquare->value = INVALID_POTENTIAL_VALUE;
					return;
				}

				counter++;
			}
		}
	}
	else
	{
		utilities::MapCoordinate offset = goalPosition;
		offset -= unitSize >> 1;

		for (int y = 0; y < unitSize.y; y++)
		{
			currentGetGoalValuePosition.y = offset.y + y;
			for (int x = 0; x < unitSize.x; x++)
			{
				currentGetGoalValuePosition.x = offset.x + x;

				// The player's own generated field
				float value = pPlayer->getPotentialValue(currentGetGoalValuePosition);
				if (value != INVALID_POTENTIAL_VALUE)
				{
					totalValue += value;
				}
				// If the position isn't valid then we set the whole goal as invalid
				else
				{
					pPotentialSquare->value = INVALID_POTENTIAL_VALUE;
					return;
				}

				counter++;
			}
		}
	}

	// Divide the value with the counter
	totalValue /= static_cast<float>(counter);
	pPotentialSquare->value += totalValue;
}

void PathFinder::calculateGoalOnPositionWithMap(
	const utilities::MapCoordinate& goalPosition,
	const AgentAi* pAgentAi,
	PotentialSquare* pPotentialSquare) const
{
	// Skip the whole calculation if we already found out that the
	// goal is invalid
	if (pPotentialSquare->value == INVALID_POTENTIAL_VALUE) {
		return;
	}

	utilities::MapCoordinate currentGetGoalValuePosition;
	utilities::MapCoordinate offset = goalPosition;
	const utilities::MapCoordinate unitSize = pAgentAi->getSize();

	offset -= unitSize >> 1;

	float totalValue = 0.0f;
	// Using a counter since it's more efficient than multiplying two integers.
	int counter = 0;
	for (int y = 0; y < unitSize.y; y++)
	{
		currentGetGoalValuePosition.y = offset.y + y;
		for (int x = 0; x < unitSize.x; x++)
		{
			currentGetGoalValuePosition.x = offset.x + x;

			// The player's own generated field
			float value = mpMap->getPotentialValue(currentGetGoalValuePosition);
			if (value != INVALID_POTENTIAL_VALUE)
			{
				totalValue += value;
			}
			// If the position isn't valid then we set the whole goal as invalid
			else
			{
				pPotentialSquare->value = INVALID_POTENTIAL_VALUE;
				return;
			}

			counter++;
		}
	}

	// Divide the value with the counter
	totalValue /= static_cast<float>(counter);
	pPotentialSquare->value += totalValue;
}

logic::KeyType PathFinder::checkForConflicts(const utilities::MapCoordinate& mapPosition, const utilities::MapCoordinate& unitSize, GoalTypes goalType) const
{
	const utilities::MapCoordinate& offset = mapPosition - (unitSize >> 1);

	utilities::MapCoordinate currentPos;

	const std::map<utilities::MapCoordinate, logic::KeyType>& pGoalPos = getConflictMap(goalType);
	std::map<utilities::MapCoordinate, logic::KeyType>::const_iterator it;
	for (int y = 0; y < unitSize.y; y++)
	{
		currentPos.y = offset.y + y;
		for (int x = 0; x < unitSize.x; x++)
		{
			currentPos.x = offset.x + x;

			// Return the other agent's keyId if we found one at the same position
			it = pGoalPos.find(currentPos);
			if (it != pGoalPos.end())
			{
				return it->second;
			}
		}
	}

	return logic::INVALID_KEY;
}

void PathFinder::resolveConflicts()
{
	AgentAi* pFirstAgent = NULL;
	AgentAi* pConflictingAgent = NULL;
	//std::map<utilities::MapCoordinate, logic::KeyType>::iterator keyIt;

	while (!mGoalConflictQueue.empty())
	{
		// Get the conflicting agent
		pConflictingAgent = mpUnitHandler->getAgentAi(mGoalConflictQueue.front());
		// If we have deleted the conflicting agent in the meanwhile, pop the queue and continue
		if (pConflictingAgent == NULL)
		{
			mGoalConflictQueue.pop();
			continue;
		}

		// Only continue with the goal calculation if we don't have a goal in the conflict map
		if (!pConflictingAgent->getConflictInfo(GoalType_Goal).inConflictMap && pConflictingAgent->getBestGoalPosition() != utilities::INVALID_MAP_COORDINATE)
		{
			// Find the first agent
			logic::KeyType keyId = checkForConflicts(pConflictingAgent->getBestGoalPosition(), pConflictingAgent->getSize(), GoalType_Goal);
			
			if (keyId != logic::INVALID_KEY)
			{
				pFirstAgent = mpUnitHandler->getAgentAi(keyId);
				if(pFirstAgent != NULL)
				{
					//assert(pFirstAgent != NULL);

					// If both the conflicting agent and the first agent is the same, print an error message and assert
					if (pFirstAgent == pConflictingAgent)
					{
						ERROR_MESSAGE("Both the conflicting and first agent is the same agent. This should never happen!");
						//assert (pFirstAgent != pConflictingAgent);
					}

					resolveConflict(pFirstAgent, pConflictingAgent, GoalType_Goal);
				}
			}
			// If the conflict was resolved we could just try to add the goal again to the conflict map
			else
			{
				addToConflictMap(pConflictingAgent->getBestGoalPosition(), pConflictingAgent->getKey(), GoalType_Goal, pConflictingAgent->getSize());
			}
		}

		// Pop the queue
		mGoalConflictQueue.pop();
	}

	while (!mNextGoalConflictQueue.empty())
	{
		// Get the conflicting agent
		pConflictingAgent = mpUnitHandler->getAgentAi(mNextGoalConflictQueue.front());

		// If we have deleted the conflicting agent in the meanwhile, pop the queue and continue
		if (pConflictingAgent == NULL)
		{
			mNextGoalConflictQueue.pop();
			continue;
		}

		// Only continue with the goal calculation if we don't have a goal in the conflict map
		// And our best position isn't an invalid one
		if (!pConflictingAgent->getConflictInfo(GoalType_NextGoal).inConflictMap && pConflictingAgent->getBestNextGoalPosition() != utilities::INVALID_MAP_COORDINATE)
		{
			// Find the first agent
			logic::KeyType keyId = checkForConflicts(pConflictingAgent->getBestNextGoalPosition(), pConflictingAgent->getSize(), GoalType_NextGoal);

			if (keyId != logic::INVALID_KEY)
			{
				pFirstAgent = mpUnitHandler->getAgentAi(keyId);
				if(pFirstAgent != NULL)
				{
					//assert(pFirstAgent != NULL);

					// If both the conflicting agent and the first agent is the same, print an error message and assert
					if (pFirstAgent == pConflictingAgent)
					{
						ERROR_MESSAGE("Both the conflicting and first agent is the same agent. This should never happen!");
						//assert (pFirstAgent != pConflictingAgent);
					}

					resolveConflict(pFirstAgent, pConflictingAgent, GoalType_NextGoal);
				}
			}
			// If the conflict was resolved we could just try to add the goal again to the conflict map
			else
			{
				addToConflictMap(pConflictingAgent->getBestNextGoalPosition(), pConflictingAgent->getKey(), GoalType_NextGoal, pConflictingAgent->getSize());
			}
		}

		// Pop the queue
		mNextGoalConflictQueue.pop();
	}
}

void PathFinder::resolveConflict(AgentAi *pFirstAgent, AgentAi *pConflictingAgent, GoalTypes goalType)
{
	AgentAi* pNewAgent = NULL;
	AgentAi* pNotNewAgent = NULL;

	calculateNewAgent(pFirstAgent, pConflictingAgent, goalType, pNewAgent, pNotNewAgent);

	// If the new and not new agents are null, return
	if (pNewAgent == NULL || pNotNewAgent == NULL)
	{
		return;
	}

	PotentialSquare* pNewGoals = pNewAgent->getGoals(goalType);
	PotentialSquare* pNotNewGoals = pNotNewAgent->getGoals(goalType);

	// Remove the best goal and move all goals one step
	int i = 0;
	utilities::MapCoordinate removedGoalPos = pNewGoals[0].position;
	while (i < AgentAi::GOAL_ARRAY_SIZE-1 && pNewGoals[0].value != INVALID_POTENTIAL_VALUE && pNewGoals[0].value != NEUTRAL_POTENTIAL_VALUE)
	{
		pNewGoals[i] = pNewGoals[i+1];
		i++;
	}

	// Invalidate the last goal
	pNewGoals[i].position = utilities::INVALID_MAP_COORDINATE;
	pNewGoals[i].value = INVALID_POTENTIAL_VALUE;

	// Send the updated goals, via events
	logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew EvtData_To_AgentAiGoals(pNewAgent->getKey(), goalType, pNewAgent->getConflictInfo(goalType).conflictPosition, pNewGoals)));

	// If we changed the agent in the goal conflict map it should be removed before the
	// conflicting agent can add it to the conflict map. I.e. the conflicting agent got 
	// the first agent's goal position.
	// Thus we want the notNewGoal position (conflicting agent) but the newGoal key
	// (first agent)
	if (pNewAgent == pFirstAgent)
	{
		removeFromConflictMap(removedGoalPos, pNewAgent->getKey(), goalType);

		// Now we can safely add the conflicting goals
		addToConflictMap(pNotNewGoals[0].position, pNotNewAgent->getKey(), goalType, pNotNewAgent->getSize());
	}

	// If we have a valid goal, add it
	if (pNewGoals[0].value != INVALID_POTENTIAL_VALUE && pNewGoals[0].value != NEUTRAL_POTENTIAL_VALUE)
	{
		addToConflictMap(pNewGoals[0].position, pNewAgent->getKey(), goalType, pNewAgent->getSize());
	}
}

void PathFinder::calculateNewAgent(AgentAi* const pFirstAgent, AgentAi* const pConflictingAgent, GoalTypes goalType, AgentAi*& pNewAgent, AgentAi*& pNotNewAgent) const
{
	DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pFirstAgent->getKey() << ".log", "PathFinder::calculateNewAgent | " << pConflictingAgent->getKey() << " wants our position");
	DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pConflictingAgent->getKey() << ".log", "PathFinder::calculateNewAgent | We want " << pFirstAgent->getKey() << " position");


	const PotentialSquare* pFirstGoals = pFirstAgent->getGoals(goalType);
	const PotentialSquare* pConflictingGoals = pConflictingAgent->getGoals(goalType);

	pNewAgent = NULL;
	pNotNewAgent = NULL;

	// Check so that we actually got goals with both, if we don't return
	if(pFirstGoals[0].value == INVALID_POTENTIAL_VALUE || pFirstGoals[0].value == INVALID_POTENTIAL_VALUE)
	{
		return;
	}

	// If an agent has less than 6 goals, calculate a new goal for the one with most goals
	if (pConflictingGoals[AiConstants::PRIORITIZE_LESS_THAN_VALID_GOALS].value == INVALID_POTENTIAL_VALUE ||
		pFirstGoals[AiConstants::PRIORITIZE_LESS_THAN_VALID_GOALS].value == INVALID_POTENTIAL_VALUE)
	{
		if (pConflictingAgent->nrOfValidGoals(goalType) < pFirstAgent->nrOfValidGoals(goalType))
		{
			pNewAgent = pFirstAgent;
			pNotNewAgent = pConflictingAgent;
		}
		else
		{
			pNewAgent = pConflictingAgent;
			pNotNewAgent = pFirstAgent;
		}
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Had MORE +++ goals, changing goals");
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNotNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Had LESS --- goals, keeping goals");
	}
	// If the conflicting agent's second goal is better than the first agent's, the conflicting will get a new goal.
	else if (pConflictingGoals[1].value > pFirstGoals[1].value)
	{
		pNewAgent = pConflictingAgent;
		pNotNewAgent = pFirstAgent;	
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Had BETTER second goal +++, changing goals");;
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNotNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Had WORSE second goal ---, keeping goals");

	}
	// Else if the first agent's second goal is better than the conflicting agent's, the first agent will get a new goal.
	else if (pFirstGoals[1].value > pConflictingGoals[1].value)
	{
		pNewAgent = pFirstAgent;
		pNotNewAgent = pConflictingAgent;
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Had BETTER second goal +++, changing goals");;
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNotNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Had WORSE second goal ---, keeping goals");
	}
	// Second goal value is equal, prioritize the one that's closest.
	else
	{
		if ((pFirstAgent->getPositionWithOffset() - pFirstGoals[0].position.convertToWorldCoordinates()).longerThan(
			(pConflictingAgent->getPositionWithOffset() - pFirstGoals[0].position.convertToWorldCoordinates(), false)))
		{
			pNewAgent = pFirstAgent;
			pNotNewAgent = pConflictingAgent;
		}
		else
		{
			pNewAgent = pConflictingAgent;
			pNotNewAgent = pFirstAgent;
		}
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Were further away from the goal +++, changing goals");;
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNotNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | Were closer to the goal ---, keeping goals");
	}

	// Check so that we can remove the goal, i.e. has more or equal to 2 goals.
	// If we have less, swap to the other agent.
	if (pNewAgent->getGoals(goalType)[1].value == INVALID_POTENTIAL_VALUE)
	{
		std::swap(pNewAgent, pNotNewAgent);
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | The other had only had one valid goal, changing goals +++");;
		DEBUG_MESSAGE_FILE(DEFINED, FILE_PATH << pNotNewAgent->getKey() << ".log", "Pathfinder::calculateNewAgent | We only have one goal, keeping goals ---");
	}
}

void PathFinder::addPotentialValueFromPlayerToField(
	const Player* pPlayer,
	const AgentAi* pAgentAi,
	PotentialSquare* goalField) const
{	
	utilities::MapCoordinate currentMapPosition;
	utilities::MapCoordinate& offset = goalField[0].position;

	for (int y = 0; y < AgentAi::GOAL_WIDTH; y++)
	{
		currentMapPosition.y = offset.y + y;
		int goalArrayOffset = y * AgentAi::GOAL_WIDTH;

		for (int x = 0; x < AgentAi::GOAL_WIDTH; x++)
		{
			currentMapPosition.x = offset.x + x;

			calculateGoalOnPositionWithPlayer(currentMapPosition, pAgentAi, pPlayer, &goalField[goalArrayOffset + x]);
		}
	}
}

void PathFinder::addPotentialValueFromMapToField(const AgentAi* pAgentAi, PotentialSquare* goalField) const
{
	utilities::MapCoordinate currentMapPosition;
	utilities::MapCoordinate& offset = goalField[0].position;

	for (int y = 0; y < AgentAi::GOAL_WIDTH; y++)
	{
		currentMapPosition.y = offset.y + y;
		int goalArrayOffset = y * AgentAi::GOAL_WIDTH;

		for (int x = 0; x < AgentAi::GOAL_WIDTH; x++)
		{
			currentMapPosition.x = offset.x + x;

			calculateGoalOnPositionWithMap(currentMapPosition, pAgentAi, &goalField[goalArrayOffset + x]);
		}
	}
}

void PathFinder::addPotentialValueFromPheromones(const ai::AgentAi *pAgentAi, ai::PathFinder::GoalTypes goalType, ai::PotentialSquare *goalField) const
{
	utilities::MapCoordinate currentMapPosition;
	utilities::MapCoordinate& offset = goalField[0].position;

	for (int y = 0; y < AgentAi::GOAL_WIDTH; y++)
	{
		currentMapPosition.y = offset.y + y;
		int goalArrayOffsetY = y * AgentAi::GOAL_WIDTH;

		for (int x = 0; x < AgentAi::GOAL_WIDTH; x++)
		{
			currentMapPosition.x = offset.x + x;
			int arrayOffset = goalArrayOffsetY + x;

			if (goalField[arrayOffset].value != INVALID_POTENTIAL_VALUE)
			{
				// Add a negative value if we have been here before
				switch (goalType)
				{
					case GoalType_Goal:
						if (currentMapPosition == pAgentAi->getLastMapPosition())
						{
							goalField[arrayOffset].value -= AiConstants::PHEROMONE_START_VALUE;
						}
						else if (currentMapPosition == pAgentAi->getPenultimateMapPosition())
						{
							goalField[arrayOffset].value -= AiConstants::PHEROMONE_START_VALUE - AiConstants::PHEROMONE_DECREASE_VALUE;
						}
						break;

					case GoalType_NextGoal:
						if (currentMapPosition == pAgentAi->getBestGoalPosition())
						{
							goalField[arrayOffset].value -= AiConstants::PHEROMONE_START_VALUE;
						}
						if (currentMapPosition == pAgentAi->getLastMapPosition())
						{
							goalField[arrayOffset].value -= AiConstants::PHEROMONE_START_VALUE - AiConstants::PHEROMONE_DECREASE_VALUE;
						}
						else if (currentMapPosition == pAgentAi->getPenultimateMapPosition())
						{
							goalField[arrayOffset].value -= AiConstants::PHEROMONE_START_VALUE - AiConstants::PHEROMONE_DECREASE_VALUE - AiConstants::PHEROMONE_DECREASE_VALUE;
						}

				}
			}
		}
	}
}

std::map<utilities::MapCoordinate, logic::KeyType>& PathFinder::getConflictMap(GoalTypes goalType)
{
	switch (goalType)
	{
		case GoalType_Goal:
			return mAgentGoalPositions;
			break;
		case GoalType_NextGoal:
			return mAgentNextGoalPositions;
			break;
		// This actually never happens (for now) but it needs to be implemented
		default:
			ERROR_MESSAGE("Default GoalType, this should never happen!");
			assert(false);
			return mAgentGoalPositions;
			break;
	}
}

const std::map<utilities::MapCoordinate, logic::KeyType>& PathFinder::getConflictMap(GoalTypes goalType) const
{
	switch (goalType)
	{
		case GoalType_Goal:
			return mAgentGoalPositions;
			break;
		case GoalType_NextGoal:
			return mAgentNextGoalPositions;
			break;
		// This actually never happens (for now) but it needs to be implemented
		default:
			ERROR_MESSAGE("Default GoalType, this should never happen!");
			assert(false);
			return mAgentGoalPositions;
			break;
	}
}