/**
 * @file ServerGroup.h
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

#ifndef _SERVERGROUP_H_
#define _SERVERGROUP_H_

#include <string>
#include <pthread.h>
#include "Network.h"
#include "ConnectionManager.h"

/// A Prototype class for the ConnectionManager.
class ConnectionManager;

/////////////////////////////////////////////////
/// ... ServerGroup Class ...
/////////////////////////////////////////////////
/**
 * Class Description
 * The ServerGroup represents a buch of servers grouped as one server.
 * This means that you only need one ipaddress when sending multicasting to the group.
 */
class ServerGroup
{
private:
	SOCKET mListeningSocket;
	SOCKET mMulticastSocket;
	SOCKADDR_IN  mMulticastAddr;
	pthread_t mSocketListener;

	static void* socketFunc(void *args);

	string mGroupAddr;
	int mPort;
	string mGreetingMessage;
	bool mMember;
	
	ConnectionManager *mConnectionManager;
public:

	/**
    * A constructor.
	* This will setup a udp socket for multicasting to the server group.
	* @param groupAddr the address to the multicast group.
	* @param port the listening port for the multicast group.
    */
	ServerGroup(const char *groupAddr, int port);
	
	/**
    * Check if you are a member of the server group.
	* Useful to se if the join operation was successful.
	* @return true if you are a member.
	* @see isMember()
    */
	bool isMember();
	
	/**
    * When joining the group a udp listening socket is created for receiving multicast messages.
    */
	void join();
	
	/**
    * This is the oposite of joining a group.
    */
	void drop();
	
	/**
    * Set a ConnectionManager that will be responsible for handleing client connections.
	* @param connectionM is a pointer to a ConnectionManager.
    */
	void setConnectionManager(ConnectionManager *connectionM);
	
	/**
    * Returns a socket associated with the specified id.
	* @param greetingMessage is the auto-reply message that will be sent back to the client when the connection is established
	* i.ex: a useful message would be to reply with the name of the server, if its a game server then it would be the name of the game.
    */
	void setGreetingMessage(string greetingMessage);
	
	/**
    * Multicasts a message to the servergourp.
	* @param message a pointer to the message data.
	* @param len the lenght of the message, in bytes.
    */
	void multicast(const char *message, int len);
};

#endif
