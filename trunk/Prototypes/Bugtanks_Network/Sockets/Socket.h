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
 * 
 * @section DESCRIPTION
 *
 * Class Description
 */

#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <iostream>
using namespace std;

#define NETWORK_ERROR -1
#define NETWORK_OK     0

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


inline bool InitializeSockets()
{
	#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    return WSAStartup( MAKEWORD(1,1), &WsaData ) == NO_ERROR;
    #else
    return true;
    #endif
}

inline void ShutdownSockets()
{
	#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
    #endif
}

class Socket
{
private:
	SOCKET mSocket;
public:
	Socket();
	Socket(SOCKET soc);

	int tcpConnect(const char *address, int port);


	int connectTo(const char *address, int port); // Connect to host by name.

	int listenTo(int port);	// Setup the port to listen for incoming connections.
	int waitToAccept(Socket &sock);	// Wait until a connection appears and accepts it.



	void close(); //Close the socket.

	// implicitly act as a SOCKET
	inline operator SOCKET() {
		return mSocket;
	}
};


#endif