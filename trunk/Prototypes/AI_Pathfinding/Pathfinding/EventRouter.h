/**
* @file
* @author Matteus Magnusson <senth.wallace@gmail.com>
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
* Routes events to listening clients
*/

#ifndef __EVENT_ROUTER_H__
#define __EVENT_ROUTER_H__

#include <list>
#include <queue>
#include <pthread.h>

// Forward declarations
class Event;
class EventHandler;
namespace utilities {
	class Timer;
}

struct EventPacket
{
	Event*	mpEvent;
	double	mTimeStamp;
};

class EventRouter
{
public:
	static EventRouter* getInstance();

	~EventRouter();

	void addListener(EventHandler* pEventHandler);
	void pushEvent(Event* pEvent);
	void setLatency(double latency);

	void detachAndRun();
	void kill();

private:
	static void* mainLoop(void* vpNull);
	EventRouter();
	bool getQuit();
	void processEvents();

	static EventRouter* mpsInstance;

	std::list<EventHandler*> mEventHandlers;
	std::queue<EventPacket> mEventPackets;
	double mLatency;
	bool mQuit;
	pthread_t mThread;
	pthread_mutex_t mQuitMutex;
	pthread_mutex_t mEventMutex;
	utilities::Timer* mpTimer;
};

#endif