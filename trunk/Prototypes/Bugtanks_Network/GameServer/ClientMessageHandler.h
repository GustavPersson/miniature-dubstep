#ifndef _CLIENTMESSAGEHANDLER_H_
#define _CLIENTMESSAGEHANDLER_H_

#include <iostream>
#include <string>
using namespace std;

#include "../Sockets/Network.h"
#include "GameServer.h"

class GameServer;

class ClientMessageHandler : public MessageHandler
{
private:
	GameServer *mServer;
public:
	ClientMessageHandler(GameServer *server);
	void handleMessage(SOCKET src, int len, int type, const char *message);
};

#endif
