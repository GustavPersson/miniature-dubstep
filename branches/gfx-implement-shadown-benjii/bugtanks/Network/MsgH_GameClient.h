/**
 * @file MsgH_GameClient.h
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

#ifndef _MSGH_GAMECLIENT_H_
#define _MSGH_GAMECLIENT_H_

#include "MessageHandler.h"
#include "Peer.h"

namespace network
{

/////////////////////////////////////////////////
/// ... MsgH_GameClient Class ...
/////////////////////////////////////////////////
/**
 * Class Description
 * The MsgH_GameClient is a derived class from the MessageHandler.
 * This class updates the peer network whenever a new updated publish list
 * is received from the server.
 */
class MsgH_GameClient : public MessageHandler
{
private:
	Peer *mPeer;

public:

	/**
    * A constructor.
    * @param peer a pointer to a Peer object.
    */
	MsgH_GameClient(Peer *peer);

	/**
    * All incoming messages goes trough this function. It handles peer-identification messages.
	* When constructed it starts listening for incoming connections on the specified port.
    * @param src important to identify routself in the publish list.
    * @param len witch port to listen to.
	* @param type witch port to listen to.
	* @param message witch port to listen to.
    */
	void handleMessage(SOCKET src, int len, int type, const char *message);
};
}

#endif
