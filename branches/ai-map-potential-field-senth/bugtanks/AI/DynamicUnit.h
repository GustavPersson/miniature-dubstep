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

#ifndef __AI__DYNAMIC_UNIT_H__
#define __AI__DYNAMIC_UNIT_H__

#include "../Utilities/Macros.h"
#include "PotentialUnit.h"

namespace ai
{

/**
* A dynamic potential unit. Mainly for updating units in the player's potential field
* when they are in range, only if they should be applied to the field.
*/
class DynamicUnit : public PotentialUnit
{
public:
	/**
	* Constructor
	* @param pActor pointer to the unit's actor class
	* @param applyOnGlobalPotentialField if the unit's potential field should be added to the global potential field.
	*/
	DynamicUnit(logic::Actor* pActor, bool applyOnGlobalPotentialField);

	/**
	* Destructor
	*/
	virtual ~DynamicUnit();

	/**
	* Update function
	*/
	virtual void update();

	/**
	* Set focal unit, use this function instead of the mpActor's own set focal unit
	* This function ensures that the rotation is being checked, before the next frame.
	* Only applicable on units
	* @param keyId the key of the unit to set the focal direction
	*/
	void setFocalUnit(logic::KeyType keyId);

protected:
};
}

#endif