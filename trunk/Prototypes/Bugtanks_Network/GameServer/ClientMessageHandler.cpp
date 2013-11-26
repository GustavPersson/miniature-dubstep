#include <iostream>
#include <sstream>
using namespace std;

#include "../Sockets/Protocol.h"
#include "ClientMessageHandler.h"

ClientMessageHandler::ClientMessageHandler(GameServer *server)
{
	mServer = server;
}
void ClientMessageHandler::handleMessage(SOCKET src, int len, int type, const char *message)
{
	cout << "[handleMessage]: ";
	switch(type)
	{
	case MessageType_JoinRequest:
		cout << "MessageType_JoinRequest" << endl;
		// if mServer->getNumberOfParticipants() < mServer->getMaxParticipants() then joinrequest is granted and player is added.
		if(this->mServer->getNumberOfPlayers() < this->mServer->getMaxPlayers())
		{
			// Add player and send join_granted message.
			string name;
			char *ipaddr;
			int port;

			string in(message);
			in = in.substr(0, len-1);

			istringstream iss (in, istringstream::in);
			iss >> name;
			iss >> port;

			this->mServer->getPeerAddress(src, &ipaddr);

			this->mServer->addPlayer(src, name, ipaddr, port);

			// send granted message.
			this->mServer->mConnectionManager.sendTo(src, 0, MessageType_JoinRequest_Granted, NULL);
		}
		else
		{

			//send join_denied message.
			this->mServer->mConnectionManager.sendTo(src, 0, MessageType_JoinRequest_Denied, NULL);
		}
		break;
	case MessageType_Dropout:
		// remove player from the id manager.
		cout << "MessageType_Dropout" << endl;
		mServer->mIdManager.removeID(src);
		break;
	case MessageType_TextMessage:
		cout << "MessageType_TextMessage" << endl;
		// Send TextMessage to all the players.
		mServer->mConnectionManager.sendToAll(len, type, message);
		break;
	}
}
