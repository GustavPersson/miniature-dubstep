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

#include "Macros.h"
#include "EventRouter.h"
#include "Event.h"
#include "EventHandler.h"
#include "../../../bugtanks/Utilities/Timer.h"
#include <windows.h>
#include <list>

const int SLEEP_TIME = 1000;

using utilities::Timer;

EventRouter* EventRouter::mpsInstance = NULL;

EventRouter* EventRouter::getInstance()
{
	if (mpsInstance == NULL) {
		mpsInstance = myNew EventRouter();
	}
	return mpsInstance;
}

EventRouter::EventRouter()
{
	mLatency = 0;
	mpTimer = myNew Timer();
	mpTimer->start();
	mQuit = false;
	pthread_mutex_init(&mQuitMutex, NULL);
	pthread_mutex_init(&mEventMutex, NULL);
}

EventRouter::~EventRouter()
{
	mpsInstance = NULL;
	SAFE_DELETE(mpTimer);
	
	// Delete the rest of the events in the queue
	while (!mEventPackets.empty())
	{
		SAFE_DELETE(mEventPackets.front().mpEvent);
		mEventPackets.pop();
	}

	// kill the thread
	kill();
	pthread_join(mThread, NULL);
	pthread_mutex_destroy(&mQuitMutex);
	pthread_mutex_destroy(&mEventMutex);
}

void EventRouter::addListener(EventHandler *pEventHandler)
{
	DEBUG_MESSAGE("EventRouter::addListener()");
	pthread_mutex_lock(&mEventMutex);
	mEventHandlers.push_back(pEventHandler);
	pthread_mutex_unlock(&mEventMutex);
}

void EventRouter::pushEvent(Event* pEvent)
{
	// Get timestamp and add the event to the event queue
	DEBUG_MESSAGE("EventRouter::pushEvent()");
	double timeStamp = mpTimer->getTime(Timer::ReturnType_MilliSeconds);
	EventPacket packet;
	packet.mpEvent = pEvent;
	packet.mTimeStamp = timeStamp;

	pthread_mutex_lock(&mEventMutex);
	mEventPackets.push(packet);
	pthread_mutex_unlock(&mEventMutex);
}

void EventRouter::setLatency(double latency)
{
	pthread_mutex_lock(&mEventMutex);
	mLatency = latency;
	pthread_mutex_unlock(&mEventMutex);
}

void EventRouter::detachAndRun()
{
	mQuit = false;
	pthread_create(&mThread, NULL, mainLoop, NULL);	
}

void EventRouter::kill()
{
	pthread_mutex_lock(&mQuitMutex);
	mQuit = true;
	pthread_mutex_unlock(&mQuitMutex);
}

void* EventRouter::mainLoop(void *vpNull)
{
	EventRouter* pEventRouter = getInstance();
	DEBUG_MESSAGE("EventRouter::mainLoop()");
	while (!pEventRouter->getQuit())
	{
		pEventRouter->processEvents();
		Sleep(SLEEP_TIME);
	}

	DEBUG_MESSAGE("EventRouter::mainLoop() | ending");
	return NULL;
}

bool EventRouter::getQuit()
{
	pthread_mutex_lock(&mQuitMutex);
	bool quit = mQuit;
	pthread_mutex_unlock(&mQuitMutex);
	return quit;
}

void EventRouter::processEvents()
{
	// If the timestamp is over the latency we can pop it
	pthread_mutex_lock(&mEventMutex);
	if (!mEventPackets.empty())
	{
		DEBUG_MESSAGE("packetTimestamp: " << mEventPackets.front().mTimeStamp << ", timer: " << mpTimer->getTime(Timer::ReturnType_MilliSeconds) + mLatency);
	}
	while (!mEventPackets.empty() && mEventPackets.front().mTimeStamp <= mpTimer->getTime(Timer::ReturnType_MilliSeconds) + mLatency)
	{
		DEBUG_MESSAGE("EventRouter::processEvents() | We have a packet");
		EventPacket packet = mEventPackets.front();
		mEventPackets.pop();

		// Forward it to the listeners
		std::list<EventHandler*>::iterator it;
		for (it = mEventHandlers.begin(); it != mEventHandlers.end(); ++it)
		{
			DEBUG_MESSAGE("EventRouter::processEvents() | Sending event");
			(*it)->pushEvent(packet.mpEvent);
		}

		// delete the event
		DEBUG_MESSAGE("EventRouter::processEvents() | Deleting event");
		SAFE_DELETE(packet.mpEvent);
	}
	pthread_mutex_unlock(&mEventMutex);
}