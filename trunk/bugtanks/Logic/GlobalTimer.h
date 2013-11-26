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
*/

#ifndef __GLOBAL_TIMER_H__
#define __GLOBAL_TIMER_H__

#include "../Utilities/Macros.h"
#include "../Utilities/Timer.h"
#include <pthread.h>

namespace logic
{

/**
 * This is a global timer so that all classes can access the same time.
 */

class GlobalTimer
{
public:
	/**
	* The destructor.
	*/
	~GlobalTimer();

	/**
	* Returns the only instance of this object.
	* @return Returns the instance of this object
	*/
	static GlobalTimer* getInstance();

	/**
	* Return the time since the GlobalTimer was created.
	* @param retType Decides which timeformat that is returned. Millisec, seconds or minutes.
	* @return Return the time since the timer started.
	*/
	float getTime(utilities::Timer::ReturnTypes retType);

	/**
	* Returns the time in seconds since the last time this function was called.
	* @return Returns the time since the last tick.
	*/

	float tick();



	
private:
	/**
	* The constructor.
	*/
	GlobalTimer();
	
	utilities::Timer *mpTimer;
	
	static GlobalTimer *mpsInstance;




};
}

#endif