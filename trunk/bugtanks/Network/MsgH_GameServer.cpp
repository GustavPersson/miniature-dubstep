#include <iostream>
#include <sstream>

#include "Protocol.h"
#include "NetworkUtils.h"
#include "MsgH_GameServer.h"

using namespace std;
using namespace network;

void MsgH_GameServer::sendPublishList()
{
	// Compile publish list.
	ostringstream pList(ios_base::in | ios_base::out);
	pList << this->mIDManager;

	int playerID;

	vector<IDstructure> players = this->mIDManager.getIds();
	for(int i=0; i<this->mIDManager.size(); i++)
	{
		// First send the players ID
		playerID = this->mIDManager.getIdBySocket(players.at(i).socket);
		this->mConnectionManager->sendTo(players.at(i).socket, sizeof(playerID),
			MessageType_PlayerID, (char*)&playerID);

		// Then send the publishList
		this->mConnectionManager->sendTo(players.at(i).socket, pList.str().length(),
			MessageType_PublishList, pList.str().c_str());
	}
}

MsgH_GameServer::MsgH_GameServer(std::string gameName, ConnectionManager *CM, int maxPlayers)
{	
	this->mMaxPlayers = maxPlayers;
	this->mGameName = gameName;
	this->mConnectionManager = CM;
}
MsgH_GameServer::~MsgH_GameServer()
{
	DEBUG_MESSAGE(LEVEL_LOWEST, "MsgH_GameServer::~MsgH_GameServer()");

	vector<IDstructure> ids = this->mIDManager.getIds();
	for(int i=0; i<(int)ids.size(); i++)
	{
		this->mConnectionManager->removeConnection(ids[i].socket);
	}
	this->mIDManager.clear();
}
void MsgH_GameServer::handleMessage(SOCKET src, int len, int type, const char *message)
{
	char ac[80];
	struct hostent *phe;
	struct in_addr addr;

	SOCKADDR_IN remoteAddr;
	int addrLen = sizeof(remoteAddr);

	int port;
	string name,ipaddr;
	istringstream iss (string(message, len), istringstream::in);

	switch(type)
	{
	case MessageType_JoinRequest:

		iss >> name;
		iss >> port;


		//ipaddr = getPeerAddress(src);
		getpeername(src, (struct sockaddr*)&remoteAddr, &addrLen);
		ipaddr = string(inet_ntoa(remoteAddr.sin_addr));

		// When the hosting peers socket connects it's going to return the loop back address
		// but we want the real address so this is how we get that.
		if(ipaddr.compare("127.0.0.1")==0)
		{
			
			if (gethostname(ac, sizeof(ac)) == SOCKET_ERROR) {
				DEBUG_MESSAGE(LEVEL_MEDIUM, "Error(MsgH_GameServer::handleMessage): "
					<< WSAGetLastError() << " when getting local host name.");
			}
			//cout << "Host name is " << ac << "." << endl;

			phe = gethostbyname(ac);
			if (phe == 0) {
				DEBUG_MESSAGE(LEVEL_MEDIUM, "Error(MsgH_GameServer::handleMessage): Yow! Bad host lookup.");
			}

			memcpy(&addr, phe->h_addr_list[0], sizeof(struct in_addr));
			ipaddr = string(inet_ntoa(addr));
		}

		// If number of players equals MAX_PLAYERS, send MessageType_JoinRequest_Denied.
		if(this->mIDManager.size() >= this->mMaxPlayers)
		{
			this->mConnectionManager->sendTo(src, 0, MessageType_JoinRequest_Denied, NULL);
			break;
		}
		
		this->mIDManager.registerID(src, 0, name, ipaddr, port);
		this->mIDManager.setIdsToIndexValue();

		DEBUG_MESSAGE(LEVEL_MEDIUM, name << " has joined the game!");
		DEBUG_MESSAGE(LEVEL_LOW, this->mIDManager);

		// Send updated publish list to all players.
		sendPublishList();

		break;

	case MessageType_Dropout:
		
		if(this->mIDManager.getIdBySocket(src)!=-1)
		{
			// Remove src from id manager.
			this->mIDManager.removeID(src);
			this->mIDManager.setIdsToIndexValue();

			// Send updated publish list to all players.
			sendPublishList();
		}
		break;
	}
}
