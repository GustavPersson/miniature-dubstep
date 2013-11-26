/**
* @file
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

#ifndef __SERVER_LOOKUP_H__
#define __SERVER_LOOKUP_H__

#include <winsock.h>
#include <pthread.h>
#include <string>
#include <vector>

namespace network
{

struct ServerInfo
{
	ServerInfo(std::string name_, std::string addr_, int port_)
	{
		name = name_;
		addr = addr_;
		port = port_;
	}
	std::string name;
	std::string addr;
	int port;
};

/**
* ServerLookup chekcs the network for hosted servers.
*/
class ServerLookup
{
public:
	/**
    * A constructor.
	* @param multicastAddr is the address to the multicastgroup
	* that we send the lookup message to, default set to 234.5.6.7.
	* @param multicastPort the port on witch the servers provide this lookup service,
	* default set to 9633.
    */
	ServerLookup(std::string multicastAddr = std::string("234.5.6.7"),
		int multicastPort = 9633);

	/**
	* Open a listening socket and start the listening to receive server info.
	* @param listeningPort the port we bind our socket to.
	* @see stopLookup();
	*/
	void startLookup(int listeningPort);

	/**
	* Send a lookup message,
	* this is only usefull after you've called startLookup();
	* @see startLookup();
	* @see stopLookup();
	*/
	void sendLookupMessage();

	/**
	* Stop the listening, and closes the listening socket.
	* @see startLookup();
	*/
	void stopLookup();

	/**
	* Register server so clients can find it using ServerLookup.
	* @param ServerName the name of the server.
	* @param ServerPort the listening port of the server.
	*/
	void registerServer(std::string ServerName, int ServerPort);

	/**
	* Get list of found servers.
	* @return a vector with ServerInfo.
	*/
	std::vector<ServerInfo> getServerList()const;

	
private:
	int mListeningPort;
	SOCKET mListeningSocket;
	SOCKET mMulticastSocket;
	SOCKET mServerGroupSocket;
	SOCKADDR_IN  mMulticastAddr;
	pthread_t mSocketListener;
	std::vector<ServerInfo> mServerList;

	static void* listeningFunc(void *args);
};
}

#endif