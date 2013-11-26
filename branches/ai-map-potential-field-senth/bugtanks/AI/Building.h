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

#ifndef __BUILDING_H__
#define __BUILDING_H__

#include "StaticUnit.h"

namespace ai
{

/**
* Building with potential information.
*/
class Building : public StaticUnit
{
public:
	/**
	* Constructor
	* @param pBuilding an actor that is treated as a building
	*/
	Building(logic::Actor* pBuilding);

	/**
	* Destructor
	*/
	virtual ~Building();

	/**
	* Initializes the potential field for the building
	* @param maxDistance the maximum distance the building will affect the global map
	*/
	static void initPotentialField(int maxDistance);

	/**
	* Destroys the building's potential fields.
	* @pre All buildings have been destroyed
	*/
	static void destroyPotentialField();

	/**
	* Returns the size (columns/rows) of the potential field
	* @return size (columns/rows) of the potential field
	*/
	virtual inline const utilities::MapCoordinate& getPotentialSize() const {return mPotentialSize;}

	/**
	* Returns the value of the potential field on the specified map location.
	* I.e. not in local coordinates but in map coordinates.
	* @param mapPosition the map position we want to get the information from
	* @return the value on the specified location, INVALID_POTENTIAL_VALUE if it's out of bounds.
	*/
	virtual float getPotentialValue(const utilities::MapCoordinate& mapPosition) const;

	/**
	* Returns the name of the potential unit
	* @return name of the potential unit
	*/
	virtual inline std::string getName() const { return "Building"; }

	/**
	* Update function. Should only be run when the building changed size or
	* position.
	*/
	virtual void update();

private:
	/**
	* Returns the potential value with the specified offset (from an edge)
	* @pre x and y values of positionOffset should be between 0 and msBuildingPotentialRange.
	* @param positionOffset the offset in x and y from an edge.
	* @return the potential value on the specified offset
	*/
	static float getPotentialValueFromEdge(const utilities::MapCoordinate& positionOffset);

	/**
	* Returns true if the index is valid in the current building
	* @param indexPosition the index position in the unit
	* @return true if the index is valid
	*/
	inline bool validIndex(const utilities::MapCoordinate& indexPosition) const
	{
		return (indexPosition.x >= 0 && indexPosition.x < mPotentialSize.x &&
				indexPosition.y >= 0 && indexPosition.y < mPotentialSize.y);
	}


	/**
	* Returns the reversed index
	* E.g. msBuildingPotentialRange is 3 and we insert 0 we get 3.
	* E.g. msBuildingPotentialRange is 3 and we insert 1 we get 2.
	* @param index the index to reverse
	* @return the reversed index
	*/
	inline int reverseIndex(int index) const
	{
		return abs(index - msBuildingPotentialRange);
	}

	static int msBuildingPotentialRange;
	static float msStartValue;
	/**
	* The potential values that are used to calculate the potential field.
	* The first values (msBuildingPotentialRange) are those that are orthogonal to the building,
	* i.e. not in the corners.
	*/
	static float* mpsPotentialValues;

	utilities::MapCoordinate mPotentialSize; /**< The width and height of the potential field */

#ifdef AI_DRAW_POTENTIAL_BLOCKS
	gfx::TransformObject** mpGoalPositionBlocks;
#endif
};
}

#endif