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

#ifndef __AI__STATIC_UNIT_H__
#define __AI__STATIC_UNIT_H__

#include "../Utilities/Macros.h"
#include "PotentialUnit.h"

namespace ai
{

/**
* A static potential unit.
*/
class StaticUnit : public PotentialUnit
{
public:
	/**
	* Constructor
	* @param pActor pointer to the unit's actor class
	* @param applyOnGlobalPotentialField if the unit's potential field should be added to the global potential field.
	*/
	StaticUnit(logic::Actor* pActor, bool applyOnGlobalPotentialField) :
	  PotentialUnit(pActor, applyOnGlobalPotentialField) {}
	
private:

};
}

#endif