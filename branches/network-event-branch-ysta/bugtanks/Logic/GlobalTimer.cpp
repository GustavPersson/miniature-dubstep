/**
* @file
* @author Jonathan Udd <jonis.x@gmail.com>
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
* This is a global timer so that all classes can access the same time. 
*/

#include "GlobalTimer.h"

using namespace logic;

GlobalTimer* GlobalTimer::mpsInstance = NULL;

GlobalTimer::~GlobalTimer()
{
	// Delete the timer
	SAFE_DELETE(mpTimer);
	mpsInstance = NULL;
}

GlobalTimer* GlobalTimer::getInstance()
{
	// If there is no instance of this object
	if(mpsInstance == NULL)
	{
		// Create an instance of this object
		mpsInstance = myNew GlobalTimer();
	}
	// return GlobalTimer
	return mpsInstance;
}

float GlobalTimer::getTime(Timer::ReturnTypes retType)
{
	// Call getTime from the timer class, provide retType
	return mpTimer->getTime(retType);
}

GlobalTimer::GlobalTimer()
{
	// Create a timer
	mpTimer = myNew Timer();
	// Start the timer
	mpTimer->start();
}

float GlobalTimer::tick()
{
	return mpTimer->tick();
}

