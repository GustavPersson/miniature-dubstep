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

	vector<IDstructure> players = this->mIDManager.getIds();
	for(int i=0; i<this->mIDManager.size(); i++)
	{
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
	int port;
	string name,ipaddr;
	istringstream iss (string(message, len), istringstream::in);

	switch(type)
	{
	case MessageType_JoinRequest:

		iss >> name;
		iss >> port;


		ipaddr = getPeerAddress(src);

		// If already joined, send MessageType_JoinRequest_Denied.
		if(this->mIDManager.getSocketByName(name)!=NULL)
		{
			this->mConnectionManager->sendTo(src, 0, MessageType_JoinRequest_Denied, NULL);
			break;
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

		// Remove src from id manager.
		this->mIDManager.removeID(src);
		this->mIDManager.setIdsToIndexValue();

		// Send updated publish list to all players.
		sendPublishList();

		break;
	}
}
