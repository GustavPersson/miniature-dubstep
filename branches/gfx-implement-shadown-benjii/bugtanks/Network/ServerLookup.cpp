#include <iostream>
#include <sstream>

#include "ServerLookup.h"

using namespace std;
using namespace network;

ServerLookup::ServerLookup(std::string multicastAddr, int multicastPort)
{
	// ---Setup multicasting socket---
	// Create a socket.
	mMulticastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	char TTL = 32 ; // Restrict to our school network, for example
	setsockopt(mMulticastSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&TTL, sizeof(TTL));

	// Set the local interface from which multicast is to be transmitted
	unsigned long addr = inet_addr("localhost");
	setsockopt(mMulticastSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr));

	// ---Set mMulticastAddr---
	this->mMulticastAddr.sin_family = AF_INET;
	// Target multicast group address
	this->mMulticastAddr.sin_addr.s_addr = inet_addr(multicastAddr.c_str());
	// Port on which client is set to receive data packets
	this->mMulticastAddr.sin_port = htons(multicastPort);
	// Something unoriginal to send
}

void ServerLookup::startLookup(int listeningPort)
{
	this->mListeningPort = listeningPort;
	SOCKADDR_IN addrLocal;

	if((this->mListeningSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
	{
		cout << "Euston, we have a problem";
		return;
	}

	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(listeningPort);
	addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(this->mListeningSocket, (LPSOCKADDR)&addrLocal, sizeof(addrLocal)) == -1)
	{
		cout << "Euston, we have a problem";
		return;
	}

	// Start listening thread.
	pthread_create(&mSocketListener, NULL, listeningFunc, this);
}

void ServerLookup::sendLookupMessage()
{
	// Compile message.
	ostringstream lookupMessage(ios_base::in | ios_base::out);
	lookupMessage << "SLookup: " << this->mListeningPort;

	sendto(this->mMulticastSocket,
		lookupMessage.str().c_str(),
		lookupMessage.str().length(),
		0,
		(struct sockaddr*)&this->mMulticastAddr,
		sizeof(this->mMulticastAddr));
}

void ServerLookup::stopLookup()
{
	// Stop listening thread.
	pthread_cancel(mSocketListener);

	// Close listening socket.
	closesocket(this->mListeningSocket);
}

vector<ServerInfo> ServerLookup::getServerList()const
{
	return this->mServerList;
}

void* ServerLookup::listeningFunc(void *args)
{
	if(pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL) != 0)
	{
		cout << "ERROR(ServerLookup::listeningFunc): pthread_setcancelstate -> PTHREAD_CANCEL_ENABLE" << endl;
	}
	if(pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
	{
		cout << "ERROR(ServerLookup::listeningFunc): setcanceltype -> PTHREAD_CANCEL_ASYNCHRONOUS" << endl;
	}

	ServerLookup *SL = (ServerLookup*)args;

	string ServerName;
	int ServerPort;

	SOCKADDR_IN addrSrc;
	int addrSrcSize = sizeof(addrSrc);

	const int BUFLEN=256;
	char buffer[BUFLEN];

	int brecv;

	do{
		brecv = recvfrom(SL->mListeningSocket, buffer, BUFLEN, 0, (LPSOCKADDR)&addrSrc, &addrSrcSize);
		if (brecv==-1)
		{
			cout << "[ServerLookup::listeningFunc]: Trouble receving!" << endl;
		}
		else
		{
			cout << "I got a message" << endl;
			istringstream iss (string(buffer, brecv), istringstream::in);
			iss >> ServerName >> ServerPort;
			SL->mServerList.push_back(ServerInfo(ServerName,
				inet_ntoa(addrSrc.sin_addr), ServerPort));
		}
	}while(true);
}
