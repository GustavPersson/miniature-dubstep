#include <iostream>
#include <sstream>

#include "ServerLookup.h"

using namespace std;
using namespace network;

#pragma warning (disable: 4127)


ServerInfo::ServerInfo()
{
	name = "NULL";
	addr = "NULL";
	port = 0;
	timeToLive = 2.0f;
}
ServerInfo::ServerInfo(std::string name_, std::string addr_, u_short port_, float timeToLive_)
{
	name = name_;
	addr = addr_;
	port = port_;
	timeToLive = timeToLive_;
}

ServerLookup::ServerLookup(std::string multicastAddr, u_short multicastPort)
{
	// Initiate multicastSocket.
	// Create a socket.
	this->mMulticastSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	char TTL = 32 ; // Restrict to our school network, for example

	if(setsockopt(mMulticastSocket, IPPROTO_IP, IP_MULTICAST_TTL, (char *)&TTL, sizeof(TTL)) != 0)
	{
		DEBUG_MESSAGE(LEVEL_MEDIUM, "[ServerLookup::ServerLookup]: failed to set socketopt(IP_MULTICAST_TTL). errcode: " << WSAGetLastError());
	}

	// Set the local interface from which multicast is to be transmitted
	unsigned long addr = inet_addr("localhost");

	if(setsockopt(mMulticastSocket, IPPROTO_IP, IP_MULTICAST_IF, (char *)&addr, sizeof(addr)) != 0)
	{
		DEBUG_MESSAGE(LEVEL_MEDIUM, "[ServerLookup::ServerLookup]: failed to set socketopt(IP_MULTICAST_IF). errcode: " << WSAGetLastError());
	}

	// Bind the mMulticastSocket to an available port for receiving.
	SOCKADDR_IN addrLocal;
	addrLocal.sin_family = AF_INET;
	addrLocal.sin_port = htons(0);
	addrLocal.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(this->mMulticastSocket, (LPSOCKADDR)&addrLocal, sizeof(addrLocal)) == SOCKET_ERROR)
	{
		DEBUG_MESSAGE(LEVEL_MEDIUM, "[ServerLookup::ServerLookup]: Failed to bind socket. errcode: " << WSAGetLastError());
		return;
	}

	// Start listening thread.
	pthread_create(&mSocketListener, NULL, listeningFunc, this);

	this->mMulticastAddr = multicastAddr;
	this->mMulticastPort = multicastPort;
}
ServerLookup::~ServerLookup()
{
	// Stop listening thread.
	pthread_cancel(mSocketListener);

	// Close listening socket.
	closesocket(this->mMulticastSocket);
}
void ServerLookup::refresh(float deltaTime)
{
	vector<ServerInfo>::iterator server = this->mServerList.begin();
	while( server != mServerList.end() )
	{
		server->timeToLive -= deltaTime;
		std::cout << "time to live = " << server->timeToLive << std::endl;
		if(server->timeToLive <= 0)
		{
			DEBUG_MESSAGE(LEVEL_MEDIUM, "[ServerLookup::refresh]: removing " << server->name << " from server list.");
			server = mServerList.erase( server );
		}
		else
			++server;
	}

	// Compile message.
	ostringstream lookupMessage(ios_base::in | ios_base::out);
	lookupMessage << "SLookup";

	// Set address to target the multicast group.
	SOCKADDR_IN destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_addr.s_addr = inet_addr(this->mMulticastAddr.c_str());
	destAddr.sin_port = htons(this->mMulticastPort);

	sendto(this->mMulticastSocket,
		lookupMessage.str().c_str(),
		lookupMessage.str().length(),
		0,
		(struct sockaddr*)&destAddr,
		sizeof(this->mMulticastAddr));
}
void ServerLookup::sendLookupMessage()
{
	// Clear the serverList
	//this->mServerList.clear();

	// Compile message.
	ostringstream lookupMessage(ios_base::in | ios_base::out);
	lookupMessage << "SLookup";

	// Set address to target the multicast group.
	SOCKADDR_IN destAddr;
	destAddr.sin_family = AF_INET;
	destAddr.sin_addr.s_addr = inet_addr(this->mMulticastAddr.c_str());
	destAddr.sin_port = htons(this->mMulticastPort);

	sendto(this->mMulticastSocket,
		lookupMessage.str().c_str(),
		lookupMessage.str().length(),
		0,
		(struct sockaddr*)&destAddr,
		sizeof(this->mMulticastAddr));
}

void ServerLookup::registerServer(std::string ServerName, u_short ServerPort)
{
	// If this is the first server beeing registered the multicastRecvFunc
	// sould also be initiated.
	bool StartMulticastRecvFunc = false;
	if(mRegisteredServers.empty())
		StartMulticastRecvFunc = true;

	ServerInfo SI(ServerName, " ", ServerPort);
	this->mRegisteredServers.push_back(SI);

	if(StartMulticastRecvFunc)
	{
		// Initiate multicastRecvSocket.
		// Create a socket.
		this->mMulticastRecvSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		SOCKADDR_IN addrLocal;
		// We want to use the Internet address family
		addrLocal.sin_family = AF_INET;
		// Use any local address
		addrLocal.sin_addr.s_addr = INADDR_ANY;
		// Use arbitrary port - but the same as on other clients/servers
		addrLocal.sin_port = htons(this->mMulticastPort); 
		// Bind socket to our address
		if(bind(mMulticastRecvSocket, (LPSOCKADDR)&addrLocal, sizeof(struct sockaddr)) == SOCKET_ERROR)
		{
			DEBUG_MESSAGE(LEVEL_MEDIUM, "[ServerLookup::registerServer]: Failed to bind socket. errcode: " << WSAGetLastError());
		}
		// Ready to switch to multicasting mode

		// Add membership to the multicast group.
		struct ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = inet_addr(this->mMulticastAddr.c_str());
		mreq.imr_interface.s_addr = htonl(INADDR_ANY);
		
		if(setsockopt(this->mMulticastRecvSocket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) != 0)
		{
			DEBUG_MESSAGE(LEVEL_MEDIUM, "[ServerLookup::registerServer]: failed to set socketopt(IP_ADD_MEMBERSHIP). errcode: " << WSAGetLastError());
		}

		// Start multicastRecvThread.
		pthread_create(&this->mMulticastRecvTread, NULL, this->multicastRecvFunc, this);
	}
}

void ServerLookup::unregisterServer(std::string ServerName)
{
	vector<ServerInfo>::iterator server;
	for(server = mRegisteredServers.begin(); server != mRegisteredServers.end(); server++)
	{
		if((*server).name.compare(ServerName)==0)
		{
			mRegisteredServers.erase(server);
			break;
		}
	}
	if(mRegisteredServers.empty())
	{
		// Drop from the multicast group.
		struct ip_mreq mreq;
		mreq.imr_multiaddr.s_addr = inet_addr(this->mMulticastAddr.c_str());
		mreq.imr_interface.s_addr = INADDR_ANY;

		if(setsockopt(this->mMulticastRecvSocket, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mreq, sizeof(mreq)) != 0)
		{
			ERROR_MESSAGE("[ServerLookup::registerServer]: failed to set socketopt(IP_DROP_MEMBERSHIP). errcode: " << WSAGetLastError());
		}

		// Stop multicastRecvThread.
		//pthread_cancel(this->mMulticastRecvTread);

		// Close multicastRecvSocket.
		closesocket(this->mMulticastRecvSocket);
	}
}

vector<ServerInfo> ServerLookup::getServerList()const
{
	return this->mServerList;
}

void* ServerLookup::listeningFunc(void *args)
{
	if(pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL) != 0)
	{
		ERROR_MESSAGE("ERROR(ServerLookup::listeningFunc): pthread_setcancelstate -> PTHREAD_CANCEL_ENABLE")
	}
	if(pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
	{
		ERROR_MESSAGE("ERROR(ServerLookup::listeningFunc): setcanceltype -> PTHREAD_CANCEL_ASYNCHRONOUS");
	}

	ServerLookup *SL = (ServerLookup*)args;

	string ServerName;
	u_short ServerPort;

	SOCKADDR_IN addrSrc;
	int addrSrcSize = sizeof(addrSrc);

	const int BUFLEN=256;
	char buffer[BUFLEN];

	int brecv;

	do{
		//brecv = recvfrom(SL->mListeningSocket, buffer, BUFLEN, 0, (LPSOCKADDR)&addrSrc, &addrSrcSize);
		brecv = recvfrom(SL->mMulticastSocket, buffer, BUFLEN, 0, (LPSOCKADDR)&addrSrc, &addrSrcSize);
		if (brecv==-1)
		{
			DEBUG_MESSAGE(LEVEL_MEDIUM, "[ServerLookup::listeningFunc]: Trouble receving! errcode: " << WSAGetLastError());
			pthread_exit(NULL);
		}
		else
		{
			std::cout << "Got mail!!" << std::endl;
			istringstream iss (string(buffer, brecv), istringstream::in);
			iss >> ServerName >> ServerPort;

			// Check if server is already listed
			vector<ServerInfo>::iterator server;
			bool serverIsListed = false;
			for(server = SL->mServerList.begin(); server != SL->mServerList.end(); server++)
			{
				if(server->name.compare(ServerName)==0)
				{
					server->timeToLive = 0.5f; // restore time to live.
					serverIsListed=true;
					break;
				}
			}
			// If not listed, add to list.
			if(!serverIsListed)
			{
				SL->mServerList.push_back(ServerInfo(ServerName,
					inet_ntoa(addrSrc.sin_addr), ServerPort, 0.5));
			}
		}
	}while(true);
	pthread_exit(NULL);
	return NULL;
}
void* ServerLookup::multicastRecvFunc(void *args)
{
	if(pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL) != 0)
	{
		ERROR_MESSAGE("ERROR(ServerLookup::multicastRecvFunc): pthread_setcancelstate -> PTHREAD_CANCEL_ENABLE");
	}
	if(pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL))
	{
		ERROR_MESSAGE("ERROR(ServerLookup::multicastRecvFunc): setcanceltype -> PTHREAD_CANCEL_ASYNCHRONOUS");
	}

	ServerLookup *SL = (ServerLookup*)args;
	
	SOCKADDR_IN addrSrc;
	int addrSrcSize = sizeof(addrSrc);
	char buffer[50];
	int nRet;

	do{
		nRet = recvfrom(SL->mMulticastRecvSocket, buffer, 50,
						0, (struct sockaddr*)&addrSrc, &addrSrcSize);

		if(nRet==-1)
		{
			int error = WSAGetLastError();
			error;
			pthread_exit(NULL);
			ERROR_MESSAGE("[ServerLookup::multicastRecvFunc]: Trouble receving! ERROR code: " << error);
		}
		else
		{
			string msgType;
			istringstream iss (string(buffer, nRet), istringstream::in);
			iss >> msgType;

			SOCKADDR_IN remoteAddr;
			int len = sizeof(remoteAddr);
			getpeername(SL->mMulticastRecvSocket, (struct sockaddr*)&remoteAddr, &len);

			std::cout << "ip: " <<  inet_ntoa(addrSrc.sin_addr) << " port: " << ntohs(addrSrc.sin_port) << std::endl;

			// Only on "lookup" messages shall we respond.
			if(msgType.compare("SLookup")==0)
			{
				// Print adderss and port.
				//cout << inet_ntoa(addrSrc.sin_addr) << " port = " << port;

				// We send back the server info on the same address but on their listening port instead.
				SOCKADDR_IN sendBackTo;
				sendBackTo.sin_family = AF_INET;
				sendBackTo.sin_port = htons(ntohs(addrSrc.sin_port));
				sendBackTo.sin_addr.s_addr = inet_addr(inet_ntoa(addrSrc.sin_addr));

				// For all registered servers we send a ServerInfo message
				vector<ServerInfo>::iterator server;
				for(server = SL->mRegisteredServers.begin(); server != SL->mRegisteredServers.end(); server++)
				{
					ostringstream ServerInfoMessage(ios_base::in | ios_base::out);
					ServerInfoMessage << server->name << " " << server->port;

					sendto(SL->mMulticastRecvSocket,
						ServerInfoMessage.str().c_str(),
						ServerInfoMessage.str().length(),
						0, (struct sockaddr*)&sendBackTo, addrSrcSize);
				}
			}
		}

	}while(true);

	pthread_exit(NULL);
	return NULL;
}
