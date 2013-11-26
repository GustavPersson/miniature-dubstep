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

#ifndef __AI__AI_CONSTANTS_H__
#define __AI__AI_CONSTANTS_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Vectors.h"

namespace ai
{

/**
* Contains all the AI constants that is used by the AI Component.
* These values can be changed, but it's only meant that it should be changed
* by the game options. To enforce that they shouldn't be changed all the names
* are capitalized.
*/
struct AiConstants
{
	// Potential fields
	static int POTENTIAL_BUILDING_RANGE; /**< How far out the buildings will affect the potential fields */
	static float POTENTIAL_BUILDING_START_VALUE; /**< The start value the potential field should have on the buildings' corners, is negated. */
	static int POTENTIAL_WIDTH_AGENT;	/**< The rows/columns the agent's potential fields will affect other agents */
	static int POTENTIAL_WIDTH_PLAYER; /**< The rows/columns the players' potential fields will affect the agents */

	// AgentAi
	static float GOAL_CLOSE_RANGE; /**< The distance when we're close to the goal */
	static int PRIORITIZE_LESS_THAN_VALID_GOALS; /**< The number of goals before we prioritize the agent with the lowest number of valid goals. */
	static float AGENT_SEARCH_TARGET_TIME;	/**< How often we should search for a target. */

	// RangedAi
	static float RANGED_AI_TOO_CLOSE_OFFSET; /**< The offset of the range of the AI. E.g. if the range of the AI is 5.0f and offset is 1.0f then the close range would be 4.0f */

	// PotentialUnit
	static float ROTATED_THRESHOLD; /**< Threshold for how much extra we need the angle to be before we switch the rotated state */

	// Update time
	static float UPDATE_TIME_AGENT; /**< How many seconds until we update the agent */
	static float UPDATE_TIME_PLAYER; /**< How many seconds until we update the player */
	static float UPDATE_TIME_BUILDING; /**< How many seconds until we update the building */

	// Map potential values
	static float MAP_POTENTIAL_MIN_DRAW_VALUE;
	static float MAP_POTENTIAL_MAX_DRAW_VALUE;

	/**
	* Sets the values of the AiConstants to the options specified in the .ini-file.
	*/
	static void loadConstants();

	/**
	* Writes the current values of the AiConstants into an .ini-file.	
	*/
	static void saveConstants();

	// -------------------------
	// THESE ARE REAL CONSTANTS
	// -------------------------
	// Goal stuff
	static const int GOAL_WIDTH = 3;	/**< The width of agent's goal */
	static const int GOAL_ARRAY_SIZE = GOAL_WIDTH * GOAL_WIDTH;
	
	// Pheromones
	static const float PHEROMONE_START_VALUE;
	static const float PHEROMONE_DECREASE_VALUE;

	// Height information when debugging fields
	static const float DRAW_HEIGHT_PLAYER;
	static const float DRAW_HEIGHT_AGENT;
	static const float DRAW_HEIGHT_MAP;
	static const float DRAW_HEIGHT_ORIGIN;
	static const float DRAW_HEIGHT_UNIT_SIZE;
};
}

#endif