#include <iostream>
#include <sstream>

#include "Protocol.h"
#include "MsgH_GameClient.h"

using namespace std;
using namespace network;

MsgH_GameClient::MsgH_GameClient(Peer *peer)
{
	this->mPeer = peer;
}
void MsgH_GameClient::handleMessage(SOCKET src, int len, int type, const char *message)
{
	//cout << "[handleMessage]: ";
	switch(type)
	{
	case MessageType_PublishList:
		//cout << "MessageType_PublishList" << endl;
		// Update peer network.
		cout << string(message, len);
		this->mPeer->updateNetwork(message, len);
		break;
	}
}
