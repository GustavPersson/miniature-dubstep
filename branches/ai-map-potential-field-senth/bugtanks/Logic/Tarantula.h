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

#ifndef __TARANTULA_H__
#define __TARANTULA_H__


namespace logic
{

class Tarantula:public Enemy
{

/**
 * This is the class for the Tarantula
 */

public:
/**
 * Constructor
 * @param networkId the network id of the player
 * @param hp the enemies healthpoints
 * @param damage the amount of damage this enemy does
 * @param size the size of the enemy, used in damage calculations
 * @param range how long range the unit have, incase of melee unit the range is a low value, probably between 0 and 1
 * @param pos the position of this actor
 */
	Tarantula(int networkId, utilities::Position pos, utilities::Direction dir);
/**
 * Deconstructor
 */
	~Tarantula();
/**
 * Function that is called when an enemy wants to attack a player, this is likely an event from AI
 */
	void attack();
/**
 * update position for tarantula
 * @param deltaTime time between calls, used to calculate movement
 */
	void update(float deltaTime);
/**
 * handles various events that is relevant for this class
 * @param eventMessage the event to be handled
 */
	void handleEventId(EventData const &eventMessage);
/**
 * function that returns the name of the actor class
 */
	std::string getName(void) {return "Tarantula";}
	static std::string getEnemyName(void) {return "Tarantula";}
	static std::string getEnemyModelName(void) {return "Spider";}

	/**
	* Returns the ai behavior of the enemy
	* @return ai behavior of the enemy
	*/
	virtual inline ai::AiBehaviors getAiBehavior() const {return ai::AiBehavior_MeleeAttacker;}

};
}

#endif