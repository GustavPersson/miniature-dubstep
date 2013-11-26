/**
 * @file MsgH_PlayerID.h
 * @author Martin Wexö Olsson <mawm06@student.bth.se>
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

#ifndef _MSGH_PLAYERID_H_
#define _MSGH_PLAYERID_H_

#include <iostream>
#include <string>

#include "MessageHandler.h"
#include "IdManager.h"

namespace network
{

/**
 * The MsgH_PlayerID is a derived class from the MessageHandler.
 * It handles player identification messages.
 */
class MsgH_PlayerID : public MessageHandler
{
public:

	/**
    * A constructor.
    */
	MsgH_PlayerID();

	/**
    * All incoming messages goes trough this function. It handles peer-identification messages.
	* When constructed it starts listening for incoming connections on the specified port.
    * @param src important to identify routself in the publish list.
    * @param len witch port to listen to.
	* @param type witch port to listen to.
	* @param message witch port to listen to.
    */
	void handleMessage(SOCKET src, int len, int type, const char *message);
	
private:
	IdManager *mIdManager;
};
}

#endif
