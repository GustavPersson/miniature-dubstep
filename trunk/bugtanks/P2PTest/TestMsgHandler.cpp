/**
* @file
* @author Gustav Persson <gustav@alltomminrov.se>
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
#include "TestMsgHandler.h"
#include "../Network/Protocol.h"
#include <strstream>

using namespace network;

TestMsgHandler::TestMsgHandler()
{
	mIdManager = IdManager::getInstance();
}

void TestMsgHandler::handleMessage(SOCKET src, int len, int type, const char * message)
{
	if(MessageType_Test==type)
	{
		int msg;
		
		std::istrstream in(message, len);
		
		in >> msg;

		DEBUG_MESSAGE(LEVEL_LOW, "PACKET NR: " << msg);
	}
}