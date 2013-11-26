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
#include "Shotgun.h"

#ifndef __DRIVER_H__
#define __DRIVER_H__


namespace logic
{

/**
 * This is the driver class, it holds information and functions for the driver, I have defined that as the man driving the tank here
 */

class Driver : public Actor
{

public:
/**
 * Constructor
 * @param hp the amount of hit points the driver has
 */ 
	Driver(double hp);
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
 */
	void handleEventId(EventData const &eventMessage);


private:
	Shotgun *mpShotgun;
	bool mFire;
	double mHealthPoints;



};
}

#endif