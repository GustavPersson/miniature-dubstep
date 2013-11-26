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
 *
 * @section DESCRIPTION
 *
 * This is the driver class, it holds information and functions for the driver, I have defined that as the man driving the tank here
 */

#include "Driver.h"

using namespace logic;


Driver::Driver(double hp)
{
	mHealthPoints = hp;
	mFire = false;
	mpShotgun = myNew Shotgun(10,3,7,1);
	mActorType = ActorType_Driver;	
}


Driver::~Driver()
{
	SAFE_DELETE(mpShotgun);
}

void Driver::thrownOutOfTank()
{
  //send event to GFX to render a player model beside the tank
  //send event to AI to now take control over tank
  //change state to outsideTank, meaning input will now move Driver and position will be updated for Driver
}


void Driver::fireShotgun()
{
   if(mFire)
	   mpShotgun->fireWeapon();
   //if gamestate is outsideTank the fire command will now be directed here
   //send event to GFX for shotgun firing effects and collisioninformation
}


void Driver::enterTank()
{
  //if a tank is in range the driver will jump in
  //stop rendering driver
  //change gamestate to insideTank, meaning input will now move the tank and positions will be update for tank
}

void Driver::update(float deltaTime)
{
	//position will not be updated for driver while gamestate is insideTank
	Actor::mPosition.x += (PLAYER_MOVE_SPEED * Actor::mDirection.x * deltaTime);
	Actor::mPosition.y += (PLAYER_MOVE_SPEED * Actor::mDirection.y * deltaTime);
	Actor::mPosition.z += (PLAYER_MOVE_SPEED * Actor::mDirection.z * deltaTime);
}

void Driver::handleEventId(EventData const &eventMessage)
{
	if(EvtData_To_DriverStartFire::mEventType == eventMessage.getEventType())
	{
		mFire = true;
	}
	if(EvtData_To_DriverStopFire::mEventType == eventMessage.getEventType())
	{
		mFire = false;
	}
	
}
