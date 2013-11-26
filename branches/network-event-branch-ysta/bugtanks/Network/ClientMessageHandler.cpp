#include "ClientMessageHandler.h"
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include "Protocol.h"
#include "../Logic/Events.h"
#include "../Utilities/HashedString.h"

using namespace network;

ClientMessageHandler::ClientMessageHandler(GameServer *server)
{
	mServer = server;
}
void ClientMessageHandler::handleMessage(SOCKET src, int len, int type, const char *message)
{
	
	std::istrstream in(message+sizeof(u_long), (len-sizeof(u_long)));

	std::cout << "[handleMessage]: ";
	switch(type)
	{
	case MessageType_JoinRequest:
		std::cout << "MessageType_JoinRequest" << endl;
		// if mServer->getNumberOfParticipants() < mServer->getMaxParticipants() then joinrequest is granted and player is added.
		if(this->mServer->getNumberOfPlayers() < this->mServer->getMaxPlayers())
		{
			// Add player and send join_granted message.
			std::string name;
			char *ipaddr;
			int port;

			std::string in(message);
			in = in.substr(0, len-1);

			std::istringstream iss (in, std::istringstream::in);
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
		std::cout << "MessageType_Dropout" << endl;
		mServer->mIdManager.removeID(src);
		break;
	case MessageType_TextMessage:
		std::cout << "MessageType_TextMessage" << endl;
		// Send TextMessage to all the players.
		mServer->mConnectionManager.sendToAll(len, type, message);
		break;

	case MessageType_Event:
		createEvent(in);
		break;


	}
}

void ClientMessageHandler::createEvent(std::istrstream &in)
{
	char eventType[256];
	in >> eventType;

	if (!_stricmp(eventType, logic::EvtData_To_ActorStartMove::mEventType.getStr().c_str()))
	{
		logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStartMove(in)));
	}
	else if (!_stricmp(eventType, logic::EvtData_To_ActorStopMove::mEventType.getStr().c_str()))
	{
		logic::safeThreadSafeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorStopMove(in)));
	}
	else
	{
		DEBUG_MESSAGE("ERROR: Unknown event type from remote: " << eventType);
	}
	
}