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

#ifndef __AI__PATH_FINDER_H__
#define __AI__PATH_FINDER_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"
#include "../Logic/KeyHandler.h"
#include "../Logic/PlayerInfo.h"
#include <map>
#include <queue>

namespace ai
{

// Forward declarations
class UnitHandler;
class AgentAi;
class Player;
class PotentialUnit;
class AiListener;
class Map;
struct PotentialSquare;

/**
* Handles all the path finding calculation of agents, is a singleton.
*/
class PathFinder
{
public:
	/**
	* The different types of goals that exists
	*/
	enum GoalTypes
	{
		GoalType_First = 0,
		GoalType_Goal = GoalType_First,
		GoalType_NextGoal,
		GoalType_Lim,
	};

	/**
	* Returns the instance of PathFinder
	* @return instance of PathFinder
	*/
	static PathFinder* getInstance();

	/**
	* Destructor
	*/
	~PathFinder();

	/**
	* Clears the pathfinder
	*/
	void clear();

	/**
	* Restarts the pathfinder. Removes all the goal and calculates them.
	*/
	void restart();

	/**
	* Update function should be run every frame
	* @param deltaTime the delta time since the last frame
	*/
	void update(float deltaTime);

	/**
	* This uses 
	* @param goalPosition the goal position on the map
	* @param keyId the key id of the agent
	* @param goalType the goal type we want to set
	* @param unitSize the current unit size
	*/
	void addToConflictMap(const utilities::MapCoordinate& goalPosition, const logic::KeyType& keyId, GoalTypes goalType, const utilities::MapCoordinate& unitSize);

	/**
	* Removes a goal position from the conflict map
	* @param goalPosition the goal position on the map
	* @param keyId the key id of the agent
	* @param goalType the goal type we want to set
	*/
	void removeFromConflictMap(const utilities::MapCoordinate& goalPosition, const logic::KeyType& keyId, GoalTypes goalType);

	/**
	* Calculates a goal for the specified agent
	* @param pAgentAi pointer to the agent we want to calculate a goal for
	* @param goalType the goal type we want to calculate 
	*/
	void calculateGoal(AgentAi* pAgentAi, GoalTypes goalType);

	/**
	* Recalculates all the goals and invalidates all the next goals for agents we own.
	*/
	void invalidateNextGoals();

	/**
	* Adds a player field to the pathfinder. The agents then take values from these fields.
	* @param[in] pPlayer pointer to a player.
	* @param[in] buildField if we want add actors to the field we'll create
	*/
	void addPlayer(const Player* pPlayer, bool buildField);

	/**
	* Removes a player field from the pathfinder.
	* @param[in] keyId the keyId of the player to remove
	* @param[in] recalculateGoals if we should recalculate the goals or not
	*/
	void removePlayer(const logic::KeyType& keyId, bool recalculateGoals);

	/**
	* Tries to add the goal position to the conflict map, should only be called by an event
	* or addToConflictMap itself. Use addToConflictMap if you want to add a conflict to the map
	* @see addToConflictMap
	* @param goalPosition the goal position on the map
	* @param keyId the key id of the agent
	* @param goalType the goal type we want to set
	* @param unitSize the current unit size
	*/
	void addToConflictMapFromEvent(const utilities::MapCoordinate& goalPosition, const logic::KeyType& keyId, GoalTypes goalType, const utilities::MapCoordinate& unitSize);

	/**
	* Removes a goal position from the conflict map, should only be called by an event
	* or removeFromConflictMap itself. Use removeFromConflictMap if you want to remove a conflict from the map
	* @see removeFromConflictMap
	* @param goalPosition the goal position on the map
	* @param keyId the key id of the agent
	* @param goalType the goal type we want to set
	*/
	void removeFromConflictMapFromEvent(const utilities::MapCoordinate& goalPosition, const logic::KeyType& keyId, GoalTypes goalType);

private:
	/**
	* Private constructor to enforce the singleton pattern.
	*/
	PathFinder();

	/**
	* Calculates the value of the goal on the specified location from the specified player.
	* Calculates the close goals depending on the unit size.
	* @param[in] goalPosition where the goal position is located on the map
	* @param[in] pAgentAi pointer to the agent who's goal field we want to calculate
	* @param[in] pPlayer pointer to the player we want to use the getPotentialValue from
	* @param[in,out] pPotentialSquare the goal field to calculate
	*/
	void calculateGoalOnPositionWithPlayer(
		const utilities::MapCoordinate& goalPosition,
		const AgentAi* pAgentAi,
		const Player* pPlayer,
		PotentialSquare* pPotentialSquare
	) const;

	/**
	* Calculates the value of the goal on the specified location on the map.
	* Calculates the close goals depending on the unit size.
	* @param[in] goalPosition where the goal position is located on the map
	* @param[in] pAgentAi pointer to the agent who's goal field we want to calculate
	* @param[in,out] pPotentialSquare the goal field position to calculate
	*/
	void calculateGoalOnPositionWithMap(
		const utilities::MapCoordinate& goalPosition,
		const AgentAi* pAgentAi,
		PotentialSquare* pPotentialSquare
	) const;

	/**
	* Checks for conflicts in the specified agent and goal
	* @param mapPosition the map position to check the conflict with
	* @param unitSize the size of the unit (to check the surroundings)
	* @param goalType the goal type we want to check the conflict
	* @return the keyId of the other agent we have a conflict with
	*/
	logic::KeyType checkForConflicts(const utilities::MapCoordinate& mapPosition, const utilities::MapCoordinate& unitSize, GoalTypes goalType) const;

	/**
	* Tries to resolve some conflicts in the conflict queues. Prioritizes the
	* goal conflict queue before the next goal conflict queue.
	*/
	void resolveConflicts();

	/**
	* Tries to resolve a specified conflict between two agents.
	* @param pFirstAgent the actor already in the conflict map
	* @param pConflictingAgent the actor conflicting with the one in the conflict map.
	* @param goalType the goal type we want to resolve
	*/
	void resolveConflict(AgentAi* pFirstAgent, AgentAi* pConflictingAgent, GoalTypes goalType);

	/**
	* Calculates which agent of two that should have their goals changed. I.e. which one that
	* should recieve new goals. This is one of the main functions when solving a conflict and
	* thus it greatly affects the agents behavior.
	* @param[in] pFirstAgent the agent who has the original position
	* @param[in] pConflictingAgent the agent who want the same goal position as pFirstAgent
	* @param[in] goalType the type of the goal that we sohuld compare
	* @param[out] pNewAgent reference pointer to the agent that will recieve the new goal
	* @param[out] pNotNewAgent reference pointer to the agent that will keep it's goals
	*/
	void calculateNewAgent(
		AgentAi* const pFirstAgent,
		AgentAi* const pConflictingAgent,
		GoalTypes goalType,
		AgentAi*& pNewAgent,
		AgentAi*& pNotNewAgent
	) const;

	/**
	* Adds potential values from player to a specified agent.
	* @pre The invalid places to go are set to INVALID_POTENTIAL_VALUE
	* Whereas those who are in range should not be set to that.
	* @param[in] pPlayer pointer to the current player we want to get the values from
	* @param[in] pAgentAi pointer to the agent who owns the goalField.
	* @param[in,out] goalField the goal field array we're adding the potential values to
	*/
	void addPotentialValueFromPlayerToField(const Player* pPlayer, const AgentAi* pAgentAi, PotentialSquare* goalField) const;

	/**
	* Adds potential values from the map to a specified agent.
	* @param[in] pAgentAi pointer to the agent who owns the goalField
	* @param[in,out] goalField the goal field array we're adding the potential values to
	*/
	void addPotentialValueFromMapToField(const AgentAi* pAgentAi, PotentialSquare* goalField) const;

	/**
	* Adds potential values from pheromones to a specified agent.
	* @param[in] pAgentAi pointer to the agent who owns the goaldField and has the pheromones
	* @param[in] goalType the type of goal we're calculating
	* @param[in,out] goalField the goal field array we're adding the potential values to
	*/
	void addPotentialValueFromPheromones(const AgentAi* pAgentAi, GoalTypes goalType, PotentialSquare* goalField) const;

	/**
	* Returns the right conflict map depending on the goal type
	* @param goalType the type of conflict map we want to have
	* @return reference to the specified conflict map
	*/
	std::map<utilities::MapCoordinate, logic::KeyType>& getConflictMap(GoalTypes goalType);

	/**
	* Returns a const reference to the right conflict map depending on the goal type
	* @param goalType the type of conflict map we want to have
	* @return reference to the specified conflict map
	*/
	const std::map<utilities::MapCoordinate, logic::KeyType>& getConflictMap(GoalTypes goalType) const;
	
	std::map<utilities::MapCoordinate, logic::KeyType> mAgentGoalPositions;
	std::map<utilities::MapCoordinate, logic::KeyType> mAgentNextGoalPositions;
	
	std::queue<logic::KeyType> mGoalConflictQueue;
	std::queue<logic::KeyType> mNextGoalConflictQueue;

	UnitHandler* mpUnitHandler;
	Map* mpMap;

	static PathFinder* mpsInstance;
};
}

#endif