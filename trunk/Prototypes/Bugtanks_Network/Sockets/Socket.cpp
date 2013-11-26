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

#include "Socket.h"

Socket::Socket()
{

}

int Socket::connectTo(const char *hostName, int port)
{
	int nret; // ErrorCode

	// Store information about the server
	LPHOSTENT hostEntry;

	hostEntry = gethostbyname(hostName);	// Specifying the server by its name;
												// another option: gethostbyaddr()
	if (!hostEntry)
	{
		nret = WSAGetLastError();
		return NETWORK_ERROR;
	}


	mSocket = socket(AF_INET,			// Go over TCP/IP
					   SOCK_STREAM,		// This is a stream-oriented socket
					   IPPROTO_TCP);	// Use TCP rather than UDP

	if (mSocket == INVALID_SOCKET)
	{
		nret = WSAGetLastError();
		return NETWORK_ERROR;
	}

	

	// Fill a SOCKADDR_IN struct with address information
	SOCKADDR_IN serverInfo;

	serverInfo.sin_family = AF_INET;

	// At this point, we've successfully retrieved vital information about the server,
	// including its hostname, aliases, and IP addresses.  Wait; how could a single
	// computer have multiple addresses, and exactly what is the following line doing?
	// See the explanation below.

	serverInfo.sin_addr = *((LPIN_ADDR)*hostEntry->h_addr_list);

	serverInfo.sin_port = htons(port);		// Change to network-byte order and
											// insert into port field


	// Connect to the server
	nret = connect(mSocket,
		       (LPSOCKADDR)&serverInfo,
		       sizeof(struct sockaddr));

	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		return NETWORK_ERROR;
	}
	return NETWORK_OK;
}

int Socket::listenTo(int port)
{
	int nret; // ErrorCode
	mSocket = socket(AF_INET,			// Go over TCP/IP
					   SOCK_STREAM,		// This is a stream-oriented socket
					   IPPROTO_TCP);	// Use TCP rather than UDP

	if (mSocket == INVALID_SOCKET)
	{
		nret = WSAGetLastError();
		return NETWORK_ERROR;
	}


	// Use a SOCKADDR_IN struct to fill in address information
	SOCKADDR_IN serverInfo;

	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;	// Since this socket is listening for connections,
												// any local address will do

	serverInfo.sin_port = htons(port);			// Convert integer 8888 to network-byte order
												// and insert into the port field


	// Bind the socket to our local server address
	nret = bind(mSocket, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));

	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		return NETWORK_ERROR;
	}


	// Make the socket listen
	nret = listen(mSocket, 10);		// Up to 10 connections may wait at any
									// one time to be accept()'ed

	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		return NETWORK_ERROR;
	}
	return NETWORK_OK;
}

int Socket::waitToAccept(Socket &sock)
{
	int nret; // ErrorCode
	sock.mSocket = accept(mSocket,
							NULL,	// Optionally, address of a SOCKADDR_IN struct
							NULL);	// Optionally, address of variable containing
									// sizeof ( struct SOCKADDR_IN )

	if (sock.mSocket == INVALID_SOCKET)
	{
		nret = WSAGetLastError();
		return NETWORK_ERROR;
	}
	return NETWORK_OK;
}

void Socket::close()
{
	closesocket(mSocket);
}