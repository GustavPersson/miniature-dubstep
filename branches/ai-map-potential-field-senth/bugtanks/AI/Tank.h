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

#ifndef __AI__TANK_H__
#define __AI__TANK_H__

#include "../Utilities/Macros.h"
#include "Player.h"

namespace ai
{

/**
* This class is represented when the player is in a tank.
*/
class Tank : public Player
{
public:
	/**
	* Constructor
	* @param pTank pointer to the logic tank
	*/
	Tank(logic::Actor* pTank);

	/**
	* Destructor
	*/
	virtual ~Tank();

	/**
	* Returns the value of the potential field on the specified map location.
	* I.e. not in local coordinates but in map coordinates.
	* Fixes some issues when the agents are close to the tank, those locations should be
	* have the same values and positions on the tank should be 0.
	* @param mapPosition the map position we want to get the information from
	* @return the value on the specified location, INVALID_POTENTIAL_VALUE if it's out of bounds.
	*/
	virtual float getPotentialValue(const utilities::MapCoordinate& mapPosition) const;

	/**
	* Returs the name of the potential unit
	* @return name of the potential unit
	*/
	virtual inline std::string getName() const { return "Tank"; }

};
}

#endif