/**
 * @file MsgH_GameServer.h
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

#ifndef _MSGH_GAMESERVER_H_
#define _MSGH_GAMESERVER_H_

#include <string>

#include "ConnectionManager.h"
#include "MessageHandler.h"
#include "IdManager.h"

namespace network
{

/////////////////////////////////////////////////
/// ... MsgH_GameServer Class ...
/////////////////////////////////////////////////
/**
 * Class Description
 * The MsgH_GameServer is a derived class from the MessageHandler.
 * The GameServer is implemeted as a message handler because then we
 * can easely turn a peer into a gameserver as an add-on feature just
 * by adding the MsgH_GameServer object as one of the message handler
 * to the peer.
 */
class MsgH_GameServer : public MessageHandler
{
private:
	const int MAX_PLAYERS;
	std::string mGameName;
	IdManager mIdManager;
	ConnectionManager *mConnectionManager;

	void sendPublishList();
public:

	/**
    * A constructor.
    * @param gameName the name of the server.
	* @param CM a pointer to a ConnectionManager object.
	* @param maxPlayers maximum number of players that can join the game, default set to 4.
    */
	MsgH_GameServer(std::string gameName, ConnectionManager *CM, int maxPlayers=4);

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
