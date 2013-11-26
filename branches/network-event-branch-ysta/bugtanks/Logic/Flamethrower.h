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


#ifndef __FLAMETHROWER_H__
#define __FLAMETHROWER_H__

namespace logic
{

/**
 * This is a subclass to weapon, flamethrower is the closecombat tanks second weapon
 */

class Flamethrower: public Weapon
{
public:
/**
* Constructor
* @param damage How much damage this weapon does
* @param weaponCooldown How long time you have to wait before being able to fire again
* @param weaponId Id for gamelogical recognition
* @param efficiency Tells against which kind of enemy this weapon is efficient
* @param range how long range this weapon has, of all weapons only flamethrower has a limited range in the game 
*/
	Flamethrower(double damage,float weaponCooldown,int weaponId,int efficiency,int range);
/**
 * Deconstructor
 */
	~Flamethrower();
/**
 * Makes the weapon fire if the conditions are met
 */
    void fireWeapon();

private:
	int mRange;

};
}

#endif