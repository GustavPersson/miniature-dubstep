/**
 * @file Network.h
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

#ifndef __NETWORK_H__
#define __NETWORK_H__

#include "Protocol.h"

#include "ConnectionManager.h"

#include "MessageHandler.h"
#include "MsgH_GameServer.h"
#include "MsgH_GameClient.h"
#include "MsgH_PlayerID.h"
#include "MsgH_TextMessage.h"
#include "EventMsgHandler.h"

#include <pthread.h>

#include "Connector.h"
#include "ServerLookup.h"

#include "Peer.h"

#include "IdManager.h"

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif


#if PLATFORM == PLATFORM_WINDOWS

#include <winsock.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>
#endif

namespace network
{

/**
 * This function initializes winsoc.
 * @see ShutdownNetwork()
 */
inline bool InitializeNetwork()
{
	//InitializeSockets
	#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    return WSAStartup( MAKEWORD(1,1), &WsaData ) == NO_ERROR;
    #else
    return true;
    #endif
}

/**
 * This function shuts down winsoc.
 * @see InitializeNetwork()
 */
inline void ShutdownNetwork()
{
	#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
    #endif
}


/**
* Network is a wrapper class.
*/
class Network
{
public:

	/**
    * A constructor.
	* @param port the port you will be using as listening port.
    */
	Network(int port=9876);

	/**
	* A destructor.
	*/
	~Network();

	/**
    * Set you'r profile nickname, this must be done before you can use any of the other functions.
	* @param nickname this is the nick that will be seen by the other players.
    */
	void setProfile(std::string nickname);

	/**
    * Get a ponter to the peer obejct.
	* @return a pointer to the peer object.
    */
	Peer* getPeer();

	/**
    * A constructor.
	* @param ServerName the name of the server.
	* @param MaxPlayers the maximum number of clients that can join the server. 
    */
	void startServer(std::string ServerName, int MaxPlayers=4);
	
	/**
    * Stop the server. Do this right before starting the game.
	* The clients will still be connected to each other.
    */
	void stopServer();
	
	/**
    * join a server.
    */
	void join(std::string addr, int port=9876);
	
private:
	std::string mNickName;
	std::string mServerName;
	int mListeningPort;

	Peer *mPeer;
	ServerLookup mServerLookup;

	MessageHandler *mServerHandler, *mClientHandler;

	pthread_mutex_t mMutex;
	pthread_cond_t mCondVar;

};

} // endof namespace network.



#endif
