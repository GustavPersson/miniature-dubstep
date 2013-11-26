#include <iostream>
#include <sstream>
#include <string>

#include "Protocol.h"
#include "MsgH_GameClient.h"
#include "../Logic/PlayerInfo.h"

using namespace std;
using namespace network;

#pragma warning (disable: 4100)

MsgH_GameClient::MsgH_GameClient(Peer *peer)
{
	mPeer = peer;
}
void MsgH_GameClient::handleMessage(SOCKET src, int len, int type, const char *message)
{
	int id;
	switch(type)
	{
	case MessageType_PlayerID:
		id = *((int*)message);

		this->mPeer->setID(id);
		logic::PlayerInfo::getInstance()->threadSafeSetNetworkId(id);
		break;

	case MessageType_PublishList:
		// Update peer network.
		DEBUG_MESSAGE(LEVEL_LOW, "I got the publish list");
		DEBUG_MESSAGE(LEVEL_LOW, string(message, len));
		mPeer->updateNetwork(message, len);
		break;
	}
}
