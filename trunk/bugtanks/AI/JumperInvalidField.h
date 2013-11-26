/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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

#ifndef __JUMPER_INVALID_FIELD_H__
#define __JUMPER_INVALID_FIELD_H__

#include "StaticUnit.h"

namespace ai
{

/**
* Invalid field where a jumper is supposed to land.
*/
class JumperInvalidField : StaticUnit
{
public:
	/**
	* Constructor.
	*/
	JumperInvalidField();

	/**
	* Destructor.
	*/
	~JumperInvalidField();

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
	virtual inline std::string getName() const { return "JumperInvalidField"; }

private:

};
}

#endif