/**
 * @file
 * @author Dennis Jönsson <zelebsel@gmail.com>
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

#include "../Utilities/Macros.h"
#include "Weapon.h"

#ifndef __SHOTGUN_H__
#define __SHOTGUN_H__


namespace logic
{

/**
 * This is a subclass to weapon, the shotgun is the last resort if you get thrown out of your tank
 */

class Shotgun: public Weapon
{

public:
/**
 * Constructor
 * @param damage How much damage this weapon does
 * @param weaponCooldown How long time you have to wait before being able to fire again
 * @param weaponId Id for gamelogical recognition
 * @param efficiency Tells against which kind of enemy this weapon is efficient
 */
	Shotgun(double damage, float weaponCooldown, int weaponId,int efficiency);
/**
 * Deconstructor
 */
	~Shotgun();

	void fireWeapon();
};
}

#endif