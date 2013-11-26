/**
 * @file
 * @author Gustav Persson <ninjamuff@gmail.com>
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
#ifndef __REALTIMEEVENTLISTENER_H__
#define __REALTIMEEVENTLISTENER_H__

#include "IEventManager.h"

namespace logic
{
	ThreadSafeEventQueue gRealTimeEventQueue;

/**
 * Event listener that handles events fired in real time,
 * if needed for two real time processes to communicate,
 * it lets the threads themselves handle the events, by just
 * putting them in a queue.
 */
class RealTimeEventListener : public EventListener
{
public:
	char const* getName(void) {return "RealTimeEventListener"; }
	/*
	* Handles events.
	* @param eventData The event that needs handling.
	*/
	virtual bool handleEvent(EventData const & eventData);

private:
	ThreadSafeEventQueue mRealTimeEventQueue;
};
}
#endif