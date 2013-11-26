#pragma once
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
 * the License, or (at your option) any later version.e
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 * 
 * @section DESCRIPTION
 *
 * The Event Snooper class accepts all fired events, and logs them to
 * the debug console.
 */


#include "snooper.h"
#include "Events.h"

using namespace logic;

EventSnooper::EventSnooper(char const * const mLogFileName) : mOutFile(0)
{
	if(mLogFileName)
	{
		fopen_s(&mOutFile, mLogFileName, "w+");
	}
}

EventSnooper::~EventSnooper()
{
	if(mOutFile)
		fclose(mOutFile);
}

bool EventSnooper::handleEvent(EventData const &eventData)
{

#ifdef _DEBUG

	if (mOutFile)
	{
		fprintf_s(mOutFile, "Event Snoop: Event %p Time: %f Type: %s \n", &eventData, eventData.getTimeStamp(), eventData.getEventType().getStr().c_str());
	}
	
	if (eventData.getEventType() == EvtData_From_ActorHandlerSyncActors::msEventType)
	{
		DEBUG_MESSAGE(LEVEL_LOWEST, "Event Snoop: Event " << &eventData << " Time: " << eventData.getTimeStamp() << " Type: " << eventData.getEventType().getStr()
			<< " Origin: " << eventData.getNetworkId());
	}
	else if (eventData.getEventType() == EvtData_System_MouseMove::msEventType)
	{
		DEBUG_MESSAGE(LEVEL_HEATON, "Event Snoop: Event " << &eventData << " Time: " << eventData.getTimeStamp() << " Type: " << eventData.getEventType().getStr()
			<< " Origin: " << eventData.getNetworkId());
	}

	else if (eventData.getEventType() == EvtData_System_MouseKeyUp::msEventType || eventData.getEventType() == EvtData_System_MouseKeyDown::msEventType)
	{
		DEBUG_MESSAGE(LEVEL_HEATON, "Event Snoop: Event " << &eventData << " Time: " << eventData.getTimeStamp() << " Type: " << eventData.getEventType().getStr()
			<< " Origin: " << eventData.getNetworkId());
	}
	else
	{
		DEBUG_MESSAGE(LEVEL_MEDIUM, "Event Snoop: Event " << &eventData << " Time: " << eventData.getTimeStamp() <<  " type: " << eventData.getEventType().getStr()
			<< " Origin: " << eventData.getNetworkId());
	}

#endif

	return false;
}
