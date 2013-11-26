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

#ifndef __TIMER_H__
#define __TIMER_H__

#include "../Utilities/Macros.h"

#include <time.h>

namespace logic
{

/**
 * This class is used to time an event of your choice. 
 */
	
class Timer
{
public:
	enum ReturnTypes
	{
		ReturnType_MilliSeconds, 
		ReturnType_Seconds,
		ReturnType_Minutes,
	};

	/**
	* The constructor.
	*/
	Timer();

	/**
	* The destructor.
	*/
	~Timer();
	
	/**
	* Starts the timer.
	*/
	void start();

	/**
	* Returns the time since start() was called in milliseconds, seconds or minutes.
	* @param retType Determines return type depending on ReturnType enumeration.
	* @return Time elapsed since start.
	*/
	float getTime(ReturnTypes retType);

	/**
	* Returns the time in seconds since the last time this function was called.
	* @return Returns the time since the last tick.
	*/
	float tick();

private:
	clock_t mBegin, mLastTime;

};
}
#endif