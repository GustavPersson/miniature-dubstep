#include <iostream>
using namespace std;

#include "Connector.h"

void* Connector::socketFunc(void *args)
{
	if(pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL) != 0)
	{
		cout << "ERROR(socketFunc): pthread_setcancelstate -> PTHREAD_CANCEL_ENABLE" << endl;
	}
	if(pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
	{
		cout << "ERROR(socketFunc): setcanceltype -> PTHREAD_CANCEL_ASYNCHRONOUS" << endl;
	}

	Connector *connector = (Connector*)args;
	SOCKET connection;
	do{
		connection = accept(connector->mListeningSocket, NULL, NULL);
		if (connector->mListeningSocket != INVALID_SOCKET)
		{
			if(connector->mConnectionManager != NULL)
				connector->mConnectionManager->addConnection(connection);
		}
	}while(true);
}

Connector::Connector(ConnectionManager *connectionManager)
{
	mConnectionManager = connectionManager;
	mListeningSocket = socket(AF_INET, 0, IPPROTO_TCP);
}
void Connector::setConnectionManager(ConnectionManager *CM)
{
	mConnectionManager = CM;
}
bool Connector::startAccept(int port)
{
	int nret;

	// Use a SOCKADDR_IN struct to fill in address information
	SOCKADDR_IN serverInfo;

	serverInfo.sin_family = AF_INET;
	serverInfo.sin_addr.s_addr = INADDR_ANY;	// Since this socket is listening for connections,
												// any local address will do

	serverInfo.sin_port = htons(port);			// Convert integer 8888 to network-byte order
												// and insert into the port field

	// Bind the socket to our local server address
	nret = bind(mListeningSocket, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr));

	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		return false;
	}

	// Make the socket listen
	nret = listen(mListeningSocket, 10);		// Up to 10 connections may wait at any
												// one time to be accept()'ed

	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		return false;
	}

	pthread_create(&mSocketListener, NULL, socketFunc, this);

	return true;
}
void Connector::stopAccept()
{
	pthread_cancel(mSocketListener);
}
bool Connector::connectTo(const char *addr, int port, SOCKET *connection)
{
	 int nret; // ErrorCode

	// Store information about the server
	LPHOSTENT hostEntry;

	hostEntry = gethostbyname(addr);	// Specifying the server by its name;
												// another option: gethostbyaddr()
	if (!hostEntry)
	{
		nret = WSAGetLastError();
		return false;
	}


	SOCKET soc = socket(AF_INET, 0, IPPROTO_TCP);

	if (soc == INVALID_SOCKET)
	{
		nret = WSAGetLastError();
		return false;
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
	nret = connect(soc,
		       (LPSOCKADDR)&serverInfo,
		       sizeof(struct sockaddr));

	if (nret == SOCKET_ERROR)
	{
		nret = WSAGetLastError();
		return false;
	}
	if(connection !=NULL)
		*connection = soc;

	if(mConnectionManager != NULL)
		mConnectionManager->addConnection(soc);

	return true;
}
