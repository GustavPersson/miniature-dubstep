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
#include "Actor.h"

#ifndef __MUNITIONS_H__
#define __MUNITIONS_H__

namespace logic
{

/**
 * This is the base class of all kinds of ammo fired from the different weapons 
 */

class Munitions : public Actor
{

public:
/**
 * Constructor 
 * @param position the origin of the projectile
 * @param efficiency the weapon shooting this ammos efficiency rating
 * @param damage the damage of the weapon shooting this ammo
 */
	Munitions(utilities::Position position, int efficiency, double damage);
/**
 * Pure virtual update
 * @param deltaTime the time between calls, used in calculating movement
 */
	virtual void update(float deltaTime) = 0;
/**
 * Pure virtual handleEvent
 * @param eventMessage the event to be handled
 */
	virtual void handleEventId(EventData const &eventMessage) = 0;
/**
 * method for getting the origin
 */
	utilities::Position getOrigin();
/**
 * method for getting weaponefficiency
 */
	int getEfficiency();
/**
 * method for getting damage
 */
	double getDamage();

private:
	int mEfficiency;
	double mDamage;

};
}

#endif
