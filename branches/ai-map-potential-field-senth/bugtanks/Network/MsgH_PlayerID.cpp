#include <iostream>
#include <sstream>

#include "Protocol.h"
#include "NetworkUtils.h"
#include "MsgH_PlayerID.h"

using namespace std;
using namespace network;

MsgH_PlayerID::MsgH_PlayerID()
{
	mIdManager = IdManager::getInstance();
}
void MsgH_PlayerID::handleMessage(SOCKET src, int len, int type, const char *message)
{
	int id, port;
	string name, ipaddr;
	istringstream iss (string(message, len), istringstream::in);

	switch(type)
	{
	case MessageType_PlayerID:
		//cout << "MessageType_PlayerID" << endl;

		iss >> id;
		iss >> name;
		iss >> port;

		ipaddr = getPeerAddress(src);

		// Check if they are already registered. We dont want double registers.
		if(this->mIdManager->getSocketByID(id)==NULL && this->mIdManager->getSocketByName(name)==NULL)
			this->mIdManager->registerID(src, id, name, ipaddr, port);

		break;
	}
}
