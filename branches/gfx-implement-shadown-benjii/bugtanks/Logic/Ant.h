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
#include "Enemy.h"

#ifndef __ANT_H__
#define __ANT_H__


namespace logic
{

/**
 * This is the class for the most basic of the games enemies
 */


class Ant:public Enemy
{

public:
/**
 * Constructor
 * @param hp the enemies healthpoints
 * @param damage the amount of damage this enemy does
 * @param size the size of the enemy, used in damage calculations
 * @param aiStyle a flag that tells the ai in what way this enemy should behave
 * @param range how long range the unit have, incase of melee unit the range is a low value, probably between 0 and 1
 */
	Ant(double hp,double damage,Sizes size,int aiStyle,float range);
/**
 * Deconstructor
 */
	~Ant();
/**
 * Function that is called when an enemy wants to attack a driver, this is likely an event from AI
 */
	void attack();
/**
 * update position for ant
 * @param deltaTime time between calls, used to calculate movement
 */
	void update(float deltaTime);
/**
 * handles various events that is relevant for this class
 * @param eventMessage the event to be handled
 */
	void handleEventId(EventData const &eventMessage);

};
}

#endif