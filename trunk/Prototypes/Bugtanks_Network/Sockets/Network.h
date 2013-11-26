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

#ifndef _NETWORK_H_
#define _NETWORK_H_

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

#include "Protocol.h"
#include "Connector.h"
#include "ServerGroup.h"
#include "ConnectionManager.h"
#include "MessageHandler.h"
#include "IdManager.h"


#endif
