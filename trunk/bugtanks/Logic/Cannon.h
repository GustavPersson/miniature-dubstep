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

#ifndef __CANNON_H__
#define __CANNON_H__


namespace logic
{

// Forward declarations
class Bullet;

/**
 * This is a subclass to weapon, Cannon is the longrange tanks basic weapon
 */

class Cannon: public Weapon
{
public:
/**
 * Constructor
 * @param damage How much damage this weapon does
 * @param weaponCooldown How long time you have to wait before being able to fire again
 * @param efficiency Tells against which kind of enemy this weapon is efficient
 * @param upgrade A bool indicating if the upgrade have been bought or not
 * @param radius is initially 0, when upgrade is bought it is increased to simulate explosive splash damage 
 */
	Cannon();
/**
 * Deconstructor
 */
    ~Cannon();
/**
 * Makes the weapon fire if the conditions are met
 */
	void fireWeapon(KeyType ownerId);

	void addBullet(KeyType bulletId);

private:
	KeyType mBulletKeys[2];
};
}

#endif