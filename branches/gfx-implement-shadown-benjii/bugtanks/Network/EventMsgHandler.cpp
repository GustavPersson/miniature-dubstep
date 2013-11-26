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
* 
*/
#include "../Utilities/Macros.h"
#include "EventMsgHandler.h"
#include "Protocol.h"
#include "../Logic/Events.h"
#include <iostream>
#include <sstream>

using namespace network;

EventMsgHandler::EventMsgHandler(IdManager *idManager)
{
	mIdManager = idManager;
}

void EventMsgHandler::handleMessage(SOCKET src, int len, int type, const char * message)
{
	std::istrstream in(message, len);

	switch(type)
	{
	case MessageType_Event:
		char eventType[256];
		in >> eventType;
		if (!_stricmp(eventType, logic::EvtData_To_ActorStartMove::mEventType.getStr().c_str()))
		{
			logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStartMove(in)));
		}
		else if (!_stricmp(eventType, logic::EvtData_To_ActorStopMove::mEventType.getStr().c_str()))
		{
			logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStopMove(in)));
		}
		else
		{
			DEBUG_MESSAGE("ERROR: Unknown event type from remote: " << eventType);
		}
	}
}