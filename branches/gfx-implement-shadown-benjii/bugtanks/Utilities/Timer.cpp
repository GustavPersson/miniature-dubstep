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
 * This class is used to time an event of your choice. 
 */

#include "Timer.h"

using namespace utilities;

const float TICKS_IN_MILLISECONDS	= 1000;
const float TICKS_IN_SECONDS		= 1000 * TICKS_IN_MILLISECONDS;
const float TICKS_IN_MINUTES		= 60 * TICKS_IN_SECONDS;

Timer::Timer()
{
	mBegin = 0;
	mLastTime = 0;
}

Timer::~Timer()
{
}

void Timer::start()
{
	mBegin = clock() * CLK_TCK;
	mLastTime = mBegin;
}

float Timer::getTime(ReturnTypes retType)
{
	clock_t time = clock() * CLK_TCK;
	float mElapTicks = (float)(time - mBegin);

	switch(retType)
	{
	case ReturnType_MilliSeconds:
		return mElapTicks / TICKS_IN_MILLISECONDS;
	case ReturnType_Seconds:
		return mElapTicks / TICKS_IN_SECONDS;
	case ReturnType_Minutes:
		return mElapTicks / TICKS_IN_MINUTES;
	default:
		ERROR_MESSAGE(20001, "Invalid returntype in timer.cpp");
		break;
	}
	return 0.0;
}

float Timer::tick()
{
	clock_t newTime = clock() * CLK_TCK;
	float deltaTime = (float)(newTime - mLastTime) / TICKS_IN_SECONDS;
	mLastTime = newTime;
	return deltaTime;
}
