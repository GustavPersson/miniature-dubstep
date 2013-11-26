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

#ifndef __AGENT_AI_H__
#define __AGENT_AI_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"
#include "../Utilities/VectorList.h"
#include "../Logic/Enemy.h"
#include "DynamicUnit.h"
#include "PathFinder.h"
#include "AiConstants.h"

#ifdef AI_DRAW_AGENT_GOALS
#include "../GFX/DX10Module.h"
#endif

namespace ai
{

/**
* The Base-class for Agent AI:s
*/
class AgentAi : public DynamicUnit
{
public:
	/**
	* struct that contains information about goals in the conflict map
	*/
	struct ConflictInfo
	{
		bool inConflictMap;
		utilities::MapCoordinate conflictPosition;	/**< Only valid value if inConflictMap is true */
		utilities::MapCoordinate unitSize; /**< Only valid value if inConflictMap is true */
	};

	/**
	* Constructor
	* @pre init should have been called before creating AgentAi objects
	* @param pEnemy pointer to the enemy actor
	*/
	AgentAi(logic::Enemy* pEnemy);

	/**
	* Destructor
	*/
	virtual ~AgentAi();

	/**
	* Initializes the agent including it's potential field
	* @param maxDistance the maximum distance the agent will affect other units
	*/
	static void init(int maxDistance);

	/**
	* Destroy the agent's static members.
	* @pre All AgentAis have been destroyed.
	*/
	static void destroy();

	/**
	* Update function should be run somewhat each frame, not necessary though.
	*/
	virtual void update();

	/**
	* Update function should only be called if we're the owner of the unit
	*/
	virtual void updateOnlyOwner();

	/**
	* Sets the goals of the AI
	* @param newGoals array of goals, size should be GOAL_ARRAY_SIZE
	*/
	void setGoals(const PotentialSquare* newGoals);

	/**
	* Returns a const array to the goals of the Agent
	* @return const array to the goals of the Agent
	* @note the array has the size of GOAL_ARRAY_SIZE
	*/
	inline const PotentialSquare* getGoals() const {return mSortedGoals;}

	/**
	* Returns an array to the goals of the Agent
	* @return array to the goals of the Agent
	* @note the array has the size of GOAL_ARRAY_SIZE
	*/
	inline PotentialSquare* getGoals() {return mSortedGoals;}

	/**
	* Returns the best available goal position
	* @return the best available goal position
	*/
	inline utilities::MapCoordinate getBestGoalPosition() const {return mSortedGoals[0].position;}

	/**
	* Set the next goal of the AI
	* @param newGoals array of goals, size should be GOAL_ARRAY_SIZE
	*/
	void setNextGoals(const PotentialSquare* newGoals);

	/**
	* Sets the specified goals
	* @param newGoals array of goals, size should be GOAL_ARRAY_SIZE
	* @param conflictPosition the position in the conflict map, used to check if the event is out of date
	* @param isOwner if it was the owner who called this function, thus it should always override the old goal
	* @param goalType the type of the goal to set
	*/
	void setGoals(const PotentialSquare* newGoals, const utilities::MapCoordinate& conflictPosition, bool isOwner, PathFinder::GoalTypes goalType);

	/**
	* Returns a const pointer to the goal depending on the specified goalType
	* @param goalType the type of the goal to return
	* @return const pointer to the goal depending on the specified goalType
	*/
	const PotentialSquare* getGoals(PathFinder::GoalTypes goalType) const;

	/**
	* Returns a pointer to the goal depending on the specified goalType
	* @param goalType the type of the goal to return
	* @return pointer to the goals of the specified type
	*/
	PotentialSquare* getGoals(PathFinder::GoalTypes goalType);

	/**
	* Invalidates all the goals by the specified type. Almost the same functionality
	* as removeGoals, however this function doesn't remove the goals from the
	* pathfinder's conflict map.
	* @param goalType the type of goals to invalidate.
	* @see removeGoals
	*/
	void invalidateGoals(PathFinder::GoalTypes goalType);

	/**
	* Returns a const array of the next goals of the Agent
	* @return const array of the next goals of the Agent
	* @note the array has the size of GOAL_FIELD_SIZE
	*/
	inline const PotentialSquare* getNextGoals() const {return mSortedNextGoals;}

	/**
	* Returns an array of the next goals of the Agent
	* @return const array of the next goals of the Agent
	* @note the array has the size of GOAL_FIELD_SIZE
	*/
	inline PotentialSquare* getNextGoals() {return mSortedNextGoals;}

	/**
	* Returns the best available next goal position
	* @return the best available next goal position
	*/
	inline utilities::MapCoordinate getBestNextGoalPosition() const {return mSortedNextGoals[0].position;}
	
	/**
	* Returns the size (columns/rows) of the potential field
	* @return size (columns/rows) of the potential field
	*/
	virtual inline const utilities::MapCoordinate& getPotentialSize() const
	{
		return msPotentialRange;
	}

	/**
	* Returns the value of the potential field on the specified map location.
	* I.e. not in local coordinates but in map coordinates.
	* @note This functionality is quite slow since it needs to iterate through all PotentialSquares.
	* @param mapPosition the map position we want to get the information from
	* @return the value on the specified location, INVALID_POTENTIAL_VALUE if it's out of bounds.
	*/
	virtual float getPotentialValue(const utilities::MapCoordinate& mapPosition) const;

	/**
	* Sorts the specified goals
	* @param goalType the type of goals to sort
	*/
	void sortGoals(PathFinder::GoalTypes goalType);

	/**
	* Returns the name of the potential unit
	* @return name of the potential unit
	*/
	virtual inline std::string getName() const { return "AgentAi"; }

	/**
	* Returns the struct containing the conflict information
	* @param goalType the type of goal we want to have the information from
	*/
	inline const ConflictInfo& getConflictInfo(PathFinder::GoalTypes goalType) const
	{
		return mConflictInfo[goalType];
	}

	/**
	* Sets the current conflict info. Should be done when we add a goal
	* to pathfinder's conflict map. Automatically sets inConflict to true
	* @param conflictPosition the position in the conflict map
	* @param unitSize the current unitSize when we added the conflict
	* @param goalType the type of goal we added to the conflict map
	*/
	inline void setConflictInfo(const utilities::MapCoordinate& conflictPosition, const utilities::MapCoordinate& unitSize, PathFinder::GoalTypes goalType)
	{
		mConflictInfo[goalType].inConflictMap = true;
		mConflictInfo[goalType].unitSize = unitSize;
		mConflictInfo[goalType].conflictPosition = conflictPosition;
	}

	/**
	* Sets in conflict map to false. Used when a goal has been removed from the
	* pathfinder's conflict map
	* @param goalType the type of goal we removed to the conflict map
	*/
	inline void unsetInConflict(PathFinder::GoalTypes goalType)
	{
		mConflictInfo[goalType].inConflictMap = false;
	}

	/**
	* Returns the number of valid goals of the specified type that the agent has left.
	* @param[in] goalType the type of the number of valid goals we want to return
	* @return number of valid goals of the specified type
	*/
	int nrOfValidGoals(PathFinder::GoalTypes goalType) const;

	/**
	* Stops the agent
	*/
	virtual void pause();

	/**
	* Resumes the agent.
	*/
	virtual void resume();

	/**
	* Removes the agent's goals. Also removes them from the conflict map if they had any goals there.
	* This is almost the same functionality as invalidate goals, however this function
	* also removes the best goal from the pathfinder's conflict map.
	* @param goalType the type of goal that we want to remove.
	* @see invalidateGoals
	*/
	void removeGoals(PathFinder::GoalTypes goalType);

	/**
	* Returns the range of which we are too close to the player.
	* For the generic AI this will be 0.0f since we want to get as close to
	* the player as possible.
	* @return the range of which we are too close to the player
	*/
	virtual inline float getTooCloseRange() const {return 0.0f;}

	/**
	* Sets the target of the AI
	* @param targetId the key of the target
	*/
	inline void setTarget(const logic::KeyType& targetId)
	{
		mTarget = targetId;
		mpEnemy->setTarget(mTarget);
	}

	static const int GOAL_WIDTH = AiConstants::GOAL_WIDTH;	/**< The width of the goal */
	static const int GOAL_ARRAY_SIZE = AiConstants::GOAL_ARRAY_SIZE; /**< The goal array size */

protected:
	/**
	* This function gets called just before a unit changes it rotation.
	* Removes the goals of the agent if we are the owner.
	*/
	virtual void beforeRotateChange();

	/**
	* This function gets called just after a unit changes it rotation.
	* Calculates new goals for the agent if we are the owner.
	*/
	virtual void afterRotateChange();
 
	 /**
	 * If the agent should use its goals
	 * @param use bool determining whether or not to use goals
	 */
	void setUseGoals(bool use);
	
	logic::Enemy* mpEnemy;
	logic::KeyType mTarget;

private:
	/**
	* The focal point of the agent.
	* @return the direction the agent is looking at
	*/
	virtual inline const utilities::Vec3Float& getFocalDirection() const
	{
		return mpEnemy->getFocalDirection();
	}

	/**
	* Returns true if we are close to the goal
	* @return true if we are close to the goal
	*/
	bool arrivedAtGoal() const;

	/**
	* Swaps the next goal and goals
	*/
	inline void swapGoals() {std::swap(mSortedGoals, mSortedNextGoals);}

	/**
	* Returns true if the index is valid, i.e. in bounds of the array
	* @param indexPosition the index position in the unit
	* @return true if the index is valid, i.e. in bounds of the array
	*/
	inline bool validIndex(const utilities::MapCoordinate& indexPosition) const
	{
		return (indexPosition.x >= 0 && 
				indexPosition.x < msPotentialRange.x && 
				indexPosition.y >= 0 && 
				indexPosition.y < msPotentialRange.y);
	}

	
	float mLastTargetTime;	/**< When we search for a player the last time */
	bool mUseGoals;

	utilities::VectorList<PotentialSquare> mPheromones;		/**< All the active pheromones in the trail. */
	// Using dynamic array instead of static for fast switching on the arrays
	PotentialSquare* mSortedGoals;							/**< Possible goals we can move to. */
	PotentialSquare* mSortedNextGoals;						/**< Possible next goals we can move to. */

	ConflictInfo mConflictInfo[PathFinder::GoalType_Lim];

	static float* mpsPotentialField;
	static utilities::MapCoordinate msPotentialRange; /**< The number of columns/rows the potential field has */

#ifdef AI_DRAW_AGENT_GOALS
	gfx::TransformObject* mpGoalPositionBlocks[GOAL_ARRAY_SIZE];
#endif
};

}

#endif