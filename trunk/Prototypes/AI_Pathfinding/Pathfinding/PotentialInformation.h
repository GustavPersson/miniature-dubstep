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
* 
* @section DESCRIPTION
*
* Holds all the potential information that is needed by every Agent.
*/

#ifndef __POTENTIAL_INFORMATION_H__
#define __POTENTIAL_INFORMATION_H__

#include "PathFinder.h"
#include "VectorList.h"
#include "Vector.h"

const int GOAL_FIELD_WIDTH = 3;
const int GOAL_FIELD_HEIGHT = 3;
const int GOAL_FIELD_SIZE = GOAL_FIELD_WIDTH * GOAL_FIELD_HEIGHT;
const float INVALID_POTENTIAL_VALUE = -100.0f;

struct PotentialSquare
{
	FixedCoordinate position;
	float value;

	PotentialSquare(float value = 0.0f, FixedCoordinate position = INVALID_COORDINATE) :
		value(value), position(position)
	{
	}
};


class PotentialInformation
{
public:
	PotentialInformation();
	void updateGoalAndPheromones();

	inline const PotentialSquare* getGoals() const {return mSortedGoals;}
	inline PotentialSquare* getGoals() {return mSortedGoals;}
	inline FixedCoordinate getBestGoalPosition() const {return mSortedGoals[0].position;}
	void setGoals(const PotentialSquare* newGoals);
	inline const PotentialSquare* getNextGoals() const {return mSortedNextGoals;}
	inline PotentialSquare* getNextGoals() {return mSortedNextGoals;}
	inline FixedCoordinate getBestNextGoalPosition() const {return mSortedNextGoals[0].position;}
	void setNextGoals(const PotentialSquare* newGoals);
	inline const utilities::VectorList<PotentialSquare>& getPheromones() const {return mPheromones;}

private:
	utilities::VectorList<PotentialSquare> mPheromones;	/**< All the active pheromones in the trail. */
	PotentialSquare mSortedGoals[GOAL_FIELD_SIZE]; /**< Possible goals we can move to. */
	PotentialSquare mSortedNextGoals[GOAL_FIELD_SIZE]; /**< Possible next goal we can move to. */
};

#endif