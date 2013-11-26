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

#ifndef __PEDESTRIANGUN_H__
#define __PEDESTRIANGUN_H__


namespace logic
{

/**
 * This is the class for the pedestrian discouragement gun, with which you force away people trying to hijack your tank without hurting them
 */

class PedestrianGun: public Weapon
{

public:
/**
 * Constructor
 * @param weaponCooldown How long time you have to wait before being able to fire again
 */
	PedestrianGun();
/**
 * Deconstructor
 */
	~PedestrianGun();
/**
 * Fires this weapon
 * @param ownerId the owner of this weapon
 */
	void fireWeapon(KeyType ownerId);
/**
 * Adds a bullet
 * @param bulletId the id of the bullet to be added
 */
	void addBullet(KeyType bulletId);
	
private:
	KeyType mBulletKeys[2];
};
}

#endif