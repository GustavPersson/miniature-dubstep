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
#include "Munitions.h"

#ifndef __PLASMA_H__
#define __PLASMA_H__

namespace logic
{

/**
 * Subclass to munitions, it holds plasma spew projectile  
 */

class Plasma : public Munitions
{
public:

/**
 * Constructor
 * @param networkId the network id of the player
 * @param position the plasma origin
 * @param direction the plasma direction
 * @param efficiency the weapon shooting this ammos efficiency rating
 * @param damage the damage of the weapon shooting this ammo
 */
	Plasma(int networkId, utilities::Position position, utilities::Direction direction, int efficiency, float damage);
/**
 * Deconstructor
 */
	~Plasma();
/**
 * update plasma position
 * @param deltaTime the time between calls, used to calculate movement
 */
	void update(float deltaTime);

/**
 * handles various events related to plasma
 * @param eventMessage the event to be handled
 */
	void handleEventId(EventData const &eventMessage);

/**
 * function that returns the name of the actor class
 */
	std::string getName(void) {return "Plasma";}

};
}

#endif