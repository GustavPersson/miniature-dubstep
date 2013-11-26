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

#ifndef __DRIVER_H__
#define __DRIVER_H__

#include "../Utilities/Macros.h"
#include "Unit.h"

namespace logic
{

// Forward declarations
class Weapon;
class Shotgun;

/**
 * This is the driver class, it holds information and functions for the driver, I have defined that as the man driving the tank here
 */

class Driver : public Unit
{

public:
/**
 * Constructor
 * @param networkId the networkId of the player
 * @param hp the amount of hit points the driver has
 * @param pos the position of this actor
 * @param dir the direction of this actor
 * @param playerMovingInfo information about how the previous playeractor was moving 
 */ 
	Driver(int networkId, utilities::Position pos, utilities::Direction dir, Actor::PlayerMovingInfo playerMovingInfo);
/**
 *Deconstructor
 */
	~Driver();
/**
 * function that sends events to relevant modules and changes state to have driver in control instead of tank
 */
	void thrownOutOfTank();
/**
 * Fires the drivers last resort shotgun
 */
	void fireShotgun();
/**
 * Enters the tank if you where thrown out
 */
	void enterTank();
/**
 * update driver position
 */
	void update(float deltaTime);
/**
 * handles various events for driver
 * @param eventMessage the event to be handled
 */
	void handleEventId(EventData const &eventMessage);
/**
 * reduces the drivers hp by damage
 * @param damage the amount of damage to be taken
 */
	void reduceHp(float damage);

/**
 * function that returns a drivers weapon
 */
	Weapon* getWeapon();
/**
 * function that returns the name of the actor class
 */
	std::string getName(void) {return "Driver";}

/**
 * Toggles god mode in the tank
 */
	inline void toggleGodMode() {mGodMode = !mGodMode;}

private:
	Shotgun *mpShotgun;

	utilities::Direction mStrafeDirection;

	bool mFlying;
	bool mStrafing;
	bool mStrafeLeft;

	float mRotateSpeed;
	float mStrafeSpeed;

	bool mGodMode;
};
}

#endif