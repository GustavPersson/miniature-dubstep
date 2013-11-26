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
	pList << this->mIdManager;

	vector<IDstructure> players = this->mIdManager.getIds();
	for(int i=0; i<this->mIdManager.size(); i++)
	{
		this->mConnectionManager->sendTo(players.at(i).socket, pList.str().length(),
			MessageType_PublishList, pList.str().c_str());
	}
}

MsgH_GameServer::MsgH_GameServer(string gameName, ConnectionManager *CM, int maxPlayers):
MAX_PLAYERS(maxPlayers)
{
	this->mGameName = gameName;
	this->mConnectionManager = CM;
}
void MsgH_GameServer::handleMessage(SOCKET src, int len, int type, const char *message)
{
	//cout << "[handleMessage]: ";
	int port;
	string name;
	char *ipaddr;
	istringstream iss (string(message, len), istringstream::in);

	switch(type)
	{
	case MessageType_JoinRequest:
		//cout << "MessageType_JoinRequest" << endl;

		iss >> name;
		iss >> port;
		
		getPeerAddress(src, &ipaddr, NULL);

		// If allready joined, send MessageType_JoinRequest_Denied.
		if(this->mIdManager.getSocketByName(name)!=NULL)
		{
			this->mConnectionManager->sendTo(src, 0, MessageType_JoinRequest_Denied, NULL);
			break;
		}

		// If number of players equals MAX_PLAYERS, send MessageType_JoinRequest_Denied.
		if(this->mIdManager.size() >= this->MAX_PLAYERS)
		{
			this->mConnectionManager->sendTo(src, 0, MessageType_JoinRequest_Denied, NULL);
			break;
		}
		
		this->mIdManager.registerID(src, 0, name, string(ipaddr), port);
		this->mIdManager.setIdsToIndexValue();
		
		// Send updated publish list to all players.
		sendPublishList();

		break;

	case MessageType_Dropout:
		//cout << "MessageType_Dropout" << endl;

		// Remove src from id manager.
		this->mIdManager.removeID(src);
		this->mIdManager.setIdsToIndexValue();

		// Send updated publish list to all players.
		sendPublishList();

		break;
	}
}
