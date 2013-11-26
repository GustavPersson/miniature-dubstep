/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
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

#include "../utilities/Macros.h"
#include "AgentAi.h"


#ifndef __RANGEDAI_H__
#define __RANGEDAI_H__

namespace ai
{

class RangedAi : public AgentAi
{

public:

/**
 * Constructor 
 * @param pEnemy the logic enemy 
 */
	RangedAi(logic::Enemy *pEnemy);
/**
/* Destructor
 */	
	~RangedAi();
/**
* Returns the value of the potential field on the specified map location.
* I.e. not in local coordinates but in map coordinates.
* @note This functionality is quite slow since it needs to iterate through all PotentialSquares.
* @param mapPosition the map position we want to get the information from
* @return the value on the specified location, INVALID_POTENTIAL_VALUE if it's out of bounds.
*/
	float getPotentialValue(const utilities::MapCoordinate& mapPosition) const;
/**
* Returns the name of the potential unit
* @return name of the potential unit
*/
	virtual inline std::string getName() const { return "RangedAi"; }
	
/**
* Returns the range of which we are too close to the player.
* @return the range of which we are too close to the player
*/
	virtual inline float getTooCloseRange() const {return mpEnemy->getRange() - AiConstants::RANGED_AI_TOO_CLOSE_OFFSET;}

};
}

#endif