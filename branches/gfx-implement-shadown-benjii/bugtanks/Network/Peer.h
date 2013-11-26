/**
 * @file Peer.h
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

#ifndef _PEER_H_
#define _PEER_H_

#include <winsock.h>

#include <map>

#include "Connector.h"
#include "ConnectionManager.h"
#include "MessageHandler.h"
#include "IdManager.h"

namespace network
{

/////////////////////////////////////////////////
/// ... Peer Class ...
/////////////////////////////////////////////////
/**
 * Class Description
 * The Peer class represent a peer node in a peer to peer network.
 * All nodes in the network has an ID number and a name. To connect to
 * the other peers in the network we need the publish list witch contains
 * information about all the nodes such as id, name, ipaddress and port.
 * When connected, a node can send and receive messages to and from all the
 * other nodes. To handle incoming messages it's possible to add a MessageHandler
 * object.
 */
class Peer
{
public:
	/**
    * A constructor.
	* When constructed it starts listening for incoming connections on the specified port.
    * @param name important to identify routself in the publish list.
    * @param port witch port to listen to.
    */
	Peer(std::string name, int port);

	/**
	* Get the peer's IdManager.
    * @return the peer's IdManager.
    */
	IdManager* getIdManager();

	/**
	* Get the peer's PeerManager.
    * @return a ConnectionManager that holds the peer connections.
    */
	ConnectionManager* getPeerManager();

	/**
	* Get the peer's ServerManager.
    * @return a ConnectionManager that holds the server connection.
    */
	ConnectionManager* getServerManager();

	/**
	* Connects to server.
    * @param serverAddr a string containing the server address.
    * @param port the servers listening port.
    */
	void joinNetwork(std::string serverAddr, int port);

	/**
	* Updates the network according to publish list.
    * @param publishList a pointer to the publish list file.
    * @param size the size of the publishList, in bytes.
    */
	void updateNetwork(const char *publishList, unsigned int size);
	
	
	/**
    * Send to all connected peer.
    * @param len length of the message beeing pointed to by the message parameter.
    * @param type an identifier that describes the content of the message.
	* @param message a pointer to the data beeing sent.
	* @see sendTo()
    * @see Protocol.h
    */
	void sendToAll(int len, int type, const char *message);

	/**
    * Send to peer by id.
	* @param id id number of peer.
    * @param len length of the message beeing pointed to by the message parameter.
    * @param type an identifier that describes the content of the message.
	* @param message a pointer to the data beeing sent.
	* @see sendToAll()
    */
	void sendTo(int id, int len, int type, const char *message);

	/**
    * Send to peer by name.
	* @param name name of peer.
    * @param len length of the message beeing pointed to by the message parameter.
    * @param type an identifier that describes the content of the message.
	* @param message a pointer to the data beeing sent.
	* @see sendToAll()
    */
	void sendTo(std::string name, int len, int type, const char *message);


	/// Data structure that holds information about a peer.
	struct PeerInfo_t
	{
		int id, port;
		std::string name, ip;
	};

private:
	std::string mName;
	int mID;
	int mPort;

	std::map<int, PeerInfo_t> mPeerList;

	Connector mConnector;
	ConnectionManager mPeerManager;
	ConnectionManager mServerManager;

	IdManager mIdManager;
};
}

#endif
