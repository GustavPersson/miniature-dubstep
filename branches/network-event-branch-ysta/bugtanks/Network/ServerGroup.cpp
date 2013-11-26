#include <iostream>
using namespace std;

#include "ServerGroup.h"

void* ServerGroup::socketFunc(void *args)
{
	ServerGroup *group = (ServerGroup*)args;
	
	SOCKET connection;
	SOCKADDR_IN addrSrc;
	int addrSrcSize = sizeof(addrSrc);
	int port;
	int nRet;
	do{
		nRet = recvfrom(group->mListeningSocket, (char *)&port, sizeof(int),
						0, (struct sockaddr*)&addrSrc, &addrSrcSize);

		if(group->mConnectionManager != NULL)
		{
			connection = socket(AF_INET, 0, IPPROTO_TCP);
			SOCKADDR_IN serverInfo;

			serverInfo.sin_family = AF_INET;
			serverInfo.sin_addr = addrSrc.sin_addr;
			serverInfo.sin_port = htons(port);

			if(connect(connection, (LPSOCKADDR)&serverInfo, sizeof(struct sockaddr)) == 0)
			{
				group->mConnectionManager->addConnection(connection);
			}
			else
			{
				cout << "Unable to connecto client!" << endl;
			}

			if(!group->mGreetingMessage.empty())
				send(connection, group->mGreetingMessage.c_str(), group->mGreetingMessage.length(), 0);
		}

	}while(group->isMember());
	pthread_exit(NULL);
	return NULL;
}

ServerGroup::ServerGroup(const char *groupAddr, int port)
{
	/* ---Setup listening socket--- */
	// Create a socket.
	mListeningSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	SOCKADDR_IN addrLocal;
	// We want to use the Internet address family
	addrLocal.sin_family = AF_INET;
	// Use any local address
	addrLocal.sin_addr.s_addr = INADDR_ANY;
	// Use arbitrary port - but the same as on other clients/servers
	addrLocal.sin_port = htons(port); 
	// Bind socket to our address
	if(SOCKET_ERROR == bind(mListeningSocket, (LPSOCKADDR)&addrLocal, 
							sizeof(struct sockaddr)))
	{cout << "Euston, we have a problem";}
	// Ready to switch to multicasting mode


	/* ---Setup multicasting socket--- */
	// Create a socket.
	mMulticastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	char TTL = 32 ; // Restrict to our school network, for example
	setsockopt(mMulticastSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&TTL, sizeof(TTL));

	// Set the local interface from which multicast is to be transmitted
	unsigned long addr = inet_addr("localhost");
	setsockopt(mMulticastSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr));

	/* ---Set mMulticastAddr--- */
	mMulticastAddr.sin_family = AF_INET;
	// Target multicast group address
	mMulticastAddr.sin_addr.s_addr = inet_addr(groupAddr);
	// Port on which client is set to receive data packets
	mMulticastAddr.sin_port = htons(port);
	// Something unoriginal to send

	mGroupAddr = groupAddr;
	mPort = port;
	mMember = false;
	mConnectionManager = NULL;
}
	
bool ServerGroup::isMember()
{
	return mMember;
}
	
void ServerGroup::join()
{
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(mGroupAddr.c_str());
	mreq.imr_interface.s_addr = INADDR_ANY;
	setsockopt(mListeningSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

	// start socket listener.
	pthread_create(&mSocketListener, NULL, socketFunc, this);

	mMember = true;
}
void ServerGroup::drop()
{
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr(mGroupAddr.c_str());
	mreq.imr_interface.s_addr = INADDR_ANY;
	setsockopt(mListeningSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq));

	mMember = false;
}
void ServerGroup::setConnectionManager(ConnectionManager *connectionM)
{
	mConnectionManager = connectionM;
}
void ServerGroup::setGreetingMessage(string greetingMessage)
{
	mGreetingMessage = greetingMessage;
}
void ServerGroup::multicast(const char *message, int len)
{
	int nRet = sendto(mMulticastSocket, message, len, 0,
				  (struct sockaddr*)&mMulticastAddr, sizeof(mMulticastAddr));
}
