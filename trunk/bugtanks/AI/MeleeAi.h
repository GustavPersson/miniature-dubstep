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

#ifndef __AI__MELEE_AI_H__
#define __AI__MELEE_AI_H__

#include "../Utilities/Macros.h"
#include "AgentAi.h"

namespace ai
{

/**
* A melee AI, overloads the range and returns 0.
*/
class MeleeAi : public AgentAi
{
public:
	/**
	* Constructor
	* @param pEnemy pointer to the logic enemy
	*/
	inline MeleeAi(logic::Enemy* pEnemy) : AgentAi(pEnemy) {}

	/**
	* Destructor
	*/
	inline ~MeleeAi() {}

	/**
	* Returns the range of which we are too close to the player. For melees this is always 0.0f
	* @return the range of which we are too close to the player
	*/
	virtual inline float getTooCloseRange() const {return 0.0f;}

	/**
	* Returns the name of melee unit
	* @return name of the melee unit
	*/
	virtual inline std::string getName() const {return "MeleeAi";}

private:

};
}

#endif