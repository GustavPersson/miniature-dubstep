#include <iostream>
#include <sstream>
using namespace std;

#include "../Sockets/Protocol.h"
#include "PeerMessageHandler.h"

PeerMessageHandler::PeerMessageHandler(Peer *peer)
{
	mPeer = peer;
}
void PeerMessageHandler::handleMessage(SOCKET src, int len, int type, const char *message)
{
	cout << "[handleMessage]: ";
	int id;
	string name;
	istringstream iss (string(message, len), istringstream::in);

	switch(type)
	{
	case MessageType_PlayerID:
		cout << "MessageType_PlayerID" << endl;

		iss >> id;
		iss >> name;

		// Check if they are already registered. We dont want double registers.
		if(this->mPeer->mIdManager.getSocketByID(id)==NULL && this->mPeer->mIdManager.getSocketByName(name)==NULL)
			this->mPeer->mIdManager.registerID(src, id, name);

		cout << " ----Players---- " << endl << this->mPeer->mIdManager << endl;

		break;

	case MessageType_TextMessage:
		cout << "MessageType_TextMessage" << endl;
		string msg;
		msg = *this->mPeer->mIdManager.getNameBySocket(src);
		if(!msg.empty())
		{
			msg.append(" says: ");
			msg.append(string(message, len));
			cout << msg << endl;
		}
		break;
	}
}
