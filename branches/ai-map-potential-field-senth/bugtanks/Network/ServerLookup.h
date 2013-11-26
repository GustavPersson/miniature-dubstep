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

#include "../Utilities/Macros.h"
#include <winsock.h>
#include <pthread.h>
#include <string>
#include <vector>

namespace network
{

class ServerInfo
{
public:
	ServerInfo();
	ServerInfo(std::string name_, std::string addr_, u_short port_, float timeToLive_=2.0f);

	std::string name;
	std::string addr;
	u_short port;
	float timeToLive;
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
		u_short multicastPort = 4563);

	/**
	* A destructor.
	*/
	~ServerLookup();

	/**
	* Clear the server list and makes a new request;
	*/
	void refresh(float deltaTime);

	/**
	* Send a lookup message,
	* this is only usefull after you've called startLookup();
	* @see startLookup();
	* @see stopLookup();
	*/
	void sendLookupMessage();

	/**
	* Register server so clients can find it using ServerLookup.
	* @param ServerName the name of the server.
	* @param ServerPort the listening port of the server.
	*/
	void registerServer(std::string ServerName, u_short ServerPort);

	/**
	* Unregisters a server so it's not vissible for other clients anymore.
	* If a server is registered it must be unregistered before shutting down winsock.
	* @param ServerName the name of the server.
	*/
	void unregisterServer(std::string ServerName);

	/**
	* Get list of found servers.
	* @return a vector with ServerInfo.
	*/
	std::vector<ServerInfo> getServerList()const;

	
private:
	u_short mMulticastPort;
	std::string mMulticastAddr;

	SOCKET mMulticastSocket;
	SOCKET mServerGroupSocket;
	//SOCKADDR_IN  mMulticastAddr;
	pthread_t mSocketListener;
	std::vector<ServerInfo> mServerList;

	static void* listeningFunc(void *args);


	std::vector<ServerInfo> mRegisteredServers;
	SOCKET mMulticastRecvSocket;
	pthread_t mMulticastRecvTread;
	static void* multicastRecvFunc(void *args);
};
}

#endif