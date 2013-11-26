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

#ifndef __SPECTATOR_H__
#define __SPECTATOR_H__

#include "../Utilities/Macros.h"
#include "Unit.h"
#include "ActorHandler.h"

namespace logic
{

class Spectator : public Unit
{

public:
/**
 * Constructor
 * @param networkId the networkId of the player
 * @param pos the position of this actor
 * @param dir the direction of this actor
 * @param playerMovingInfo information about how the previous playeractor was moving 
 */ 
	Spectator(int networkId,utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo);
/**
/* Deconstructor
 */
	~Spectator();
/**
 * Update function
 * @param deltaTime the time between calls
 */
	void update(float deltaTime);

/**
 * handles various events for spectator
 * @param eventMessage the event to be handled
 */
	void handleEventId(EventData const &eventMessage);

/**
 * Enters tank, only for debugging purposes
 */
	void enterTank(); 
/**
 * function that returns the name of the actor class
 */
	std::string getName(void) {return "Spectator";}


};
}

#endif