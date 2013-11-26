#include <iostream>
#include <sstream>

#include "Protocol.h"
#include "MsgH_TextMessage.h"
#include "../Logic/Application.h"

using namespace std;
using namespace network;

MsgH_TextMessage::MsgH_TextMessage()
{
	mIdManager = IdManager::getInstance();
}
void MsgH_TextMessage::handleMessage(SOCKET src, int len, int type, const char *message)
{
	switch(type)
	{
	case MessageType_TextMessage:
		string msg;
		msg = *this->mIdManager->getNameBySocket(src);
		if(!msg.empty())
		{

			std::string chat = logic::Application::getInstance()->getStartMenu()->getChatBox()->getTextFrame()->getTextString();

			string *name = IdManager::getInstance()->getNameBySocket(src);
			if(name==NULL)
				break;

			chat.append("\n" + *name + " says   " + string(message, len));

			logic::Application::getInstance()->getStartMenu()->getChatBox()->getTextFrame()->setText(chat);

			msg.append(" says: ");
			msg.append(string(message, len));
			DEBUG_MESSAGE(LEVEL_LOW, msg);
		}

		break;
	}
}
