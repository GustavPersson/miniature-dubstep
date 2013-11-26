#include <iostream>
#include <sstream>

#include "Protocol.h"
#include "MsgH_TextMessage.h"

using namespace std;
using namespace network;

MsgH_TextMessage::MsgH_TextMessage(IdManager *idManager)
{
	mIdManager = idManager;
}
void MsgH_TextMessage::handleMessage(SOCKET src, int len, int type, const char *message)
{
	//cout << "[handleMessage]: ";
	switch(type)
	{
	case MessageType_TextMessage:
		//cout << "MessageType_TextMessage" << endl;
		string msg;
		msg = *this->mIdManager->getNameBySocket(src);
		if(!msg.empty())
		{
			msg.append(" says: ");
			msg.append(string(message, len));
			cout << msg << endl;
		}
		break;
	}
}
