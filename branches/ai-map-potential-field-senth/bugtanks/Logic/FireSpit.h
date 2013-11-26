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
#include "EnemyWeapon.h"


#ifndef __FIRESPIT_H__
#define __FIRESPIT_H__

namespace logic 
{

/**
 * This is a subclass to enemy weapon, this is the fire ants weapon
 */

class FireSpit : public EnemyWeapon
{

public:
/**
 * Constructor
 * @param damage How much damage this weapon does
 * @param weaponCooldown How long time you have to wait before being able to fire again
 * @param range this weapons range 
 */
	FireSpit(float damage, float weaponCooldown, float range);

/**
 * Deconstructor
 */
	~FireSpit();

/**
 * Makes the weapon fire when the conditions are met
 */
	void fireWeapon(KeyType ownerId);

	virtual void addBullet(KeyType bulletId);

private:
	KeyType mBulletKeys[2];
};
}

#endif