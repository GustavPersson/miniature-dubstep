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

#include "../Sockets/Network.h"
#include "PeerMessageHandler.h"


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
	Peer(string name, int port);

	/**
    * Parses the publish list and connects to all peers that is listed below itself
    * @param publishList a pointer to the publish list file.
    * @param size the size of the publishList, in bytes.
    */
	void joinNetwork(const char *publishList, unsigned int size);
	
	
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
	void sendTo(string name, int len, int type, const char *message);


	/**
    * Add additional message handlers to handle incoming messages.
	* @param msgH a pointer to the MessageHandler object.
    */
	void addMessageHandler(MessageHandler *msgH);



private:
	string mName;
	int mID;

	Connector mConnector;
	ConnectionManager mConnectionManager;
	IdManager mIdManager;

	friend class PeerMessageHandler;
};

#endif
