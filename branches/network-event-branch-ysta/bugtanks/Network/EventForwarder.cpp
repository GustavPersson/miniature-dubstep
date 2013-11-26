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

#include "EventForwarder.h"
#include "../Logic/Events.h"
#include "Peer.h"
#include "Protocol.h"

using namespace network;

bool EventForwarder::handleEvent(const logic::EventData &eventData)
{
	std::ostrstream out;

	//out << static_cast<int>(MessageType_Event) << " ";
	out << eventData.getEventType().getStr() << " ";

	eventData.serialize(out);

	out << "\r\n";

	mPeer->sendToAll(out.pcount(), static_cast<int>(MessageType_Event), out.rdbuf()->str());

	return true;
}