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

#ifndef __TEST_MSG_HANDLER_H__
#define __TEST_MSG_HANDLER_H__

#include "../Network/MessageHandler.h"
#include "../Network/IdManager.h"

namespace network
{

/**
* Handles incoming event messages.
*/
class TestMsgHandler : public MessageHandler
{
public:
	TestMsgHandler();

	void handleMessage(SOCKET src, int len, int type, const char * message);
	
private:
	IdManager *mIdManager;
};
}

#endif