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

#ifndef __AI__PLAYER_H__
#define __AI__PLAYER_H__

#include "DynamicUnit.h"
#include "../Logic/Actor.h"
#include "../Utilities/Vector2D.h"
#include <map>

#ifdef AI_DRAW_PLAYER_POTENTIAL_FIELD
#include "../GFX/DX10Module.h"
const int POTENTIAL_BLOCK_WIDTH = 19;
const int POTENTIAL_BLOCK_ARRAY_SIZE = POTENTIAL_BLOCK_WIDTH * POTENTIAL_BLOCK_WIDTH;
#endif

namespace ai
{

/**
* The potential unit of a player
*/
class Player : public DynamicUnit
{
public:
	/**
	* Constructor
	* @pre init should have been called before creating Player objects
	* @param pPlayer pointer to the logic player
	*/
	Player(logic::Actor* pPlayer);

	/**
	* Destructor
	*/
	virtual ~Player();

	/**
	* Initializes the players potential field for all tanks
	* @param maxDistance the maximum distance the player will affect other units
	*/
	static void init(int maxDistance);

	/**
	* Destroy the player's potential field.
	* @pre All players have been destroyed
	*/
	static void destroy();

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
	* The goals closest to the player will have the same value.
	* @param mapPosition the map position we want to get the information from
	* @return the value on the specified location, NEUTRAL_POTENTIAL_VALUE if it's out of bounds.
	*/
	virtual float getPotentialValue(const utilities::MapCoordinate& mapPosition) const;

	/**
	* Returns the name of the potential unit
	* @return name of the potential unit
	*/
	virtual inline std::string getName() const { return "Player"; }

protected:
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

	/**
	* This function gets called just after a unit has move it's position
	*/
	virtual void unitMoved();

	static int msCenterIndex;
	static utilities::MapCoordinate msPotentialRange;
	static float* mpsPotentialField;

private:
#ifdef AI_DRAW_PLAYER_POTENTIAL_FIELD
	gfx::TransformObject** mpGoalPositionBlocks;
#endif
};
}

#endif